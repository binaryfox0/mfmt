#include "msav.h"

#include <stdio.h>
#include <stdlib.h>

#include <mfmtio.h>
#include <zlib-ng.h>

#include "msav_zlib.h"
#include "msav_log_priv.h"
#include "msav_ubjson.h"
#include "msav_object.h"
#include "msav_blocks_gen.h"
#include "msav_blocks_gen_priv.h"
#include "msav_alloc_priv.h"

#define MSAV__ARRSZ(array) (sizeof(array) / sizeof((array)[0]))

typedef struct msav__file_reader msav__file_reader_t;
typedef msav_err_t (*msav__payload_read_fn)(
        mfmtio_rstream_t *stream,
        msav__file_reader_t *reader);
typedef msav_err_t (*msav__chunk_read_fn)(
        mfmtio_rstream_t *stream,
        msav__file_reader_t *reader,
        const char *name,
        msav__payload_read_fn payload_read);
typedef struct
{
    int32_t version;
    msav__chunk_read_fn chunk_read;
    msav__chunk_read_fn entity_chunk_read;
    const char *entity_chunk_format;
} msav__version_readers_t;

struct msav__file_reader
{
    mfmtio_rstream_t *stream;
    msav_file_t *out;
    int32_t version;
    msav__chunk_read_fn chunk_read;
    msav__chunk_read_fn entity_chunk_read;
    char *entity_names[256];
    int16_t current_block;
};

static const uint8_t msav__header[] = {'M', 'S', 'A', 'V'};


static msav_err_t msav__read_chunk_modern(
        mfmtio_rstream_t *stream,
        msav__file_reader_t *reader,
        const char *name,
        msav__payload_read_fn payload_read);
static msav_err_t msav__read_chunk_legacy(
        mfmtio_rstream_t *stream,
        msav__file_reader_t *reader,
        const char *name,
        msav__payload_read_fn payload_read);
static msav_err_t msav__read_tile_entity(
    mfmtio_rstream_t *stream,
    msav__file_reader_t *reader);

#define MSAV__VERSION_READER(version_number, entity_reader, entity_format) \
    {version_number, msav__read_chunk_modern, entity_reader, entity_format}

static const msav__version_readers_t msav__version_readers[] =
{
    MSAV__VERSION_READER(1, msav__read_chunk_legacy, "legacy 16-bit length"),
    MSAV__VERSION_READER(2, msav__read_chunk_legacy, "legacy 16-bit length"),
    MSAV__VERSION_READER(3, msav__read_chunk_legacy, "legacy 16-bit length"),
    MSAV__VERSION_READER(4, msav__read_chunk_legacy, "legacy 16-bit length"),
    MSAV__VERSION_READER(5, msav__read_chunk_legacy, "legacy 16-bit length"),
    MSAV__VERSION_READER(6, msav__read_chunk_legacy, "legacy 16-bit length"),
    MSAV__VERSION_READER(7, msav__read_chunk_legacy, "legacy 16-bit length"),
    MSAV__VERSION_READER(8, msav__read_chunk_modern, "32-bit length"),
    MSAV__VERSION_READER(9, msav__read_chunk_modern, "32-bit length"),
    MSAV__VERSION_READER(10, msav__read_chunk_modern, "32-bit length"),
    MSAV__VERSION_READER(11, msav__read_chunk_modern, "32-bit length"),
    MSAV__VERSION_READER(12, msav__read_chunk_modern, "32-bit length"),
    MSAV__VERSION_READER(13, msav__read_chunk_modern, "32-bit length")
};

#undef MSAV__VERSION_READER

static const char *msav__ctype_strings[MSAV__CTYPE_COUNT] =
{
    [MSAV_CTYPE_ITEM]           = "item",
    [MSAV_CTYPE_BLOCK]          = "block",
    [MSAV_CTYPE_MECH]           = "mech",       /* unused */
    [MSAV_CTYPE_BULLET]         = "bullet",
    [MSAV_CTYPE_LIQUID]         = "liquid",
    [MSAV_CTYPE_STATUS]         = "status",
    [MSAV_CTYPE_UNIT]           = "unit",
    [MSAV_CTYPE_WEATHER]        = "weather",
    [MSAV_CTYPE_EFFECT]         = "effect",     /* unused */
    [MSAV_CTYPE_SECTOR]         = "sector",
    [MSAV_CTYPE_LOADOUT]        = "loadout",    /* unused */
    [MSAV_CTYPE_TYPEID]         = "typeid",     /* unused */
    [MSAV_CTYPE_ERROR]          = "error",
    [MSAV_CTYPE_PLANET]         = "planet",
    [MSAV_CTYPE_AMMO]           = "ammo",       /* unused */
    [MSAV_CTYPE_TEAM]           = "team",
    [MSAV_CTYPE_UNIT_COMMAND]   = "unitCommand",
    [MSAV_CTYPE_UNIT_STANCE]    = "unitStance",
};

static int msav__has_bytes(const mfmtio_rstream_t *stream, uint32_t count)
{
    return stream && !stream->eos && stream->idx <= stream->bufsz &&
           count <= stream->bufsz - stream->idx;
}

static msav_err_t msav__read_chunk_common(
        mfmtio_rstream_t *stream,
        msav__file_reader_t *reader,
        const char *name,
        msav__payload_read_fn payload_read,
        int legacy_length);

static msav_err_t msav__read_chunk_modern(
        mfmtio_rstream_t *stream,
        msav__file_reader_t *reader,
        const char *name,
        msav__payload_read_fn payload_read)
{
    return msav__read_chunk_common(stream, reader, name, 
            payload_read, 0);
}

static msav_err_t msav__read_chunk_legacy(
        mfmtio_rstream_t *stream,
        msav__file_reader_t *reader,
        const char *name,
        msav__payload_read_fn payload_read)
{
    return msav__read_chunk_common(stream, reader, name, 
            payload_read, 1);
}


static const msav__version_readers_t *msav__get_version_readers(
        const int32_t version)
{
    for(size_t i = 0; i < MSAV__ARRSZ(msav__version_readers); i++)
    {
        if(msav__version_readers[i].version == version)
            return &msav__version_readers[i];
    }
    return NULL;
}

static msav_err_t msav__skip_payload(
        mfmtio_rstream_t *stream,
        msav__file_reader_t *reader);
static msav_err_t msav__read_chunk_common(
        mfmtio_rstream_t *stream,
        msav__file_reader_t *reader,
        const char *name,
        msav__payload_read_fn payload_read_fn,
        int legacy_length)
{
    uint32_t prefix_size = legacy_length ? 2u : 4u;
    uint32_t payload_start = 0;
    uint32_t payload_end = 0;
    uint32_t consumed = 0;
    uint32_t length = 0;
    int is_skip = 0;
    msav_err_t rc = MSAV_ERR_OK;
    const char *format = legacy_length ? "legacy-u16" : "i32";

    if(!stream || !reader || !name || !payload_read_fn)
        return MSAV_ERR_PARAM;
    if(!msav__has_bytes(stream, prefix_size))
    {
        MSAV__ERROR("chunk '%s': missing %s length at offset 0x%08X",
                name, format, stream->idx);
        return MSAV_ERR_CORRUPT;
    }

    if(legacy_length)
        length = mfmtio_read_u16(stream);
    else
    {
        int32_t signed_length = mfmtio_read_i32(stream);
        if(signed_length < 0)
        {
            MSAV__ERROR("chunk '%s': negative length %d at offset 0x%08X",
                    name, signed_length, stream->idx - prefix_size);
            return MSAV_ERR_CORRUPT;
        }
        length = (uint32_t)signed_length;
    }

    if(stream->eos || !msav__has_bytes(stream, length))
    {
        MSAV__ERROR("chunk '%s': length %u exceeds remaining input "
                "at offset 0x%08X", name, length, stream->idx);
        return MSAV_ERR_CORRUPT;
    }

    payload_start = stream->idx;
    payload_end = payload_start + length;
    is_skip = payload_read_fn == msav__skip_payload;
    if(is_skip && legacy_length)
        MSAV__DEBUG("chunk '%s': format=%s offset=0x%08X length=%u",
                name, format, payload_start - prefix_size, length);
    else
        MSAV__INFO("chunk '%s' begin: format=%s offset=0x%08X length=%u",
                name, format, payload_start - prefix_size, length);

    uint32_t stream_end = stream->bufsz;
    stream->bufsz = payload_end;
    rc = payload_read_fn(stream, reader);
    stream->bufsz = stream_end;
    if(rc != MSAV_ERR_OK)
    {
        MSAV__ERROR("chunk '%s' failed at offset 0x%08X (error=%d)",
                name, stream->idx, rc);
        return rc;
    }
    if(stream->eos)
    {
        MSAV__ERROR("chunk '%s' reached end of input at offset 0x%08X",
                name, stream->idx);
        return MSAV_ERR_CORRUPT;
    }

    consumed = stream->idx - payload_start;
    if(is_skip)
        MSAV__DEBUG("chunk '%s' payload skipped: %u bytes", name, length);
    else if(consumed != length)
        MSAV__WARN("chunk '%s' payload size differs: consumed=%u expected=%u",
                name, consumed, length);

    stream->idx = payload_end;
    if(!(is_skip && legacy_length))
        MSAV__INFO("chunk '%s' end: offset=0x%08X", name, stream->idx);
    return MSAV_ERR_OK;
}

static msav_err_t msav__skip_payload(
        mfmtio_rstream_t *stream,
        msav__file_reader_t *reader)
{
    (void)stream;
    (void)reader;
    return MSAV_ERR_OK;
}

static int msav__dump_map(
        const int16_t *map,
        uint16_t width,
        uint16_t height,
        const char *file_name)
{
    FILE *file = NULL;

    if(!map || width == 0 || height == 0 || !file_name)
        return -1;

    file = fopen(file_name, "w");
    if(!file)
    {
        MSAV__WARN("could not write map dump '%s'", file_name);
        return -1;
    }

    for(uint16_t y = 0; y < height; y++)
    {
        for(uint16_t x = 0; x < width; x++)
        {
            fprintf(file, "%04X", (uint16_t)*map++);
            if(x + 1 < width)
                fputc(' ', file);
        }
        fputc('\n', file);
    }

    if(fclose(file) != 0)
    {
        MSAV__WARN("failed to close map dump '%s'", file_name);
        return -1;
    }

    return 0;
}

msav_err_t msav__read_meta(
        mfmtio_rstream_t *stream, 
        msav__file_reader_t *reader)
{
    int16_t size = mfmtio_read_i16(stream);
    MSAV__INFO("meta: entries=%d", size);
    for(int16_t i = 0; i < size; i++)
    {
        char* key = mfmtio_read_utf(stream);
        char* val = mfmtio_read_utf(stream);
        MSAV__DEBUG("meta entry %d: key=\"%s\", val=\"%s\"", 
                i, key, val);
        mfmtio_free(key);
        mfmtio_free(val);
    }
    MSAV__INFO("meta decoded: entries=%d", size);
    return MSAV_ERR_OK;
}

msav_err_t msav__read_content(
        mfmtio_rstream_t *stream, 
        msav__file_reader_t *reader)
{
    
    int8_t mapped = mfmtio_read_i8(stream);
    MSAV__INFO("content: mapped=%d", mapped);
    for(int i = 0; i < mapped; i++)
    {
        char ctype = mfmtio_read_i8(stream);
        int16_t total = mfmtio_read_i16(stream);
        
        MSAV__INFO("content: type=%d (%s), total=%d", 
                ctype, msav__ctype_strings[ctype], total);
        for(int16_t j = 0; j < total; j++)
        {
            char* name = mfmtio_read_utf(stream);
            MSAV__INFO("content entry: name=\"%s\"", name);
            mfmtio_free(name);
        }
    }
    MSAV__INFO("content decoded: mapped=%d", mapped);
    return MSAV_ERR_OK;
}

static msav_err_t msav__read_map(
        mfmtio_rstream_t *stream,
        msav__file_reader_t *reader)
{
    uint16_t width = 0;
    uint16_t height = 0;
    uint32_t tile_count = 0;
    int16_t *floors = NULL;
    int16_t *ores = NULL;
    int16_t *blocks = NULL;
    msav_err_t rc = MSAV_ERR_OK;

    (void)reader;

    if(!msav__has_bytes(stream, 4))
        return MSAV_ERR_CORRUPT;

    width = (uint16_t)mfmtio_read_i16(stream);
    height = (uint16_t)mfmtio_read_i16(stream);
    if(stream->eos || width == 0 || height == 0 ||
       (uint32_t)width > UINT32_MAX / (uint32_t)height)
    {
        MSAV__ERROR("map: invalid dimensions %u x %u", width, height);
        return MSAV_ERR_CORRUPT;
    }

    tile_count = (uint32_t)width * height;
    if((size_t)tile_count > SIZE_MAX / sizeof(*floors))
    {
        MSAV__ERROR("map: dimensions %u x %u exceed allocation limits", 
                width, height);
        return MSAV_ERR_CORRUPT;
    }

    floors = msav__calloc(tile_count, sizeof(*floors));
    ores = msav__calloc(tile_count, sizeof(*ores));
    blocks = msav__calloc(tile_count, sizeof(*blocks));
    if(!floors || !ores || !blocks)
    {
        MSAV__ERROR("map: unable to allocate %u tiles", tile_count);
        rc = MSAV_ERR_NO_MEM;
        goto cleanup;
    }

    MSAV__INFO("map begin: width=%u height=%u tiles=%u", 
            width, height, tile_count);

    for(uint32_t i = 0; i < tile_count;)
    {
        int16_t floor_id = 0;
        int16_t ore_id = 0;
        uint8_t run = 0;
        uint32_t run_length = 0;

        if(!msav__has_bytes(stream, 5))
        {
            rc = MSAV_ERR_CORRUPT;
            goto malformed_map;
        }

        floor_id = mfmtio_read_i16(stream);
        ore_id = mfmtio_read_i16(stream);
        run = mfmtio_read_u8(stream);
        run_length = (uint32_t)run + 1;
        if(stream->eos || run_length > tile_count - i)
        {
            rc = MSAV_ERR_CORRUPT;
            goto malformed_map;
        }

        for(uint32_t j = 0; j < run_length; j++)
        {
            floors[i + j] = floor_id;
            ores[i + j] = ore_id;
        }
        i += run_length;
    }

    for(uint32_t i = 0; i < tile_count;)
    {
        int16_t block = 0;
        uint8_t packed = 0;
        int had_entity = 0;
        int had_data = 0;

        if(!msav__has_bytes(stream, 3))
        {
            rc = MSAV_ERR_CORRUPT;
            goto malformed_map;
        }

        block = mfmtio_read_i16(stream);
        packed = mfmtio_read_u8(stream);
        had_entity = (packed & 0x01) != 0;
        had_data = (packed & 0x04) != 0;
        blocks[i] = block;

        if(had_data)
        {
            if(!msav__has_bytes(stream, 7))
            {
                rc = MSAV_ERR_CORRUPT;
                goto malformed_map;
            }
            mfmtio_read_i8(stream);
            mfmtio_read_i8(stream);
            mfmtio_read_i8(stream);
            mfmtio_read_i32(stream);
        }

        if(had_entity)
        {
            int is_center = 0;
            if(!msav__has_bytes(stream, 1))
            {
                rc = MSAV_ERR_CORRUPT;
                goto malformed_map;
            }
            is_center = mfmtio_read_bool(stream);
            if(stream->eos)
            {
                rc = MSAV_ERR_CORRUPT;
                goto malformed_map;
            }

            if(is_center)
            {
                const char *chunk_name = "tile entity";
                reader->current_block = block;
                rc = reader->entity_chunk_read(
                        stream, reader, chunk_name, 
                    msav__read_tile_entity);
                if(rc != MSAV_ERR_OK)
                    goto cleanup;
            }
            i++;
        }
        else if(!had_data)
        {
            uint8_t run = 0;
            uint32_t run_length = 0;
            if(!msav__has_bytes(stream, 1))
            {
                rc = MSAV_ERR_CORRUPT;
                goto malformed_map;
            }
            run = mfmtio_read_u8(stream);
            run_length = (uint32_t)run + 1;
            if(stream->eos || run_length > tile_count - i)
            {
                rc = MSAV_ERR_CORRUPT;
                goto malformed_map;
            }
            for(uint32_t j = 1; j < run_length; j++)
                blocks[i + j] = block;
            i += run_length;
        }
        else
        {
            i++;
        }
    }

    for(uint32_t i = 0; i < tile_count; i++)
    {
        int16_t block = blocks[i];
        const char *name = msav_block_name((msav_block_t)block);

        MSAV__DEBUG("map block at (%u,%u): id=%d name=%s",
                i % width, i / width, block, name ? name : "unknown");
    }

    MSAV__INFO("map decoded: width=%u height=%u tiles=%u", 
            width, height, tile_count);
    msav__dump_map(floors, width, height, "floors_dump.txt");
    msav__dump_map(ores, width, height, "ores_dump.txt");
    msav__dump_map(blocks, width, height, "blocks_dump.txt");
    goto cleanup;

malformed_map:
    MSAV__ERROR("map: malformed tile data at offset 0x%08X", stream->idx);

cleanup:
    msav__free(floors);
    msav__free(ores);
    msav__free(blocks);
    return rc;
}

static msav_err_t msav__skip_bytes(mfmtio_rstream_t *stream, uint64_t count)
{
    if(count > UINT32_MAX || !msav__has_bytes(stream, (uint32_t)count))
        return MSAV_ERR_CORRUPT;
    stream->idx += (uint32_t)count;
    return MSAV_ERR_OK;
}

static msav_err_t msav__skip_typeio_object(
        mfmtio_rstream_t *stream,
        uint32_t depth,
        int allow_arrays)
{
    msav__obj_t obj = {0};
    int rc = 0;

    (void)depth;
    (void)allow_arrays;

    if(!msav__has_bytes(stream, 1))
        return MSAV_ERR_CORRUPT;

    rc = msav__obj_read(stream, &obj);
    if(rc < 0)
        return MSAV_ERR_CORRUPT;

    msav__obj_destroy(&obj);
    return MSAV_ERR_OK;
}

static msav_err_t msav__read_entity(
        mfmtio_rstream_t *stream,
        msav__file_reader_t *reader)
{
    uint8_t type = 0;
    int32_t id = 0;
    uint32_t remaining = 0;

    if(!msav__has_bytes(stream, 5))
        return MSAV_ERR_CORRUPT;

    type = mfmtio_read_u8(stream);
    id = mfmtio_read_i32(stream);
    if(stream->eos)
        return MSAV_ERR_CORRUPT;

    remaining = stream->bufsz - stream->idx;
    MSAV__DEBUG("entity: class=%s type=%u id=%d payload=%u bytes",
            reader->entity_names[type] ? reader->entity_names[type] : "unknown",
            type, id, remaining);
    return msav__skip_bytes(stream, remaining);
}

static msav_err_t msav__read_entities(
        mfmtio_rstream_t *stream,
        msav__file_reader_t *reader)
{
    int16_t mapping_count = 0;
    int32_t team_count = 0;
    int32_t entity_count = 0;
    msav_err_t rc = MSAV_ERR_OK;

    if(!msav__has_bytes(stream, 2))
        return MSAV_ERR_CORRUPT;
    mapping_count = mfmtio_read_i16(stream);
    if(stream->eos || mapping_count < 0 ||
       (uint32_t)mapping_count > (stream->bufsz - stream->idx) / 4)
        return MSAV_ERR_CORRUPT;

    for(int16_t i = 0; i < mapping_count; i++)
    {
        int16_t type = 0;
        char *name = NULL;
        if(!msav__has_bytes(stream, 2))
        {
            rc = MSAV_ERR_CORRUPT;
            goto cleanup;
        }
        type = mfmtio_read_i16(stream);
        name = mfmtio_read_utf(stream);
        if(stream->eos || type < 0 || !name)
        {
            mfmtio_free(name);
            rc = stream->oom ? MSAV_ERR_NO_MEM : MSAV_ERR_CORRUPT;
            goto cleanup;
        }
        if(type < (int16_t)MSAV__ARRSZ(reader->entity_names))
        {
            mfmtio_free(reader->entity_names[type]);
            reader->entity_names[type] = name;
        }
        else
        {
            mfmtio_free(name);
        }
    }

    if(!msav__has_bytes(stream, 4))
    {
        rc = MSAV_ERR_CORRUPT;
        goto cleanup;
    }
    team_count = mfmtio_read_i32(stream);
    if(stream->eos || team_count < 0 ||
       (uint32_t)team_count > (stream->bufsz - stream->idx) / 8)
    {
        rc = MSAV_ERR_CORRUPT;
        goto cleanup;
    }
    MSAV__INFO("entities: mappings=%d teams=%d", mapping_count, team_count);

    for(int32_t i = 0; i < team_count; i++)
    {
        int32_t team_id = 0;
        int32_t plans = 0;
        if(!msav__has_bytes(stream, 8))
        {
            rc = MSAV_ERR_CORRUPT;
            goto cleanup;
        }
        team_id = mfmtio_read_i32(stream);
        plans = mfmtio_read_i32(stream);
        if(stream->eos || plans < 0 ||
           (uint32_t)plans > (stream->bufsz - stream->idx) / 9)
        {
            rc = MSAV_ERR_CORRUPT;
            goto cleanup;
        }

        for(int32_t j = 0; j < plans; j++)
        {
            if(!msav__has_bytes(stream, 8))
            {
                rc = MSAV_ERR_CORRUPT;
                goto cleanup;
            }
            stream->idx += 8;
            rc = msav__skip_typeio_object(stream, 0, 1);
            if(rc != MSAV_ERR_OK)
                goto cleanup;
        }
        MSAV__DEBUG("entity plans: team=%d count=%d", team_id, plans);
    }

    if(!msav__has_bytes(stream, 4))
    {
        rc = MSAV_ERR_CORRUPT;
        goto cleanup;
    }
    entity_count = mfmtio_read_i32(stream);
     uint32_t minimum_record = (reader->version < 8 ? 2u : 4u) + 5u;
     if(stream->eos || entity_count < 0 ||
         (uint32_t)entity_count > (stream->bufsz - stream->idx) / minimum_record)
    {
        rc = MSAV_ERR_CORRUPT;
        goto cleanup;
    }
    MSAV__INFO("entities: world=%d", entity_count);

    for(int32_t i = 0; i < entity_count; i++)
    {
        rc = reader->entity_chunk_read(
                stream, reader, "world entity", msav__read_entity);
        if(rc != MSAV_ERR_OK)
            goto cleanup;
    }

    MSAV__INFO("entities decoded: world=%d", entity_count);

cleanup:
    for(uint32_t i = 0; i < MSAV__ARRSZ(reader->entity_names); i++)
    {
        mfmtio_free(reader->entity_names[i]);
        reader->entity_names[i] = NULL;
    }
    return rc;
}

static msav_err_t msav__read_tile_entity(
        mfmtio_rstream_t *stream,
        msav__file_reader_t *reader)
{
    uint8_t revision = 0;
    uint8_t modules = 0;
    const char *block_name = NULL;

    if(!msav__has_bytes(stream, 1))
        return MSAV_ERR_CORRUPT;

    revision = mfmtio_read_u8(stream);
    if(reader->current_block >= 0 && reader->current_block < MSAV_BLOCK_MAX)
    {
        msav__block_data_t *block = &msav__block_db[reader->current_block];
        modules = block->modules_bitmask;
        block_name = msav_block_name((msav_block_t)reader->current_block);
    }

    MSAV__DEBUG("tile entity: block=%s revision=%u modules=%s%s%s body=%u bytes",
            block_name ? block_name : "unknown",
            revision,
            (modules & MSAV_MODULE_BITMASK_HAS_ITEMS) ? "items " : "",
            (modules & MSAV_MODULE_BITMASK_HAS_POWER) ? "power " : "",
            (modules & MSAV_MODULE_BITMASK_HAS_LIQUIDS) ? "liquids" : "",
            stream->bufsz - stream->idx);

    return msav__skip_bytes(stream, stream->bufsz - stream->idx);
}

msav_err_t msav__read_patches(
        mfmtio_rstream_t *stream,
        msav__file_reader_t *reader)
{
    int32_t format_version = 0;
    int32_t total = 0;

    (void)reader;

    if(!msav__has_bytes(stream, 8))
        return MSAV_ERR_CORRUPT;

    format_version = mfmtio_read_i32(stream);
    total = mfmtio_read_i32(stream);
    if(stream->eos || format_version < 0 || total < 0 ||
       (uint32_t)total > stream->bufsz - stream->idx)
    {
        MSAV__ERROR("patches: invalid header format=%d assets=%d",
                format_version, total);
        return MSAV_ERR_CORRUPT;
    }

    MSAV__INFO("patches: format=%d assets=%d", format_version, total);
    for(int32_t i = 0; i < total; i++)
    {
        uint8_t type = 0;
        char *path = NULL;
        int embedded = 0;
        int32_t length = 0;
        msav_err_t rc = MSAV_ERR_OK;

        if(!msav__has_bytes(stream, 3))
            return MSAV_ERR_CORRUPT;

        type = mfmtio_read_u8(stream);
        path = mfmtio_read_utf(stream);
        embedded = mfmtio_read_bool(stream);
        if(stream->eos || !path)
        {
            mfmtio_free(path);
            MSAV__ERROR("patches: malformed asset record %d", i);
            return stream->oom ? MSAV_ERR_NO_MEM : MSAV_ERR_CORRUPT;
        }
        if(type >= 6)
        {
            MSAV__ERROR("patches: invalid asset type %u for '%s'", type, path);
            mfmtio_free(path);
            return MSAV_ERR_CORRUPT;
        }

        if(embedded)
        {
            if(type == 1)
            {
                if(!msav__has_bytes(stream, 2))
                {
                    mfmtio_free(path);
                    return MSAV_ERR_CORRUPT;
                }
                (void)mfmtio_read_i16(stream);
            }

            if(!msav__has_bytes(stream, 4))
            {
                mfmtio_free(path);
                return MSAV_ERR_CORRUPT;
            }
            length = mfmtio_read_i32(stream);
            if(stream->eos || length < 0)
            {
                MSAV__ERROR("patches: invalid embedded length for '%s'", path);
                mfmtio_free(path);
                return MSAV_ERR_CORRUPT;
            }
            rc = msav__skip_bytes(stream, (uint32_t)length);
            if(rc != MSAV_ERR_OK)
            {
                MSAV__ERROR("patches: truncated embedded asset '%s'", path);
                mfmtio_free(path);
                return rc;
            }
        }
        else
        {
            rc = msav__skip_bytes(stream, 32);
            if(rc != MSAV_ERR_OK)
            {
                MSAV__ERROR("patches: truncated hash for '%s'", path);
                mfmtio_free(path);
                return rc;
            }
        }

        MSAV__DEBUG("patch asset: type=%u path=\"%s\" embedded=%s",
                type, path, embedded ? "true" : "false");
        mfmtio_free(path);
    }

    MSAV__INFO("patches decoded: assets=%d", total);
    return stream->eos ? MSAV_ERR_CORRUPT : MSAV_ERR_OK;
}

static msav_err_t msav__read_markers(
        mfmtio_rstream_t *stream,
        msav__file_reader_t *reader)
{
    int type = 0;
    uint64_t marker_count = 0;
    msav_err_t rc = MSAV_ERR_OK;

    (void)reader;

    if(!msav__has_bytes(stream, 1))
        return MSAV_ERR_CORRUPT;
    type = mfmtio_read_u8(stream);
    if(type != '{')
    {
        MSAV__ERROR("markers: expected UBJSON object, found 0x%02X", type);
        return MSAV_ERR_CORRUPT;
    }

    rc = msav__ubjson_count_entries(stream, type, &marker_count);
    if(rc != MSAV_ERR_OK)
    {
        MSAV__ERROR("markers: malformed UBJSON at offset 0x%08X", stream->idx);
        return rc;
    }

    MSAV__INFO("markers decoded: count=%llu",
            (unsigned long long)marker_count);
    return MSAV_ERR_OK;
}

static msav_err_t msav__read_custom(
        mfmtio_rstream_t *stream,
        msav__file_reader_t *reader)
{
    int32_t amount = 0;

    if(!msav__has_bytes(stream, 4))
        return MSAV_ERR_CORRUPT;
    amount = mfmtio_read_i32(stream);
    if(stream->eos || amount < 0 ||
       (uint32_t)amount > (stream->bufsz - stream->idx) / 6)
        return MSAV_ERR_CORRUPT;

    MSAV__INFO("custom: chunks=%d", amount);
    for(int32_t i = 0; i < amount; i++)
    {
        char *name = NULL;
        msav_err_t rc = MSAV_ERR_OK;
        name = mfmtio_read_utf(stream);
        if(stream->eos || !name)
        {
            mfmtio_free(name);
            return stream->oom ? MSAV_ERR_NO_MEM : MSAV_ERR_CORRUPT;
        }

        MSAV__DEBUG("custom: name=\"%s\"", name);
        rc = reader->chunk_read(stream, reader, name, msav__skip_payload);
        mfmtio_free(name);
        if(rc != MSAV_ERR_OK)
            return rc;
    }
    MSAV__INFO("custom decoded: chunks=%d", amount);
    return MSAV_ERR_OK;
}

static msav_err_t msav__read_file(
        mfmtio_rstream_t *stream,
        msav__file_reader_t *reader)
{
    msav_err_t rc = MSAV_ERR_OK;

    rc = reader->chunk_read(
            stream, reader, 
            "metadata", 
            msav__read_meta);
    if(rc != MSAV_ERR_OK)
        return rc;

    if(reader->version >= 12)
    {
        rc = reader->chunk_read(
                stream, reader, 
                "patches", 
                msav__read_patches);
        if(rc != MSAV_ERR_OK)
            return rc;
    }

    rc = reader->chunk_read(
            stream, reader,
            "content mapping",
            msav__read_content);
    if(rc != MSAV_ERR_OK)
        return rc;

    rc = reader->chunk_read(
            stream, reader, 
            "map", 
            msav__read_map);
    if(rc != MSAV_ERR_OK)
        return rc;

    rc = reader->chunk_read(
            stream, reader,
            "entities",
            msav__read_entities);
    if(rc != MSAV_ERR_OK)
        return rc;

    if(reader->version >= 8)
    {
        rc = reader->chunk_read(
            stream, reader,
            "markers",
            msav__read_markers);
        if(rc != MSAV_ERR_OK)
            return rc;
    }

    rc = reader->chunk_read(
            stream, reader,
            "custom",
            msav__read_custom);
    if(rc != MSAV_ERR_OK)
        return rc;

    if(stream->idx != stream->bufsz)
    {
        MSAV__WARN("%u trailing bytes remain after reading",
                stream->bufsz - stream->idx);
    }
    return MSAV_ERR_OK;
}

msav_err_t msav_parse_from_memory(
        const uint8_t *buf,
        const uint32_t bufsz,
        msav_file_t *out)
{
    mfmtio_rstream_t stream = {0};
    msav__file_reader_t reader = {0};
    uint8_t *decompressed_buf = NULL;
    int64_t decompressed_bufsz = 0;
    int32_t zlib_rc = Z_OK;
    int use_decompressed = 0;
    msav_err_t rc = MSAV_ERR_OK;
    const msav__version_readers_t *version_readers = NULL;

    if(!buf || bufsz == 0 || !out)
        return MSAV_ERR_PARAM;

    zlib_rc = msav__decompress(
            buf, 
            (int64_t)bufsz, 
            &decompressed_buf, 
            &decompressed_bufsz);

    if(zlib_rc == Z_OK && decompressed_buf && decompressed_bufsz > 0 &&
       (uint64_t)decompressed_bufsz <= UINT32_MAX)
    {
        use_decompressed = 1;
        MSAV__INFO("input decompressed: %u bytes -> %lld bytes",
                bufsz, (long long)decompressed_bufsz);
    }
    else if(zlib_rc == Z_DATA_ERROR)
    {
        msav__free(decompressed_buf);
        decompressed_buf = NULL;
        decompressed_bufsz = 0;
        MSAV__INFO("input is not a zlib stream; parsing as raw save data");
    }
    else
    {
        MSAV__ERROR("decompression failed: zlib result=%d", zlib_rc);
        msav__free(decompressed_buf);
        return MSAV_ERR_COMPRESS;
    }

    stream.buf = use_decompressed ? decompressed_buf : buf;
    stream.bufsz = use_decompressed ? (uint32_t)decompressed_bufsz : bufsz;

    if(!msav__has_bytes(&stream, (uint32_t)MSAV__ARRSZ(msav__header)))
    {
        MSAV__ERROR("save header is truncated");
        rc = MSAV_ERR_FORMAT;
        goto cleanup;
    }
    for(uint32_t i = 0; i < (uint32_t)MSAV__ARRSZ(msav__header); i++)
    {
        if((uint8_t)mfmtio_read_i8(&stream) != msav__header[i])
        {
            MSAV__ERROR("invalid save header at byte %u", i);
            rc = MSAV_ERR_FORMAT;
            goto cleanup;
        }
    }

    if(!msav__has_bytes(&stream, 4))
    {
        MSAV__ERROR("save version is missing");
        rc = MSAV_ERR_FORMAT;
        goto cleanup;
    }

    reader.stream = &stream;
    reader.out = out;
    reader.version = mfmtio_read_i32(&stream);
    if(stream.eos || reader.version <= 0)
    {
        MSAV__ERROR("invalid save version %d", reader.version);
        rc = MSAV_ERR_FORMAT;
        goto cleanup;
    }
    version_readers = msav__get_version_readers(reader.version);
    if(!version_readers)
    {
        MSAV__ERROR("unsupported save version %d", reader.version);
        rc = MSAV_ERR_FORMAT;
        goto cleanup;
    }

    reader.chunk_read = version_readers->chunk_read;
    reader.entity_chunk_read = version_readers->entity_chunk_read;
    out->version = reader.version;

    MSAV__INFO("save header valid: version=%d", reader.version);
    MSAV__INFO("chunk readers selected: generic=i32, entity=%s",
            version_readers->entity_chunk_format);

    rc = msav__read_file(&stream, &reader);
    if(rc == MSAV_ERR_OK)
        MSAV__INFO("save decoded successfully: "
                "version=%d consumed=%u/%u bytes",
                reader.version, stream.idx, stream.bufsz);

cleanup:
    if(use_decompressed)
        msav__free(decompressed_buf);
    return rc;
}

msav_err_t msav_parse(
        const char *path, 
        msav_file_t *out)
{
    FILE *file = NULL;
    long file_size = 0;
    uint8_t *file_data = NULL;
    size_t read_size = 0;
    msav_err_t rc = MSAV_ERR_OK;

    if(!path || !out)
        return MSAV_ERR_PARAM;

    file = fopen(path, "rb");
    if(!file)
    {
        MSAV__ERROR("could not open save file '%s'", path);
        return MSAV_ERR_IO;
    }

    if(fseek(file, 0, SEEK_END) != 0 || 
            (file_size = ftell(file)) < 0 ||
            fseek(file, 0, SEEK_SET) != 0)
    {
        MSAV__ERROR("could not determine size of save file '%s'", path);
        fclose(file);
        return MSAV_ERR_IO;
    }
    if((uint64_t)file_size > UINT32_MAX)
    {
        MSAV__ERROR("save file '%s' exceeds supported size", path);
        fclose(file);
        return MSAV_ERR_CORRUPT;
    }

    file_data = msav__malloc(file_size ? (size_t)file_size : 1);
    if(!file_data)
    {
        MSAV__ERROR("could not allocate %ld bytes for '%s'", file_size, path);
        fclose(file);
        return MSAV_ERR_NO_MEM;
    }

    read_size = fread(file_data, 1, (size_t)file_size, file);
    if(read_size != (size_t)file_size || ferror(file))
    {
        MSAV__ERROR("could not read save file '%s' completely", path);
        msav__free(file_data);
        fclose(file);
        return MSAV_ERR_IO;
    }
    fclose(file);

    MSAV__INFO("opened save file '%s': %zu bytes", path, read_size);
    rc = msav_parse_from_memory(file_data, (uint32_t)read_size, out);
    msav__free(file_data);
    return rc;
}


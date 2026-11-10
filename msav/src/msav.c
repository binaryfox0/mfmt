#include "msav.h"

#include <stdio.h>
#include <stdlib.h>

#include <mfmtio.h>
#include <zlib-ng.h>

#include "msav_zlib.h"
#include "mfmtio_alloc_priv.h"
#include "msav_log_priv.h"
#include "msav_blocks_gen.h"

#define MSAV__ARRSZ(array) (sizeof(array) / sizeof((array)[0]))

static const uint8_t msav__header[] = {'M', 'S', 'A', 'V'};

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
};

static msav_err_t msav__read_chunk_common(
        mfmtio_rstream_t *stream,
        msav__file_reader_t *reader,
        const char *name,
        msav__payload_read_fn payload_read,
        int legacy_length);
static msav_err_t msav__read_map(
        mfmtio_rstream_t *stream,
        msav__file_reader_t *reader);
static msav_err_t msav__skip_payload(
    mfmtio_rstream_t *stream,
    msav__file_reader_t *reader);

static int msav__has_bytes(const mfmtio_rstream_t *stream, uint32_t count)
{
    return stream && !stream->eos && stream->idx <= stream->bufsz &&
           count <= stream->bufsz - stream->idx;
}

static msav_err_t msav__read_chunk_modern(
        mfmtio_rstream_t *stream,
        msav__file_reader_t *reader,
        const char *name,
        msav__payload_read_fn payload_read)
{
    return msav__read_chunk_common(stream, reader, name, payload_read, 0);
}

static msav_err_t msav__read_chunk_legacy(
        mfmtio_rstream_t *stream,
        msav__file_reader_t *reader,
        const char *name,
        msav__payload_read_fn payload_read)
{
    return msav__read_chunk_common(stream, reader, name, payload_read, 1);
}

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

static const msav__version_readers_t *msav__get_version_readers(int32_t version)
{
    for(size_t i = 0; i < MSAV__ARRSZ(msav__version_readers); i++)
    {
        if(msav__version_readers[i].version == version)
            return &msav__version_readers[i];
    }
    return NULL;
}

static msav_err_t msav__read_chunk_common(
        mfmtio_rstream_t *stream,
        msav__file_reader_t *reader,
        const char *name,
        msav__payload_read_fn payload_read,
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

    if(!stream || !reader || !name || !payload_read)
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
        MSAV__ERROR("chunk '%s': length %u exceeds remaining input at offset 0x%08X",
                name, length, stream->idx);
        return MSAV_ERR_CORRUPT;
    }

    payload_start = stream->idx;
    payload_end = payload_start + length;
    is_skip = payload_read == msav__skip_payload;
    if(is_skip && legacy_length)
        MSAV__DEBUG("chunk '%s': format=%s offset=0x%08X length=%u",
                name, format, payload_start - prefix_size, length);
    else
        MSAV__INFO("chunk '%s' begin: format=%s offset=0x%08X length=%u",
                name, format, payload_start - prefix_size, length);

    rc = payload_read(stream, reader);
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

static msav_err_t msav__skip_region(
        mfmtio_rstream_t *stream,
        const char *name)
{
    int32_t signed_length = 0;
    uint32_t start = 0;
    uint32_t length = 0;

    if(!msav__has_bytes(stream, 4))
    {
        MSAV__ERROR("region '%s': missing length at offset 0x%08X", name, stream->idx);
        return MSAV_ERR_CORRUPT;
    }

    start = stream->idx;
    signed_length = mfmtio_read_i32(stream);
    if(stream->eos || signed_length < 0)
    {
        MSAV__ERROR("region '%s': invalid length %d at offset 0x%08X",
                name, signed_length, start);
        return MSAV_ERR_CORRUPT;
    }

    length = (uint32_t)signed_length;
    if(!msav__has_bytes(stream, length))
    {
        MSAV__ERROR("region '%s': length %u exceeds remaining input at offset 0x%08X",
                name, length, stream->idx);
        return MSAV_ERR_CORRUPT;
    }

    stream->idx += length;
    MSAV__INFO("region '%s' skipped: offset=0x%08X length=%u",
            name, start, length);
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
        MSAV__ERROR("map: dimensions %u x %u exceed allocation limits", width, height);
        return MSAV_ERR_CORRUPT;
    }

    floors = mfmtio__calloc(tile_count, sizeof(*floors));
    ores = mfmtio__calloc(tile_count, sizeof(*ores));
    blocks = mfmtio__calloc(tile_count, sizeof(*blocks));
    if(!floors || !ores || !blocks)
    {
        MSAV__ERROR("map: unable to allocate %u tiles", tile_count);
        rc = MSAV_ERR_NO_MEM;
        goto cleanup;
    }

    MSAV__INFO("map begin: width=%u height=%u tiles=%u", width, height, tile_count);

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
                rc = reader->entity_chunk_read(stream, reader, chunk_name, msav__skip_payload);
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

    MSAV__INFO("map decoded: width=%u height=%u tiles=%u", width, height, tile_count);
    msav__dump_map(floors, width, height, "floors_dump.txt");
    msav__dump_map(ores, width, height, "ores_dump.txt");
    msav__dump_map(blocks, width, height, "blocks_dump.txt");
    goto cleanup;

malformed_map:
    MSAV__ERROR("map: malformed tile data at offset 0x%08X", stream->idx);

cleanup:
    mfmtio__free(floors);
    mfmtio__free(ores);
    mfmtio__free(blocks);
    return rc;
}

static msav_err_t msav__read_file(
        mfmtio_rstream_t *stream,
        msav__file_reader_t *reader)
{
    msav_err_t rc = MSAV_ERR_OK;

    rc = msav__skip_region(stream, "metadata");
    if(rc != MSAV_ERR_OK)
        return rc;
    rc = msav__skip_region(stream, "content mapping");
    if(rc != MSAV_ERR_OK)
        return rc;

    if(reader->version >= 11)
    {
        rc = reader->chunk_read(stream, reader, "patches", msav__skip_payload);
        if(rc != MSAV_ERR_OK)
            return rc;
    }

    rc = reader->chunk_read(stream, reader, "map", msav__read_map);
    if(rc != MSAV_ERR_OK)
        return rc;

    if(stream->idx < stream->bufsz)
        MSAV__INFO("%u trailing bytes remain after the map region",
                stream->bufsz - stream->idx);
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

    if(!buf || bufsz == 0)
    {
        MSAV__ERROR("parse: input buffer is empty");
        return MSAV_ERR_PARAM;
    }

    zlib_rc = msav__decompress(buf, (int64_t)bufsz, &decompressed_buf, &decompressed_bufsz);
    if(zlib_rc == Z_OK && decompressed_buf && decompressed_bufsz > 0 &&
       (uint64_t)decompressed_bufsz <= UINT32_MAX)
    {
        use_decompressed = 1;
        MSAV__INFO("input decompressed: %u bytes -> %lld bytes",
                bufsz, (long long)decompressed_bufsz);
    }
    else if(zlib_rc == Z_DATA_ERROR)
    {
        mfmtio__free(decompressed_buf);
        decompressed_buf = NULL;
        decompressed_bufsz = 0;
        MSAV__INFO("input is not a zlib stream; parsing as raw save data");
    }
    else
    {
        MSAV__ERROR("decompression failed: zlib result=%d", zlib_rc);
        mfmtio__free(decompressed_buf);
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
    if(out)
        out->version = reader.version;

    MSAV__INFO("save header valid: version=%d", reader.version);
    MSAV__INFO("chunk readers selected: generic=i32, entity=%s",
            version_readers->entity_chunk_format);

    rc = msav__read_file(&stream, &reader);
    if(rc == MSAV_ERR_OK)
        MSAV__INFO("save decoded successfully: version=%d consumed=%u/%u bytes",
                reader.version, stream.idx, stream.bufsz);

cleanup:
    if(use_decompressed)
        mfmtio__free(decompressed_buf);
    return rc;
}

msav_err_t msav_parse(const char *file_name, msav_file_t *out)
{
    FILE *file = NULL;
    long file_size = 0;
    uint8_t *file_data = NULL;
    size_t read_size = 0;
    msav_err_t rc = MSAV_ERR_OK;

    if(!file_name)
    {
        MSAV__ERROR("parse: file name is null");
        return MSAV_ERR_PARAM;
    }

    file = fopen(file_name, "rb");
    if(!file)
    {
        MSAV__ERROR("could not open save file '%s'", file_name);
        return MSAV_ERR_IO;
    }

    if(fseek(file, 0, SEEK_END) != 0 || (file_size = ftell(file)) < 0 ||
       fseek(file, 0, SEEK_SET) != 0)
    {
        MSAV__ERROR("could not determine size of save file '%s'", file_name);
        fclose(file);
        return MSAV_ERR_IO;
    }
    if((uint64_t)file_size > UINT32_MAX)
    {
        MSAV__ERROR("save file '%s' exceeds supported size", file_name);
        fclose(file);
        return MSAV_ERR_CORRUPT;
    }

    file_data = mfmtio__malloc(file_size ? (size_t)file_size : 1);
    if(!file_data)
    {
        MSAV__ERROR("could not allocate %ld bytes for '%s'", file_size, file_name);
        fclose(file);
        return MSAV_ERR_NO_MEM;
    }

    read_size = fread(file_data, 1, (size_t)file_size, file);
    if(read_size != (size_t)file_size || ferror(file))
    {
        MSAV__ERROR("could not read save file '%s' completely", file_name);
        mfmtio__free(file_data);
        fclose(file);
        return MSAV_ERR_IO;
    }
    fclose(file);

    MSAV__INFO("opened save file '%s': %zu bytes", file_name, read_size);
    rc = msav_parse_from_memory(file_data, (uint32_t)read_size, out);
    mfmtio__free(file_data);
    return rc;
}


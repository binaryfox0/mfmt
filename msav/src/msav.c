#include "msav.h"

#include <stdio.h>
#include <stdlib.h>

#include <aparse.h>

#include "msav_stream.h"
#include "msav_object.h"

#define MSAV__ERROR aparse_prog_error
#define MSAV__WARN aparse_prog_warn
#define MSAV__INFO aparse_prog_info
#define MSAV__MARKER() MSAV__INFO("%s has started", __FUNCTION__)
#define MSAV__ARRSZ(arr) (sizeof((arr)) / sizeof((arr)[0]))

void mfmtio_read_chunk(msav__rstream_t *stream, void *ptr, void (*func)(msav__rstream_t*, void*))
{
    int len = 0;
    uint32_t before = 0;
    long after = 0;
    long read_len = 0;

    MSAV__MARKER();

    len = mfmtio_read_i32(stream);
    before = stream->idx;

    MSAV__INFO("offset = 0x%08X, len = 0x%08X bytes", (int)before - 4, len);
    func(stream, ptr);
    after = stream->idx;
    read_len = after - before;
    if(read_len != (long)len)
        MSAV__WARN("chunk length mismatch: %ld bytes, expected %d bytes", read_len, len);
    stream->eos = 0;
    stream->idx = before + len; // force seek
}

void msav__skip_region(msav__rstream_t *stream)
{
    int32_t len = 0;

    MSAV__MARKER();
    len = mfmtio_read_i32(stream);
    MSAV__INFO("offset = 0x%08X, len = 0x%08X bytes", (int)stream->idx - 4, len);
    stream->idx += (uint32_t)len;
}

void msav__dummy(msav__rstream_t *stream, void *ptr) {}
void mfmtio_read_meta(msav__rstream_t *stream, void *ptr)
{
    int16_t size = 0;

    MSAV__MARKER();

    size = mfmtio_read_i16(stream);
    for(int16_t i = 0; i < size; i++)
    {
        char* key = mfmtio_read_utf(stream);
        char* val = mfmtio_read_utf(stream);
        MSAV__INFO("key = \"%s\", val = \"%s\"", key, val);
        free(key);
        free(val);
    }
}

void mfmtio_read_content(msav__rstream_t *stream, void *ptr)
{
    int8_t mapped = 0;

    MSAV__MARKER();

    mapped = mfmtio_read_i8(stream);
    MSAV__INFO("mapped = %d", mapped);
    for(int i = 0; i < mapped; i++)
    {
        char content_type = mfmtio_read_i8(stream);
        int16_t total = mfmtio_read_i16(stream);
        
        MSAV__INFO("content_type = %d", content_type);
        MSAV__INFO("total = %d", total);
        for(int16_t j = 0; j < total; j++)
        {
            char* name = mfmtio_read_utf(stream);
            MSAV__INFO("name = \"%s\"", name);
            free(name);
        }
    }
}

static struct {
} msav__modules_state;
static inline uint8_t msav__construct_module_bitmask(void)
{
}

void mfmtio_read_building(msav__rstream_t *stream, void *ptr)
{
    float health = 0;
    int8_t rot = 0;
    msav__team_t team_id = 0;
    uint8_t module_bits = 0;

    health = mfmtio_read_f32(stream);
    rot = mfmtio_read_i8(stream);
    team_id = mfmtio_read_i8(stream);

    if(rot & 0x80)
    {
        int8_t version = 0;
        version = mfmtio_read_i8(stream);
        if(version >= 1)
        {
            int8_t on = 0;
            on = mfmtio_read_i8(stream);
        }

        if(version >= 2)
            module_bits = mfmtio_read_u8(stream);
    }

    if(module_bits & 0x01)
        ;
}

void msav__dump_map(
        const int16_t *map, 
        const int width, 
        const int height,
        const char *file_name
)
{
    FILE* file = 0;

    if(!map || width <= 0 || height <= 0)
        return;

    file = fopen(file_name, "w");
    if(!file) {
        MSAV__ERROR("unable to create map dump");
        return;
    }

    for(int y = 0; y < height; y++)
    {
        for(int x = 0; x < width; x++)
        {
            fprintf(file, "%04X", *map);
            if(x < width - 1)
                fputc(' ', file);
            map++;
        }
        fputc('\n', file);
    }

    fclose(file);
}
void mfmtio_read_map(msav__rstream_t *stream, void *ptr)
{
    uint16_t width = 0, height = 0;
    uint32_t tile_count = 0;
    int16_t *floors = 0, *ores = 0, *blocks = 0;

    MSAV__MARKER();

    width = mfmtio_read_i16(stream);
    height = mfmtio_read_i16(stream);
    tile_count = width * height;

    floors = calloc(tile_count, sizeof(*floors));
    ores = calloc(tile_count, sizeof(*ores));
    blocks = calloc(tile_count, sizeof(*blocks));

    if(!floors || !ores)
    {
        free(floors);
        free(ores);
        free(blocks);
        MSAV__ERROR("out of memory");
        return;
    }

    MSAV__INFO("width = %d, height = %d", width, height);

    for(uint32_t i = 0; i < tile_count; i++)
    {
        // mfmtio_read floors and create tiles first
        int16_t floorid = 0;
        int16_t oreid = 0;
        uint8_t consecutives = 0;
        
        floorid = mfmtio_read_i16(stream);
        oreid = mfmtio_read_i16(stream);
        consecutives = mfmtio_read_u8(stream);

        floors[i] = floorid;
        ores[i] = oreid;
        for(int j = i + 1; j < i + 1 + consecutives && j < tile_count; j++) 
        {
            floors[j] = floorid;
            ores[j] = oreid;
        }
        i += consecutives;
    }

    for(int i = 0; i < tile_count; i++)
    {
        uint16_t block = 0;
        char packed_chk = 0;
        int had_entity = 0, had_data = 0;
        char data = 0, floor_data = 0, overlay_data = 0;
        int extra_data = 0;
        int is_center = 1;

        block = mfmtio_read_i16(stream);
        packed_chk = mfmtio_read_i8(stream);
        had_entity = (packed_chk & 0x01) != 0;
        had_data =   (packed_chk & 0x04) != 0;

        blocks[i] = block;

        if(had_data)
        {
            data = mfmtio_read_i8(stream);
            floor_data = mfmtio_read_i8(stream);
            overlay_data = mfmtio_read_i8(stream);
            extra_data = mfmtio_read_i32(stream);
        }

        if(had_entity)
        {
            MSAV__INFO("offset: 0x%08X", stream->idx);
            is_center = mfmtio_read_bool(stream);
        }

        if(had_entity)
        {
            if(is_center)
            {
                msav__skip_region(stream);     
            }
        } else if(!had_data)
        {
            uint8_t consecutives = mfmtio_read_u8(stream);

            for(int j = i + 1; j < i + 1 + consecutives && j < tile_count; j++) 
                blocks[j] = block;
            i += consecutives;
        }
    }

    msav__dump_map(floors, width, height, "floors_dump.txt");
    msav__dump_map(ores, width, height, "ores_dump.txt");
    msav__dump_map(blocks, width, height, "blocks_dump.txt");

    free(floors);
    free(ores);
    free(blocks);
}

/* readWorldEntities */
void mfmtio_read_world_entities(msav__rstream_t *stream, void *ptr)
{
    int32_t amount = 0;

    MSAV__MARKER();
    amount = mfmtio_read_i32(stream);
    MSAV__INFO("amount: %d", amount);

    for(int32_t i = 0; i < amount; i++)
    {
        mfmtio_read_chunk(stream, 0, msav__dummy);
    }
}

void mfmtio_read_entities(msav__rstream_t *stream, void *ptr)
{
    int16_t amount = 0;
    int32_t team_count = 0;

    MSAV__MARKER();

    /* readEntityMapping */
    amount = mfmtio_read_i16(stream);
    MSAV__INFO("amount: %d", amount);
    for(int16_t i = 0; i < amount; i++)
    {
        int16_t id = mfmtio_read_i16(stream);
        char *name = mfmtio_read_utf(stream);
        MSAV__INFO("id=0x%04X, name=\"%s\"", id, name);
        free(name);
    }

    /* readTeamBlocks */
    team_count = mfmtio_read_i32(stream);
    MSAV__INFO("team count: %d", team_count);
    for(int32_t i = 0; i < team_count; i++)
    {
        int32_t team_id = mfmtio_read_i32(stream);
        int32_t blocks = mfmtio_read_i32(stream);
        MSAV__INFO("blocks: %d", blocks);
        for(int32_t j = 0; j < blocks; j++)
        {
            int16_t x = 0;
            int16_t y = 0;
            int16_t rot = 0;
            int16_t bid = 0;
            msav__obj_t obj = {0};

            x = mfmtio_read_i16(stream);
            y = mfmtio_read_i16(stream);
            rot = mfmtio_read_i16(stream);
            bid = mfmtio_read_i16(stream);
            MSAV__INFO("offset: 0x%08X", stream->idx);
            msav__obj_read(stream, &obj);
            MSAV__INFO("block: x=%d, y=%d, rot=%d, bid=%d, obj=%d", x, y, rot, bid, obj.type);
            msav__obj_destroy(&obj);
        }
    }

    MSAV__INFO("offset: 0x%08X", stream->idx);

    mfmtio_read_world_entities(stream, ptr);
}


void mfmtio_read_patches(msav__rstream_t *stream, void *ptr)
{
    uint8_t amount = 0;

    MSAV__MARKER();

    amount = mfmtio_read_i8(stream);
    for(int32_t i = 0; i < amount; i++)
    {
        int32_t len = 0;
        int8_t *bytes = 0;

        len = mfmtio_read_i32(stream);
        bytes = malloc((len + 1) * sizeof(int8_t));
        if(!bytes)
            break;
        mfmtio_read_fully(stream, bytes, len);
        bytes[len] = '\0';
        free(bytes);
    }
}
    
void mfmtio_read_markers(msav__rstream_t *stream, void *ptr)
{
    MSAV__MARKER();
}

void mfmtio_read_custom(msav__rstream_t *stream, void *ptr)
{
    int32_t amount = 0;

    MSAV__MARKER();

    amount = mfmtio_read_i32(stream);
    for(int32_t i = 0; i < amount; i++)
    {
        char *name = 0;
        name = mfmtio_read_utf(stream);
        MSAV__INFO("name: \"%s\"", name);
        free(name);
    }
}

msav_err_t msav_parse(
        const char *file_name,
        msav_file_t *out
)
{
    FILE *file = 0;
    long file_size = 0;
    uint8_t *file_data = 0;
    msav_err_t rc = MSAV_ERR_OK;

    if(!file_name)
        return MSAV_ERR_INVALID_ARG;

    file = fopen(file_name, "rb");
    if(!file)
        return MSAV_ERR_IO;
    
    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    file_data = malloc(file_size);
    if(!file_data)
    {
        fclose(file);
        return MSAV_ERR_OUT_OF_MEM;
    }
    fread(file_data, 1, (size_t)file_size, file);
    fclose(file);

    rc = msav_parse_from_memory(file_data, (uint32_t)file_size, out);
    free(file_data);

    return rc;
}

const uint8_t msav__header[4] = { 'M', 'S', 'A', 'V' };
msav_err_t msav_parse_from_memory(
        const uint8_t *buf, 
        const uint32_t bufsz,
        msav_file_t *out
)
{
    msav__rstream_t stream = {0};
    msav_file_t tmp = {0};
    if(!buf || bufsz == 0)
        return MSAV_ERR_INVALID_ARG;

    stream = (msav__rstream_t){.buf = buf, .bufsz = bufsz};

    for(int i = 0; i < (int)MSAV__ARRSZ(msav__header); i++)
    {
        if(mfmtio_read_i8(&stream) != msav__header[i])
            return MSAV_ERR_FORMAT;
    }

    tmp.version = mfmtio_read_i32(&stream);
    MSAV__INFO("version: %d", tmp.version);
   
    msav__skip_region(&stream);
    msav__skip_region(&stream);
    // mfmtio_read_chunk(&stream, 0, mfmtio_read_meta);
    // mfmtio_read_chunk(&stream, 0, mfmtio_read_content);
    if(tmp.version >= 11)
        mfmtio_read_chunk(&stream, 0, mfmtio_read_patches);
    
    mfmtio_read_chunk(&stream, 0, mfmtio_read_map);
    // mfmtio_read_chunk(&stream, 0, mfmtio_read_entities);
    // if(tmp.version >= 8)
    //     mfmtio_read_chunk(&stream, 0, mfmtio_read_markers);
    // mfmtio_read_chunk(&stream, 0, mfmtio_read_custom);

    return MSAV_ERR_OK;
}


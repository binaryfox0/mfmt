#ifndef MFMTIO_H
#define MFMTIO_H

#include <stdint.h>
#include <string.h>
#include <mfmtio_alloc.h>

typedef enum
{
    MFMTIO_ERR_OK = 0,
    MFMTIO_ERR_EOF,
    MFMTIO_ERR_NO_MEM
} mfmtio_err_t;

typedef struct mfmtio_rstream
{
    const uint8_t *buf;
    uint32_t idx;
    uint32_t bufsz;
    mfmtio_err_t err;
} mfmtio_rstream_t;

typedef struct mfmtio_wstream
{
    uint8_t *buf;
    uint32_t size;
    uint32_t cap;
    mfmtio_err_t err;
} mfmtio_wstream_t;

static inline int mfmtio_has_bytes(
        const mfmtio_rstream_t *stream,
        uint32_t count)
{
    return stream && stream->err == MFMTIO_ERR_OK && stream->idx <= stream->bufsz &&
           count <= stream->bufsz - stream->idx;
}

static inline int8_t mfmtio_read_i8(mfmtio_rstream_t *stream)
{
    uint8_t v = 0;
    if(!mfmtio_has_bytes(stream, 1))
    {
        if(stream && stream->err == MFMTIO_ERR_OK)
            stream->err = MFMTIO_ERR_EOF;
        return 0;
    }
    v = stream->buf[stream->idx++];
    return (int8_t)v;
}

static inline uint8_t mfmtio_read_u8(mfmtio_rstream_t *stream)
{
    uint8_t v = 0;
    if(!mfmtio_has_bytes(stream, 1))
    {
        if(stream && stream->err == MFMTIO_ERR_OK)
            stream->err = MFMTIO_ERR_EOF;
        return 0;
    }
    v = stream->buf[stream->idx++];
    return v;
}

static inline int mfmtio_read_bool(mfmtio_rstream_t *stream)
{
    if(!mfmtio_has_bytes(stream, 1))
    {
        if(stream && stream->err == MFMTIO_ERR_OK)
            stream->err = MFMTIO_ERR_EOF;
        return 0;
    }
    return (int)stream->buf[stream->idx++] != 0;
}

static inline int16_t mfmtio_read_i16(mfmtio_rstream_t *stream)
{
    uint16_t v = 0;
    if(!mfmtio_has_bytes(stream, 2))
    {
        if(stream && stream->err == MFMTIO_ERR_OK)
            stream->err = MFMTIO_ERR_EOF;
        return 0;
    }
    v = ((uint16_t)stream->buf[stream->idx++] << 8) |
         (uint16_t)stream->buf[stream->idx++];
    return (int16_t)v;
}

static inline uint16_t mfmtio_read_u16(mfmtio_rstream_t *stream)
{
    uint16_t v = 0;
    if(!mfmtio_has_bytes(stream, 2))
    {
        if(stream && stream->err == MFMTIO_ERR_OK)
            stream->err = MFMTIO_ERR_EOF;
        return 0;
    }
    v = ((uint16_t)stream->buf[stream->idx++] << 8) |
         (uint16_t)stream->buf[stream->idx++];
    return v;
}

static inline int32_t mfmtio_read_i32(mfmtio_rstream_t *stream)
{
    uint32_t v = 0;
    if(!mfmtio_has_bytes(stream, 4))
    {
        if(stream && stream->err == MFMTIO_ERR_OK)
            stream->err = MFMTIO_ERR_EOF;
        return 0;
    }
    v = ((uint32_t)stream->buf[stream->idx++] << 24) | 
        ((uint32_t)stream->buf[stream->idx++] << 16) | 
        ((uint32_t)stream->buf[stream->idx++] << 8)  | 
         (uint32_t)stream->buf[stream->idx++];
    return (int32_t)v;
}

static inline float mfmtio_read_f32(mfmtio_rstream_t *stream)
{
    uint32_t v = 0;
    float f = 0;
    if(!mfmtio_has_bytes(stream, 4))
    {
        if(stream && stream->err == MFMTIO_ERR_OK)
            stream->err = MFMTIO_ERR_EOF;
        return 0;
    }
    v = ((uint32_t)stream->buf[stream->idx++] << 24) | 
        ((uint32_t)stream->buf[stream->idx++] << 16) | 
        ((uint32_t)stream->buf[stream->idx++] << 8)  | 
         (uint32_t)stream->buf[stream->idx++];

    memcpy(&f, &v, sizeof(f));
    return f;
}

static inline double mfmtio_read_f64(mfmtio_rstream_t *stream)
{
    uint64_t v = 0;
    double d = 0;
    if(!mfmtio_has_bytes(stream, 8))
    {
        if(stream && stream->err == MFMTIO_ERR_OK)
            stream->err = MFMTIO_ERR_EOF;
        return 0;
    }
    v = ((uint64_t)stream->buf[stream->idx++] << 56) | 
        ((uint64_t)stream->buf[stream->idx++] << 48) | 
        ((uint64_t)stream->buf[stream->idx++] << 40) | 
        ((uint64_t)stream->buf[stream->idx++] << 32) | 
        ((uint64_t)stream->buf[stream->idx++] << 24) | 
        ((uint64_t)stream->buf[stream->idx++] << 16) | 
        ((uint64_t)stream->buf[stream->idx++] << 8)  | 
         (uint64_t)stream->buf[stream->idx++];

    memcpy(&d, &v, sizeof(d));
    return d;
}

static inline void mfmtio_read_fully(
        mfmtio_rstream_t *stream,
        void *buf,
        const uint32_t bufsz
)
{
    if(!mfmtio_has_bytes(stream, bufsz))
    {
        if(stream && stream->err == MFMTIO_ERR_OK)
            stream->err = MFMTIO_ERR_EOF;
        return;
    }
    if(bufsz)
        memcpy(buf, &stream->buf[stream->idx], bufsz);
    stream->idx += bufsz;
}

char* mfmtio_read_utf(mfmtio_rstream_t *stream);

int mfmtio_wensure(mfmtio_wstream_t *stream, const uint32_t extra);
uint8_t *mfmtio_wstream_strip(mfmtio_wstream_t *stream);

static inline void mfmtio_write_i8(mfmtio_wstream_t *stream, const int8_t v)
{
    if(!mfmtio_wensure(stream, 1)) return;
    stream->buf[stream->size++] = (uint8_t)v;
}

static inline void mfmtio_write_bool(mfmtio_wstream_t *stream, const int v)
{
    if(!mfmtio_wensure(stream, 1)) return;
    stream->buf[stream->size++] = (uint8_t)(v ? 1 : 0);
}

static inline void mfmtio_write_i16(mfmtio_wstream_t *stream, const int16_t v)
{
    if(!mfmtio_wensure(stream, 2)) return;
    stream->buf[stream->size++] = (uint8_t)((v >> 8) & 0xFF);
    stream->buf[stream->size++] = (uint8_t)(v & 0xFF);
}

static inline void mfmtio_write_i32(mfmtio_wstream_t *stream, const int32_t v)
{
    if(!mfmtio_wensure(stream, 4)) return;
    stream->buf[stream->size++] = (uint8_t)((v >> 24) & 0xFF);
    stream->buf[stream->size++] = (uint8_t)((v >> 16) & 0xFF);
    stream->buf[stream->size++] = (uint8_t)((v >> 8) & 0xFF);
    stream->buf[stream->size++] = (uint8_t)(v & 0xFF);
}

int mfmtio_write_utf(mfmtio_wstream_t *stream, const char *str);
#endif

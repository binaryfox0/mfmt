#ifndef MSAV_STREAM_H
#define MSAV_STREAM_H

#include <stdint.h>

#include "msav.h"

typedef struct msav__wstream
{
    uint8_t *buf;
    uint32_t size;
    uint32_t cap;
    msav_err_t err; /* End of stream, Out of memory */
} msav__wstream_t;

int msav__wensure(msav__wstream_t *stream, const uint32_t extra);
uint8_t *msav__wstream_strip(msav__wstream_t *stream);

static inline void mfmtio_write_i8(msav__wstream_t *stream, const int8_t v)
{
    if(!msav__wensure(stream, 1)) return;
    stream->buf[stream->size++] = (uint8_t)v;
}

static inline void mfmtio_write_bool(msav__wstream_t *stream, const int v)
{
    if(!msav__wensure(stream, 1)) return;
    stream->buf[stream->size++] = (uint8_t)(v ? 1 : 0);
}

static inline void mfmtio_write_i16(msav__wstream_t *stream, const int16_t v)
{
    if(!msav__wensure(stream, 2)) return;
    stream->buf[stream->size++] = (uint8_t)((v >> 8) & 0xFF);
    stream->buf[stream->size++] = (uint8_t)(v & 0xFF);
}

static inline void mfmtio_write_i32(msav__wstream_t *stream, const int32_t v)
{
    if(!msav__wensure(stream, 4)) return;
    stream->buf[stream->size++] = (uint8_t)((v >> 24) & 0xFF);
    stream->buf[stream->size++] = (uint8_t)((v >> 16) & 0xFF);
    stream->buf[stream->size++] = (uint8_t)((v >> 8) & 0xFF);
    stream->buf[stream->size++] = (uint8_t)(v & 0xFF);
}
void mfmtio_write_utf(msav__wstream_t *stream, const char *str);
#endif

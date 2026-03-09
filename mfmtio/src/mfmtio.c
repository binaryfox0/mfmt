#include "mfmtio.h"

#include <stdlib.h>
#include <string.h>

char* mfmtio_read_utf(mfmtio_rstream_t *stream)
{
    int16_t len = 0;
    char *out = 0;

    len = mfmtio_read_i16(stream);
    if(len <= 0)
        return 0;

    if(stream->idx + (uint32_t)(len - 1) >= stream->bufsz
            || stream->eos)
    {
        stream->eos = 1;
        return 0;
    }
    out = malloc(len + 1);
    if(!out)
    {
        stream->oom = 1;
        return 0;
    }
    for(int16_t i = 0; i < len; i++)
        out[i] = mfmtio_read_i8(stream);
    out[len] = '\0';
    return out;
}

int mfmtio_wensure(mfmtio_wstream_t *stream, const uint32_t extra)
{
    uint32_t need = 0;

    need = (uint64_t)stream->size + extra;
    if(need <= stream->cap) 
        return 1;

    if(stream->oom)
        return 0;

    uint32_t new_cap = stream->cap ? stream->cap : 64;
    while(new_cap < need) {
        new_cap *= 2;
    }

    uint8_t *nbuf = realloc(stream->buf, new_cap);
    if(!nbuf){
        stream->oom = 1;
        return 0;
    }

    stream->buf = nbuf;
    stream->cap = new_cap;
    return 1;
}

int mfmtio_write_utf(mfmtio_wstream_t *stream, const char *str)
{
    uint32_t len = 0;
    if(!str){
        mfmtio_write_i16(stream, 0);
        return -1;
    }

    len = (uint32_t)strlen(str);
    if(len > 0x7FFF)
        return -1;

    if(!mfmtio_wensure(stream, 2 + len))
        return -1;

    mfmtio_write_i16(stream, (int16_t)len);
    memcpy(stream->buf + stream->size, str, len);
    stream->size += len;

    return 0;
}

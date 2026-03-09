#include "msav_stream.h"

#include <stdlib.h>
#include <string.h>

char* mfmtio_read_utf(msav__rstream_t *stream)
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

int msav__wensure(msav__wstream_t *s, const uint32_t extra)
{
    uint32_t need = 0;
    if(s->err) 
        return 0;

    need = (uint64_t)s->size + extra;
    if(need <= s->cap) 
        return 1;

    uint32_t new_cap = s->cap ? s->cap : 64;
    while(new_cap < need) {
        new_cap *= 2;
    }

    uint8_t *nbuf = realloc(s->buf, new_cap);
    if(!nbuf){
        s->err = MSAV_ERR_OUT_OF_MEM;
        return 0;
    }

    s->buf = nbuf;
    s->cap = new_cap;
    return 1;
}

void mfmtio_write_utf(msav__wstream_t *s, const char *str)
{
    uint32_t len = 0;
    if(s->err)
        return;

    if(!str){
        mfmtio_write_i16(s, 0);
        return;
    }

    len = (uint32_t)strlen(str);
    if(len > 0x7FFF){
        s->err = MSAV_ERR_CORRUPT;
        return;
    }

    if(!msav__wensure(s, 2 + len))
        return;

    mfmtio_write_i16(s, (int16_t)len);
    memcpy(s->buf + s->size, str, len);
    s->size += len;
}

#include "msav_stream.h"

#include <stdlib.h>
#include <string.h>

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

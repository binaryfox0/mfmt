#include "msav_zlib.h"

#include <stdlib.h>

#include <zlib-ng.h>

typedef struct
{
    uint8_t *data;
    int64_t size;
    int64_t capacity;
} dynbuf_t;

#define MSAV__BUF_DEFAULT_CAP 16384
int msav__buf_append(
        dynbuf_t *dynbuf, 
        const uint8_t* buf, 
        const int32_t bufsize
)
{
    if(dynbuf->size + bufsize > dynbuf->capacity)
    {
        uint8_t *new_data = 0;
        int64_t new_cap = 0;

        new_cap = dynbuf->capacity ? dynbuf->capacity * 2 : MSAV__BUF_DEFAULT_CAP;
        new_data = realloc(dynbuf->data, new_cap);
        if(!new_data)
            return -1;

        dynbuf->capacity = new_cap;
        dynbuf->data = new_data;
    }

    memcpy(&dynbuf->data[dynbuf->size], buf, bufsize);
    dynbuf->size += bufsize;
    return 0;
}

int32_t msav__decompress(
        const uint8_t *in_buf,
        const int64_t in_bufsiz,
        uint8_t **out_buf,
        int64_t *out_bufsize
)
{
    int32_t err = Z_OK;
    zng_stream stream = {0};
    dynbuf_t dynbuf = {0};
    uint8_t out_tmp[8192] = {0};

    stream.next_in = (uint8_t*)in_buf;
    stream.avail_in = in_bufsiz;

    err = zng_inflateInit(&stream);
    if(err != Z_OK)
        return err;

    for(;;)
    {
        stream.next_out = out_tmp;
        stream.avail_out = sizeof(out_tmp);

        err = zng_inflate(&stream, Z_NO_FLUSH);

        if(err != Z_OK && err != Z_STREAM_END)
        {
            zng_inflateEnd(&stream);
            free(dynbuf.data);
            return err;
        }

        int32_t produced = sizeof(out_tmp) - stream.avail_out;

        if(produced > 0)
        {
            if(msav__buf_append(&dynbuf, out_tmp, produced) != 0)
            {
                zng_inflateEnd(&stream);
                free(dynbuf.data);
                return -1;
            }
        }

        if(err == Z_STREAM_END)
            break;
    }

    zng_inflateEnd(&stream);

    *out_buf = dynbuf.data;
    *out_bufsize = dynbuf.size;
    return 0;
}


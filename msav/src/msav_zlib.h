#ifndef MSAV_ZLIB_H
#define MSAV_ZLIB_H

#include <stdint.h>

int32_t msav__decompress(
        const uint8_t *in_buf,
        const int64_t in_bufsiz,
        uint8_t **out_buf,
        int64_t *out_bufsize
);

#endif

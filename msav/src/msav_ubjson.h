#ifndef MSAV_UBJSON_PRIV_H
#define MSAV_UBJSON_PRIV_H

#include <stdint.h>

#include <mfmtio.h>

#include "msav.h"

msav_err_t msav__ubjson_count_entries(
        mfmtio_rstream_t *stream,
        int root_type,
        uint64_t *count);

#endif

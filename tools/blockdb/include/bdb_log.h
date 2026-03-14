#ifndef BLOCKDB_LOG_H
#define BLOCKDB_LOG_H

#include <aparse.h>

#define bdb__error aparse_prog_error
#define bdb__warn aparse_prog_warn
#define bdb__info aparse_prog_info

#define bdb__error_errno(msg, ...) \
    { \
        bdb__error(msg, ##__VA_ARGS__); \
        bdb__info("reason: %s", strerror(errno)); \
    }

#endif

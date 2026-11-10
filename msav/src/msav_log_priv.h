#ifndef MSAV_LOG_PRIV_H
#define MSAV_LOG_PRIV_H

#include <msav_log.h>

#if defined(__GNUC__) || defined(__clang__)
#define MSAV__PRINTF(fmt_index, arg_index) \
    __attribute__((format(printf, fmt_index, arg_index)))
#else
#define MSAV__PRINTF(fmt_index, arg_index)
#endif

MSAV__PRINTF(2, 3) void msav__log(
        msav_log_level_t level,
        const char *format,
        ...);

#define MSAV__DEBUG(...) msav__log(MSAV_LOG_DEBUG, __VA_ARGS__)
#define MSAV__INFO(...)  msav__log(MSAV_LOG_INFO, __VA_ARGS__)
#define MSAV__WARN(...)  msav__log(MSAV_LOG_WARN, __VA_ARGS__)
#define MSAV__ERROR(...) msav__log(MSAV_LOG_ERROR, __VA_ARGS__)

#endif

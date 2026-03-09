#ifndef MSAV_H
#define MSAV_H

#include <stdint.h>

/**
 * @enum msav_err_t
 * @brief Error codes returned by libmsav functions.
 */
typedef enum {
    MSAV_ERR_OK = 0,        /**< Operation completed successfully */
    MSAV_ERR_INVALID_ARG,   /**< Invalid argument passed to function */
    MSAV_ERR_IO,            /**< I/O failure during file operations */
    MSAV_ERR_ZLIB,
    MSAV_ERR_OUT_OF_MEM,    /**< Memory allocation failed */
    MSAV_ERR_FORMAT,        /**< Invalid or unsupported file format */
    MSAV_ERR_CORRUPT,       /**< Corrupted or inconsistent data */
    __MSAV_ERR_END__
} msav_err_t;

typedef struct
{
    int32_t version;
} msav_file_t;

msav_err_t msav_parse(
        const char *file_name,
        msav_file_t *out
);

msav_err_t msav_parse_from_memory(
        const uint8_t *buf, 
        const uint32_t bufsz,
        msav_file_t *out
);

#endif

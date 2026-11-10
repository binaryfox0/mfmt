#ifndef MSAV_LOG_H
#define MSAV_LOG_H

typedef enum
{
    MSAV_LOG_DEBUG,
    MSAV_LOG_INFO,
    MSAV_LOG_WARN,
    MSAV_LOG_ERROR,
    MSAV_LOG_LEVEL_COUNT
} msav_log_level_t;

typedef void (*msav_log_fn)(
        void *userdata,
        msav_log_level_t level,
        const char *message);

/* A NULL callback disables logging and restores the default error threshold. */
int msav_log_set(
        msav_log_fn callback,
        void *userdata,
        msav_log_level_t level);
void msav_log_set_func(msav_log_fn callback);
void msav_log_set_userdata(void *userdata);
int msav_log_set_level(msav_log_level_t level);

#endif

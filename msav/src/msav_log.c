#include <msav_log.h>

#include <stdarg.h>
#include <stdio.h>

#include "msav_log_priv.h"

#define MSAV__LOG_BUFFER_SIZE 4096

static msav_log_fn msav__log_callback = NULL;
static void *msav__log_userdata = NULL;
static msav_log_level_t msav__log_level = MSAV_LOG_ERROR;

static int msav__valid_log_level(msav_log_level_t level)
{
    return level >= MSAV_LOG_DEBUG && level < MSAV_LOG_LEVEL_COUNT;
}

int msav_log_set(
        msav_log_fn callback,
        void *userdata,
        msav_log_level_t level)
{
    if(!msav__valid_log_level(level))
        return -1;

    if(!callback)
    {
        msav__log_callback = NULL;
        msav__log_userdata = NULL;
        msav__log_level = MSAV_LOG_ERROR;
        return 0;
    }

    msav__log_callback = callback;
    msav__log_userdata = userdata;
    msav__log_level = level;
    return 0;
}

void msav_log_set_func(msav_log_fn callback)
{
    if(!callback)
    {
        msav__log_callback = NULL;
        msav__log_userdata = NULL;
        msav__log_level = MSAV_LOG_ERROR;
        return;
    }
    msav__log_callback = callback;
}

void msav_log_set_userdata(void *userdata)
{
    msav__log_userdata = userdata;
}

int msav_log_set_level(msav_log_level_t level)
{
    if(!msav__valid_log_level(level))
        return -1;
    msav__log_level = level;
    return 0;
}

void msav__log(msav_log_level_t level, const char *format, ...)
{
    char buffer[MSAV__LOG_BUFFER_SIZE] = {0};
    va_list args;

    if(!msav__log_callback || level < msav__log_level)
        return;

    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    msav__log_callback(msav__log_userdata, level, buffer);
}

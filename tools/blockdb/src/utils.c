#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "log.h"

int read_file(const char *path, char **out_buf, long *out_size)
{
    FILE *file = 0;
    long file_size = 0;

    file = fopen(path, "rb");
    if(!file)
    {
        error("failed to open file at \"%s\"", path);
        info("reason: \"%s\"", strerror(errno));
        return -1;
    }
    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    *out_buf = malloc(file_size);
    if(!*out_buf)
    {
        error("failed to allocate memory for file buffer");
        info("reason: \"%s\"", strerror(errno));
        return -1;
    }
    fread(*out_buf, 1, file_size, file);
    *out_size = file_size;
    fclose(file);

    return 0;
}


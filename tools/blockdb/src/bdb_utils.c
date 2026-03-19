#include "bdb_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "bdb_log.h"

int bdb__read_file(const char *path, char **out_buf, long *out_size)
{
    FILE *file = 0;
    long file_size = 0;

    file = fopen(path, "rb");
    if(!file)
    {
        bdb__error_errno("failed to open file at \"%s\"", path);
        return -1;
    }
    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    *out_buf = malloc(file_size);
    if(!*out_buf)
    {
        bdb__error_errno("failed to allocate memory for file buffer");
        return -1;
    }
    fread(*out_buf, 1, file_size, file);
    *out_size = file_size;
    fclose(file);

    return 0;
}


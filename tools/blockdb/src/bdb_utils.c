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

int bdb__tokcmp(const char *s1, const char *s2, const size_t l)
{
    size_t i = 0;

    if (!s1 || !s2)
        return -1;

    if (strlen(s2) != l)
        return -1;

    for (i = 0; i < l; i++)
    {
        if (s1[i] == '\0' || s1[i] != s2[i])
            return -1;
    }

    return 0;
}

char* bdb__tokdup(const char *s, const size_t l)
{
	size_t i = 0;
	size_t len = 0;
	char *out = 0;

	if (!s)
		return 0;

	while (len < l && s[len])
		len++;

	out = (char *)malloc(len + 1);
	if (!out)
		return 0;

	while (i < len)
	{
		out[i] = s[i];
		i++;
	}

	out[len] = '\0';
	return out;
}

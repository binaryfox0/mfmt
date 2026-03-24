#include "bdb_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
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

int bdb__confirm(
        const char *label, 
        const int default_ans
)
{
    int c = 0;
    int answer = 0;
    int seen = 0;

    for(;;)
    {
        /* prompt */
        if (default_ans)
            fprintf(stdout, "%s: " __aparse_info_label ": %s (Y/n): ", 
                    __aparse_progname, label);
        else
            fprintf(stdout, "%s: " __aparse_info_label ": %s (y/N): ", 
                    __aparse_progname, label);

        seen = 0;
        while ((c = getchar()) != '\n' && c != EOF)
        {
            if (isspace(c))
                continue;

            if (!seen)
            {
                seen = 1;

                if (c == 'y' || c == 'Y')
                    answer = 1;
                else if (c == 'n' || c == 'N')
                    answer = 0;
                else
                    answer = -1;
            }
        }

        if (c == EOF)
            return default_ans;

        if (!seen)
            return default_ans;

        if (answer == 0 || answer == 1)
            return answer;

        printf("Please answer y or n.\n");
    }
}

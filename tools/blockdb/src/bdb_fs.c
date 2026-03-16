#include <stdlib.h>
#include "bdb_fs.h"

#include <string.h>
#include <stddef.h>
#include <errno.h>

#include <dirent.h>
#include <sys/stat.h>

#include "bdb_log.h"

#ifdef _WIN32
#   define POSIX(x) _##x
#else
#   define POSIX(x) x
#endif

char *bdb__fs_cat_path(const char *a, const char *b)
{
    size_t len_a = 0;
    size_t len_b = 0;
    size_t need_sep = 0;
    size_t len = 0;
    char *out = 0;

    if(a)
        len_a = strlen(a);

    if(b)
        len_b = strlen(b);

    if(len_a && len_b && a[len_a - 1] != '/')
        need_sep = 1;

    len = len_a + need_sep + len_b;

    out = malloc(len + 1);
    if(!out)
        return 0;

    if(len_a)
        memcpy(out, a, len_a);

    if(need_sep)
        out[len_a] = '/';

    if(len_b)
        memcpy(out + len_a + need_sep, b, len_b);

    out[len] = '\0';

    return out;
}

int bdb__fs_is_file(const char *path)
{
    struct POSIX(stat) st = {0};
    int ret = 0;

    ret = POSIX(stat)(path, &st);
    if(ret < 0)
    {
        bdb__error_errno("failed to retrive info at \"%s\"", path);
        return 0;
    }

    return (st.st_mode & S_IFMT) == S_IFREG;
}

int bdb__fs_is_dir(const char *path)
{
    struct POSIX(stat) st = {0};
    int ret = 0;

    ret = POSIX(stat)(path, &st);
    if(ret < 0)
    {
        bdb__error_errno("failed to retrive info at \"%s\"", path);
        return 0;
    }

    return (st.st_mode & S_IFMT) == S_IFDIR;
}

int bdb__fs_collect_files(const char *path, bdb__arr_t *out)
{
    DIR *dir = 0;
    struct dirent *entry = 0;
    char *full = 0;
    struct stat st = {0};

    dir = opendir(path);
    if(!dir)
    {
        bdb__error_errno("failed to open directory at \"%s\"", path);
        return -1;
    }

    while((entry = readdir(dir)) != 0)
    {
        if(
                !strcmp(entry->d_name, ".") ||
                !strcmp(entry->d_name, "..")
          ) continue;

        full = bdb__fs_cat_path(path, entry->d_name);
        if(!full)
        {
            bdb__error_errno("out of memory while creating file path");
            continue;
        }

        stat(full, &st);
        if(S_ISDIR(st.st_mode))
        {
            bdb__fs_collect_files(full, out);
            free(full);
        }
        else
        {
            if(bdb__arr_append(out, &full) < 0)
            {
                bdb__error_errno("out of memory while appending file path into array");
                free(full);
                bdb__arr_destroy_strings(out);
            }
        }
    }
    
    closedir(dir);
    return 0;
}


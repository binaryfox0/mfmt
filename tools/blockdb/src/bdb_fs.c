#include <stdlib.h>
#include "bdb_fs.h"

#include <string.h>
#include <stddef.h>
#include <errno.h>

#include "bdb_log.h"
#include "bdb_utils.h"
#include "gen_utils.h"

#ifdef _WIN32
#   include <io.h>
#   define BDB__POSIX(x) _##x
#else
#   include <unistd.h>
#   include <dirent.h>
#   include <sys/stat.h>
#   define BDB__POSIX(x) x
#endif

char *bdb__fs_cat_path(
        const char *a, 
        const char *b
)
{
    return bdb__sprintf("%s%s%s", 
            a ? a : "", 
                a && 
                *a && 
                a[strlen(a) - 1] != BDB_PATH_SEP ? 
                    BDB__STRINGIFY(BDB_PATH_SEP) : "", 
            b ? b : ""
    );
}

char *bdb__fs_cat_fname(
        const char *path, 
        const char *stem, 
        const char *ext
)
{
    return bdb__sprintf("%s%s%s%s%s",
            path ? path : "",
                path &&
                *path &&
                path[strlen(path) - 1] != BDB_PATH_SEP ?
                    BDB__STRINGIFY(BDB_PATH_SEP) : "",
            stem ? stem : "",
            ext ? "." : "",
            ext ? ext : ""
    );
}

int bdb__fs_exist(const char *path)
{
    int ret = 0;
    if(!path)
        return -1;
    ret = BDB__POSIX(access)(path, F_OK);
    if(ret < 0)
    {
        if(errno != ENOENT)
        {
            bdb__error_errno("failed to check file/dir existence at \"%s\"", path);
            return -1;
        } else {
            return 0;
        }
    }
    return 1;
}

int bdb__fs_is_file(const char *path)
{
    struct BDB__POSIX(stat) st = {0};
    int ret = 0;

    if(!path)
        return -1;

    ret = BDB__POSIX(stat)(path, &st);
    if(ret < 0)
    {
        bdb__error_errno("failed to retrive info at \"%s\"", path);
        return -1;
    }

    return (st.st_mode & S_IFMT) == S_IFREG;
}

int bdb__fs_is_dir(const char *path)
{
    struct BDB__POSIX(stat) st = {0};
    int ret = 0;

    if(!path)
        return -1;

    ret = BDB__POSIX(stat)(path, &st);
    if(ret < 0)
    {
        bdb__error_errno("failed to retrive info at \"%s\"", path);
        return -1;
    }

    return (st.st_mode & S_IFMT) == S_IFDIR;
}

int bdb__fs_create_dir(const char *path)
{
    if(!path)
        return -1;
    if(BDB__POSIX(mkdir)(path, 0755) < 0)
    {
        bdb__error_errno("failed to create directory at \"%s\"", path);
        return -1;
    }
    return 0;
}

int bdb__fs_remove(const char *path)
{
    if(!path)
        return -1;
    if(remove(path) < 0)
    {
        bdb__error_errno("failed to remove file/dir at \"%s\"", path);
        return -1;
    }
    return 0;
}

int bdb__fs_collect_files(const char *path, bdb__arr_t *out)
{
    DIR *dir = 0;
    struct dirent *entry = 0;
    char *full = 0;
    struct stat st = {0};
    
    if(!path || !out)
        return -1;

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

int bdb__fs_ensure_dir(const char *path)
{
    int ret = 0;

    if(!path)
        return -1;
    ret = bdb__fs_exist(path);
    if(ret < 0)
        return -1;
    if(ret == 1)
    {
        ret = bdb__fs_is_dir(path);
        if(ret < 0)
            return -1;
        if(ret == 1)
            return 1;
        bdb__warn("file was found at \"%s\"", path);
        if(!bdb__confirm("do you want to remove the file", 0))
            return 0;
        ret = bdb__fs_remove(path);
        if(ret < 0)
            return -1;
    }
    bdb__warn("no directory was found at \"%s\"", path);
    ret = bdb__confirm("do you want to create directory",0);
    if(bdb__fs_create_dir(path) < 0)
        return -1;
    return 1;

    return 1;
}

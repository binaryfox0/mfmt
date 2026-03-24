#include "gen_emit.h"

#include <ctype.h>
#include <errno.h>

#include "bdb_log.h"
#include "bdb_utils.h"
#include "bdb_fs.h"
#include "gen_utils.h"
#include "gen_vars.h"

#define BDB__INDENT 4
static inline void bdb__fprint_upper_path(
        FILE *file, 
        const char *str, 
        const char sep)
{
    if(!file || !str)
        return;

    for(const char *p = str; *p; p++)
    {
        if(*p == '.')
            fputc(sep, file);
        else
            fputc(toupper((unsigned char)*p), file);
    }
}


static void bdb__fprint_prefixed_upper(
        FILE *file,
        const bdb_gen_name_fmt_t *fmt,
        const char *name
)
{
    if(!file || !name)
        return;

    if(fmt->prefix)
    {
        for(size_t i = 0; fmt->prefix[i]; i++)
            fputc(toupper((unsigned char)fmt->prefix[i]), file);
        fputc(fmt->sep, file);
    }

    for(const char *p = name; *p; p++)
    {
        if(p != name && isupper((unsigned char)*p))
            fputc(fmt->sep, file);

        fputc(toupper((unsigned char)*p), file);
    }
}

static int bdb__gen_emit_header(
        const bdb_gen_path_config_t *pc,
        const bdb__arr_t *arr,
        const bdb_gen_name_fmt_t *fmt)
{
    int ret = -1;
    char *dir_path = 0;
    char *file_name = 0;
    char *path = 0;
    FILE *file = 0;

    bdb__info("start to generate public header");

    dir_path = bdb__fs_cat_path(pc->root_path, pc->include_dir);
    if(!dir_path)
    {
        bdb__error("failed to construct header directory path");
        return -1;
    }

    ret = bdb__fs_ensure_dir(dir_path);
    if(ret < 1)
    {
        if(ret == 0)
            bdb__error("the action has been canceled by user");
        goto cleanup;   
    }

    file_name = bdb__sprintf("%s%s%s", 
            pc->stem, 
            pc->header_ext && *pc->header_ext ? "." : "",
            pc->header_ext ? pc->header_ext : "");
    if(
            !file_name ||
            strlen(file_name) == 0
    )
    {
        bdb__error("failed to construct header file name");
        goto cleanup;
    }

    path = bdb__fs_cat_path(dir_path, file_name);
    if(!path)
    {
        bdb__error("failed to construct header path");
        goto cleanup;
    }

    file = fopen(path, "w");
    if(!file)
    {
        bdb__error_errno("failed to open output header path");
        goto cleanup;
    }

    fprintf(file, "#ifndef ");
    bdb__fprint_upper_path(file, file_name, fmt->sep);
    fputc('\n', file);
    fprintf(file, "#define ");
    bdb__fprint_upper_path(file, file_name, fmt->sep);
    fputc('\n', file);

    fputs("\ntypedef enum\n{\n", file);
    for(size_t i = 0; i < arr->count; i++)
    {
        const uint8_t *base = bdb__arr_get_base(arr, i);
        const char *name = *(const char**)(base + fmt->name_offset);

        if(!name)
            continue;

        fprintf(file, "%*s", BDB__INDENT, "");
        bdb__fprint_prefixed_upper(file, fmt, name);
        fputc(',', file);
        fputc('\n', file);
    }
    
    fprintf(file, "%*s__", BDB__INDENT, "");
    bdb__fprint_prefixed_upper(file, fmt, "max");
    fprintf(file, "__\n");

    fprintf(file, "} %s_t;\n\n#endif\n",
            fmt->prefix);
    fclose(file);

    bdb__info("generate public header successfully");
    ret = 0;

cleanup:
    free(dir_path);
    free(file_name);
    free(path);

    return ret;
}

static const char *bdb__gen_var_type(
        const bdb__gen_var_desc_t *vd)
{
    if(vd->kind != BDB__GEN_VAR_BITS)
    {
        bdb__error("no type was associate with variable \"%s\"", vd->name);
        bdb__info("type: %d", vd->kind);
        return 0;
    }

    switch(vd->size)
    {
        case sizeof(uint8_t): return "uint8_t";
        case sizeof(uint16_t): return "uint16_t";
        case sizeof(uint32_t): return "uint32_t";
        case sizeof(uint64_t): return "uint64_t";
    }

    bdb__error("variable \"%s\" has invalid size", vd->name);
    bdb__info("type: %d, size: %d", vd->kind, vd->size);
    return 0;
}

static int bdb__gen_emit_private_header(
        const bdb_gen_path_config_t *pc,
        const char *dir_path,
        const bdb__arr_view_t *vds,
        const bdb_gen_name_fmt_t *fmt
)
{
    int ret = -1;
    char *file_name = 0;
    char *path = 0;
    FILE *file = 0;

    bdb__info("start to generate private header");

    file_name = bdb__sprintf("%s_priv%s%s", 
            pc->stem, 
            pc->header_ext && *pc->header_ext ? "." : "",
            pc->header_ext ? pc->header_ext : "");
    if(!file_name)
    {
        bdb__error("failed to construct private header file name");
        goto cleanup;
    }

    path = bdb__sprintf("%s%s%s",
            dir_path ? dir_path : "",
                dir_path &&
                *dir_path &&
                dir_path[strlen(dir_path) - 1] != BDB_PATH_SEP ?
                    BDB__STRINGIFY(BDB_PATH_SEP) : "",
            file_name
    );
    if(!path)
    {
        bdb__error("failed to construct private header path");
        goto cleanup;
    }

    file = fopen(path, "w");
    if(!file)
    {
        bdb__error_errno("failed to open private header");
        goto cleanup;
    }
    
    fprintf(file, "#ifndef ");
    bdb__fprint_upper_path(file, file_name, fmt->sep);
    fputc('\n', file);
    fprintf(file, "#define ");
    bdb__fprint_upper_path(file, file_name, fmt->sep);
    fputc('\n', file);

    fputc('\n', file);

    fprintf(file, "#include \"%s%s%s\"",
            pc->stem,
            pc->header_ext ? "." : "",
            pc->header_ext ? pc->header_ext : ""
    );

    fputc('\n', file);

    fputs("typedef struct\n{\n", file);
    for(size_t i = 0; i < vds->count; i++)
    {
        bdb__gen_var_desc_t vd = bdb__arr_get(vds, bdb__gen_var_desc_t, i);
        const char *type_name = bdb__gen_var_type(&vd);

        if(!type_name)
            continue;

        fprintf(file, "%*s%s %s;\n",
                BDB__INDENT, "", type_name, vd.name);
    }

    fprintf(file, "} %s__data_t;\n\n", fmt->prefix);

    fprintf(file, "extern %s__data_t %s__db[__",
            fmt->prefix, fmt->prefix); 
    bdb__fprint_prefixed_upper(file, fmt, "max");
    fputs("__];\n", file);

    fputc('\n', file);

    fputs("#endif\n", file);
    fclose(file);

    ret = 0;
    bdb__info("generate private header successfully");

cleanup:
    free(file_name);
    free(path);
    return ret;
}

static int bdb__gen_emit_source_file(
        const bdb_gen_path_config_t *pc,
        const char *dir_path,
        const bdb__arr_t *arr,
        const bdb__arr_view_t *vds,
        const bdb_gen_name_fmt_t *fmt
)
{
    int ret = -1;
    char *path = 0;
    FILE *file = 0;
    
    bdb__info("start to generate source file");

    path = bdb__fs_cat_fname(dir_path, pc->stem, pc->source_ext);
    if(!path)
    {
        bdb__error("failed to construct source path");
        goto cleanup;
    }

    file = fopen(path, "w");
    if(!file)
    {
        bdb__error_errno("failed to open output source path");
        return -1;
    }

    fprintf(file, "#include \"%s%s%s\"\n",
            pc->stem,
            pc->header_ext && *pc->header_ext ? "." : "",
            pc->header_ext ? pc->header_ext : ""
    );
    fprintf(file,
        "#include <stdint.h>\n"
        "#include \"%s_priv%s%s\"\n",
        pc->stem,
        pc->header_ext && *pc->header_ext ? "." : "",
        pc->header_ext ? pc->header_ext : ""
    );

    fputc('\n', file);

    fprintf(file, "%s__data_t %s__db[__",
            fmt->prefix, fmt->prefix);

    bdb__fprint_prefixed_upper(file, fmt, "max");
    fputs("__] =\n{\n", file);

    for(size_t i = 0; i < arr->count; i++)
    {
        const uint8_t *base = bdb__arr_get_base(arr, i);
        const char *name = *(const char**)(base + fmt->name_offset);

        fprintf(file, "%*s[", BDB__INDENT, "");
        bdb__fprint_prefixed_upper(file, fmt, name);
        fprintf(file, "] =\n%*s{\n", BDB__INDENT, "");

        for(size_t j = 0; j < vds->count; j++)
        {
            bdb__gen_var_desc_t vd = bdb__arr_get(vds, bdb__gen_var_desc_t, j);

            if(vd.kind != BDB__GEN_VAR_BITS)
            {
                bdb__error("unhandled variable \"%s\" to emit value", vd.name);
                bdb__info("type: %d", vd.kind);
                continue;
            }

            fprintf(file, "%*s.%s = 0x", BDB__INDENT * 2, "", vd.name);
            for(int k = 0; k < vd.size; k++)
            {
                uint8_t byte = base[vd.offset + (vd.size - 1 - k)];
                fprintf(file, "%02X", byte);
            }

            if(j != vds->count - 1)
                fputc(',', file);
            fputc('\n', file);
        }

        fprintf(file, "%*s}", BDB__INDENT, "");

        if(i != arr->count - 1)
            fputc(',', file);
        fputs("\n\n", file);
    }

    fputs("};\n", file);

    fclose(file);
    
    bdb__info("generate source file successfully");
    ret = 0;

cleanup:
    free(path);
    return ret;
}

static int bdb__gen_emit_source(
        const bdb_gen_path_config_t *pc,
        const bdb__arr_t *arr,
        const bdb__arr_view_t *vds,
        const bdb_gen_name_fmt_t *fmt)
{
    int ret = -1;
    char *dir_path = 0;
    char *file_name = 0;

    bdb__info("start to generate sources");
    dir_path = bdb__fs_cat_path(pc->root_path, pc->src_dir);
    if(!dir_path)
    {
        bdb__error("failed to construct source directory path");
        return -1;
    }

    ret = bdb__fs_ensure_dir(dir_path);
    if(ret < 1)
    {
        if(ret == 0)
            bdb__error("the action has been canceled by user");
        goto cleanup;
    }

    if(bdb__gen_emit_private_header(pc, dir_path, vds, fmt) < 0)
    {
        bdb__error("failed to generate private header");
        goto cleanup;
    }
    if(bdb__gen_emit_source_file(pc, dir_path, arr, vds, fmt) < 0) 
    {
        bdb__error("failed to generate source file");
        goto cleanup;
    }
    
    ret = 0;

cleanup:
    free(dir_path);
    free(file_name);
    return ret;
}

int bdb_gen_emit_code(
        const bdb_gen_path_config_t *pc,
        const bdb__arr_t *arr, 
        const bdb__arr_view_t *vds, 
        const bdb_gen_name_fmt_t *fmt
)
{
    if(!pc || !pc->stem || !arr || !vds || !fmt)
        return -1;

    if(bdb__gen_emit_header(pc, arr, fmt) < 0)
    {
        bdb__error("failed to generate header file");
        return -1;
    }
    if(bdb__gen_emit_source(pc, arr, vds, fmt) < 0)
    {
        bdb__error("failed to generate source file");
        return -1;
    }

    return 0;
}

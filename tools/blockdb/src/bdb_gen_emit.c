#include "bdb_gen_emit.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#include "bdb_log.h"
#include "bdb_utils.h"
#include "bdb_fs.h"
#include "bdb_ui.h"

#include "bdb_gen_utils.h"
#include "bdb_gen_vars.h"

#define BDB__INDENT 4

const char *bdb__fs_fname(const char *path)
{
    const char *out = 0;
    if(!path)
        return 0;
    out = strrchr(path, BDB_PATH_SEP);
    return out ? out + 1 : path;
}

static FILE *bdb__open_file(
        char **out_fname,
        const char *dir,
        const char *stem,
        const char *ext)
{
    FILE *f = 0;
    char *fname = 0;
    char *path = 0;

    if(out_fname) 
        *out_fname = 0;

    fname = bdb__sprintf("%s.%s", stem, ext);
    path = bdb__fs_cat_path(dir, fname);
    if(!path)
        return 0;

    f = fopen(path, "w");
    if(!f)
    {
        bdb__error_errno("fopen failed");
        free(fname);
        free(path);
        return 0;
    }

    if(out_fname)
        *out_fname = fname;
    else
        free(fname);
    free(path);

    return f;
}

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

static void bdb__emit_include_guard_begin(
        FILE *f,
        const char *file_name,
        char sep)
{
    fputs("#ifndef ", f);
    bdb__fprint_upper_path(f, file_name, sep);
    fputc('\n', f);

    fputs("#define ", f);
    bdb__fprint_upper_path(f, file_name, sep);
    fputc('\n', f);
}

static void bdb__emit_include_guard_end(FILE *f)
{
    fputs("\n#endif\n", f);
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

static void bdb__fprint_bit_name(
        FILE *file,
        const bdb_gen_name_fmt_t *fmt,
        const bdb__gen_var_desc_t *vd,
        const bdb__gen_var_desc_t *bit)
{
    const char *prefix = vd->enum_prefix ? vd->enum_prefix : fmt->prefix;
    const char *enum_name = vd->enum_name ? vd->enum_name : vd->name;
    char *name = bdb__sprintf("%s_%s", enum_name, bit->name);
    if(!name)
        return;

    bdb_gen_name_fmt_t enum_fmt = *fmt;
    enum_fmt.prefix = prefix;
    bdb__fprint_prefixed_upper(file, &enum_fmt, name);
    free(name);
}

static int bdb__gen_emit_bitmask(
        FILE *f,
        const uint8_t *base,
        const bdb__gen_var_desc_t *vd,
        const bdb_gen_name_fmt_t *fmt)
{
    uint8_t known_mask[sizeof(uint64_t)] = {0};
    int has_term = 0;

    if(vd->size > (int)sizeof(known_mask))
        return -1;

    for(size_t i = 0; i < vd->children.count; i++)
    {
        const bdb__gen_var_desc_t *bit =
            &bdb__arr_get(&vd->children, bdb__gen_var_desc_t, i);
        size_t byte_idx = (size_t)bit->offset / 8;
        uint8_t mask = (uint8_t)(1u << (bit->offset % 8));

        known_mask[byte_idx] |= mask;
        if(!(base[vd->offset + byte_idx] & mask))
            continue;

        if(has_term)
            fputs(" | ", f);
        bdb__fprint_bit_name(f, fmt, vd, bit);
        has_term = 1;
    }

    int has_unknown_bits = 0;
    for(int i = 0; i < vd->size; i++)
    {
        uint8_t value = base[vd->offset + i] & (uint8_t)~known_mask[i];
        if(value)
            has_unknown_bits = 1;
    }

    if(has_unknown_bits)
    {
        if(has_term)
            fputs(" | ", f);
        fputs("0x", f);
        for(int i = vd->size - 1; i >= 0; i--)
        {
            uint8_t value = base[vd->offset + i] & (uint8_t)~known_mask[i];
            fprintf(f, "%02X", value);
        }
        has_term = 1;
    }

    if(!has_term)
    {
        bdb__fprint_bit_name(f, fmt, vd,
                &(bdb__gen_var_desc_t){ .name = "none" });
    }

    return 0;
}

static int bdb__gen_emit_header(
        const bdb_gen_path_config_t *pc,
        const bdb__arr_t *arr,
        const bdb__arr_view_t *vds,
        const bdb_gen_name_fmt_t *fmt)
{
    int ret = -1;

    char *dir = 0;
    char *file_name = 0;
    FILE *f = 0;

    dir = bdb__fs_cat_path(pc->root_path, pc->include_dir);
    if(!dir)
        return -1;

    if(bdb__fs_ensure_dir(dir) < 1)
        goto cleanup;

    f = bdb__open_file(&file_name, dir, pc->stem, pc->header_ext);
    if(!f)
        goto cleanup;

    bdb__emit_include_guard_begin(f, file_name, fmt->sep);

    for(size_t i = 0; i < vds->count; i++)
    {
        bdb__gen_var_desc_t vd =
            bdb__arr_get(vds, bdb__gen_var_desc_t, i);
        if(vd.kind != BDB__GEN_VAR_BITS)
            continue;

        fputs("\ntypedef enum\n{\n", f);
        fprintf(f, "%*s", BDB__INDENT, "");
        bdb__fprint_bit_name(f, fmt, &vd,
                &(bdb__gen_var_desc_t){ .name = "none" });
        fputs(" = 0,\n", f);

        for(size_t j = 0; j < vd.children.count; j++)
        {
            const bdb__gen_var_desc_t *bit =
                &bdb__arr_get(&vd.children, bdb__gen_var_desc_t, j);
            fprintf(f, "%*s", BDB__INDENT, "");
            bdb__fprint_bit_name(f, fmt, &vd, bit);
            fprintf(f, " = 1u << %d,\n", bit->offset);
        }

        fprintf(f, "} %s_%s_t;\n",
            vd.enum_prefix ? vd.enum_prefix : fmt->prefix,
            vd.enum_name ? vd.enum_name : vd.name);
    }

    bdb__info("generating blocks enum");
    fputs("\ntypedef enum\n{\n", f);
    for(size_t i = 0; i < arr->count; i++)
    {
        const uint8_t *base = bdb__arr_get_base(arr, i);
        const char *name = *(const char**)(base + fmt->name_offset);

        bdb_ui_progress(i + 1, arr->count);
        if(!name) 
            continue;

        fprintf(f, "%*s", BDB__INDENT, "");
        bdb__fprint_prefixed_upper(f, fmt, name);
        fputs(",\n", f);
    }
    fprintf(f, "%*s", BDB__INDENT, "");
    bdb__fprint_prefixed_upper(f, fmt, "max");
    fputc('\n', f);

    fprintf(f, "} %s_t;\n", fmt->prefix);
    fprintf(f, "\nconst char *%s_name(%s id);\n", fmt->prefix, fmt->type_name);
    bdb_ui_progress_end();

    bdb__emit_include_guard_end(f);

    ret = 0;

cleanup:
    if(f) fclose(f);
    free(dir);
    free(file_name);
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
        const char *dir,
        const bdb__arr_view_t *vds,
        const bdb_gen_name_fmt_t *fmt)
{
    int ret = -1;
    char *file_name = 0;
    FILE *f = 0;

    char *stem = bdb__sprintf("%s_priv", pc->stem);
    if(!stem)
        return -1;

    f = bdb__open_file(&file_name, dir, stem, pc->header_ext);
    if(!f)
        goto cleanup;

    bdb__emit_include_guard_begin(f, file_name, fmt->sep);

    fprintf(f, "\n#include \"%s.%s\"\n\n",
            pc->stem,
            pc->header_ext ? pc->header_ext : "");

    fputs("typedef struct\n{\n", f);
    for(size_t i = 0; i < vds->count; i++)
    {
        bdb__gen_var_desc_t vd =
            bdb__arr_get(vds, bdb__gen_var_desc_t, i);

        const char *type = bdb__gen_var_type(&vd);
        if(!type) 
            continue;

        fprintf(f, "%*s%s %s;\n",
                BDB__INDENT, "", type, vd.name);
    }

        fprintf(f, "} %s_data_t;\n\n", fmt->internal_prefix);

    fprintf(f, "extern %s_data_t %s_db[",
            fmt->internal_prefix, fmt->internal_prefix);

    bdb__fprint_prefixed_upper(f, fmt, "max");
    fputs("];\n", f);

    bdb__emit_include_guard_end(f);

    ret = 0;

cleanup:
    if(f) fclose(f);
    free(file_name);
    free(stem);
    return ret;
}

static int bdb__gen_emit_source_file(
        const bdb_gen_path_config_t *pc,
        const char *dir,
        const bdb__arr_t *arr,
        const bdb__arr_view_t *vds,
        const bdb_gen_name_fmt_t *fmt)
{
    FILE *f = 0;

    f = bdb__open_file(0, dir, pc->stem, pc->source_ext);
    if(!f)
        return -1;

    fprintf(f, "#include \"%s.%s\"\n",
            pc->stem,
            pc->header_ext ? pc->header_ext : "");

    fprintf(f,
        "#include <stdint.h>\n"
        "#include \"%s_priv.%s\"\n\n",
        pc->stem,
        pc->header_ext ? pc->header_ext : "");

    fprintf(f, "%s_data_t %s_db[",
            fmt->internal_prefix, fmt->internal_prefix);

    bdb__info("generating block database");

    bdb__fprint_prefixed_upper(f, fmt, "max");
    fputs("] =\n{\n", f);
    for(size_t i = 0; i < arr->count; i++)
    {
        const uint8_t *base = bdb__arr_get_base(arr, i);
        const char *name = *(const char**)(base + fmt->name_offset);

        bdb_ui_progress(i + 1, arr->count);

        if(!name) 
            continue;

        fprintf(f, "%*s[", BDB__INDENT, "");
        bdb__fprint_prefixed_upper(f, fmt, name);
        fprintf(f, "] =\n%*s{\n", BDB__INDENT, "");

        for(size_t j = 0; j < vds->count; j++)
        {
            bdb__gen_var_desc_t vd =
                bdb__arr_get(vds, bdb__gen_var_desc_t, j);

            if(vd.kind != BDB__GEN_VAR_BITS)
                continue;

            fprintf(f, "%*s.%s = ", BDB__INDENT * 2, "", vd.name);
            if(bdb__gen_emit_bitmask(f, base, &vd, fmt) < 0)
            {
                fclose(f);
                return -1;
            }

            int has_later_var = 0;
            for(size_t k = j + 1; k < vds->count; k++)
            {
                bdb__gen_var_desc_t next =
                    bdb__arr_get(vds, bdb__gen_var_desc_t, k);
                if(next.kind == BDB__GEN_VAR_BITS)
                {
                    has_later_var = 1;
                    break;
                }
            }
            if(has_later_var)
                fputc(',', f);

            fputc('\n', f);
        }

        fprintf(f, "%*s}", BDB__INDENT, "");

        if(i + 1 < arr->count)
            fputc(',', f);

        fputs("\n\n", f);
    }
    fputs("};\n", f);

    fprintf(f, "\nstatic const char *const %s_names[", fmt->internal_prefix);
    bdb__fprint_prefixed_upper(f, fmt, "max");
    fputs("] =\n{\n", f);
    for(size_t i = 0; i < arr->count; i++)
    {
        const uint8_t *base = bdb__arr_get_base(arr, i);
        const char *name = *(const char**)(base + fmt->name_offset);

        if(!name)
            continue;

        fprintf(f, "%*s[", BDB__INDENT, "");
        bdb__fprint_prefixed_upper(f, fmt, name);
        fprintf(f, "] = \"%s\",\n", name);
    }
    fputs("};\n\n", f);
    fprintf(f,
        "const char *%s_name(%s id)\n"
        "{\n"
        "%*sif ((unsigned int)id >= (unsigned int)",
        fmt->prefix, fmt->type_name, BDB__INDENT, "");
    bdb__fprint_prefixed_upper(f, fmt, "max");
    fprintf(f,
        ")\n"
        "%*sreturn 0;\n"
        "%*sreturn %s_names[id];\n"
        "}\n",
        BDB__INDENT * 2, "", BDB__INDENT, "", fmt->internal_prefix);
    bdb_ui_progress_end();

    fclose(f);
    return 0;
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

    if(bdb__gen_emit_header(pc, arr, vds, fmt) < 0)
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

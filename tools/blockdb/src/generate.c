#include "generate.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <limits.h>
#include <errno.h>

#include <dirent.h>
#include <sys/stat.h>

#include <tree_sitter/api.h>

#include "utils.h"
#include "log.h"

#define ARRSZ(arr) (sizeof((arr)) / sizeof((arr)[0]))

typedef struct
{
    void *data;
    size_t elem_size;
    size_t capacity;
    size_t count;
} bdb__arr_t;

#define bdb__arr_get(arr, type, idx) (((type*)(arr)->data)[(idx)])

int bdb__arr_append(bdb__arr_t *arr, const void *elem)
{
    if(arr->count >= arr->capacity)
    {
        size_t new_cap = 0;
        void *new_data = 0;

        new_cap = arr->capacity > 0 ? arr->capacity * 2 : 4;
        new_data = realloc(arr->data, arr->elem_size * new_cap);
        if(!new_data)
            return -1;

        arr->capacity = new_cap;
        arr->data = new_data;
    }

    memcpy((uint8_t*)arr->data + (arr->elem_size * arr->count), elem, arr->elem_size);
    arr->count++;

    return 0;
}

void bdb__arr_destroy_strings(bdb__arr_t *list)
{
    for(size_t i = 0; i < list->count; i++)
        free(bdb__arr_get(list, char*, i));
    free(list->data);
    *list = (bdb__arr_t){0};
}

static int bdb__collect_files(const char *path, bdb__arr_t *out)
{
    DIR *dir = 0;
    struct dirent *entry = 0;
    char full[PATH_MAX] = {0};
    struct stat st = {0};

    dir = opendir(path);
    if(!dir)
    {
        error("failed to open directory at \"%s\"", path);
        info("reason: %s", strerror(errno));
        return -1;
    }

    while((entry = readdir(dir)) != 0)
    {
        if(
                !strcmp(entry->d_name, ".") ||
                !strcmp(entry->d_name, "..")
          ) continue;

        snprintf(full, sizeof(full), "%s/%s", path, entry->d_name);

        stat(full, &st);
        if(S_ISDIR(st.st_mode))
            bdb__collect_files(full, out);
        else
        {
            char *file_path = strdup(full);
            if(!file_path)
                continue;
            if(bdb__arr_append(out, &file_path) < 0)
            {
                error("failed to append file path into array");
                info("reason: %s", strerror(errno));
                bdb__arr_destroy_strings(out);
            }
        }
    }
    
    closedir(dir);
    return 0;
}


typedef struct tree_node
{
    char *name;
    char *parent_name;

    /*
     * !!! Info - The order of each bits is flexible - Info !!! *
     * bit 0: update == true
     * bit 1: destructible == true
    */
    uint8_t has_building;

    /*
     * !!! Warning - The order MUST be fixed - Warning !!! *
     * bit 0: items != null (hasItems == true)
     * bit 1: power != null (hasPower == true)
     * bit 2: liquids != null (hasLiquids == true)
     * bit 3: always on (compatibility with ConsumeModule)
     * bit 4: timeScale != 1f
     * bit 5: lastDisabler != null && lastDisabler.isValid()
     */
    uint8_t modules_bitmask;

    struct tree_node *parent;
    size_t child_count;
    struct tree_node **children;
} tree_node_t;

/*
 * compare first l bytes of s1 with s2
 * s2 must be exactly length l
 */
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

typedef struct
{
    const char *name;
    int is_bool;
    int bit_idx;
    int out_offset;
    int out_size;
} var_query_t;

var_query_t* var_query_find(const bdb__arr_t *vqs, const char *str, const size_t len)
{
    if(!vqs || !str)
        return 0;

    for(size_t i = 0; i < vqs->count; i++)
    {
        if(!bdb__tokcmp(str, bdb__arr_get(vqs, var_query_t, i).name, len))
            return &bdb__arr_get(vqs, var_query_t, i);
    }

    return 0;
}

static void bdb__gen_handle_assign(
        const bdb__arr_t *vqs,
        const char *name,
        size_t name_len,
        TSNode right,
        const char *src,
        tree_node_t *tn)
{
    int val = -1;
    uint32_t rs = 0;
    uint32_t re = 0;
    size_t rlen = 0;

    var_query_t *vq = 0;
    uint8_t *target = 0;

    if(!vqs || !name || !tn)
        return;

    vq = var_query_find(vqs, name, name_len);
    if(!vq)
        return;

    if(!vq->is_bool)
    {
        error("variable \"%s\" is not supported", vq->name);
        info("support may be added in future version");
        return; /* unsupported for now */
    }

    rs = ts_node_start_byte(right);
    re = ts_node_end_byte(right);
    rlen = re - rs;

    if(!bdb__tokcmp(src + rs, "true", rlen))
        val = 1;
    else if(!bdb__tokcmp(src + rs, "false", rlen))
        val = 0;
    else
    {
        TSPoint p = ts_node_start_point(right);
        error("invalid boolean value at %u, %u", p.row + 1, p.column + 1);
        return;
    }

    target = ((uint8_t*)tn) + vq->out_offset;

    if(val)
        *target |= (1u << vq->bit_idx);
    else
        *target &= ~(1u << vq->bit_idx);
}

static void bdb__gen_scan_ctor_body(
        TSNode node, const char *src, 
        tree_node_t *tn, const bdb__arr_t *vqs)
{
    uint32_t count = 0;

    count = ts_node_named_child_count(node);

    for(uint32_t i = 0; i < count; i++)
    {
        TSNode stmt = {0};
        TSNode expr = {0};
        TSNode left = {0};
        TSNode right = {0};

        stmt = ts_node_named_child(node, i);
        if(strcmp(ts_node_type(stmt), "expression_statement"))
            continue;

        expr = ts_node_named_child(stmt, 0);
        if(strcmp(ts_node_type(expr), "assignment_expression"))
            continue;

        left = ts_node_child_by_field_name(expr, "left", 4);
        right = ts_node_child_by_field_name(expr, "right", 5);

        if(ts_node_is_null(left) || ts_node_is_null(right))
            continue;

        /* identifier = value */
        if(!strcmp(ts_node_type(left), "identifier"))
        {
            uint32_t s = 0;
            uint32_t e = 0;

            s = ts_node_start_byte(left);
            e = ts_node_end_byte(left);

            bdb__gen_handle_assign(
                vqs,
                src + s,
                e - s,
                right,
                src,
                tn
            );
        }

        /* this.xxx = value */
        else if(!strcmp(ts_node_type(left), "field_access"))
        {
            TSNode field = {0};

            field = ts_node_child_by_field_name(left, "field", 5);
            if(ts_node_is_null(field))
                continue;

            uint32_t s = 0;
            uint32_t e = 0;

            s = ts_node_start_byte(field);
            e = ts_node_end_byte(field);

            bdb__gen_handle_assign(
                vqs,
                src + s,
                e - s,
                right,
                src,
                tn
            );
        }
    }
}

static void bdb__gen_scan_ctor(
        TSNode node, const char *src, 
        tree_node_t *tn, const bdb__arr_t *vqs)
{
    TSNode body = {0};

    body = ts_node_child_by_field_name(node, "body", 4);
    if(ts_node_is_null(body))
        return;

    uint32_t count = ts_node_named_child_count(body);

    for(uint32_t i = 0; i < count; i++)
    {
        TSNode child = {0};
        TSNode cbody = {0};

        child = ts_node_named_child(body, i);

        if(strcmp(ts_node_type(child), "constructor_declaration"))
            continue;

        cbody = ts_node_child_by_field_name(child, "body", 4);

        if(!ts_node_is_null(cbody))
            bdb__gen_scan_ctor_body(cbody, src, tn, vqs);
    }
}

static const var_query_t queries[] =
{
    { "update",        1, 0, offsetof(tree_node_t, has_building), 1 },
    { "destructible",  1, 1, offsetof(tree_node_t, has_building), 1 },

    { "hasItems",      1, 0, offsetof(tree_node_t, modules_bitmask), 1 },
    { "hasPower",      1, 1, offsetof(tree_node_t, modules_bitmask), 1 },
    { "hasLiquids",    1, 2, offsetof(tree_node_t, modules_bitmask), 1 },
};

static tree_node_t* bdb__gen_parse_class(
        TSNode node, const char *src)
{
    TSNode name_node = {0};
    TSNode super_node = {0};
    uint32_t s = 0;
    uint32_t e = 0;
    tree_node_t *tn = 0;

    name_node = ts_node_child_by_field_name(node, "name", 4);
    super_node = ts_node_child_by_field_name(node, "superclass", 10);

    if(ts_node_is_null(name_node))
    {
        return 0;
    }

    tn = calloc(1, sizeof(*tn));

    s = ts_node_start_byte(name_node);
    e = ts_node_end_byte(name_node);

    tn->name = bdb__tokdup(src + s, (size_t)(e - s));

    if(!ts_node_is_null(super_node))
    {
        TSNode type = {0};
        type = ts_node_named_child(super_node, 0);
        if(!ts_node_is_null(type))
        {
            s = ts_node_start_byte(type);
            e = ts_node_end_byte(type);

            tn->parent_name = bdb__tokdup(src + s, (size_t)(e - s));
        }
    }

    bdb__gen_scan_ctor(node, src, tn, 
            (bdb__arr_t[1]){{
                .data = (void*)queries,
                .count = ARRSZ(queries),
                .capacity = ARRSZ(queries)
            }}
    );

    info("%s -> %s, has_building: 0x%02X, modules_bitmask; 0x%02X", 
            tn->name, tn->parent_name, tn->has_building, tn->modules_bitmask);

    free(tn->name);
    free(tn->parent_name);

    return tn;
}

static void bdb__gen_walk_ast(
        TSNode node, const char *src)
{
    const char *type = 0;
    uint32_t count = 0;

    type = ts_node_type(node);
    if(!strcmp(type, "class_declaration"))
        bdb__gen_parse_class(node, src);

    count = ts_node_child_count(node);
    for(uint32_t i = 0; i < count; i++)
    {
        TSNode child = {0};
        child = ts_node_child(node, i);
        bdb__gen_walk_ast(child, src);
    }
}


extern const TSLanguage *tree_sitter_java(void);
void generate_command(void *args)
{
    const char *path = *(const char**)args;
    bdb__arr_t file_list = { .elem_size = sizeof(void*) };
    TSParser *parser = 0;

    if(bdb__collect_files(path, &file_list) < 0)
        return;

    parser = ts_parser_new();
    ts_parser_set_language(parser, tree_sitter_java());

    for(size_t i = 0; i < file_list.count; i++)
    {
        char *path = 0;
        char *src = 0; 
        long src_len = 0;

        TSTree *tree = 0;
        TSNode node = {0};
    
        path = bdb__arr_get(&file_list, char*, i);
        //info("parsing \"%s\"", path);
        if(read_file(path, &src, &src_len) < 0)
            continue;

        tree = ts_parser_parse_string(parser, 0, src, (uint32_t)src_len);
       
        node = ts_tree_root_node(tree);
        bdb__gen_walk_ast(node, src);

        ts_tree_delete(tree);
        free(src);
    }

    ts_parser_delete(parser);
    bdb__arr_destroy_strings(&file_list);
}

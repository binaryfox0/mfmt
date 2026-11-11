#include "bdb_entity_gen.h"

#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tree_sitter/api.h>

#include "bdb_arr.h"
#include "bdb_fs.h"
#include "bdb_gen_utils.h"
#include "bdb_log.h"
#include "bdb_utils.h"

extern const TSLanguage *tree_sitter_java(void);

typedef struct
{
    char *name;
    char *full_name;
    int is_base;
    bdb__arr_t interfaces;
} bdb_entity_component_t;

typedef struct
{
    char *key;
    int32_t id;
} bdb_entity_property_t;

typedef struct
{
    int32_t id;
    int has_id;
    char *name;
    bdb__arr_t aliases;
    bdb__arr_t direct_components;
    bdb__arr_t components;
} bdb_entity_info_t;

typedef struct
{
    bdb__arr_t components;
    bdb__arr_t properties;
    bdb__arr_t entities;
} bdb_entity_data_t;

typedef struct
{
    const char *root;
    bdb_entity_data_t *data;
    int collect_components;
    int error;
} bdb_entity_walk_t;

typedef struct
{
    const char *root_path;
    const char *include_dir;
    const char *src_dir;
    const char *stem;
    const char *header_ext;
    const char *source_ext;
} bdb_entity_output_t;

static int bdb_entity_strcmp_ptr(const void *left, const void *right)
{
    const char *const *a = left;
    const char *const *b = right;
    return strcmp(*a, *b);
}

static int bdb_entity_compare_component(const void *left, const void *right)
{
    const bdb_entity_component_t *a = left;
    const bdb_entity_component_t *b = right;
    return strcmp(a->name, b->name);
}

static int bdb_entity_compare_id(const void *left, const void *right)
{
    const bdb_entity_info_t *a = left;
    const bdb_entity_info_t *b = right;
    return (a->id > b->id) - (a->id < b->id);
}

static int bdb_entity_add_string(bdb__arr_t *arr, const char *value)
{
    if(!arr || !value)
        return -1;

    for(size_t i = 0; i < arr->count; i++)
    {
        char *current = bdb__arr_get(arr, char*, i);
        if(!strcmp(current, value))
            return 0;
    }

    char *copy = bdb__tokdup(value, strlen(value));
    if(!copy)
        return -1;
    if(bdb__arr_append(arr, &copy) < 0)
    {
        free(copy);
        return -1;
    }
    return 0;
}

static int bdb_entity_add_kebab_alias(bdb__arr_t *aliases, const char *name)
{
    char *kebab = malloc(strlen(name) * 2 + 1);
    if(!kebab)
        return -1;

    size_t index = 0;
    for(const char *p = name; *p; p++)
    {
        if(isupper((unsigned char)*p))
            kebab[index++] = '-';
        kebab[index++] = (char)tolower((unsigned char)*p);
    }
    kebab[index] = '\0';

    int result = strcmp(kebab, name) ? bdb_entity_add_string(aliases, kebab) : 0;
    free(kebab);
    return result;
}

static void bdb_entity_free_strings(bdb__arr_t *arr)
{
    if(!arr)
        return;
    for(size_t i = 0; i < arr->count; i++)
        free(bdb__arr_get(arr, char*, i));
    bdb__arr_destroy(arr);
}

static TSNode bdb_entity_find_descendant(TSNode node, const char *type)
{
    if(ts_node_is_null(node))
        return (TSNode){0};
    if(!strcmp(ts_node_type(node), type))
        return node;

    uint32_t count = ts_node_named_child_count(node);
    for(uint32_t i = 0; i < count; i++)
    {
        TSNode found = bdb_entity_find_descendant(ts_node_named_child(node, i), type);
        if(!ts_node_is_null(found))
            return found;
    }
    return (TSNode){0};
}

static TSNode bdb_entity_child_of_type(TSNode node, const char *type)
{
    uint32_t count = ts_node_child_count(node);
    for(uint32_t i = 0; i < count; i++)
    {
        TSNode child = ts_node_child(node, i);
        if(!strcmp(ts_node_type(child), type))
            return child;
    }
    return (TSNode){0};
}

static char *bdb_entity_node_text_from_src(TSNode node, const char *src);

static TSNode bdb_entity_annotation(TSNode modifiers, const char *src, const char *name)
{
    if(ts_node_is_null(modifiers))
        return (TSNode){0};
    uint32_t count = ts_node_child_count(modifiers);
    for(uint32_t i = 0; i < count; i++)
    {
        TSNode child = ts_node_child(modifiers, i);
        const char *kind = ts_node_type(child);
        if(strcmp(kind, "annotation") && strcmp(kind, "marker_annotation"))
            continue;

        TSNode name_node = ts_node_child_by_field_name(child, "name", 4);
        if(ts_node_is_null(name_node))
            continue;
        uint32_t start = ts_node_start_byte(name_node);
        uint32_t end = ts_node_end_byte(name_node);
        char *text = bdb_entity_node_text_from_src(name_node, src);
        if(text)
        {
            size_t len = strlen(text);
            const char *short_name = strrchr(text, '.');
            short_name = short_name ? short_name + 1 : text;
            int matches = !strcmp(short_name, name);
            free(text);
            if(matches)
                return child;
        }
    }
    return (TSNode){0};
}

static char *bdb_entity_node_text_from_src(TSNode node, const char *src)
{
    uint32_t start = ts_node_start_byte(node);
    uint32_t end = ts_node_end_byte(node);
    if(ts_node_is_null(node) || end < start)
        return NULL;
    return bdb__tokdup(src + start, (size_t)(end - start));
}

static int bdb_entity_class_literals(
        TSNode node,
        const char *src,
        bdb__arr_t *out)
{
    if(ts_node_is_null(node))
        return 0;
    if(!strcmp(ts_node_type(node), "class_literal"))
    {
        TSNode type = ts_node_named_child(node, 0);
        if(ts_node_is_null(type))
            return -1;
        char *name = bdb_entity_node_text_from_src(type, src);
        if(!name)
            return -1;
        int rc = bdb_entity_add_string(out, name);
        free(name);
        return rc;
    }

    uint32_t count = ts_node_named_child_count(node);
    for(uint32_t i = 0; i < count; i++)
        if(bdb_entity_class_literals(ts_node_named_child(node, i), src, out) < 0)
            return -1;
    return 0;
}

static int bdb_entity_parse_annotation(
        TSNode annotation,
        const char *src,
        bdb__arr_t *direct,
        int *legacy)
{
    TSNode args = ts_node_child_by_field_name(annotation, "arguments", 9);
    if(ts_node_is_null(args))
        return -1;

    uint32_t count = ts_node_named_child_count(args);
    int saw_value = 0;
    for(uint32_t i = 0; i < count; i++)
    {
        TSNode arg = ts_node_named_child(args, i);
        const char *kind = ts_node_type(arg);
        if(!strcmp(kind, "element_value_pair"))
        {
            TSNode key_node = ts_node_child_by_field_name(arg, "key", 3);
            TSNode value_node = ts_node_child_by_field_name(arg, "value", 5);
            if(ts_node_is_null(key_node) || ts_node_is_null(value_node))
                continue;
            char *key = bdb_entity_node_text_from_src(key_node, src);
            if(!key)
                return -1;
            if(!strcmp(key, "value"))
            {
                saw_value = 1;
                if(bdb_entity_class_literals(value_node, src, direct) < 0)
                {
                    free(key);
                    return -1;
                }
            }
            else if(!strcmp(key, "legacy") && legacy)
            {
                char *value = bdb_entity_node_text_from_src(value_node, src);
                if(value && !strcmp(value, "true"))
                    *legacy = 1;
                free(value);
            }
            free(key);
        }
        else if(!saw_value)
        {
            if(bdb_entity_class_literals(arg, src, direct) < 0)
                return -1;
            saw_value = 1;
        }
    }
    return direct->count ? 0 : -1;
}

static char *bdb_entity_package(TSNode root, const char *src)
{
    TSNode package = bdb_entity_find_descendant(root, "package_declaration");
    if(ts_node_is_null(package))
        return bdb__tokdup("", 0);

    char *text = bdb_entity_node_text_from_src(package, src);
    if(!text)
        return NULL;
    char *start = text;
    while(isspace((unsigned char)*start)) start++;
    if(!strncmp(start, "package", 7))
        start += 7;
    while(isspace((unsigned char)*start)) start++;
    char *end = strrchr(start, ';');
    if(end) *end = '\0';
    while(end && end > start && isspace((unsigned char)end[-1])) *--end = '\0';
    char *result = bdb__tokdup(start, strlen(start));
    free(text);
    return result;
}

static int bdb_entity_has_annotation(TSNode node, const char *src, const char *name)
{
    TSNode modifiers = bdb_entity_child_of_type(node, "modifiers");
    if(ts_node_is_null(modifiers))
        return 0;

    uint32_t count = ts_node_child_count(modifiers);
    for(uint32_t i = 0; i < count; i++)
    {
        TSNode child = ts_node_child(modifiers, i);
        const char *kind = ts_node_type(child);
        if(strcmp(kind, "annotation") && strcmp(kind, "marker_annotation"))
            continue;
        TSNode name_node = ts_node_child_by_field_name(child, "name", 4);
        if(ts_node_is_null(name_node))
            continue;
        char *text = bdb_entity_node_text_from_src(name_node, src);
        if(!text)
            continue;
        const char *short_name = strrchr(text, '.');
        short_name = short_name ? short_name + 1 : text;
        int found = !strcmp(short_name, name);
        free(text);
        if(found)
            return 1;
    }
    return 0;
}

static int bdb_entity_add_component(
        bdb_entity_data_t *data,
        const char *name,
        const char *full_name,
        int is_base,
        bdb__arr_t *interfaces)
{
    bdb_entity_component_t component = {0};
    component.name = bdb__tokdup(name, strlen(name));
    component.full_name = bdb__tokdup(full_name, strlen(full_name));
    component.is_base = is_base;
    component.interfaces = *interfaces;
    memset(interfaces, 0, sizeof(*interfaces));
    if(!component.name || !component.full_name ||
       bdb__arr_append(&data->components, &component) < 0)
    {
        free(component.name);
        free(component.full_name);
        bdb_entity_free_strings(&component.interfaces);
        return -1;
    }
    return 0;
}

static int bdb_entity_collect_interfaces(TSNode node, const char *src, bdb__arr_t *out)
{
    if(ts_node_is_null(node))
        return 0;
    if(!strcmp(ts_node_type(node), "type_identifier"))
    {
        char *name = bdb_entity_node_text_from_src(node, src);
        if(!name)
            return -1;
        int rc = bdb_entity_add_string(out, name);
        free(name);
        return rc;
    }
    uint32_t count = ts_node_named_child_count(node);
    for(uint32_t i = 0; i < count; i++)
        if(bdb_entity_collect_interfaces(ts_node_named_child(node, i), src, out) < 0)
            return -1;
    return 0;
}

static int bdb_entity_component_pass(
        TSNode node,
        const char *src,
        const char *package,
        const char *scope,
        bdb_entity_data_t *data)
{
    const char *kind = ts_node_type(node);
    char *next_scope = NULL;
    if(!strcmp(kind, "class_declaration"))
    {
        TSNode name_node = ts_node_child_by_field_name(node, "name", 4);
        char *simple = bdb_entity_node_text_from_src(name_node, src);
        if(!simple)
            return -1;
        next_scope = scope && *scope ? bdb__sprintf("%s.%s", scope, simple) :
            (package && *package ? bdb__sprintf("%s.%s", package, simple) :
                bdb__tokdup(simple, strlen(simple)));
        free(simple);
        if(!next_scope)
            return -1;

        if(bdb_entity_has_annotation(node, src, "Component"))
        {
            TSNode interfaces_node = ts_node_child_by_field_name(node, "interfaces", 10);
            bdb__arr_t interfaces = { .elem_size = sizeof(char*) };
            if(bdb_entity_collect_interfaces(interfaces_node, src, &interfaces) < 0 ||
               bdb_entity_add_component(data, strrchr(next_scope, '.') ? strrchr(next_scope, '.') + 1 : next_scope,
                   next_scope, bdb_entity_has_annotation(node, src, "BaseComponent"), &interfaces) < 0)
            {
                bdb_entity_free_strings(&interfaces);
                free(next_scope);
                return -1;
            }
        }
    }

    const char *child_scope = next_scope ? next_scope : scope;
    uint32_t count = ts_node_named_child_count(node);
    for(uint32_t i = 0; i < count; i++)
    {
        if(bdb_entity_component_pass(ts_node_named_child(node, i), src, package,
                child_scope, data) < 0)
        {
            free(next_scope);
            return -1;
        }
    }
    free(next_scope);
    return 0;
}

static bdb_entity_component_t *bdb_entity_component_for_interface(
        bdb_entity_data_t *data,
        const char *interface_name)
{
    size_t len = strlen(interface_name);
    if(len < 2 || interface_name[len - 1] != 'c')
        return NULL;
    char *component_name = bdb__sprintf("%.*sComp", (int)(len - 1), interface_name);
    if(!component_name)
        return NULL;
    bdb_entity_component_t *found = NULL;
    for(size_t i = 0; i < data->components.count; i++)
    {
        bdb_entity_component_t *candidate = &bdb__arr_get(&data->components, bdb_entity_component_t, i);
        if(!strcmp(candidate->name, component_name))
        {
            found = candidate;
            break;
        }
    }
    free(component_name);
    return found;
}

static int bdb_entity_component_closure(
        bdb_entity_data_t *data,
        bdb_entity_component_t *component,
        bdb__arr_t *out,
        unsigned depth)
{
    if(depth > data->components.count + 1)
        return -1;
    if(bdb_entity_add_string(out, component->name) < 0)
        return -1;

    for(size_t i = 0; i < component->interfaces.count; i++)
    {
        char *interface_name = bdb__arr_get(&component->interfaces, char*, i);
        bdb_entity_component_t *dependency =
            bdb_entity_component_for_interface(data, interface_name);
        if(dependency && dependency != component &&
           bdb_entity_component_closure(data, dependency, out, depth + 1) < 0)
            return -1;
    }

    if(!component->is_base)
    {
        for(size_t i = 0; i < data->components.count; i++)
        {
            bdb_entity_component_t *base = &bdb__arr_get(&data->components, bdb_entity_component_t, i);
            if(base->is_base && bdb_entity_component_closure(data, base, out, depth + 1) < 0)
                return -1;
        }
    }
    return 0;
}

static int bdb_entity_property_id(bdb_entity_data_t *data, const char *key, int32_t *id)
{
    for(size_t i = 0; i < data->properties.count; i++)
    {
        bdb_entity_property_t *property = &bdb__arr_get(&data->properties, bdb_entity_property_t, i);
        if(!strcmp(property->key, key))
        {
            *id = property->id;
            return 1;
        }
    }
    return 0;
}

static bdb_entity_info_t *bdb_entity_find_or_add(
        bdb_entity_data_t *data,
        const char *name,
    bdb__arr_t *direct,
    int *is_new)
{
    for(size_t i = 0; i < data->entities.count; i++)
    {
        bdb_entity_info_t *entity = &bdb__arr_get(&data->entities, bdb_entity_info_t, i);
        if(!strcmp(entity->name, name))
        {
            *is_new = 0;
            return entity;
        }
    }

    bdb_entity_info_t entity = {0};
    entity.name = bdb__tokdup(name, strlen(name));
    entity.id = -1;
    entity.aliases.elem_size = sizeof(char*);
    entity.direct_components = *direct;
    memset(direct, 0, sizeof(*direct));
    entity.components.elem_size = sizeof(char*);
    entity.aliases.elem_size = sizeof(char*);
    if(!entity.name || bdb__arr_append(&data->entities, &entity) < 0)
    {
        free(entity.name);
        bdb_entity_free_strings(&entity.direct_components);
        return NULL;
    }
    *is_new = 1;
    return &bdb__arr_get(&data->entities, bdb_entity_info_t, data->entities.count - 1);
}

static char *bdb_entity_legacy_name(const char *base, const char *suffix)
{
    char *capitalized = bdb__tokdup(suffix, strlen(suffix));
    if(!capitalized)
        return NULL;
    if(*capitalized)
        capitalized[0] = (char)toupper((unsigned char)capitalized[0]);
    char *name = bdb__sprintf("%sLegacy%s", base, capitalized);
    free(capitalized);
    return name;
}

static char *bdb_entity_class_name(bdb__arr_t *direct)
{
    char **names = calloc(direct->count ? direct->count : 1, sizeof(char*));
    if(!names)
        return NULL;
    for(size_t i = 0; i < direct->count; i++)
    {
        const char *interface_name = bdb__arr_get(direct, char*, i);
        bdb_entity_component_t *component = NULL;
        (void)component;
        size_t len = strlen(interface_name);
        if(len < 2 || interface_name[len - 1] != 'c')
        {
            free(names);
            return NULL;
        }
        names[i] = bdb__sprintf("%.*s", (int)(len - 1), interface_name);
        if(!names[i])
        {
            for(size_t j = 0; j < i; j++) free(names[j]);
            free(names);
            return NULL;
        }
    }
    qsort(names, direct->count, sizeof(char*), bdb_entity_strcmp_ptr);
    size_t total = 1;
    for(size_t i = 0; i < direct->count; i++)
        total += strlen(names[i]);
    char *result = malloc(total);
    if(result)
    {
        result[0] = '\0';
        for(size_t i = 0; i < direct->count; i++)
            strcat(result, names[i]);
    }
    for(size_t i = 0; i < direct->count; i++) free(names[i]);
    free(names);
    return result;
}

static int bdb_entity_parse_definition(
        TSNode annotation,
        const char *src,
        const char *entity_key,
        const char *type_name,
        int class_definition,
        bdb_entity_data_t *data,
        const char *source_path)
{
    bdb__arr_t direct = { .elem_size = sizeof(char*) };
    int legacy = 0;
    if(bdb_entity_parse_annotation(annotation, src, &direct, &legacy) < 0)
    {
        bdb__error("failed to parse EntityDef in %s", source_path);
        bdb_entity_free_strings(&direct);
        return -1;
    }

    char *generated_name = NULL;
    if(class_definition)
    {
        generated_name = bdb__tokdup(type_name, strlen(type_name));
        const char *suffixes[] = {"Def", "Comp"};
        for(size_t i = 0; i < 2 && generated_name; i++)
        {
            size_t len = strlen(generated_name), suffix_len = strlen(suffixes[i]);
            if(len >= suffix_len && !strcmp(generated_name + len - suffix_len, suffixes[i]))
                generated_name[len - suffix_len] = '\0';
        }
    }
    else
    {
        generated_name = bdb_entity_class_name(&direct);
        if(generated_name && legacy)
        {
            char *legacy_name = bdb_entity_legacy_name(generated_name, entity_key);
            free(generated_name);
            generated_name = legacy_name;
        }
    }
    if(!generated_name || !*generated_name)
    {
        bdb__error("failed to determine generated entity name for %s", entity_key);
        free(generated_name);
        bdb_entity_free_strings(&direct);
        return -1;
    }

    int is_new = 0;
    bdb_entity_info_t *entity = bdb_entity_find_or_add(data, generated_name, &direct, &is_new);
    if(!entity)
    {
        free(generated_name);
        bdb_entity_free_strings(&direct);
        return -1;
    }

    if(bdb_entity_add_kebab_alias(&entity->aliases, generated_name) < 0)
    {
        free(generated_name);
        bdb_entity_free_strings(&direct);
        return -1;
    }
    free(generated_name);

    for(size_t i = 0; i < direct.count; i++)
        if(bdb_entity_add_string(&entity->direct_components, bdb__arr_get(&direct, char*, i)) < 0)
            return -1;

    if(!class_definition)
    {
        if(bdb_entity_add_string(&entity->aliases, entity_key) < 0)
            return -1;
        if(bdb_entity_add_kebab_alias(&entity->aliases, entity_key) < 0)
            return -1;
    }

    int32_t id = 0;
    if(is_new && bdb_entity_property_id(data, entity_key, &id))
    {
        if(entity->has_id && entity->id != id)
        {
            bdb__error("entity %s has conflicting IDs %d and %d", entity->name, entity->id, id);
            return -1;
        }
        entity->id = id;
        entity->has_id = 1;
    }

    bdb_entity_free_strings(&direct);
    return 0;
}

static int bdb_entity_entity_pass(
        TSNode node,
        const char *src,
        const char *package,
        const char *scope,
        bdb_entity_data_t *data,
        const char *source_path)
{
    const char *kind = ts_node_type(node);
    char *next_scope = NULL;
    if(!strcmp(kind, "class_declaration"))
    {
        TSNode name_node = ts_node_child_by_field_name(node, "name", 4);
        char *simple = bdb_entity_node_text_from_src(name_node, src);
        if(!simple)
            return -1;
        next_scope = scope && *scope ? bdb__sprintf("%s.%s", scope, simple) :
            (package && *package ? bdb__sprintf("%s.%s", package, simple) :
                bdb__tokdup(simple, strlen(simple)));
        if(!next_scope)
        {
            free(simple);
            return -1;
        }

        TSNode modifiers = bdb_entity_child_of_type(node, "modifiers");
        TSNode annotation = bdb_entity_annotation(modifiers, src, "EntityDef");
        if(!ts_node_is_null(annotation) &&
           bdb_entity_parse_definition(annotation, src, next_scope, simple, 1, data, source_path) < 0)
        {
            free(simple);
            free(next_scope);
            return -1;
        }
        free(simple);
    }
    else if(!strcmp(kind, "field_declaration"))
    {
        TSNode modifiers = bdb_entity_child_of_type(node, "modifiers");
        TSNode annotation = bdb_entity_annotation(modifiers, src, "EntityDef");
        if(!ts_node_is_null(annotation))
        {
            uint32_t count = ts_node_named_child_count(node);
            for(uint32_t i = 0; i < count; i++)
            {
                TSNode declarator = ts_node_named_child(node, i);
                if(strcmp(ts_node_type(declarator), "variable_declarator"))
                    continue;
                TSNode name_node = ts_node_child_by_field_name(declarator, "name", 4);
                char *name = bdb_entity_node_text_from_src(name_node, src);
                if(!name)
                    return -1;
                if(bdb_entity_parse_definition(annotation, src, name, NULL, 0, data, source_path) < 0)
                {
                    free(name);
                    return -1;
                }
                free(name);
            }
        }
    }

    const char *child_scope = next_scope ? next_scope : scope;
    uint32_t count = ts_node_named_child_count(node);
    for(uint32_t i = 0; i < count; i++)
    {
        if(bdb_entity_entity_pass(ts_node_named_child(node, i), src, package,
                child_scope, data, source_path) < 0)
        {
            free(next_scope);
            return -1;
        }
    }
    free(next_scope);
    return 0;
}

static int bdb_entity_parse_files(
        TSParser *parser,
        const char *source_root,
        bdb_entity_data_t *data,
        int components_pass)
{
    bdb__arr_t files = { .elem_size = sizeof(char*) };
    if(bdb__fs_collect_files(source_root, &files) < 0)
        return -1;
    qsort(files.data, files.count, files.elem_size, bdb_entity_strcmp_ptr);

    int result = 0;
    for(size_t i = 0; i < files.count; i++)
    {
        char *path = bdb__arr_get(&files, char*, i);
        size_t path_len = strlen(path);
        if(path_len < 5 || strcmp(path + path_len - 5, ".java"))
            continue;

        char *src = NULL;
        long src_len = 0;
        TSTree *tree = NULL;
        char *package = NULL;
        if(bdb__read_file(path, &src, &src_len) < 0)
        {
            result = -1;
            continue;
        }
        tree = ts_parser_parse_string(parser, NULL, src, (uint32_t)src_len);
        if(!tree)
        {
            free(src);
            result = -1;
            continue;
        }
        TSNode root = ts_tree_root_node(tree);
        package = bdb_entity_package(root, src);
        if(!package)
            result = -1;
        else if(components_pass)
        {
            if(bdb_entity_component_pass(root, src, package, NULL, data) < 0)
                result = -1;
        }
        else if(bdb_entity_entity_pass(root, src, package, NULL, data, path) < 0)
        {
            result = -1;
        }
        free(package);
        ts_tree_delete(tree);
        free(src);
    }

    bdb__arr_destroy_strings(&files);
    return result;
}

static int bdb_entity_read_properties(const char *path, bdb_entity_data_t *data)
{
    FILE *file = fopen(path, "r");
    if(!file)
    {
        bdb__error_errno("failed to open entity ID file %s", path);
        return -1;
    }

    char line[4096];
    while(fgets(line, sizeof(line), file))
    {
        char *start = line;
        while(isspace((unsigned char)*start)) start++;
        if(!*start || *start == '#' || *start == '!')
            continue;
        char *equals = strchr(start, '=');
        if(!equals)
            continue;
        char *key_end = equals;
        while(key_end > start && isspace((unsigned char)key_end[-1])) key_end--;
        *key_end = '\0';
        char *value = equals + 1;
        while(isspace((unsigned char)*value)) value++;
        char *value_end = value + strlen(value);
        while(value_end > value && isspace((unsigned char)value_end[-1])) value_end--;
        *value_end = '\0';
        char *end = NULL;
        long id = strtol(value, &end, 10);
        if(!*start || end == value || *end || id < 0 || id > 255)
        {
            bdb__error("invalid entity ID property in %s: %s=%s", path, start, value);
            fclose(file);
            return -1;
        }
        bdb_entity_property_t property = {0};
        property.key = bdb__tokdup(start, strlen(start));
        property.id = (int32_t)id;
        if(!property.key || bdb__arr_append(&data->properties, &property) < 0)
        {
            free(property.key);
            fclose(file);
            return -1;
        }
    }
    if(ferror(file))
    {
        bdb__error_errno("failed to read entity ID file %s", path);
        fclose(file);
        return -1;
    }
    fclose(file);
    return 0;
}

static int bdb_entity_resolve(bdb_entity_data_t *data)
{
    for(size_t i = 0; i < data->entities.count; i++)
    {
        bdb_entity_info_t *entity = &bdb__arr_get(&data->entities, bdb_entity_info_t, i);
        if(!entity->has_id)
        {
            bdb__error("no class ID found for generated entity %s", entity->name);
            return -1;
        }
        qsort(entity->direct_components.data, entity->direct_components.count,
                entity->direct_components.elem_size, bdb_entity_strcmp_ptr);
        bdb__arr_t direct_classes = { .elem_size = sizeof(char*) };
        for(size_t j = 0; j < entity->direct_components.count; j++)
        {
            char *interface_name = bdb__arr_get(&entity->direct_components, char*, j);
            bdb_entity_component_t *component =
                bdb_entity_component_for_interface(data, interface_name);
            if(!component)
            {
                bdb__error("no component class found for entity interface %s", interface_name);
                bdb_entity_free_strings(&direct_classes);
                return -1;
            }
            if(bdb_entity_add_string(&direct_classes, component->name) < 0)
            {
                bdb_entity_free_strings(&direct_classes);
                return -1;
            }
            if(bdb_entity_component_closure(data, component, &entity->components, 0) < 0)
            {
                bdb_entity_free_strings(&direct_classes);
                return -1;
            }
        }
        bdb_entity_free_strings(&entity->direct_components);
        entity->direct_components = direct_classes;
        qsort(entity->components.data, entity->components.count,
                entity->components.elem_size, bdb_entity_strcmp_ptr);
        qsort(entity->aliases.data, entity->aliases.count,
                entity->aliases.elem_size, bdb_entity_strcmp_ptr);
    }
    qsort(data->entities.data, data->entities.count, data->entities.elem_size,
            bdb_entity_compare_id);
    return 0;
}

static int bdb_entity_write_strings(FILE *file, const bdb__arr_t *strings)
{
    if(!strings->count)
    {
        fputs("NULL", file);
        return 0;
    }
    fputs("{ ", file);
    for(size_t i = 0; i < strings->count; i++)
    {
        if(i) fputs(", ", file);
        fprintf(file, "\"%s\"", bdb__arr_get(strings, char*, i));
    }
    fputs(" }", file);
    return 0;
}

static int bdb_entity_emit(
        const bdb_entity_output_t *output,
        bdb_entity_data_t *data)
{
    char *include_dir = bdb__fs_cat_path(output->root_path, output->include_dir);
    char *src_dir = bdb__fs_cat_path(output->root_path, output->src_dir);
    char *header_path = include_dir ? bdb__fs_cat_fname(include_dir, output->stem, output->header_ext) : NULL;
    char *source_path = src_dir ? bdb__fs_cat_fname(src_dir, output->stem, output->source_ext) : NULL;
    if(!include_dir || !src_dir || !header_path || !source_path)
        goto failed;
    if(bdb__fs_ensure_dir(include_dir) < 1 || bdb__fs_ensure_dir(src_dir) < 1)
        goto failed;

    FILE *header = fopen(header_path, "w");
    if(!header)
    {
        bdb__error_errno("failed to open output header %s", header_path);
        goto failed;
    }
    FILE *source = fopen(source_path, "w");
    if(!source)
    {
        bdb__error_errno("failed to open output source %s", source_path);
        fclose(header);
        goto failed;
    }

    fputs("#ifndef MSAV_ENTITIES_GEN_H\n#define MSAV_ENTITIES_GEN_H\n\n"
          "#include <stddef.h>\n#include <stdint.h>\n\n", header);
    fputs("typedef struct\n{\n"
          "    int32_t id;\n"
          "    const char *name;\n"
          "    const char *const *aliases;\n"
          "    size_t alias_count;\n"
          "    const char *const *direct_components;\n"
          "    size_t direct_component_count;\n"
          "    const char *const *components;\n"
          "    size_t component_count;\n"
          "} msav_entity_info_t;\n\n", header);
    fputs("enum\n{\n", header);
    for(size_t i = 0; i < data->entities.count; i++)
    {
        bdb_entity_info_t *entity = &bdb__arr_get(&data->entities, bdb_entity_info_t, i);
        fputs("    MSAV_ENTITY_", header);
        for(const unsigned char *p = (const unsigned char*)entity->name; *p; p++)
        {
            if(p != (const unsigned char*)entity->name && isupper(*p))
                fputc('_', header);
            fputc(isalnum(*p) ? toupper(*p) : '_', header);
        }
        fprintf(header, " = %d%s\n", entity->id,
                i + 1 == data->entities.count ? "" : ",");
    }
    fputs("};\n\nextern const msav_entity_info_t msav_entity_db[];\n"
          "extern const size_t msav_entity_db_count;\n"
          "const msav_entity_info_t *msav_entity_by_id(int32_t id);\n"
          "const msav_entity_info_t *msav_entity_by_name(const char *name);\n\n#endif\n", header);

    fprintf(source, "#include \"%s.%s\"\n\n#include <string.h>\n\n",
            output->stem, output->header_ext);
    for(size_t i = 0; i < data->entities.count; i++)
    {
        bdb_entity_info_t *entity = &bdb__arr_get(&data->entities, bdb_entity_info_t, i);
        if(entity->aliases.count)
        {
            fprintf(source, "static const char *const msav_entity_%zu_aliases[] = ", i);
            bdb_entity_write_strings(source, &entity->aliases);
            fputs(";\n", source);
        }
        if(entity->direct_components.count)
        {
            fprintf(source, "static const char *const msav_entity_%zu_direct[] = ", i);
            bdb_entity_write_strings(source, &entity->direct_components);
            fputs(";\n", source);
        }
        if(entity->components.count)
        {
            fprintf(source, "static const char *const msav_entity_%zu_components[] = ", i);
            bdb_entity_write_strings(source, &entity->components);
            fputs(";\n", source);
        }
    }
    fputs("\nconst msav_entity_info_t msav_entity_db[] =\n{\n", source);
    for(size_t i = 0; i < data->entities.count; i++)
    {
        bdb_entity_info_t *entity = &bdb__arr_get(&data->entities, bdb_entity_info_t, i);
        char aliases[64], direct[64], components[64];
        if(entity->aliases.count)
            snprintf(aliases, sizeof(aliases), "msav_entity_%zu_aliases", i);
        if(entity->direct_components.count)
            snprintf(direct, sizeof(direct), "msav_entity_%zu_direct", i);
        if(entity->components.count)
            snprintf(components, sizeof(components), "msav_entity_%zu_components", i);
        fprintf(source,
                "    { %d, \"%s\", %s, %zu, %s, %zu, %s, %zu }%s\n",
                entity->id, entity->name,
                entity->aliases.count ? aliases : "NULL",
                entity->aliases.count,
                entity->direct_components.count ? direct : "NULL",
                entity->direct_components.count,
                entity->components.count ? components : "NULL",
                entity->components.count,
                i + 1 == data->entities.count ? "" : ",");
    }
    fputs("};\n\nconst size_t msav_entity_db_count = sizeof(msav_entity_db) / sizeof(msav_entity_db[0]);\n\n"
          "const msav_entity_info_t *msav_entity_by_id(int32_t id)\n{\n"
          "    for(size_t i = 0; i < msav_entity_db_count; i++)\n"
          "        if(msav_entity_db[i].id == id) return &msav_entity_db[i];\n"
          "    return NULL;\n}\n\n"
          "const msav_entity_info_t *msav_entity_by_name(const char *name)\n{\n"
          "    if(!name) return NULL;\n"
          "    for(size_t i = 0; i < msav_entity_db_count; i++)\n"
          "    {\n"
          "        const msav_entity_info_t *entity = &msav_entity_db[i];\n"
          "        if(!strcmp(entity->name, name)) return entity;\n"
          "        for(size_t j = 0; j < entity->alias_count; j++)\n"
          "            if(!strcmp(entity->aliases[j], name)) return entity;\n"
          "    }\n"
          "    return NULL;\n}\n", source);

    int failed = ferror(header) || ferror(source);
    if(fclose(header) != 0 || fclose(source) != 0)
        failed = 1;
    free(include_dir); free(src_dir); free(header_path); free(source_path);
    return failed ? -1 : 0;

failed:
    free(include_dir); free(src_dir); free(header_path); free(source_path);
    return -1;
}

static void bdb_entity_data_destroy(bdb_entity_data_t *data)
{
    for(size_t i = 0; i < data->components.count; i++)
    {
        bdb_entity_component_t *component = &bdb__arr_get(&data->components, bdb_entity_component_t, i);
        free(component->name);
        free(component->full_name);
        bdb_entity_free_strings(&component->interfaces);
    }
    for(size_t i = 0; i < data->properties.count; i++)
        free(bdb__arr_get(&data->properties, bdb_entity_property_t, i).key);
    for(size_t i = 0; i < data->entities.count; i++)
    {
        bdb_entity_info_t *entity = &bdb__arr_get(&data->entities, bdb_entity_info_t, i);
        free(entity->name);
        bdb_entity_free_strings(&entity->aliases);
        bdb_entity_free_strings(&entity->direct_components);
        bdb_entity_free_strings(&entity->components);
    }
    bdb__arr_destroy(&data->components);
    bdb__arr_destroy(&data->properties);
    bdb__arr_destroy(&data->entities);
}

void bdb_entity_gen_main(void *args)
{
    bdb_entity_gen_args_t *options = args;
    bdb_entity_data_t data = {0};
    data.components.elem_size = sizeof(bdb_entity_component_t);
    data.properties.elem_size = sizeof(bdb_entity_property_t);
    data.entities.elem_size = sizeof(bdb_entity_info_t);

    char *source_root = bdb__fs_cat_path(options->mindustry_root, "core/src/mindustry");
    char *properties = bdb__fs_cat_path(options->mindustry_root,
            "annotations/src/main/resources/classids.properties");
    TSParser *parser = ts_parser_new();
    if(!source_root || !properties || !parser)
    {
        bdb__error("failed to initialize entity generator");
        goto cleanup;
    }
    ts_parser_set_language(parser, tree_sitter_java());

    if(bdb_entity_read_properties(properties, &data) < 0)
        goto cleanup;
    if(bdb_entity_parse_files(parser, source_root, &data, 1) < 0)
    {
        bdb__error("failed to discover entity components in %s", source_root);
        goto cleanup;
    }
    if(bdb_entity_parse_files(parser, source_root, &data, 0) < 0)
    {
        bdb__error("failed to discover EntityDef declarations in %s", source_root);
        goto cleanup;
    }
    if(bdb_entity_resolve(&data) < 0)
    {
        bdb__error("failed to resolve entity registry metadata");
        goto cleanup;
    }

    bdb_entity_output_t output = {
        options->root_path ? options->root_path : ".",
        options->include_dir ? options->include_dir : "include",
        options->src_dir ? options->src_dir : "src",
        options->stem ? options->stem : "msav_entities_gen",
        options->header_ext ? options->header_ext : "h",
        options->source_ext ? options->source_ext : "c"
    };
    if(bdb_entity_emit(&output, &data) < 0)
        bdb__error("failed to emit entity registry");
    else
        bdb__info("generated metadata for %zu entities", data.entities.count);

cleanup:
    if(parser) ts_parser_delete(parser);
    free(source_root);
    free(properties);
    bdb_entity_data_destroy(&data);
}
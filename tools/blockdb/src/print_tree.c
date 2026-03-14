#include "print_tree.h"

#include <stdio.h>
#include <stdlib.h>

#include <aparse.h>
#include <tree_sitter/api.h>

#include "bdb_utils.h"


#define ANSI_RESET      "\x1b[0m"
#define ANSI_BOLD       "\x1b[1m"

#define ANSI_FG_YELLOW  "\x1b[33m"
#define ANSI_FG_GREEN   "\x1b[32m"
#define ANSI_FG_CYAN    "\x1b[36m"
#define ANSI_FG_GRAY    "\x1b[90m"

#define bdb__colorize(col, str) __aparse_ansies(col) str __aparse_ansies(ANSI_RESET)

static void bdb__print_ast(
        TSTreeCursor *cursor,
        const char *src,
        const int indent)
{
    TSNode node = {0};
    const char *type = 0;
    const char *field = 0;
    uint32_t start = 0;
    uint32_t end = 0;
    uint32_t child_count = 0;
    int i = 0;

    node = ts_tree_cursor_current_node(cursor);
    if(!ts_node_is_named(node))
        return;

    type = ts_node_type(node);
    field = ts_tree_cursor_current_field_name(cursor);
    child_count = ts_node_child_count(node);

    start = ts_node_start_byte(node);
    end = ts_node_end_byte(node);

    printf("%*s", indent * 2, "");

    if (field)
        printf(bdb__colorize(ANSI_FG_CYAN, "%s") ": ", field);

    printf(bdb__colorize(ANSI_FG_GREEN, "%s"), type);

    if (child_count == 0) {
        printf(" = " bdb__colorize(ANSI_FG_YELLOW, "\"%.*s\""), 
                (int)(end - start), src + start);
    }

    printf("\n");

    if (ts_tree_cursor_goto_first_child(cursor)) {
        do {
            bdb__print_ast(cursor, src, indent + 1);
        } while (ts_tree_cursor_goto_next_sibling(cursor));

        ts_tree_cursor_goto_parent(cursor);
    }
}

extern const TSLanguage *tree_sitter_java(void);
void print_tree_command(void *args)
{
    const char *path = *(const char**)args;

    char *src = 0;
    long src_len = 0;

    TSParser *parser = 0;
    TSTree *tree = 0;
    TSNode root = {0};

    TSTreeCursor cursor = {0};

    if (bdb__read_file(path, &src, &src_len) < 0)
        return;

    parser = ts_parser_new();
    ts_parser_set_language(parser, tree_sitter_java());

    tree = ts_parser_parse_string(parser, 0, src, (uint32_t)src_len);

    root = ts_tree_root_node(tree);
    cursor = ts_tree_cursor_new(root);

    bdb__print_ast(&cursor, src, 0);

    ts_tree_cursor_delete(&cursor);
    ts_tree_delete(tree);
    ts_parser_delete(parser);

    free(src);
}

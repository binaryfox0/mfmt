#include <aparse.h>

#include "print_tree.h"
#include "generate.h"

int main(int argc, char **argv)
{
    aparse_arg print_tree_args[] = {
        aparse_arg_string("src", 0, 0, 
                "Path to Java source code file"),
        aparse_arg_end_marker
    };
    aparse_arg generate_args[] = {
        aparse_arg_string("dir", 0, 0, 
                "Path to the directory contain Mindustry's source code\n"
        ),
        aparse_arg_string("out", 0, 0, 
                "Path to the output directory, must contain include/src subfolder\n"
        ),
        aparse_arg_end_marker
    };
    aparse_arg commands[] = {
        aparse_arg_subparser_impl(
                "print-tree", print_tree_args, print_tree_command, 
                "Print AST of a Java source file", 
                (int[]){0, 8}, 1
        ),
        aparse_arg_subparser_impl("generate", generate_args, generate_command,
                "Generate block database from Mindustry's source code", 
                (int[]){0, 8, 8, 16}, 2
        ),
        aparse_arg_end_marker
    };
    aparse_arg main_args[] = {
        aparse_arg_parser("command", commands),
        aparse_arg_end_marker
    };

    if(aparse_parse(argc, argv, main_args, 0, "Generate Mindustry's block database") != APARSE_STATUS_OK)
        return 1;

    return 0;
}

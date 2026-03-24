#include <aparse.h>

#include "print_tree.h"
#include "gen_cmd.h"

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
        aparse_arg_option("-rd", "--root-dir", 0, 0, APARSE_ARG_TYPE_STRING,
                "Path to the output directory (default: cwd)\n"
        ),
        aparse_arg_option("-id", "--include-dir", 0, 0, APARSE_ARG_TYPE_STRING,
                "Name of the output include directory (default: include)\n"
        ),
        aparse_arg_option("-sd", "--source-dir", 0, 0, APARSE_ARG_TYPE_STRING,
                "Name of the output source directory (default: src)\n"
        ),
        aparse_arg_option("-s", "--stem", 0, 0, APARSE_ARG_TYPE_STRING,
                "Stem of generated file name (default: msav_blocks_gen)\n"
        ),
        aparse_arg_option("-hx", "--header-ext", 0, 0, APARSE_ARG_TYPE_STRING,
                "Extension of generated header file (default: h)\n"
        ),
        aparse_arg_option("-sx", "--source-ext", 0, 0, APARSE_ARG_TYPE_STRING,
                "Extension of generated source file (default: c)\n"
        ),
        aparse_arg_end_marker
    };
    aparse_arg commands[] = {
        aparse_arg_subparser_impl(
                "print-tree", print_tree_args, print_tree_command, 
                "Print AST of a Java source file", 
                (int[]){0, 8}, 1
        ),
        aparse_arg_subparser("generate", generate_args, bdb_gen_main,
                "Generate block database from Mindustry's source code", 
                bdb_gen_args_t, mindustry_root, root_path, include_dir, src_dir, 
                stem, header_ext, source_ext
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

#ifndef BLOCKDB_GEN_H
#define BLOCKDB_GEN_H

typedef struct
{
    const char *mindustry_root;
    const char *root_path;
    const char *include_dir;
    const char *src_dir;
    const char *stem;
    const char *header_ext;
    const char *source_ext;
} bdb_gen_args_t;
void bdb_gen_main(void *args);

#endif

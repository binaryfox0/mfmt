#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#include <sys/stat.h>
#ifdef _WIN32
#   include <direct.h>

#   define __stat _stat
#   define S_ISDIR(m) ((m & _S_IFDIR) != 0)

typedef struct _stat _stat;
#else
#   include <unistd.h>

#   define _mkdir(path) mkdir((path), 0755)
#   define __stat stat

typedef struct stat _stat;
#endif

#include <aparse.h>
#include <aatls.h>
#include <curses.h>
#include <stb_image.h>
#include <stb_image_write.h>

#define SPACE_PER_INDENT 4

#ifdef _WIN32
#   define PATH_SEP "\\"
#else
#   define PATH_SEP "/"
#endif

#define BOOLEAN(v) ((v) ? "true" : "false")

static int indent = 0;
#define SPACE_PER_INDENT 4
#define print(fmt, ...) \
    aparse_prog_info("%*s" fmt, indent * SPACE_PER_INDENT, "", ##__VA_ARGS__)

static inline const char *wrap_to_string(const aatls_wrap_t wrap)
{
    switch(wrap)
    {
        case AATLS_WRAP_MIRRORED_REPEAT:
            return "GL_MIRRORED_REPEAT";
        case AATLS_WRAP_CLAMP_TO_EDGE:
            return "GL_CLAMP_TO_EDGE";
        case AATLS_WRAP_REPEAT:
            return "GL_REPEAT";
        default:
            return "UNKNOWN";
    }
}

static inline const char *filter_to_string(const aatls_filter_t filter)
{
    switch(filter)
    {
        case AATLS_FILTER_NEAREST:
            return "GL_NEAREST";
        case AATLS_FILTER_LINEAR:
            return "GL_LINEAR";
        case AATLS_FILTER_MIPMAP:
        case AATLS_FILTER_MIPMAP_LINEAR_LINEAR:
            return "GL_LINEAR_MIPMAP_LINEAR";
        case AATLS_FILTER_MIPMAP_NEAREST_NEAREST:
            return "GL_NEAREST_MIPMAP_NEAREST";
        case AATLS_FILTER_MIPMAP_LINEAR_NEAREST:
            return "GL_LINEAR_MIPMAP_NEAREST";
        default:
            return "UNKNOWN";
    }
}

static aatls_atlas_t atlas = {0};
void list_all_command(void *data)
{
    (void)data;

    print("atlas version: versrion %d", atlas.version);
    for(uint32_t i = 0; i < atlas.page_count; i++)
    {
        aatls_page_t *page = &atlas.pages[i];
        print("page name: \"%s\"", page->name);
        indent++;
        print("size: %dx%d pixel", page->width, page->height);
        print("filter min: %s (val: %d -> gl val: 0x%04X)", 
                filter_to_string(page->filter_min), page->filter_min,
                aatls_gl_filter(page->filter_min)
        );
        print("filter mag: %s (val: %d -> gl val: 0x%04X)", 
                filter_to_string(page->filter_mag), page->filter_mag,
                aatls_gl_filter(page->filter_mag)
        );
        print("wrap u: %s (val: %d -> gl val: 0x%04X)",
                wrap_to_string(page->u_wrap), page->u_wrap, 
                aatls_gl_wrap(page->u_wrap)
        );
        print("wrap v: %s (val: %d -> gl val: 0x%04X)",
                wrap_to_string(page->v_wrap), page->v_wrap, 
                aatls_gl_wrap(page->v_wrap)
        );

        print("region count: %d regions", page->region_count);
        indent++;
        for(int32_t j = 0; j < page->region_count; j++)
        {
            aatls_region_t *region = &page->regions[j];
            print("region name: \"%s\"", region->name);
            indent++;
            print("pos: %d,%d", region->left, region->top);
            print("size: %dx%d pixel", region->width, region->height);

            print("has offsets: %s", BOOLEAN(region->has_offsets));
            indent++;
            if(region->has_offsets)
            {
                print("offset pos: %d,%d", region->offset_x, region->offset_y);
                print("original size: %dx%d pixel", region->original_width, region->original_height);
            }
            indent--;

            print("has splits: %s", BOOLEAN(region->has_splits));
            indent++;
            if(region->has_splits)
            {
                print("left: %d", region->splits[0]);
                print("right: %d", region->splits[1]);
                print("top: %d", region->splits[2]);
                print("bottom: %d", region->splits[3]);
            }
            indent--;

            print("has pads: %s", BOOLEAN(region->has_pads));
            indent++;
            if(region->has_pads)
            {
                print("left: %d", region->pads[0]);
                print("right: %d", region->pads[1]);
                print("top: %d", region->pads[2]);
                print("bottom: %d", region->pads[3]);
            }
            indent--;

            print("flip: %s", BOOLEAN(region->flip));
            indent--;
        }
        indent -= 2;
    }
}

char* concat_path(
        const char *dir, 
        const char *file,
        const char *ext
)
{
    size_t len = 0;
    char *out = 0;
    if(!dir || !file)
        return 0;

    len = strlen(dir) + strlen(file) + (ext ? strlen(ext) : 0) + 2;
    out = malloc(len);
    if(!out)
        return 0;
    
    snprintf(out, len, "%s%s%s%s", 
            dir, PATH_SEP, file, ext ? ext : ""); 
    return out;
}

int confirm(const char *label)
{
    char buf[16] = {0};
    char *p = 0;

    while(1)
    {
        printf("%s: " __aparse_ansies("\x1b[1;34m") "info" __aparse_ansies("\x1b[0m") ": %s (y/n): ", __aparse_progname, label); 
        if(!fgets(buf, sizeof(buf), stdin))
            return 0;
        p = buf;
        while(isspace((int)*p)) p++;

        if (*p == 'y' || *p == 'Y')
            return 1;
        if (*p == 'n' || *p == 'N')
            return 0;
        else
            aparse_prog_error("please answer yes or no");
    }
}

static int create_directory(const char *path)
{
    if(_mkdir(path) == -1)
    {
        aparse_prog_error("failed to create directory at \"%s\"", path);
        return 0;
    }
    return 1;
}

static int ensure_directory(const char *path)
{
    _stat st = {0};
    if(__stat(path, &st) == -1)
    {
        if(errno == ENOENT)
            return create_directory(path);
        else {
            aparse_prog_error("failed to retrive info of \"%s\"", path);
            aparse_prog_info("reason: %s", strerror(errno));
            return 1;
        }
    }
    else {
        if(!S_ISDIR(st.st_mode))
        {
            aparse_prog_error("\"%s\" was not a directory", path);
            if(confirm("do you want to delete it?"))
            {
                if(remove("path") == -1)
                {
                    aparse_prog_error("failed to remove the file at \"%s\"", path);
                    aparse_prog_info("reason: %s", strerror(errno));
                    return 0;
                }
                return create_directory(path);
            }
            else
                return 0;
        }
            
        aparse_prog_warn("directory \"%s\" already existed", path);
        if(confirm("do you want to write into it?"))
            return 1;
        else
            return 0;
    }
}

uint8_t *crop_texture(
    const uint8_t *data,
    const int img_w,
    const int img_h,
    const int img_ch,
    const aatls_region_t *region
){
    int16_t x = region->left;
    int16_t y = region->top;
    int16_t w = region->width;
    int16_t h = region->height;

    uint32_t src_stride = 0;
    uint32_t dst_stride = 0;

    const uint8_t *src;
    uint8_t *dst;
    uint8_t *out;

    uint32_t out_size;

    if (!data || !region || img_w <= 0 || img_h <= 0 || img_ch <= 0)
        return 0;

    if (x < 0 || y < 0 || w <= 0 || h <= 0)
        return 0;

    if ((x + w) > img_w || (y + h) > img_h)
        return 0;

    src_stride = (uint32_t)(img_w * img_ch);
    dst_stride = (uint32_t)(w * img_ch);
    out_size   = (uint32_t)(w * h * img_ch);

    out = (uint8_t *)malloc(out_size);
    if (!out)
        return 0;

    src = data + (uint32_t)((y * img_w + x) * img_ch);
    dst = out;

    for (int16_t row = 0; row < h; row++) {
        memcpy(dst, src, dst_stride);
        src += src_stride;
        dst += dst_stride;
    }

    return out;
}

typedef struct extract_regions_data
{
    const char *imgdir;
    const char *outdir;
} extract_regions_data_t;

void extract_regions_command(void *raw)
{
    extract_regions_data_t *data = raw;
    const char *images_dir = data->imgdir;
    const char *out_dir = data->outdir ? data->outdir : "out";

    if(!ensure_directory(out_dir))
    {
        aparse_prog_error("aborted.");
        return;
    }

    for(uint32_t i = 0; i < atlas.page_count; i++)
    {
        aatls_page_t *page = &atlas.pages[i];
        char *image_path = 0;
        stbi_uc *img = 0;
        int img_w = 0, img_h = 0;
        int img_ch = 0;

        image_path = concat_path(images_dir, page->name, 0);
        if(!image_path)
        {
            aparse_prog_error("failed to construct image path");
            aparse_prog_info("reason: %s", strerror(errno));
            continue;
        }

        img = stbi_load(image_path, &img_w, &img_h, &img_ch, 0);
        free(image_path);
        if(!img)
        {
            aparse_prog_error("failed to load image");
            aparse_prog_info("reason: %s", stbi_failure_reason());
            continue;
        }

        if(img_w != page->width || img_h != page->height)
        {
            aparse_prog_error("image size mismatched");
            aparse_prog_info("actual: %dx%d pix, expected: %dx%d pix", img_w, img_h, page->width, page->height);
            stbi_image_free(img);
            continue;
        }

        for(int32_t j = 0; j < page->region_count; j++)
        {
            aatls_region_t *region = &page->regions[j];
            char *new_img_path = 0;
            uint8_t *new_img = 0;

            new_img_path = concat_path(out_dir, region->name, ".png");
            if(!new_img_path)
            {
                aparse_prog_error("failed to construct new image path");
                aparse_prog_info("reason: %s", strerror(errno));
                free(new_img);
                continue;
            }

            new_img = crop_texture(
                    img, img_w, img_h, img_ch, region);

            stbi_write_png(
                    new_img_path, 
                    region->width, 
                    region->height, 
                    img_ch, 
                    new_img, 
                    region->width * img_ch
            );

            free(new_img_path);
            free(new_img);

            aparse_prog_info("processed %d/%d images", j + 1, page->region_count);          
        }

        free(img);
    }
}

int main(int argc, char **argv)
{
    const char *aatls_path = 0;
    const char *images_dir = 0;
    int is_daemon = 0;

    aparse_arg list_all_args[] = {
        aparse_arg_string("atlas", &aatls_path, 0, "Path to Arc's atlas (aatls) file"),
        aparse_arg_end_marker
    };
    aparse_arg extract_regions_args[] = {
        aparse_arg_string("atlas", &aatls_path, 0, "Path to Arc's atlas (aatls) file"),
        aparse_arg_string("imgdir", 0, 0, "Path to directory contain images"),
        aparse_arg_option("-o", "--outdir", 0, 0, APARSE_ARG_TYPE_STRING, "Output directory of extracted regions"), 
        aparse_arg_end_marker
    };

    aparse_arg commands[] = {
        aparse_arg_subparser_impl("list-all", list_all_args, 
                list_all_command, 0, 0, 0),
        aparse_arg_subparser("extract-regions", extract_regions_args, 
                extract_regions_command, "Extract regions from atlas",
                extract_regions_data_t, imgdir, outdir),
        aparse_arg_end_marker
    };

    aparse_arg main_args[] = {
        aparse_arg_parser("command", commands),
        aparse_arg_end_marker
    };
    aparse_list dispatch = {0};

    aatls_err_t rc = AATLS_ERR_OK;

    if(aparse_parse(
                argc, argv, main_args,
                &dispatch,
                "Arc's aatls inspector"
    ) != APARSE_STATUS_OK)
        return 1;

    rc = aatls_parse(aatls_path, &atlas);
    if(rc != AATLS_ERR_OK)
    {
        aparse_prog_error("failed to parse aatls file");
        aparse_prog_info("reason: %s", aatls_strerror(rc));
        return 1;
    }

    aparse_dispatch_all(&dispatch);

    aatls_destroy_atlas(&atlas);

    return 0;
}

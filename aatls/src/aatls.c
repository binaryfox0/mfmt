#include "aatls.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aatls_arr.h"
#include <mfmtio.h>

#define AATLS__ARRSZ(arr) (sizeof((arr)) / sizeof((arr)[0]))
#define AATLS__TYPE_ARRSZ(t, a) (AATLS__ARRSZ(((t*)0)->a))
#define AATLS__IN_RANGE(val, beg, end) \
    (((beg) <= (val)) && ((val) <= (end)))

const char* aatls_strerror(
        const aatls_err_t err
)
{
    static const char *errmsg[] =
    {
        [AATLS_ERR_OK] = "ok",
        [AATLS_ERR_INVALID_ARG] = "invalid argument",
        [AATLS_ERR_IO] = "i/o error",
        [AATLS_ERR_OUT_OF_MEM] = "out of memory",
        [AATLS_ERR_FORMAT] = "invalid file format",
        [AATLS_ERR_CORRUPT] = "corrupted data",
    };

    if(err < 0 || err >= __AATLS_ERR_END__)
        return "Unknown error";
    return errmsg[err];
}

aatls_err_t aatls_parse(
        const char *file_name,
        aatls_atlas_t *out
)
{
    FILE *file = 0;
    long file_size = 0;
    uint8_t *file_data = 0;
    aatls_err_t rc = AATLS_ERR_OK;

    if(!file_name)
        return AATLS_ERR_INVALID_ARG;

    file = fopen(file_name, "rb");
    if(!file)
        return AATLS_ERR_IO;
    
    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    file_data = malloc(file_size);
    if(!file_data)
    {
        fclose(file);
        return AATLS_ERR_OUT_OF_MEM;
    }
    fread(file_data, 1, (size_t)file_size, file);
    fclose(file);

    rc = aatls_parse_from_memory(file_data, (uint32_t)file_size, out);
    free(file_data);

    return rc;
}

int aatls__validate_region(
        const aatls_region_t *src
);
int aatls__validate_page(
        const aatls_page_t *src
);
const uint8_t aatls__header[5] = { 'A', 'A', 'T', 'L', 'S' };
aatls_err_t aatls_parse_from_memory(
        const uint8_t *buf, 
        const uint32_t bufsz,
        aatls_atlas_t *out
)
{
    mfmtio_rstream_t stream = { .buf = buf, .idx = 0, .bufsz = bufsz };    
    int8_t version = 0;
    aatls_err_t rc = AATLS_ERR_OK;
    int32_t page_count = 0;
    int32_t page_capacity = 0;
    aatls_page_t *pages = 0;
    aatls__arr arr = { (void**)&pages, &page_count, &page_capacity, sizeof(*pages) };

    if(!buf || !bufsz || !out)
        return AATLS_ERR_INVALID_ARG;

    for(int i = 0; i < (int)AATLS__ARRSZ(aatls__header); i++)
    {
        if(mfmtio_read_i8(&stream) != aatls__header[i])
            return AATLS_ERR_FORMAT;
    }

    version = mfmtio_read_i8(&stream);

    if(stream.eos)
        return AATLS_ERR_CORRUPT;

    for(;;)
    {
        aatls_page_t pg = {0};
        int8_t c = mfmtio_read_i8(&stream);
        int32_t rg_cnt_tmp = 0;

        // Reads.checkEOF is a wrapper of Reads.c
        // which read a byte
        (void)c;
        if(stream.eos)
            break;
        
        pg.name = mfmtio_read_utf(&stream);
        if(stream.oom)
        {
            rc = AATLS_ERR_OUT_OF_MEM;
            break;
        }

        pg.width = mfmtio_read_i16(&stream);
        pg.height = mfmtio_read_i16(&stream);
        pg.filter_min = mfmtio_read_i8(&stream);
        pg.filter_mag = mfmtio_read_i8(&stream);
        pg.u_wrap = mfmtio_read_i8(&stream);
        pg.v_wrap = mfmtio_read_i8(&stream);
        rg_cnt_tmp = mfmtio_read_i32(&stream);

        if(stream.eos)
        {
            free(pg.name);
            rc = AATLS_ERR_CORRUPT;
            break;
        }
        
        if(aatls__validate_page(&pg) < 0)
        {
            aatls_destroy_page(&pg);
            rc = AATLS_ERR_CORRUPT;
            break;
        }

        pg.region_count = rg_cnt_tmp;
        pg.regions = calloc(
                pg.region_count, sizeof(*pg.regions)); 
        if(!pg.regions)
        {
            free(pg.name);
            rc = AATLS_ERR_OUT_OF_MEM;
            break;
        }
        pg.region_capacity = pg.region_count;

        for(int32_t i = 0; i < pg.region_count; i++)
        {
            aatls_region_t region = {0};
            region.name = mfmtio_read_utf(&stream);
            if(stream.oom)
            {
                rc = AATLS_ERR_OUT_OF_MEM;
                break;
            }

            region.left = mfmtio_read_i16(&stream);
            region.top = mfmtio_read_i16(&stream);
            region.width = mfmtio_read_i16(&stream);
            region.height = mfmtio_read_i16(&stream);

            region.has_offsets = mfmtio_read_bool(&stream);
            if(region.has_offsets)
            {
                region.offset_x = mfmtio_read_i16(&stream);
                region.offset_y = mfmtio_read_i16(&stream);
                region.original_width = mfmtio_read_i16(&stream);
                region.original_height = mfmtio_read_i16(&stream);
            }

            region.has_splits = mfmtio_read_bool(&stream);
            if(region.has_splits)
            {
                for(int j = 0; j < AATLS__TYPE_ARRSZ(aatls_region_t, splits); j++) 
                    region.splits[j] = mfmtio_read_i16(&stream);
            }

            region.has_pads = mfmtio_read_bool(&stream);
            if(region.has_pads)
            {
                for(int j = 0; j < AATLS__TYPE_ARRSZ(aatls_region_t, pads); j++)
                   region.pads[j] = mfmtio_read_i16(&stream);
            }

            if(
                    stream.eos || 
                    aatls__validate_region(&region) < 0)
            {
                rc = AATLS_ERR_CORRUPT;
                free(region.name);
                break;
            }

            pg.regions[i] = region;
        }
        
        if(rc != AATLS_ERR_OK){
            aatls_destroy_page(&pg);
            break;
        }

        if(aatls__arr_push(&arr, &pg) < 0){
            rc = AATLS_ERR_OUT_OF_MEM;
            break;
        }
    }

    if(rc != AATLS_ERR_OK)
    {
        aatls_destroy_atlas(
            (aatls_atlas_t[1]) {{
                .page_count = page_count,
                .page_capacity = page_capacity,
                .pages = pages
            }}
        );
        return rc;
    }

    out->version = version;
    out->page_count = page_count;
    out->page_capacity = page_capacity;
    out->pages = pages;

    return AATLS_ERR_OK;
}

aatls_err_t aatls_write(
        const char *file_name,
        const aatls_atlas_t *atlas
)
{
    aatls_err_t rc = AATLS_ERR_OK;
    uint8_t *buf = 0;
    uint32_t bufsiz = 0;
    FILE *file = 0;

    if(!file_name || !atlas)
        return AATLS_ERR_INVALID_ARG;

    rc = aatls_write_to_memory(atlas, &buf, &bufsiz);
    if(rc != AATLS_ERR_OK)
        return rc;

    file = fopen(file_name, "wb");
    if(!file)
    {
        free(buf);
        return AATLS_ERR_IO;
    }

    fwrite(buf, 1, bufsiz, file);

    fclose(file);
    free(buf);
    return AATLS_ERR_OK;
}

aatls_err_t aatls_write_to_memory(
        const aatls_atlas_t *atlas,
        uint8_t **out_buf,
        uint32_t *out_size
)
{
    mfmtio_wstream_t stream = {0};
    aatls_err_t rc = AATLS_ERR_OK;

    if(!atlas || !out_buf || !out_size)
        return AATLS_ERR_INVALID_ARG;

    for(int i = 0; i < (int)AATLS__ARRSZ(aatls__header); i++)
        mfmtio_write_i8(&stream, aatls__header[i]);

    mfmtio_write_i8(&stream, (int8_t)atlas->version);

    for(uint32_t i = 0; i < atlas->page_count; i++)
    {
        aatls_page_t *page = &atlas->pages[i];

        // for Reads.checkEOF later
        mfmtio_write_i8(&stream, 1);
        
        if(mfmtio_write_utf(&stream, page->name) < 0)
        {
            rc = AATLS_ERR_CORRUPT;
            break;
        }

        mfmtio_write_i16(&stream, page->width);
        mfmtio_write_i16(&stream, page->height);
        mfmtio_write_i8(&stream, page->filter_min);
        mfmtio_write_i8(&stream, page->filter_mag);
        mfmtio_write_i8(&stream, page->u_wrap);
        mfmtio_write_i8(&stream, page->v_wrap);
        mfmtio_write_i32(&stream, page->region_count);
            
        if(stream.oom)
        {
            rc = AATLS_ERR_OUT_OF_MEM;
            break;
        }

        for(int32_t j = 0; j < page->region_count; j++)
        {
            aatls_region_t *region = &page->regions[j];

            if(mfmtio_write_utf(&stream, region->name) < 0)
            {
                rc = AATLS_ERR_CORRUPT;
                break;
            }

            mfmtio_write_i16(&stream, region->left);
            mfmtio_write_i16(&stream, region->top);
            mfmtio_write_i16(&stream, region->width);
            mfmtio_write_i16(&stream, region->height);

            mfmtio_write_bool(&stream, region->has_offsets);
            if(region->has_offsets)
            {
                mfmtio_write_i16(&stream, region->offset_x);
                mfmtio_write_i16(&stream, region->offset_y);
                mfmtio_write_i16(&stream, region->original_width);
                mfmtio_write_i16(&stream, region->original_height);
            }

            mfmtio_write_bool(&stream, region->has_splits);
            if(region->has_splits)
            {
                for(int k = 0; k < 4; k++) 
                    mfmtio_write_i16(&stream, region->splits[k]);
            }

            mfmtio_write_bool(&stream, region->has_pads);
            if(region->has_pads)
            {
                for(int k = 0; k < 4; k++)
                   mfmtio_write_i16(&stream, region->pads[k]);
            }

            if(stream.oom)
            {
                rc = AATLS_ERR_OUT_OF_MEM;
                break;
            }
        }
        if(rc != AATLS_ERR_OK)
            break;
    }

    if(rc != AATLS_ERR_OK)
    {
        free(stream.buf);
        return rc;
    }

    // truncate
    *out_buf = realloc(stream.buf, stream.size);
    if(!out_buf)
        *out_buf = stream.buf;
    *out_size = stream.size;

    return AATLS_ERR_OK;
}

int aatls__validate_region(
        const aatls_region_t *src
)
{
    if(
        src->left < 0 ||
        src->top < 0 ||
        src->width <= 0 ||
        src->height <= 0
      ) return -1;
    
    if(src->has_offsets)
    {
        if(
            src->offset_x < 0 ||
            src->offset_y < 0 ||
            src->original_width <= 0 ||
            src->original_height <= 0
          ) return -1;
    }

    if(src->has_splits)
    {
        for(int i = 0; i < AATLS__TYPE_ARRSZ(aatls_region_t, splits); i++)
        {
            if(src->splits[i] < 0)
                return -1;
        }
    }

    if(src->has_pads)
    {
        for(int i = 0; i < AATLS__TYPE_ARRSZ(aatls_region_t, pads); i++)
        {
            if(src->pads[i] < 0)
                return -1;
        }
    }

    if(!src->name)
        return -1;

    return 0;
}

/* Use to eliminate redundant check in aatls_copy_page */
aatls_err_t aatls__copy_region(
        const aatls_region_t *src, 
        aatls_region_t *dst
)
{
    aatls_region_t tmp = {0};

    tmp.left   = src->left;
    tmp.top    = src->top;
    tmp.width  = src->width;
    tmp.height = src->height;

    tmp.has_offsets = src->has_offsets ? 1 : 0;
    if(tmp.has_offsets)
    {
        tmp.offset_x        = src->offset_x;
        tmp.offset_y        = src->offset_y;
        tmp.original_width  = src->original_width;
        tmp.original_height = src->original_height;
    }

    tmp.has_splits = src->has_splits;
    if(tmp.has_splits)
    {
        for(int i = 0; i < AATLS__TYPE_ARRSZ(aatls_region_t, splits); i++)
            tmp.splits[i] = src->splits[i];
    }

    tmp.has_pads = src->has_pads ? 1 : 0;
    if(tmp.has_pads)
    {
        for(int i = 0; i < AATLS__TYPE_ARRSZ(aatls_region_t, pads); i++)
            tmp.pads[i] = src->pads[i];
    }

    tmp.flip = src->flip ? 1 : 0;

    tmp.name = strdup(src->name);
    if(!tmp.name)
        return AATLS_ERR_OUT_OF_MEM;

    *dst = tmp;

    return AATLS_ERR_OK;
}

aatls_err_t aatls_copy_region(
        const aatls_region_t *src, 
        aatls_region_t *dst
)
{
    if(!src || !dst)
        return AATLS_ERR_INVALID_ARG;

    if(aatls__validate_region(src) < 0)
        return AATLS_ERR_CORRUPT;

    return aatls__copy_region(src, dst);
}

int aatls__validate_page(
        const aatls_page_t *page
)
{
    if(!page->name)
        return -1;

    if(
        page->width <= 0 || 
        page->height <= 0
    ) return -1;

    if(
        !AATLS__IN_RANGE(page->filter_min, 0, __AATLS_FILTER_END__) ||
        !AATLS__IN_RANGE(page->filter_mag, 0, __AATLS_FILTER_END__) ||
        !AATLS__IN_RANGE(page->u_wrap, 0, __AATLS_WRAP_END__) ||
        !AATLS__IN_RANGE(page->v_wrap, 0, __AATLS_WRAP_END__)
    ) return -1;

    if(
        page->region_count < 0 ||
        page->region_capacity < 0 ||
        page->region_count > page->region_capacity ||
        (page->region_capacity > 0 && !page->regions) ||
        (page->region_capacity == 0 && page->regions)
    ) return -1;

    for(int32_t i = 0; i < page->region_count; i++)
    {
        aatls_region_t *region = &page->regions[i];
        if(
            region->left + region->width > page->width ||
            region->top + region->height > page->height
          ) return -1;
        if(aatls__validate_region(region) < 0)
            return -1;
    }

    return 0;
}


aatls_err_t aatls__copy_page(
        const aatls_page_t *src,
        aatls_page_t *dst
)
{
    aatls_page_t tmp = {0};
 
    tmp.name = strdup(src->name);
    if(!tmp.name)
        return AATLS_ERR_OUT_OF_MEM;

    tmp.width  = src->width;
    tmp.height = src->height;
    tmp.filter_min = src->filter_min;
    tmp.filter_mag = src->filter_mag;
    tmp.u_wrap = src->u_wrap;
    tmp.v_wrap = src->v_wrap;

    if(src->region_count > 0)
    {
        tmp.regions = malloc(sizeof(*tmp.regions) * src->region_count);
        if(!tmp.regions)
        {
            free(tmp.name);
            return AATLS_ERR_OUT_OF_MEM;
        }
        for(int32_t i = 0; i < src->region_count; i++)
        {
            aatls_err_t err = AATLS_ERR_OK;
            
            err = aatls__copy_region(&src->regions[i], &tmp.regions[i]);
            /* Out-of-Memory is the only error path */
            if(err != AATLS_ERR_OK)
            {
                for(; i >= 0; i--)
                    aatls_destroy_region(&tmp.regions[i]);
                free(tmp.name);
                return err;
            }
        }
        tmp.region_count = src->region_count;
        tmp.region_capacity = src->region_count;
    }

    *dst = tmp;

    return AATLS_ERR_OK;
}


aatls_err_t aatls_copy_page(
        const aatls_page_t *src,
        aatls_page_t *dst
)
{
    if(!src || !dst)
        return AATLS_ERR_INVALID_ARG;

    if(aatls__validate_page(src) < 0)
        return AATLS_ERR_CORRUPT;

    return aatls__copy_page(src, dst);
}

int aatls__validate_atlas(
        const aatls_atlas_t *atlas
)
{
    if(atlas->version < 0)
        return -1;
    
    if(
        atlas->page_count < 0 ||
        atlas->page_capacity < 0 ||
        atlas->page_count > atlas->page_capacity ||
        (atlas->page_capacity > 0 && !atlas->pages) ||
        (atlas->page_capacity == 0 && atlas->pages)
    ) return -1;

    for(int32_t i = 0; i < atlas->page_count; i++)
    {
        if(aatls__validate_page(&atlas->pages[i]) < 0)
            return -1;
    }

    return 0;
}

aatls_err_t aatls_copy_atlas(
        const aatls_atlas_t *src,
        aatls_atlas_t *dst
)
{
    aatls_atlas_t tmp = {0};

    if(!src || !dst)
        return AATLS_ERR_INVALID_ARG;

    if(aatls__validate_atlas(src) < 0)
        return AATLS_ERR_CORRUPT;


    tmp.version = src->version;

    if(src->page_count > 0)
    {
        tmp.pages = malloc(sizeof(*tmp.pages) * src->page_count);
        if(!tmp.pages)
            return AATLS_ERR_OUT_OF_MEM;
        for(int32_t i = 0; i < src->page_count; i++)
        {
            aatls_err_t err = AATLS_ERR_OK;
            
            err = aatls__copy_page(&src->pages[i], &tmp.pages[i]);
            /* Out-of-Memory is the only error path */
            if(err != AATLS_ERR_OK)
            {
                for(; i >= 0; i--)
                    aatls_destroy_page(&tmp.pages[i]);
                return err;
            }
        }
    }

    *dst = tmp;

    return AATLS_ERR_OK;
}

aatls_err_t aatls_insert_region(
        aatls_page_t *page,
        aatls_region_t *region,
        const int32_t idx
)
{
    aatls__arr arr = {0};;
    aatls_err_t err = AATLS_ERR_OK;

    if(!page || !region)
        return AATLS_ERR_INVALID_ARG;

    arr = (aatls__arr){
        .ptr = (void**)&page->regions,
        .size = &page->region_count,
        .cap = &page->region_capacity,
        .stride = sizeof(*page->regions)
    };

    if(aatls__arr_validate(&arr) < 0)
        return AATLS_ERR_CORRUPT;

    if(aatls__arr_insert(&arr, region, idx) < 0)
        return AATLS_ERR_OUT_OF_MEM;

    memset(region, 0, sizeof(*region));

    return AATLS_ERR_OK;
}

aatls_err_t aatls_insert_page(
        aatls_atlas_t *atlas,
        aatls_page_t *page,
        const int32_t idx
)
{
    aatls__arr arr = {0};;
    aatls_err_t err = AATLS_ERR_OK;

    if(!atlas || !page)
        return AATLS_ERR_INVALID_ARG;

    arr = (aatls__arr){
        .ptr = (void**)&atlas->pages,
        .size = &atlas->page_count,
        .cap = &atlas->page_capacity,
        .stride = sizeof(*atlas->pages)
    };

    if(aatls__arr_validate(&arr) < 0)
        return AATLS_ERR_CORRUPT;

    if(aatls__arr_insert(&arr, page, idx) < 0)
        return AATLS_ERR_OUT_OF_MEM;

    memset(page, 0, sizeof(*page));

    return AATLS_ERR_OK;
}

int32_t aatls_find_region_in_page(
        const aatls_page_t *page,
        const char *name
)
{
    if(!page || !name)
        return AATLS_ERR_INVALID_ARG;

    if(aatls__arr_validate(
        (aatls__arr[1])
        {{
            (void**)&page->regions,
            (int32_t*)&page->region_count, 
            (int32_t*)&page->region_capacity
        }}
    ) < 0) return -1;

    for(int32_t i = 0; i < page->region_count; i++)
    {
        aatls_region_t *region = &page->regions[i];
        if(!region->name)
            continue;
        if(!strcmp(region->name, name))
            return i;
    }
    return -1;
}

int32_t aatls_find_region_in_atlas(
        const aatls_atlas_t *atlas,
        const char *name,
        int32_t *region_idx
)
{
    if(!atlas || !name || !region_idx)
        return AATLS_ERR_INVALID_ARG;

    if(aatls__arr_validate(
        (aatls__arr[1])
        {{
            (void**)&atlas->pages,
            (int32_t*)&atlas->page_count, 
            (int32_t*)&atlas->page_capacity
        }}
    ) < 0) return -1;

    for(int32_t i = 0; i < atlas->page_count; i++)
    {
        aatls_page_t *page = &atlas->pages[i];
        int32_t found_idx = aatls_find_region_in_page(page, name);
        if(found_idx != -1)
        {
            *region_idx = found_idx;
            return i;
        }
    }
    return -1;
}

int32_t aatls_find_page_in_atlas(
        const aatls_atlas_t *atlas,
        const char *name
)
{
    if(!atlas || !name)
        return AATLS_ERR_INVALID_ARG;

    if(aatls__arr_validate(
        (aatls__arr[1])
        {{
            (void**)&atlas->pages,
            (int32_t*)&atlas->page_count, 
            (int32_t*)&atlas->page_capacity
        }}
    ) < 0) return -1;
    
    for(int32_t i = 0; i < atlas->page_count; i++)
    {
        aatls_page_t *page = &atlas->pages[i];
        if(!page->name)
            continue;

        if(!strcmp(page->name, name))
            return i;
    }
    return -1;
}

void aatls_destroy_region(
        aatls_region_t *region
)
{
    if(!region)
        return;

    if(region->name)
        free(region->name);
    *region = (aatls_region_t){0};
}

void aatls_destroy_page(
        aatls_page_t *page
)
{
    if(!page)
        return;

    if(page->name)
        free(page->name);
    if(
        page->region_count > 0 && 
        page->region_capacity >= page->region_count && 
        page->regions
    ) {
        for(int32_t i = 0; i < page->region_count; i++)
            aatls_destroy_region(&page->regions[i]);
        free(page->regions);
    }
    *page = (aatls_page_t){0};
}

void aatls_destroy_atlas(
        aatls_atlas_t *atlas
)
{
    if(!atlas)
        return;

    if(
        atlas->page_count > 0 && 
        atlas->page_capacity >= atlas->page_count && 
        atlas->pages
    ) {
        for(int32_t i = 0; i < atlas->page_count; i++)
            aatls_destroy_page(&atlas->pages[i]);
        free(atlas->pages);
    }
    *atlas = (aatls_atlas_t){0};
}

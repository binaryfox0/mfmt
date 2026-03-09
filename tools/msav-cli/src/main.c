#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <aparse.h>
#include <mz.h>
#include <mz_os.h>
#include <mz_strm.h>
#include <mz_strm_buf.h>
#include <mz_strm_split.h>
#include <mz_zip.h>
#include <mz_zip_rw.h>

#include <zlib-ng.h>

#include <msav.h>

#define error aparse_prog_error
#define info aparse_prog_info

typedef struct msav_meta
{
    int version;
    int build;
    int64_t timestamp;
    int64_t time_played;
    void* map;
    int wave;
    void* rules;
    char** mods;
} msav_meta_t;

typedef struct msav_map
{
    int16_t width;
    int16_t height;
    
} msav_map_t;

typedef struct msav_content_header
{
//    msav_content_type_t type;
    int type;
    int16_t total;
    char *name;
} msav_content_header_t;


const char *mz_strerror_macro(const int32_t err)
{
    switch(err)
    {
        case MZ_OK:             return "MZ_OK";            
        case MZ_STREAM_ERROR:   return "MZ_STREAM_ERROR";  
        case MZ_DATA_ERROR:     return "MZ_DATA_ERROR";    
        case MZ_MEM_ERROR:      return "MZ_MEM_ERROR";     
        case MZ_BUF_ERROR:      return "MZ_BUF_ERROR";     
        case MZ_VERSION_ERROR:  return "MZ_VERSION_ERROR"; 
        case MZ_END_OF_LIST:    return "MZ_END_OF_LIST";   
        case MZ_END_OF_STREAM:  return "MZ_END_OF_STREAM"; 
        case MZ_PARAM_ERROR:    return "MZ_PARAM_ERROR";   
        case MZ_FORMAT_ERROR:   return "MZ_FORMAT_ERROR";  
        case MZ_INTERNAL_ERROR: return "MZ_INTERNAL_ERROR";
        case MZ_CRC_ERROR:      return "MZ_CRC_ERROR";     
        case MZ_CRYPT_ERROR:    return "MZ_CRYPT_ERROR";   
        case MZ_EXIST_ERROR:    return "MZ_EXIST_ERROR";   
        case MZ_PASSWORD_ERROR: return "MZ_PASSWORD_ERROR";
        case MZ_SUPPORT_ERROR:  return "MZ_SUPPORT_ERROR"; 
        case MZ_HASH_ERROR:     return "MZ_HASH_ERROR";    
        case MZ_OPEN_ERROR:     return "MZ_OPEN_ERROR";    
        case MZ_CLOSE_ERROR:    return "MZ_CLOSE_ERROR";   
        case MZ_SEEK_ERROR:     return "MZ_SEEK_ERROR";    
        case MZ_TELL_ERROR:     return "MZ_TELL_ERROR";    
        case MZ_READ_ERROR:     return "MZ_READ_ERROR";    
        case MZ_WRITE_ERROR:    return "MZ_WRITE_ERROR";   
        case MZ_SIGN_ERROR:     return "MZ_SIGN_ERROR";    
        case MZ_SYMLINK_ERROR:  return "MZ_SYMLINK_ERROR"; 
        default:                return "MZ_UNKNOWN_ERROR"; 
    }
}

const char *mz_strerror_readable(const int32_t err)
{
    switch(err)
    {
        case MZ_OK:             return "OK error (zlib)";
        case MZ_STREAM_ERROR:   return "Stream error (zlib)";
        case MZ_DATA_ERROR:     return "Data error (zlib)";
        case MZ_MEM_ERROR:      return "Memory allocation error (zlib)";
        case MZ_BUF_ERROR:      return "Buffer error (zlib)";
        case MZ_VERSION_ERROR:  return "Version error (zlib)";
        case MZ_END_OF_LIST:    return "End of list error";
        case MZ_END_OF_STREAM:  return "End of stream error";
        case MZ_PARAM_ERROR:    return "Invalid parameter error";
        case MZ_FORMAT_ERROR:   return "File format error";
        case MZ_INTERNAL_ERROR: return "Library internal error";
        case MZ_CRC_ERROR:      return "CRC error";
        case MZ_CRYPT_ERROR:    return "Cryptography error";
        case MZ_EXIST_ERROR:    return "Does not exist error";
        case MZ_PASSWORD_ERROR: return "Invalid password error";
        case MZ_SUPPORT_ERROR:  return "Library support error";
        case MZ_HASH_ERROR:     return "Hash error";
        case MZ_OPEN_ERROR:     return "Stream open error";
        case MZ_CLOSE_ERROR:    return "Stream close error";
        case MZ_SEEK_ERROR:     return "Stream seek error";
        case MZ_TELL_ERROR:     return "Stream tell error";
        case MZ_READ_ERROR:     return "Stream read error";
        case MZ_WRITE_ERROR:    return "Stream write error";
        case MZ_SIGN_ERROR:     return "Signing error";
        case MZ_SYMLINK_ERROR:  return "Symbolic link error";
        default:                return "Unknown error";
    }
}

const char *zng_strerror_macro(const int32_t err)
{
    switch(err)
    {
        case Z_OK:              return "Z_OK";
        case Z_STREAM_END:      return "Z_STREAM_END";
        case Z_NEED_DICT:       return "Z_NEED_DICT";
        case Z_ERRNO:           return "Z_ERRNO";
        case Z_STREAM_ERROR:    return "Z_STREAM_ERROR";
        case Z_DATA_ERROR:      return "Z_DATA_ERROR";
        case Z_MEM_ERROR:       return "Z_MEM_ERROR";
        case Z_BUF_ERROR:       return "Z_BUF_ERROR";
        case Z_VERSION_ERROR:   return "Z_VERSION_ERROR";
        default:                return "Z_UNKNOWN_ERROR";
    }
}

const char *zng_strerror_readable(const int32_t err)
{
    switch(err)
    {
        case Z_OK:              return "OK error";
        case Z_STREAM_END:      return "Stream end";
        case Z_NEED_DICT:       return "Need dictionary";
        case Z_ERRNO:           return "errno";
        case Z_STREAM_ERROR:    return "Stream error";
        case Z_DATA_ERROR:      return "Data error";
        case Z_MEM_ERROR:       return "Memory error";
        case Z_BUF_ERROR:       return "Buffer error";
        case Z_VERSION_ERROR:   return "Version error";
        default:                return "Unknown errror";
    }
}
int decompress_file_zip(
        void *reader,
        uint8_t **out_buf,
        int64_t *out_bufsize
)
{
    int32_t err = MZ_OK;
    mz_zip_file *file_info = {0};
    uint8_t *buf = 0;
    int64_t bufsize = 0;
    int64_t total_read = 0;

    err = mz_zip_reader_entry_get_info(reader, &file_info);
    if(err != MZ_OK)
    {
        error("failed to get entry info");
        info("reason: %s (\"%s\")", 
                mz_strerror_macro(err),
                mz_strerror_readable(err));
        return -1;
    }
    bufsize = file_info->uncompressed_size;

    buf = malloc(bufsize);
    if(!buf)
    {
        error("failed to allocate memory");
        info("reason: \"%s\"", strerror(errno));
        return -1;
    }
    err = mz_zip_reader_entry_open(reader);
    if(err != MZ_OK)
    {
        error("failed to open entry");
        info("reason: \"%s\"", strerror(errno));
        free(buf);
        return -1;
    }
    for(;;)
    {
        int32_t read_len = 0;
        read_len = mz_zip_reader_entry_read(reader, buf, bufsize);
        if(read_len > 0)
            total_read += (int64_t)read_len;
        else
            break;
    }
    err = mz_zip_reader_entry_close(reader);
    if(err != MZ_OK)
    {
        error("failed to close entry");
        info("reason: \"%s\"", strerror(errno));
        free(buf);
        return -1;
    }

    *out_buf = buf;
    *out_bufsize = total_read;
    return 0;
}

typedef struct
{
    uint8_t *data;
    int64_t size;
    int64_t capacity;
} dynbuf_t;

#define BUF_DEFAULT_CAP 16384
int buf_append(
        dynbuf_t *dynbuf, 
        const uint8_t* buf, 
        const int32_t bufsize
)
{
    if(dynbuf->size + bufsize > dynbuf->capacity)
    {
        uint8_t *new_data = 0;
        int64_t new_cap = 0;

        new_cap = dynbuf->capacity ? dynbuf->capacity * 2 : BUF_DEFAULT_CAP;
        new_data = realloc(dynbuf->data, new_cap);
        if(!new_data)
        {
            error("failed to expand buffer");
            info("reason: \"%s\"", strerror(errno));
            return -1;
        }

        dynbuf->capacity = new_cap;
        dynbuf->data = new_data;
    }

    memcpy(&dynbuf->data[dynbuf->size], buf, bufsize);
    dynbuf->size += bufsize;
    return 0;
}

int decompress_file_zlib(
        const uint8_t *in_buf,
        const int64_t in_bufsiz,
        uint8_t **out_buf,
        int64_t *out_bufsize
)
{
    int32_t err = Z_OK;
    zng_stream stream = {0};
    dynbuf_t dynbuf = {0};
    uint8_t out_tmp[8192] = {0};

    stream.next_in = (uint8_t*)in_buf;
    stream.avail_in = in_bufsiz;

    err = zng_inflateInit(&stream);
    if(err != Z_OK)
    {
        error("failed to start decompressing zlib");
        info("reason: %s (\"%s\")", 
                zng_strerror_macro(err), zng_strerror_readable(err));
        return -1;
    }

    for(;;)
    {
        stream.next_out = out_tmp;
        stream.avail_out = sizeof(out_tmp);

        err = zng_inflate(&stream, Z_NO_FLUSH);

        if(err != Z_OK && err != Z_STREAM_END)
        {
            zng_inflateEnd(&stream);
            free(dynbuf.data);
            error("failed to decompress zlib");
            info("reason: %s (\"%s\")", 
                    zng_strerror_macro(err), zng_strerror_readable(err));
            return -1;
        }

        int32_t produced = sizeof(out_tmp) - stream.avail_out;

        if(produced > 0)
        {
            if(buf_append(&dynbuf, out_tmp, produced) != 0)
            {
                zng_inflateEnd(&stream);
                free(dynbuf.data);
                return -1;
            }
        }

        if(err == Z_STREAM_END)
            break;
    }

    zng_inflateEnd(&stream);

    *out_buf = dynbuf.data;
    *out_bufsize = dynbuf.size;
    return 0;
}

void list_command(void *arg)
{
    const char *file_name = *(const char**)arg;
}

void extract_command(void *arg)
{
    const char *archive_path = *(const char**)arg;
    const char *file_path = *(const char**)((uint8_t*)arg + sizeof(void*));
    const char *out_path = *(const char**)((uint8_t*)arg + 2 * sizeof(void*));
    void *reader = 0;
    int32_t err = MZ_OK;
    
    uint8_t *buf = 0;
    int64_t bufsize = 0;
    uint8_t *rawbuf = 0;
    int64_t rawbufsize = 0;

    FILE *file = 0;
    
    reader = mz_zip_reader_create(); 
    if(!reader)
    {
        error("failed to create save file reader");
        return;
    }

    err = mz_zip_reader_open_file(reader, archive_path);
    if(err != MZ_OK)
    {
        error("failed to open save file");
        info("reason: %s (\"%s\")", 
                mz_strerror_macro(err),
                mz_strerror_readable(err));
        return;
    }

    err = mz_zip_reader_locate_entry(reader, file_path, 0);
    if(err != MZ_OK)
    {
        error("failed to locate file: \"%s\"", file_path);
        info("reason: %s (\"%s\")", 
                mz_strerror_macro(err),
                mz_strerror_readable(err));
        return;
    }
    
    if(decompress_file_zip(reader, &buf, &bufsize) < 0)
        return;
    if(decompress_file_zlib(buf, bufsize, &rawbuf, &rawbufsize) < 0)
    {
        info("this may not meant that the file was corrupted or unreadable");
        info("it just because that it wasn't a zlib compressed file");
        rawbuf = buf;
        rawbufsize = bufsize;
    } else {
        free(buf);
    }
    
    mz_zip_reader_close(reader);
    mz_zip_reader_delete(&reader);

    file = fopen(out_path, "wb");
    fwrite(rawbuf, 1, rawbufsize, file);
    fclose(file);

    free(rawbuf);
}

void decode_command(void *arg)
{
    const char *file_path = *(const char**)((uint8_t*)arg);
    msav_file_t file = {0};

    msav_parse(file_path, &file);
}

int main(int argc, char** argv)
{
    const char *file_name = 0;
    const char *msav_path = 0;
    
    aparse_arg extract_args[] = {
        aparse_arg_string("archive", 0, 0, "Mindustry save archive (.zip)"),
        aparse_arg_string("path", 0, 0, "Internal archive path of the file to extract"),
        aparse_arg_string("out", 0, 0, "Output file path"),
        aparse_arg_end_marker
    };
    aparse_arg decode_args[] = {
        aparse_arg_string("path", 0, 0, "Path of the file to decode"),
        aparse_arg_end_marker
    };
    aparse_arg subparsers[] = {
        aparse_arg_subparser_impl("extract", extract_args, 
                extract_command, "Extract specified file from Mindustry save archive (.zip)", 
                (int[]){0, 8, 8, 8, 16, 8}, 3),
        aparse_arg_subparser_impl("decode", decode_args, 
                decode_command, "Decode given Mindustry save file (.msav)",
                (int[]){0, 8}, 1),
        aparse_arg_end_marker
    };
    aparse_arg main_args[] = {
        aparse_arg_parser("command", subparsers),
        aparse_arg_end_marker
    };

    if(aparse_parse(argc, argv, main_args, 0, "Mindustry's save file inspector") != APARSE_STATUS_OK)
        return 1;

    return 0;
}

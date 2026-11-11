#include "msav_ubjson.h"

#include "msav_log_priv.h"

static int msav__ubjson_has_bytes(const mfmtio_rstream_t *stream, uint32_t count)
{
    return stream && !stream->eos && stream->idx <= stream->bufsz &&
           count <= stream->bufsz - stream->idx;
}

static msav_err_t msav__ubjson_skip_bytes(mfmtio_rstream_t *stream, uint64_t count)
{
    if(count > UINT32_MAX || !msav__ubjson_has_bytes(stream, (uint32_t)count))
        return MSAV_ERR_CORRUPT;
    stream->idx += (uint32_t)count;
    return MSAV_ERR_OK;
}

static msav_err_t msav__ubjson_read_integer(
        mfmtio_rstream_t *stream,
        int type,
        uint64_t *value)
{
    uint64_t result = 0;
    uint32_t size = 0;

    switch(type)
    {
        case 'i':
            if(!msav__ubjson_has_bytes(stream, 1)) return MSAV_ERR_CORRUPT;
            {
                int8_t signed_value = mfmtio_read_i8(stream);
                if(signed_value < 0) return MSAV_ERR_CORRUPT;
                *value = (uint8_t)signed_value;
            }
            return MSAV_ERR_OK;
        case 'U':
            if(!msav__ubjson_has_bytes(stream, 1)) return MSAV_ERR_CORRUPT;
            *value = mfmtio_read_u8(stream);
            return MSAV_ERR_OK;
        case 'I':
            if(!msav__ubjson_has_bytes(stream, 2)) return MSAV_ERR_CORRUPT;
            {
                int16_t signed_value = mfmtio_read_i16(stream);
                if(signed_value < 0) return MSAV_ERR_CORRUPT;
                *value = (uint16_t)signed_value;
            }
            return MSAV_ERR_OK;
        case 'l':
            if(!msav__ubjson_has_bytes(stream, 4)) return MSAV_ERR_CORRUPT;
            {
                int32_t signed_value = mfmtio_read_i32(stream);
                if(signed_value < 0) return MSAV_ERR_CORRUPT;
                *value = (uint32_t)signed_value;
            }
            return MSAV_ERR_OK;
        case 'L':
            size = 8;
            break;
        default:
            return MSAV_ERR_CORRUPT;
    }

    if(!msav__ubjson_has_bytes(stream, size))
        return MSAV_ERR_CORRUPT;
    for(uint32_t i = 0; i < size; i++)
        result = (result << 8) | mfmtio_read_u8(stream);
    if(result > INT64_MAX)
        return MSAV_ERR_CORRUPT;
    *value = result;
    return MSAV_ERR_OK;
}

static msav_err_t msav__ubjson_skip_string(mfmtio_rstream_t *stream)
{
    int type = 0;
    uint64_t length = 0;

    if(!msav__ubjson_has_bytes(stream, 1))
        return MSAV_ERR_CORRUPT;
    type = mfmtio_read_u8(stream);
    if(msav__ubjson_read_integer(stream, type, &length) != MSAV_ERR_OK)
        return MSAV_ERR_CORRUPT;
    return msav__ubjson_skip_bytes(stream, length);
}

static msav_err_t msav__ubjson_skip_value(
        mfmtio_rstream_t *stream,
        int type,
        uint32_t depth)
{
    uint32_t size = 0;
    uint64_t string_length = 0;

    switch(type)
    {
        case 'Z': case 'T': case 'F': case 'N':
            return MSAV_ERR_OK;
        case 'i': case 'U': case 'C':
            size = 1;
            break;
        case 'I':
            size = 2;
            break;
        case 'l': case 'd':
            size = 4;
            break;
        case 'L': case 'D':
            size = 8;
            break;
        case 'S': case 'H':
            if(msav__ubjson_skip_string(stream) != MSAV_ERR_OK)
                return MSAV_ERR_CORRUPT;
            return MSAV_ERR_OK;
        case '[':
        case '{':
            return msav__ubjson_count_entries(stream, type, &string_length);
        default:
            return MSAV_ERR_CORRUPT;
    }

    return msav__ubjson_skip_bytes(stream, size);
}

static msav_err_t msav__ubjson_count_container(
        mfmtio_rstream_t *stream,
        int object,
        uint32_t depth,
        uint64_t *count)
{
    int has_count = 0;
    uint64_t container_count = 0;
    uint64_t entries = 0;
    uint64_t declared_count = 0;

    if(depth >= 128)
        return MSAV_ERR_CORRUPT;

    while(msav__ubjson_has_bytes(stream, 1))
    {
        int metadata = stream->buf[stream->idx];
        if(metadata != '$' && metadata != '#')
            break;
        stream->idx++;

        if(metadata == '$')
        {
            if(!msav__ubjson_has_bytes(stream, 1))
                return MSAV_ERR_CORRUPT;
            (void)mfmtio_read_u8(stream);
        }
        else
        {
            int length_type = 0;
            if(!msav__ubjson_has_bytes(stream, 1))
                return MSAV_ERR_CORRUPT;
            length_type = mfmtio_read_u8(stream);
            if(msav__ubjson_read_integer(stream, length_type, &declared_count) != MSAV_ERR_OK)
                return MSAV_ERR_CORRUPT;
            if(declared_count > stream->bufsz)
                return MSAV_ERR_CORRUPT;
            has_count = 1;
            container_count = declared_count;
        }
    }

    while(has_count ? entries < container_count : msav__ubjson_has_bytes(stream, 1))
    {
        if(object)
        {
            if(!has_count && stream->buf[stream->idx] == '}')
            {
                stream->idx++;
                break;
            }
            if(stream->buf[stream->idx] == 'N')
            {
                stream->idx++;
                continue;
            }
            if(msav__ubjson_skip_string(stream) != MSAV_ERR_OK)
                return MSAV_ERR_CORRUPT;
        }
        else if(!has_count && stream->buf[stream->idx] == ']')
        {
            stream->idx++;
            break;
        }

        if(!msav__ubjson_has_bytes(stream, 1))
            return MSAV_ERR_CORRUPT;

        {
            int value_type = mfmtio_read_u8(stream);
            if(value_type == 'N')
            {
                if(object && !has_count)
                    continue;
            }
            else if(msav__ubjson_skip_value(stream, value_type, depth + 1) != MSAV_ERR_OK)
            {
                return MSAV_ERR_CORRUPT;
            }
        }

        entries++;
    }

    if(count)
        *count = entries;
    return MSAV_ERR_OK;
}

msav_err_t msav__ubjson_count_entries(
        mfmtio_rstream_t *stream,
        int root_type,
        uint64_t *count)
{
    if(!stream || !count)
        return MSAV_ERR_PARAM;

    if(root_type != '{' && root_type != '[')
        return MSAV_ERR_CORRUPT;

    return msav__ubjson_count_container(stream, root_type == '{', 0, count);
}

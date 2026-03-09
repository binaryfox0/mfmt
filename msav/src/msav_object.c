#include "msav_object.h"

#include <stdlib.h>

#include "msav_stream.h"

/* readObject */
int msav__obj_read(msav__rstream_t *stream, msav__obj_t *obj)
{
    int8_t type = 0;
    int exists = 0;
    int32_t len_i32 = 0;
    int16_t len_i16 = 0;
    int8_t len_i8 = 0;

    type = mfmtio_read_i8(stream);
    switch(type)
    {
        case MSAV__OBJ_NULL:
            break;

        case MSAV__OBJ_INT:
            obj->i = mfmtio_read_i32(stream);
            break;

        case MSAV__OBJ_FLOAT:
            obj->f = mfmtio_read_f32(stream);
            break;
        
        case MSAV__OBJ_STRING:
            /* readString */
            exists = mfmtio_read_bool(stream);
            if(exists)
                obj->s = mfmtio_read_utf(stream);
            break;

        case MSAV__OBJ_CONTENT:
            obj->c.type = mfmtio_read_i8(stream);
            obj->c.idx = mfmtio_read_i32(stream);
            break;

        case MSAV__OBJ_INTSEQ:
            len_i16 = mfmtio_read_i16(stream);
            if(len_i16 < 0)
                return -1;
            if(msav__arr_create(&obj->is, sizeof(int32_t), (size_t)len_i16) < 0)
                return -1;
            for(int16_t i = 0; i < len_i16; i++)
                msav__arr_at(&obj->is, int32_t, i) = mfmtio_read_i32(stream);
            break;

        case MSAV__OBJ_POINT2:
            obj->p.x = mfmtio_read_i32(stream);
            obj->p.y = mfmtio_read_i32(stream);
            break;

        case MSAV__OBJ_POINT2_ARRAY:
            len_i8 = mfmtio_read_i8(stream);
            if(len_i8 < 0)
                return -1;
            if(msav__arr_create(&obj->pa, sizeof(msav__point2_t), (size_t)len_i8) < 0)
                return -1;
            for(int8_t i = 0; i < len_i8; i++)
            {
                int32_t pos = 0;
                msav__point2_t *p2 = 0;

                pos = mfmtio_read_i32(stream);
                p2 = &msav__arr_at(&obj->pa, msav__point2_t, i);
                p2->x = (pos >> 16) & 0xFFFF;
                p2->y = pos & 0xFFFF;
            }
            break;

        case MSAV__OBJ_TECH_NODE:
            obj->tn.type = mfmtio_read_i8(stream);
            obj->tn.idx = mfmtio_read_i32(stream);
            break;

        case MSAV__OBJ_BOOL:
            obj->b = mfmtio_read_bool(stream);
            break;

        case MSAV__OBJ_DOUBLE:
            obj->d = mfmtio_read_f64(stream);
            break;

        case MSAV__OBJ_BUILDING:
            obj->bd.pos = mfmtio_read_i32(stream);
            break;

        case MSAV__OBJ_UNIT_COMMAND_NULL:
            /* skipped */
            mfmtio_read_i8(stream);
            break;

        case MSAV__OBJ_BOOL_ARRAY:
            len_i32 = mfmtio_read_i32(stream);
            if(msav__arr_create(&obj->ba, sizeof(msav__bool), (size_t)len_i32) < 0)
                return -1;
            for(int32_t i = 0; i < len_i32; i++)
                msav__arr_at(&obj->ba, msav__bool, i) = mfmtio_read_bool(stream);
            break;

        case MSAV__OBJ_UNIT:
            obj->u.id = mfmtio_read_i32(stream);
            break;

        case MSAV__OBJ_VEC2_ARRAY:
            len_i16 = mfmtio_read_i16(stream);
            if(msav__arr_create(&obj->va, sizeof(msav__vec2_t), (size_t)len_i16) < 0)
                return -1;
            for(int16_t i = 0; i < len_i16; i++)
            {
                msav__arr_at(&obj->va, msav__vec2_t, i) = 
                    (msav__vec2_t){
                        .x = mfmtio_read_f32(stream), .y = mfmtio_read_f32(stream)
                    };
            }
            break;
           
        case MSAV__OBJ_VEC2:
            obj->v = 
                (msav__vec2_t){
                    .x = mfmtio_read_f32(stream), .y = mfmtio_read_f32(stream)
                };
            break;

        case MSAV__OBJ_TEAM:
            obj->t = (msav__team_t)mfmtio_read_u8(stream);
            break;

        case MSAV__OBJ_INT_ARRAY:
            len_i16 = mfmtio_read_i16(stream);
            if(msav__arr_create(&obj->ia, sizeof(int32_t), (size_t)len_i16) < 0)
                return -1;
            for(int16_t i = 0; i < len_i16; i++)
                msav__arr_at(&obj->va, int32_t, i) = mfmtio_read_i32(stream);
            break;

        case MSAV__OBJ_OBJECT_ARRAY:
            len_i32 = mfmtio_read_i32(stream);
            if(msav__arr_create(&obj->oa, sizeof(msav__obj_t), (size_t)len_i32) < 0)
                return -1;
            for(int32_t i = 0; i < len_i32; i++)
            {
                int ret = 0;
                msav__obj_t *dest = 0;

                dest = &msav__arr_at(&obj->oa, msav__obj_t, i);
                ret = msav__obj_read(stream , dest);
                if(ret < 0)
                {
                    for(; i >= 0; i--)
                        msav__obj_destroy(dest);
                    msav__arr_destroy(&obj->oa);
                    return -1;
                }
            }
            break;

        case MSAV__OBJ_UNIT_COMMAND:
            obj->uc = (msav__unit_command_t)mfmtio_read_u16(stream);
            break;

        default:
            return -1;
    }

    obj->type = (msav__obj_type_t)type;

    return 0;
}

void msav__obj_destroy(msav__obj_t *obj)
{
    if(!obj)
        return;
    switch(obj->type)
    {
        case MSAV__OBJ_STRING:
            free(obj->s);
            break;

        case MSAV__OBJ_OBJECT_ARRAY:
            for(size_t i = 0; i < obj->ga.count; i++)
                msav__obj_destroy(&msav__arr_at(&obj->ga, msav__obj_t, i));
            /* fall-through */

        case MSAV__OBJ_INTSEQ:
        case MSAV__OBJ_POINT2_ARRAY:
        case MSAV__OBJ_BOOL_ARRAY:
        case MSAV__OBJ_VEC2_ARRAY:
        case MSAV__OBJ_INT_ARRAY:
            msav__arr_destroy(&obj->ga);
            break;

        default:
            break;
    }
}

#include "gen_utils.h"

int bdb__tokcmp(const char *s1, const char *s2, const size_t l)
{
    size_t i = 0;

    if (!s1 || !s2)
        return -1;

    if (strlen(s2) != l)
        return -1;

    for (i = 0; i < l; i++)
    {
        if (s1[i] == '\0' || s1[i] != s2[i])
            return -1;
    }

    return 0;
}

char* bdb__tokdup(const char *s, const size_t l)
{
	size_t i = 0;
	size_t len = 0;
	char *out = 0;

	if (!s)
		return 0;

	while (len < l && s[len])
		len++;

	out = (char *)malloc(len + 1);
	if (!out)
		return 0;

	while (i < len)
	{
		out[i] = s[i];
		i++;
	}

	out[len] = '\0';
	return out;
}

/* find class_declaration with given name */
TSNode bdb__gen_find_class(
        TSNode node,
        const char *src,
        const char *class_name
)
{
    uint32_t count = 0;

    if(!src || !class_name)
        return (TSNode){0};

    if(!strcmp(ts_node_type(node), "class_declaration"))
    {
        TSNode name_node = {0};
        uint32_t s = 0;
        uint32_t e = 0;

        name_node = ts_node_child_by_field_name(node, "name", 4);
        if(!ts_node_is_null(name_node))
        {
            s = ts_node_start_byte(name_node);
            e = ts_node_end_byte(name_node);

            if(!bdb__tokcmp(src + s, class_name, (size_t)(e - s)))
                return node;
        }
    }

    count = ts_node_child_count(node);

    for(uint32_t i = 0; i < count; i++)
    {
        TSNode child = {0};
        TSNode found = {0};

        child = ts_node_child(node, i);
        found = bdb__gen_find_class(child, src, class_name);

        if(!ts_node_is_null(found))
            return found;
    }

    return (TSNode){0};
}

char* bdb__sprintf(const char *fmt, ...)
{
    va_list ap;
    va_list ap_copy;

    int len = 0;
    char *buf = 0;

    if(!fmt)
        return 0;

    va_start(ap, fmt);
    va_copy(ap_copy, ap);

    len = vsnprintf(0, 0, fmt, ap);
    va_end(ap);

    if(len < 0)
    {
        va_end(ap_copy);
        return 0;
    }

    buf = (char*)malloc((size_t)len + 1);
    if(!buf)
    {
        va_end(ap_copy);
        return 0;
    }

    vsnprintf(buf, (size_t)len + 1, fmt, ap_copy);
    va_end(ap_copy);

    return buf;
}

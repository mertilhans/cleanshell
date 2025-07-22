int g_last_exit_status = 0;

#include "../shell.h"

int is_valid_char(char c)
{
    return ((c >= 'A' && c <= 'Z')
         || (c >= 'a' && c <= 'z')
         || (c >= '0' && c <= '9')
         || c == '_');
}


char *expand_variables(char *str, t_env *env_list) 
{
    char *res;
    int res_len = 0;
    
        if (!str || !env_list)
        {
	        if (str)
		        return ft_strdup(str);
	        else
		        return ft_strdup("");
        }
    res = gb_malloc(1024);
    if (!res) 
        return NULL;
    res[0] = '\0';
    return expander_loop(str, env_list, res, &res_len);  // env yerine env_list
}

char *expander_loop(char *str, t_env *env_list, char *res, int *res_len)  
{
    int res_cap = 1024;
    int i = 0;
    char *expanded;

    while (str[i])
    {
        if (str[i] == '$')
        {
            expanded = handle_dollar(str, &i, env_list);  
            res = push_res(res, expanded, res_len, &res_cap);
            gc_free(expanded);
        }
        else
            res = push_char_res(res, str[i++], res_len, &res_cap);
    }
    return res;
}

char *handle_dollar(char *str, int *pos, t_env *env_list) 
{
    int i = *pos + 1;
    
    if (str[i] == '?')
    {
        *pos = i + 1;
        return ft_itoa(g_last_exit_status);
    }
    if (str[i] == '$')
    {
        *pos = i + 1;
        return ft_itoa(getpid());
    }
    if (is_valid_char(str[i]))
        return handle_simple(str, pos, env_list);  
    
    (*pos)++;
    return ft_strdup("$");
}

char *handle_simple(const char *str, int *i, t_env *env_list)  // char **env yerine t_env *env_list
{
    int len = 0;
    char *var_name, *var_value;
    
    (*i)++;
    while (is_valid_char(str[*i + len]))
        len++;
    
    if (len == 0)
    {
        (*i)++;
        return ft_strdup("");
    }
    
    var_name = ft_substr(str, *i, len);
    *i += len;
    var_value = find_env(env_list, var_name); 
    gc_free(var_name);
    
    return var_value ? ft_strdup(var_value) : ft_strdup("");
}

char *ft_substr(char const *s, unsigned int start, size_t len)
{
    char *substr;

    if (!s || start >= ft_strlen(s))
        return ft_strdup("");
    
    if (len > ft_strlen(s + start))
        len = ft_strlen(s + start);
    
    substr = gb_malloc((len + 1) * sizeof(char));
    if (!substr) return NULL;
    
    ft_strlcpy(substr, (s + start), (len + 1));
    return substr;
}

size_t ft_strlcpy(char *dest, const char *src, size_t dsize)
{
    char *d = dest;
    const char *s = src;
    size_t i = 0;

    if (dsize > 0)
    {
        while (*s && i < (dsize - 1))
        {
            *d++ = *s++;
            i++;
        }
        *d = '\0';
    }
    return ft_strlen(src);
}

char *push_char_res(char *res, char c, int *res_len, int *res_cap)
{
    char *new_res;

    if (*res_len + 1 >= *res_cap)
    {
        *res_cap *= 2;
        new_res = gb_malloc(*res_cap);
        if (!new_res) return NULL;
        ft_memcpy(new_res, res, *res_len);
        gc_free(res);
        res = new_res;
    }
    res[*res_len] = c;
    (*res_len)++;
    res[*res_len] = '\0';
    return res;
}

char *push_res(char *res, const char *to_add, int *res_len, int *res_cap)
{
    int add_len = ft_strlen(to_add);
    char *new_res;

    if (*res_len + add_len >= *res_cap)
    {
        while (*res_len + add_len >= *res_cap)
            *res_cap *= 2;

        new_res = gb_malloc(*res_cap);
        if (!new_res) return NULL;
        ft_memcpy(new_res, res, *res_len);
        gc_free(res);
        res = new_res;
    }
    ft_memcpy(res + *res_len, to_add, add_len);
    *res_len += add_len;
    res[*res_len] = '\0';
    return res;
}

static int digit_count(int n)
{
    int len = 0;
    
    if (n < 0)
    {
        len++;
        n = -n;
    }
    while (n != 0)
    {
        n = n / 10;
        len++;
    }
    return len;
}

char *ft_itoa(int n)
{
    char *str;
    int len;
    long int num = n;

    len = digit_count(num);
    if (n == 0)
        return ft_strdup("0");
    
    str = gb_malloc((len + 1) * sizeof(char));
    if (!str) return NULL;
    
    if (num < 0)
    {
        str[0] = '-';
        num = -num;
    }
    str[len] = '\0';
    while (num > 0)
    {
        str[--len] = (num % 10) + '0';
        num = num / 10;
    }
    return str;
}

static int is_quote_char(char c, char current_quote)
{
    if ((c == '\'' || c == '"') && current_quote == 0)
        return 1;
    if (c == current_quote) 
        return 1;
    return 0;
}

static char update_quote_state(char c, char current_quote)
{
    if ((c == '\'' || c == '"') && current_quote == 0)
        return c;
    if (c == current_quote)
        return 0;
    return current_quote;
}

static char *append_string_res(char *result, char *str, int *res_len, int *res_cap)
{
    int i = 0;
    
    while (str[i])
    {
        result = push_char_res(result, str[i], res_len, res_cap);
        i++;
    }
    return result;
}
static char *process_expansion(char *result, char *str, int *i, t_env *env,
                              char quote_char, int *res_len, int *res_cap)
{
    char *expanded;

    if (str[*i] == '$' && quote_char != '\'')
    {
        expanded = handle_dollar(str, i, env);
        if (expanded)
        {
            result = append_string_res(result, expanded, res_len, res_cap);
            free(expanded);
        }
    }
    else
        result = push_char_res(result, str[(*i)++], res_len, res_cap);

    return result;
}

char *expand_with_quotes(char *str, t_env *env) 
{
    char *result = gb_malloc(1024);
    int res_len = 0, res_cap = 1024, i = 0;
    char quote_char = 0;
    
    if (!result) 
        return NULL;
    result[0] = '\0';
    
    while (str[i])
    {
        if (is_quote_char(str[i], quote_char))
        {
            quote_char = update_quote_state(str[i], quote_char);
            result = push_char_res(result, str[i++], &res_len, &res_cap);
        }
        else
            result = process_expansion(result, str, &i, env, quote_char, &res_len, &res_cap);
    }
    return result;
}
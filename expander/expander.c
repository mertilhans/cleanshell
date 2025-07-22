#include "gb.h"
#include "shell.h"

int     g_last_exit_status = 0;

char    *expand_variables(char *str,char **env)
{

       char *res;
       int res_len;
       int i;
       
       i = 0;
       res_len = 0;

       if(!str || !env)
       {
                if(str)
                        return(ft_strdup(str));
                else
                        return(ft_strdup(""));
       }
       res = gb_malloc(1024);
       if(!res)
                return NULL;
       return expander_loop(str,env,res, &res_len);
}
char    *expander_loop(char *str,char **env,char *res,int *res_len)
{
        int     res_cap;
        int i;
        char *expanded;

        i = 0;
        res_cap = 1024;

        while(str[i])
        {
                if(str[i] == '$')
                {
                        expanded = handle_dollar(str,&i,env);
                        res = push_res(res,expanded, res_len, &res_cap);
                        gc_free(expanded);
                }
                else
                        res = push_char_res(res,str[i++],res_len,&res_cap);
        }
        return(res);
}
char *handle_dollar(char *str,int *pos,char **env)
{
        int i;
        i = *pos;
        i++;

        if(str[i] == '?')
        {
                *pos = i + 1;
                return (itoa(g_last_exit_status));
        }
        else if(str[i] == '$')
        {
                *pos = i + 1;
                return(itoa(getpid()));
        }
        else if(is_valid_char(str[i]))
                return (handle_simple_var(str,pos,env));
        else
                return (ft_strdup("$"));
        
}
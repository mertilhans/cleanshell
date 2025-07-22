/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: merilhan <merilhan@42kocaeli.com.tr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 05:37:42 by merilhan          #+#    #+#             */
/*   Updated: 2025/07/22 07:38:38 by merilhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "../shell.h"

t_env *find_env(t_env *env_list, const char *key) 
{
    t_env *current;
    
    if (!env_list || !key)
        return NULL;
    
    current = env_list;
    while (current)
    {
        if (ft_strcmp(current->key, key) == 0)
            return current;  // t_env* döndürülüyor
        current = current->next;
    }
    return NULL;
}
int     builtin_export(char **av,t_env *env_list)
{
        if(!av[1]) // kullanıcı sadece export yazmışssa değişkken eklemeden sadece listeme işlemi yap
        {
                t_env *current = env_list;
                while(current)
                {
                        printf("declare -x %s=\"%s\"\n",current->key,current->value);
                        current = current->next;
                }
                return 0;
        }
        return 0;
}
int     builtin_unset(char **av,t_env **env_list)
{
        if(!av[1])
                return 0;
        t_env *current = *env_list;
        t_env *prev = NULL;
        while(current)
        {
                if(strcmp(current->key,av[1]) == 0)
                {
                        if(prev)
                                prev->next = current->next;
                        else
                                *env_list = current->next;
                        free(current->key);
                        free(current->value);
                        free(current);
                        return 0;
                }
                prev = current;
                current = current->next;
        }
        return 0;
}
int     builtin_env(char **av, t_env *env_list)
{
        (void)av;
        t_env   *current = env_list;
        while(current)
        {
                printf("%s=%s\n",current->key,current->value);
                current = current->next;
        }
        return 0;
}
t_env   *init_env(char **env)
{
        t_env *env_list = NULL;
        t_env *new_node;
        char *key;
        char *value;
        char *eq_pos;
        int i;
        i = 0;
        while(env[i])
        {
                eq_pos = strchr(env[i], '=');
                if(eq_pos)
                {
                        key = strndup(env[i],eq_pos - env[i]);
                        value = strdup(eq_pos + 1);
                        new_node = malloc(sizeof(t_env));
                        new_node->key = key;
                        new_node->value = value;
                        new_node->next = env_list;
                        env_list = new_node;
                }
                i++;
        }
        return(env_list);
}
char    *get_env_value(t_env *env_list,char *key)
{
    t_env *env_variable = find_env(env_list, key);
    if (env_variable)
        return env_variable->value;
    return NULL;
}

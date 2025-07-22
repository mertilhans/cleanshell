/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: merilhan <merilhan@42kocaeli.com.tr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 04:24:43 by husarpka          #+#    #+#             */
/*   Updated: 2025/07/22 04:40:49 by merilhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "../gb/gb.h"

void ft_memcpy(char *s1,char *s2, int len)
{
    while(len--)
        *s1++=*s2++;
    *s1 = '\0';
}
int is_quote(char c)
{
    return (c == '\'' || c == '"');
}
int ft_strlen(char *s)
{
    int i = 0;
    while(s[i] && s[i++])
        ;
    return(i);
}


char *ft_strdup(char *str)
{
    char *res;
    char *result;

    res = gb_malloc(strlen(str) + 1);
    result = res;
    
    while(*str)
        *res++ = *str++;
    *res = '\0';
 return (result);   
}
int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}
//
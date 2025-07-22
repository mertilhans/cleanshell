/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: merilhan <merilhan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 03:36:25 by husarpka          #+#    #+#             */
/*   Updated: 2025/07/11 16:26:43 by merilhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "../gb/gb.h"
#include "../parser/parser.h"

t_tokenizer *tokenizer_init(char *input)
{
    t_tokenizer *tokenizer;

    if (!input)
        return (NULL);
    
    tokenizer = gb_malloc(sizeof(t_tokenizer));
    if (!tokenizer)
        return (NULL);
    
    tokenizer->input = ft_strdup(input);
    if (!tokenizer->input)
    {
        free(tokenizer);
        return (NULL);
    }
    
    tokenizer->pos = 0;
    tokenizer->len = ft_strlen(input);
    
   if(tokenizer->len > 0)
      tokenizer->current = input[0];
    else
        tokenizer->current = '\0';  
    return (tokenizer);
}

t_token *create_token(t_token_type type, char *value)
{
    t_token *token;

    token = gb_malloc(sizeof(t_token)); 
    if (!token)
        return (NULL);
    
    token->type = type;
    if (value)
        token->value = ft_strdup(value); 
    else
        token->value = NULL;
    token->next = NULL;
    
    return (token);
}



void print_tokens(t_token *tokens)
{
    t_token *current = tokens;
    const char *type_names[] = {
        "WORD", "PIPE", "REDIR_IN", "REDIR_OUT", "REDIR_APPEND",
        "HEREDOC", "SEMICOLON",
        "EOF", "ERROR" ,
    };

    printf("=== TOKENS ===\n");
    while (current)
    {
        if(type_names[current->type])
            printf("Type %-8s Value  %s\n",type_names[current->type],current->value);
        else
            printf("%sNULL",current->value);
        current = current->next;
    }
    printf("==============\n");
}


int main()
{
	char *line;
	t_token *tokens;
    t_simple_cmd *cmds;
	while(1)
	{
		line = readline("MiniShell->>>   ");
        add_history(line);
		tokens = tokenize_input(line);
		if(tokens)
		{
			print_tokens(tokens);
            cmds = parse_tokens(tokens);
            t_simple_cmd *tmp = cmds;
            while (tmp)
            {
                printf("[main] Komut: ");
                for (int i = 0; tmp->argv && tmp->argv[i]; i++)
                    printf("%s ", tmp->argv[i]);
                printf("\n");
                if (tmp->redir_in)
                    printf("[main]   < input: %s\n", tmp->redir_in);
                if (tmp->redir_out)
                    printf("[main]   > output: %s (%s)\n", tmp->redir_out, tmp->append ? ">>" : ">");
                printf("--------------------------\n");
                tmp = tmp->next;
            }
		}
		else
			printf("tokenization failed");
	}
}
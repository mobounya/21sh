/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_heredoc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobounya <mobounya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/13 18:05:47 by mobounya          #+#    #+#             */
/*   Updated: 2020/11/16 13:51:43 by mobounya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	ft_join_heredoc(char *line, char **doc_str)
{
	char	*temp2;
	char	*temp;

	temp2 = ft_strjoin(line, "\n");
	temp = ft_strjoin(*doc_str, temp2);
	ft_memdel((void**)&temp2);
	ft_memdel((void**)doc_str);
	*doc_str = temp;
}

void	ft_heredoc_prompt(char *delimiter, char **doc_str)
{
	char	*line;

	while (1)
	{
		if ((line = get_line(NULL, "heredoc> ", 1)))
		{
			write(1, "\n", 1);
			if (ft_strcmp(delimiter, line))
				ft_join_heredoc(line, doc_str);
			else
			{
				ft_memdel((void**)&line);
				break ;
			}
		}
		else
			ft_putchar('\n');
	}
}

void	ft_parse_heredoc(t_tokens *head)
{
	char		*delimiter;

	while (head)
	{
		if (head->next && head->next->token_id == DLESS)
		{
			if (head->next->next)
				delimiter = head->next->next->value;
			head->next->heredoc = ft_strdup("");
			ft_heredoc_prompt(delimiter, &(head->next->heredoc));
		}
		head = head->next;
	}
}

void	ft_find_heredoc(t_ast *root)
{
	if (root == NULL)
		return ;
	ft_find_heredoc(root->left);
	if (root->token->token_id == SIMPLE_COMMAND)
		ft_parse_heredoc(root->token->command_tokens);
	ft_find_heredoc(root->right);
}
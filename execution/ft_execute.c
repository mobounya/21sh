/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobounya <mobounya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/06 17:25:33 by mobounya          #+#    #+#             */
/*   Updated: 2020/03/11 20:32:58 by mobounya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "execute.h"

t_processes		*g_procs_lst = NULL;
int				g_exit_code = 0;

const t_builtin_matcher g_builtin_tab[1] =
{
	{"echo", &ft_echo},
};

int		ft_echo(const char **strings)
{
	uint index;

	index = 1;
	while (strings[index])
	{
		ft_putstr(strings[index]);
		if (strings[index + 1])
			ft_putchar_fd(' ', 1);
		index++;
	}
	ft_putchar_fd('\n', 1);
	return (1);
}

char	**ft_lsttoa(t_tokens *list)
{
	int			size;
	char		**cmd;
	int			index;
	t_tokens	*temp;
	char		*string;

	size = 0;
	index = 0;
	temp = list;
	while (temp)
	{
		size++;
		temp = temp->next;
	}
	if ((cmd = malloc(sizeof(char *) * (size + 1))) == NULL)
		exit(ENOMEM);
	cmd[size] = NULL;
	while (index < size)
	{
		if (list->token_id == DQ_STRING || list->token_id == SQ_STRING)
		{
			list->value[ft_strlen(list->value) - 1] = '\0';
			string = ft_strdup(list->value + 1);
			free(list->value);
		}
		else
			string = list->value;
		cmd[index] = string;
		list = list->next;
		index++;
	}
	return (cmd);
}

int		is_builtin(char	**cmd)
{
	uint i;

	i = 0;
	while (i < 1)
	{
		if (!ft_strcmp(g_builtin_tab[i].name, cmd[0]))
		{
			g_builtin_tab[i].function(cmd);
			return (0);
		}
		i++;
	}
	return (1);
}

int		ft_run_binary(char *path, char **args, char **env)
{
	if (access(path, F_OK))
	{
		printf("exist\n");
		return (1);
	}
	if (access(path, X_OK))
	{
		printf("no perm\n");
		return (1);
	}
	if (execve(path, args, env) == -1)
		exit(1);
	return (0);
}

int		ft_execute_command(t_tokens *lst)
{
	static int	*pipefd;
	char		**command;

	if (lst->pipe_before || lst->pipe_after)
	{
		command = ft_lsttoa(lst->command_tokens);
		pipefd = ft_handle_pipe(lst, pipefd, command);
	}
	return (0);
}

int		*ft_execute(t_ast *root)
{
	static int		and_or[2] = {-1, -1};

	if (root == NULL)
		return (and_or);
	ft_execute(root->left);
	if (root->token->token_id == OR)
		and_or[1] = 1;
	else if (root->token->token_id == AND)
		and_or[0] = 1;
	if (root->token->token_id == SIMPLE_COMMAND)
	{
		if ((and_or[0] == -1 && and_or[1] == -1) || \
			((and_or[1] == 1 && g_exit_code != 0) \
				|| (and_or[0] == 1 && g_exit_code == 0)))
		{
			g_exit_code = ft_execute_command(root->token);
			and_or[0] = -1;
			and_or[1] = -1;
		}
	}
	ft_execute(root->right);
	return (and_or);
}

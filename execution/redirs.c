/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awali-al <awali-al@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/12 12:43:04 by mobounya          #+#    #+#             */
/*   Updated: 2020/11/19 18:41:55 by awali-al         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include <errno.h>

int		ft_redirect_to_file(int oflag, int old_fd, char *filename)
{
	char	*path;
	int		new_fd;

	g_exit_code = 0;
	path = ((*filename == '.' || *filename == '/')) ?
			ft_strdup(filename) : ft_strjoin("./", filename);
	if (access(path, F_OK) == 0)
	{
		if (old_fd == 1 && access(path, W_OK))
			g_exit_code = 3;
		else if (old_fd == 0 && access(path, R_OK))\
			g_exit_code = 3;
	}
	else if ((oflag | O_RDONLY) == 0)
		g_exit_code = 1;
	if (g_exit_code)
	{
		ft_memdel((void**)&path);
		return (g_exit_code);
	}
	new_fd = open(path, oflag, S_IRUSR | S_IWUSR);
	dup2(new_fd, old_fd);
	ft_memdel((void**)&path);
	close(new_fd);
	return (0);
}

void	ft_fdagr(char *value)
{
	int		old_fd;
	int		new_fd;

	new_fd = value[0] - '0';
	if (value[3] == '-')
		close(new_fd);
	else
	{
		old_fd = value[3] - '0';
		dup2(old_fd, new_fd);
	}
}

void	ft_fd_to_file(char *value)
{
	int		old_fd;
	char	*filename;

	old_fd = value[0] - '0';
	filename = ft_strdup(value + 2);
	if (filename == NULL)
		exit(ENOMEM);
	ft_redirect_to_file(O_TRUNC | O_WRONLY | O_CREAT, old_fd, filename);
	free(filename);
}

void	ft_heredoc(char *value)
{
	int		fd;
	char	*doc_file;

	doc_file = "/Users/mobounya/Library/.temp";
	fd = open(doc_file, O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
	write(fd, value, ft_strlen(value));
	close(fd);
	ft_redirect_to_file(O_RDONLY, STDIN_FILENO, doc_file);
}

int		ft_set_redirs(t_tokens *lst)
{
	while (lst)
	{
		if ((lst->token_id == GREAT) || (lst->token_id == DGREAT))
		{
			if (lst->token_id == GREAT)
				ft_redirect_to_file(O_TRUNC | O_WRONLY | O_CREAT,
					1, lst->next->value);
			else if (lst->token_id == DGREAT)
				ft_redirect_to_file(O_WRONLY | O_APPEND | O_CREAT,
					1, lst->next->value);
		}
		else if (lst->token_id == FD_GREAT_AGR || lst->token_id == FD_LESS_AGR)
			ft_fdagr(lst->value);
		else if (lst->token_id == FD_FILE)
			ft_fd_to_file(lst->value);
		else if (lst->token_id == FD_GREAT)
			ft_redirect_to_file(O_TRUNC | O_WRONLY | O_CREAT,
					lst->value[0] - 48, lst->next->value);
		else if (lst->token_id == DLESS)
			ft_heredoc(lst->heredoc);
		else if (lst->token_id == LESS)
			ft_redirect_to_file(O_RDONLY, 0, lst->next->value);
		lst = lst->next;
	}
	return (0);
}

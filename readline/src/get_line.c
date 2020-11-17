/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_line.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobounya <mobounya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/26 13:46:50 by awali-al          #+#    #+#             */
/*   Updated: 2020/11/17 12:54:16 by mobounya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/to_sh_rl.h"

void			reset_highlight(t_line *line)
{
	int				i;

	if (line->hgh)
	{
		line->hgh = NULL;
		line->len = 0;
		line->way = 0;
		i = line->idx;
		home(line);
		put_in_pos(line->str);
		while (line->idx < i)
			go_right(line);
	}
}

static void		store_print(t_hist **his, t_line *line)
{
	if (edit_in_pos(line))
		reset_highlight(line);
	else if (arrow_movement(line))
		reset_highlight(line);
	else if (navigation(line))
		reset_highlight(line);
	else if (his_nav(his, line))
		reset_highlight(line);
	else if (highlight(line))
		g_past = ft_strnew(1);
	else if (g_past || line->hgh)
		ccp(line);
}

static int		display_prompt(int c)
{
	char		*col;
	char		*path;
	static char	*tmp = NULL;

	if (c)
		col = GRN_COL;
	else
		col = RED_COL;
	path = getcwd(NULL, 0);
	if (path)
	{
		ft_strdel(&tmp);
		tmp = ft_strrchr(path, '/');
		tmp = tmp && tmp[1] ? ft_strdup(tmp + 1) : ft_strdup(path);
		ft_strdel(&path);
	}
	ft_putstr(tmp);
	ft_putstr(col);
	ft_putstr(" $> ");
	ft_putstr(RST_COL);
	return (ft_strlen(tmp) + 4);
}

static t_line	line_ini(char *prom, int c)
{
	struct winsize	ws;
	t_line			ret;

	ret.prm = prom ? to_putstr(prom) : display_prompt(c);
	ioctl(0, TIOCGWINSZ, &ws);
	ret.curp = get_curpos();
	ret.str = ft_strnew(1);
	ret.col = ws.ws_col;
	ret.row = ws.ws_row;
	ret.tmp = NULL;
	ret.hgh = NULL;
	ret.len = 0;
	ret.way = 0;
	ret.idx = 0;
	return (ret);
}

char			*get_line(t_hist **his, char *prom, int c)
{
	t_line			line;
	char			*ret;

	set_input_mode();
	line = line_ini(prom, c);
	while (1)
	{
		line.buf = 0;
		read(0, &line.buf, 12);
		if (line.buf != '\n' && line.buf != '\004')
			store_print(his, &line);
		else
			break ;
	}
	if (line.buf == '\004')
		ret = NULL;
	else
		ret = ft_strdup(line.str);
	ft_strdel(&line.str);
	reset_input_mode();
	return (ret);
}

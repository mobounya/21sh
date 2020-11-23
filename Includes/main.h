/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebou-nya <ebou-nya@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/19 18:51:10 by awali-al          #+#    #+#             */
/*   Updated: 2020/11/23 02:28:50 by ebou-nya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_H
#define MAIN_H
#include "tokenize.h"
#include "errors.h"
#include "execute.h"
#include "parser.h"
#include "readline.h"

int g_pid;
typedef struct s_env
{
	char **env;
	unsigned int size;
} t_env;

void ft_get_cmd(t_tokens *head, char **env);
t_ast *ft_parse(t_tokens *lst);
int *ft_trav_exec(t_ast *root, char ***env);
int ft_arraysize(char **ar);
void ft_free_ast(t_ast **root);
void free_his(t_hist **his);
void setup_files(t_ast *root);
void init_prompt(void);
#endif

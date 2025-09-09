/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 15:37:01 by psantos-          #+#    #+#             */
/*   Updated: 2025/09/09 16:15:59 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_redirs(t_redir *redir)
{
	t_redir	*tmp;

	while (redir)
	{
		tmp = redir->next;
		if (redir->target)
			free(redir->target);
		free(redir);
		redir = tmp;
	}
}

void	free_ast(t_ast *node)
{
	int	i;

	if (!node)
		return ;
	if (node->argv)
	{
		i = 0;
		while (node->argv[i])
		{
			free(node->argv[i]);
			i++;
		}
		free(node->argv);
	}
	if (node->redirs)
		free_redirs(node->redirs);
	if (node->left)
		free_ast(node->left);
	if (node->right)
		free_ast(node->right);
	free(node);
}

void	clean_shell(t_info *info)
{
	t_env	*tmp;

	if (!info)
		return ;
	if (info->tree)
		free_ast(info->tree);
	if (info->env_array)
		free_env_array(info->env_array);
	if (info->child_pids)
		free(info->child_pids);
	if (info->env_list)
	{
		while (info->env_list)
		{
			tmp = info->env_list->next;
			if (info->env_list->key)
				free(info->env_list->key);
			if (info->env_list->value)
				free(info->env_list->value);
			free(info->env_list);
			info->env_list = tmp;
		}
	}
	//clear_history();
}

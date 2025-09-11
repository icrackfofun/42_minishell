/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 01:56:53 by psantos-          #+#    #+#             */
/*   Updated: 2025/09/11 00:04:46 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	reap_children(t_info *info)
{
	int	status;
	int	i;

	i = 0;
	while (i < info->child_count)
	{
		if (waitpid(info->child_pids[i], &status, 0) > 0)
		{
			if (WIFEXITED(status))
			{
				if (WEXITSTATUS(status) != 0)
					info->last_status = 1;
				else
					info->last_status = 0;
			}
			else
				info->last_status = 1;
		}
		i++;
	}
	info->child_count = 0;
}


int	executor(t_ast *node, t_info *info)
{
	t_ast	**cmds;
	int		count;

	if (node->type == NODE_COMMAND)
	{
		info->child_pids = malloc(sizeof(pid_t));
		if (!info->child_pids)
			return (parent_error("malloc", info), 1);
		exec_command(node, info, 1);
	}
	else if (node->type == NODE_PIPE)
	{
		cmds = flatten_pipeline(node, &count);
		info->child_pids = malloc(sizeof(pid_t) * (count));
		if (!info->child_pids)
			return (parent_error("malloc", info), 1);
		if (!cmds)
			return (1);
		exec_pipeline(cmds, count, info, -1);
		free(cmds);
	}
	
	return (reap_children(info), 0);
}

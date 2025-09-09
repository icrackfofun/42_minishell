/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 01:56:53 by psantos-          #+#    #+#             */
/*   Updated: 2025/09/09 16:15:34 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	reap_children(t_info *info)
{
	int status;
	int i;

	i = 0;
	while(i < info->child_count)
	{
		if (waitpid(info->child_pids[i], &status, 0) > 0)
		{
			if (WIFEXITED(status))
				info->last_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				info->last_status = 128 + WTERMSIG(status);
		}
		i++;
	}
	info->child_count = 0;
}

void executor(t_ast *node, t_info *info)
{
	t_ast	**cmds;
	int		count;

	if (node->type == NODE_COMMAND)
	{
		info->child_pids = malloc(sizeof(pid_t));
		exec_command(node, info, 1);
		if (!info->child_pids)
		{
			perror("malloc");
			return ;
		}
	}
	else if (node->type == NODE_PIPE)
	{
		cmds = flatten_pipeline(node, &count, info);
		if (cmds)
		{
			exec_pipeline(cmds, count, info, -1);
			free(cmds);
			free(info->child_pids);
		}
	}
	reap_children(info);
}

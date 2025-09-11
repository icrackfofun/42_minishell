/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 16:07:55 by psantos-          #+#    #+#             */
/*   Updated: 2025/09/11 16:28:56 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// static int exec_builtin(t_ast *cmd, t_info *info)
// {

// }

static void	exec_child(t_ast *cmd, t_info *info)
{
	char	*path;

	path = get_path(info, cmd);
	env_list_to_array(info);
	if (cmd->redirs)
		handle_redirections(cmd->redirs);
	execve(path, cmd->argv, info->env_array);
	exit_error("execve", 1);
}

static void	exec_external(t_ast *cmd, t_info *info, int root)
{
	pid_t	pid;
	char	*path;

	if (!root)
		exec_child(cmd, info);
	if (cmd->redirs)
			prepare_heredocs(cmd, info);
	pid = fork();
	if (pid < 0)
	{
		parent_error("fork", info);
		return ;
	}
	if (pid == 0)
	{
		path = get_path(info, cmd);
		env_list_to_array(info);
		if (cmd->redirs)
			handle_redirections(cmd->redirs);
		execve(path, cmd->argv, info->env_array);
		exit_error("execve", 1);
	}
	info->child_pids[info->child_count++] = pid;
}

void	exec_command(t_ast *cmd, t_info *info, int root)
{
	if ((!cmd || !cmd->argv || !cmd->argv[0]) && root)
	{
		info->last_status = 1;
		return ;
	}
	//if (cmd->is_builtin == 1)
		//return (exec_builtin(cmd, info));
	exec_external(cmd, info, root);
}

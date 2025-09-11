/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 16:17:37 by psantos-          #+#    #+#             */
/*   Updated: 2025/09/11 16:39:45 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	create_pipe_fd(int pipefd[2], t_info *info)
{
	if (pipe(pipefd) < 0)
		return (parent_error("pipe", info), -1);
	return (0);
}

static pid_t	fork_command_stage(t_ast *cmd, t_info *info,
									int input_fd, int output_fd)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (parent_error("fork", info), -1);
	if (pid == 0)
	{
		if (input_fd != -1)
		{
			dup2(input_fd, STDIN_FILENO);
			close(input_fd);
		}
		if (output_fd != -1)
		{
			dup2(output_fd, STDOUT_FILENO);
			close(output_fd);
		}
		exec_command(cmd, info, 0);
		exit_error("exec failed", 1);
	}
	return (pid);
}

static void	update_parent_fds(int *input_fd, int pipefd[2], int i, int count)
{
	if (*input_fd != -1)
		close(*input_fd);
	if (i < count - 1)
	{
		close(pipefd[1]);
		*input_fd = pipefd[0];
	}
}

static void	track_child_pid(t_info *info, pid_t pid)
{
	if (pid > 0)
		info->child_pids[info->child_count++] = pid;
}

void	exec_pipeline(t_ast **cmds, int count, t_info *info, int input_fd)
{
	int		pipefd[2];
	pid_t	pid;
	int		i;
	int		write_fd;

	i = 0;
	while (i < count)
	{
		write_fd = -1;
		if (cmds[i]->redirs)
			prepare_heredocs(cmds[i], info);
		if (i < count - 1)
		{
			if (create_pipe_fd(pipefd, info) < 0 || info->last_status == 1)
				return ;
			write_fd = pipefd[1];
		}
		pid = fork_command_stage(cmds[i], info, input_fd, write_fd);
		if (pid < 0 || info->last_status == 1)
			return ;
		track_child_pid(info, pid);
		update_parent_fds(&input_fd, pipefd, i++, count);
	}
}

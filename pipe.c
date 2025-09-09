/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 16:17:37 by psantos-          #+#    #+#             */
/*   Updated: 2025/09/09 15:54:57 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*void exec_pipeline(t_ast **cmds, int count, t_info *info)
{
	int input_fd = -1;  // start with stdin
	int pipefd[2];
	pid_t pid;

	for (int i = 0; i < count; i++) {
		// Last command? No need for a new pipe
		if (i < count - 1) {
			if (pipe(pipefd) < 0) {
				perror("pipe");
				return;
			}
		}
		pid = fork();
		if (pid < 0) {
			perror("fork");
			return;
		}
		if (pid == 0) {
			// Child process
			if (input_fd != -1) {
				dup2(input_fd, STDIN_FILENO);
				close(input_fd);
			}
			if (i < count - 1) { // not last command
				dup2(pipefd[1], STDOUT_FILENO);
				close(pipefd[0]);
				close(pipefd[1]);
			}
			exec_command(cmds[i], info, 0);
			exit_error("exec failed", 1);
		}
		// Parent process
		if (input_fd != -1)
			close(input_fd); // no longer needed
		if (i < count - 1) {
			close(pipefd[1]);      // parent doesn’t write
			input_fd = pipefd[0];  // next command reads here
		}

		info->child_pids[info->child_count++] = pid;
	}
}*/

static int create_pipe_fd(int pipefd[2])
{
	if (pipe(pipefd) < 0)
		return (perror("pipe"), -1);
	return (0);
}

static pid_t fork_command_stage(t_ast *cmd, t_info *info, int input_fd, int output_fd)
{
	pid_t pid = fork();
	if (pid < 0)
		return (perror("pipe"), -1);
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

static void update_parent_fds(int *input_fd, int pipefd[2], int last)
{
	if (*input_fd != -1)
		close(*input_fd);
	if (!last)
	{
		close(pipefd[1]);
		*input_fd = pipefd[0];
	}
}

static void track_child_pid(t_info *info, pid_t pid)
{
	if (pid > 0)
		info->child_pids[info->child_count++] = pid;
}

void exec_pipeline(t_ast **cmds, int count, t_info *info, int input_fd)
{
	int		pipefd[2];
	pid_t	pid;
	int		i;
	int		last;
	int		write_fd;

	i = 0;
	while(i < count)
	{
		last = (i == count - 1);
		write_fd = -1;
		if (!last)
		{
			if (create_pipe_fd(pipefd) < 0)
				return ;
			write_fd = pipefd[1];
		}
		pid = fork_command_stage(cmds[i], info, input_fd, write_fd);
		track_child_pid(info, pid);
		update_parent_fds(&input_fd, pipefd, last);
		i++;
	}
}

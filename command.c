/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 16:07:55 by psantos-          #+#    #+#             */
/*   Updated: 2025/09/06 18:32:08 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/***Step 1: Execute a single command with no redirections**
- **Goal:** Run a command like `ls -l` with no pipes or redirections.
- **Tasks:**
	1. Create `executor(t_ast *node)` function.
	2. Check if `node->type == NODE_COMMAND`.
	3. Fork a child process.
	4. In child: call `execve()` with `argv` and `envp`.
	5. In parent: `waitpid()` and store exit status.
- ✅ Test: `ls`, `pwd`, `echo hello`.*/

static int	exec_external(t_ast *cmd, t_info *info)
{
	pid_t	pid;
	int		status;
	char	*path;

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return (1);
	}
	if (pid == 0)
	{
		path = get_path(info, cmd);
		env_list_to_array(info);
		execve(path, cmd->argv, info->env_array);
		perror("execve");
		exit(127);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (0);
}

int	exec_command(t_ast *cmd, t_info *info)
{
	int	status;

	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (1);
	//if (cmd->redirs)
	//	handle_redirections(cmd->redirs);
	//if (cmd->is_builtin == 1)
	//    return exec_builtin(cmd, shell);
	status = exec_external(cmd, info);
	//restore_redirections();
	return (status);
}

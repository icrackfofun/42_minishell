/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 15:43:11 by psantos-          #+#    #+#             */
/*   Updated: 2025/09/09 16:36:34 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <unistd.h>   // execve, fork
# include <sys/wait.h> // waitpid
# include <stdio.h>    // perror
# include <stdlib.h>   // exit
# include <readline/readline.h>
# include <readline/history.h>

//AST Structure Nodes
typedef enum e_node_type
{
	NODE_COMMAND,
	NODE_PIPE
}	t_node_type;

typedef enum e_redir_type
{
	REDIR_INPUT,
	REDIR_OUTPUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}	t_redir_type;

typedef struct s_redir
{
	t_redir_type	type;
	char			*target;
	struct s_redir	*next;
}	t_redir;

typedef struct s_ast
{
	t_node_type		type;
	char			**argv;
	t_redir			*redirs;
	int				is_builtin;
	struct s_ast	*left;
	struct s_ast	*right;
}	t_ast;

//Env vars List
typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

//General Info Structure
typedef struct s_info
{
	t_env	*env_list;
	char	**env_array;
	int		last_status;
	//pointer to token struct
	t_ast	*tree;
	pid_t	*child_pids;
	int		child_count;
}	t_info;

//cleaner
/*void minishell_clear_history(void)
{
#ifdef __LINUX__
	if (using_history())
		rl_clear_history();
#else
	if (using_history())
		clear_history();
#endif
}*/
void	clean_shell(t_info *info);
void	free_ast(t_ast *node);
void	free_env_array(char **arr);

//error
void	exit_error(char *message, int code);
void	fork_error(t_info *info);

//env
void	env_list_to_array(t_info *info);
t_env	*env_init(char **envp);

//env_ops
char	*get_env_value(t_env *env_list, const char *key);
void	set_env_value(t_env **env_list, const char *key, const char *value);
void	unset_env(t_env **env_list, const char *key);

//path
char	*get_path(t_info *info, t_ast *cmd);

//command execution
void	executor(t_ast *node, t_info *info);
t_ast	**flatten_pipeline(t_ast *root, int *out_count, t_info *info);
void	exec_pipeline(t_ast **cmds, int count, t_info *info, int input_fd);
void	exec_command(t_ast *cmd, t_info *info, int root);

#endif
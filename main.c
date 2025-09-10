/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 18:20:32 by psantos-          #+#    #+#             */
/*   Updated: 2025/09/10 23:45:32 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_last_signal;

static void	info_init(t_info *info, char **envp)
{
	info->env_list = env_init(envp);
	info->env_array = NULL;
	info->last_status = 0;
	info->tree = NULL;
	info->child_count = 0;
	info->heredoc = NULL;
}

int	main(int argc, char **argv, char **envp)
{
	t_info	info;
	//char	*line;

	(void)argc;
	(void)argv;
	
	info_init(&info, envp);
	/*while (1)
	{
		line = readline("minishell$ ");
		if (!line) // Ctrl+D
		{
			printf("exit\n");
			break ;
		}
		if (*line)
			add_history(line);
		info.tree = parse(line); //call free_ast at beginning of parser to clean old tree
		free(line);
		if (info.tree)
			executor(info.tree, &info);
		printf("\n");
		clean_shell(&info);
	}*/

	// 1 command test
	// t_ast	*node;
	// node = malloc(sizeof(t_ast));
	// node->type = NODE_COMMAND;
	// node->argv = malloc(3 * sizeof(char *));
	// node->argv[0] = ft_strdup("ls");
	// node->argv[1] = ft_strdup("-l");
	// node->argv[2] = NULL;
	// node->redirs = NULL;
	// node->is_builtin = 0;
	// node->left = NULL;
	// node->right = NULL;
	// info.tree = node;

	// ls -l | grep command | sort | wc -l
	// t_ast *node_ls, *node_grep, *node_sort, *node_wc;
	// t_ast *pipe1, *pipe2, *pipe3;
	// node_ls = malloc(sizeof(t_ast));
	// node_ls->type = NODE_COMMAND;
	// node_ls->argv = malloc(3 * sizeof(char *));
	// node_ls->argv[0] = ft_strdup("ls");
	// node_ls->argv[1] = ft_strdup("-l");
	// node_ls->argv[2] = NULL;
	// node_ls->redirs = NULL;
	// node_ls->is_builtin = 0;
	// node_ls->left = NULL;
	// node_ls->right = NULL;
	// node_grep = malloc(sizeof(t_ast));
	// node_grep->type = NODE_COMMAND;
	// node_grep->argv = malloc(3 * sizeof(char *));
	// node_grep->argv[0] = ft_strdup("grep");
	// node_grep->argv[1] = ft_strdup("command");
	// node_grep->argv[2] = NULL;
	// node_grep->redirs = NULL;
	// node_grep->is_builtin = 0;
	// node_grep->left = NULL;
	// node_grep->right = NULL;
	// node_sort = malloc(sizeof(t_ast));
	// node_sort->type = NODE_COMMAND;
	// node_sort->argv = malloc(2 * sizeof(char *));
	// node_sort->argv[0] = ft_strdup("sort");
	// node_sort->argv[1] = NULL;
	// node_sort->redirs = NULL;
	// node_sort->is_builtin = 0;
	// node_sort->left = NULL;
	// node_sort->right = NULL;
	// node_wc = malloc(sizeof(t_ast));
	// node_wc->type = NODE_COMMAND;
	// node_wc->argv = malloc(3 * sizeof(char *));
	// node_wc->argv[0] = ft_strdup("wc");
	// node_wc->argv[1] = ft_strdup("-l");
	// node_wc->argv[2] = NULL;
	// node_wc->redirs = NULL;
	// node_wc->is_builtin = 0;
	// node_wc->left = NULL;
	// node_wc->right = NULL;
	// pipe1 = malloc(sizeof(t_ast));
	// pipe1->type = NODE_PIPE;
	// pipe1->argv = NULL;
	// pipe1->redirs = NULL;
	// pipe1->is_builtin = 0;
	// pipe1->left = node_ls;
	// pipe1->right = node_grep;
	// pipe2 = malloc(sizeof(t_ast));
	// pipe2->type = NODE_PIPE;
	// pipe2->argv = NULL;
	// pipe2->redirs = NULL;
	// pipe2->is_builtin = 0;
	// pipe2->left = pipe1;
	// pipe2->right = node_sort;
	// pipe3 = malloc(sizeof(t_ast));
	// pipe3->type = NODE_PIPE;
	// pipe3->argv = NULL;
	// pipe3->redirs = NULL;
	// pipe3->is_builtin = 0;
	// pipe3->left = pipe2;
	// pipe3->right = node_wc;
	// info.tree = pipe3;

	// cat < input.txt
	// t_ast *node_cat = malloc(sizeof(t_ast));
	// node_cat->type = NODE_COMMAND;
	// node_cat->argv = malloc(2 * sizeof(char *));
	// node_cat->argv[0] = ft_strdup("cat");
	// node_cat->argv[1] = NULL;
	// node_cat->redirs = malloc(sizeof(t_redir));
	// node_cat->redirs->type = REDIR_INPUT;
	// node_cat->redirs->target = ft_strdup("hello.txt");
	// node_cat->redirs->next = NULL;
	// node_cat->is_builtin = 0;
	// node_cat->left = node_cat->right = NULL;
	// t_ast *node_grep = malloc(sizeof(t_ast));
	// node_grep->type = NODE_COMMAND;
	// node_grep->argv = malloc(3 * sizeof(char *));
	// node_grep->argv[0] = ft_strdup("grep");
	// node_grep->argv[1] = ft_strdup("Hello");
	// node_grep->argv[2] = NULL;
	// node_grep->redirs = malloc(sizeof(t_redir));
	// node_grep->redirs->type = REDIR_OUTPUT;
	// node_grep->redirs->target = ft_strdup("output.txt");
	// node_grep->redirs->next = NULL;
	// node_grep->is_builtin = 0;
	// node_grep->left = node_grep->right = NULL;
	// t_ast *pipe_node = malloc(sizeof(t_ast));
	// pipe_node->type = NODE_PIPE;
	// pipe_node->argv = NULL;
	// pipe_node->redirs = NULL;
	// pipe_node->is_builtin = 0;
	// pipe_node->left = node_cat;
	// pipe_node->right = node_grep;
	// info.tree = pipe_node;

	// cat << EOF
	t_ast *node;
	t_redir *redir;
	node = malloc(sizeof(t_ast));
	node->type = NODE_COMMAND;
	node->argv = malloc(2 * sizeof(char *));
	node->argv[0] = ft_strdup("cat");
	node->argv[1] = NULL;
	node->is_builtin = 0;
	node->left = NULL;
	node->right = NULL;
	redir = malloc(sizeof(t_redir));
	redir->type = REDIR_HEREDOC;
	redir->target = ft_strdup("EOF");
	redir->next = NULL;
	node->redirs = redir;
	info.tree = node;

	// Execute
	executor(info.tree, &info);
	printf("Exit status: %d\n", info.last_status);
	clean_shell(&info);
}

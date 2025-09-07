/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 18:20:32 by psantos-          #+#    #+#             */
/*   Updated: 2025/09/07 01:55:16 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_last_signal;

static void	info_init(t_info *info, char **envp)
{
	info->env_list = env_init(envp);
	info->env_array = NULL;
	info->last_status = 0;
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
	}*/

	t_ast	*node;
	// Init shell info
	info.env_list = env_init(envp);
	info.last_status = 0;
	info.tree = NULL;
	// Create node
	node = malloc(sizeof(t_ast));
	node->type = NODE_COMMAND;
	node->argv = malloc(3 * sizeof(char *));
	node->argv[0] = ft_strdup("cat");
	node->argv[1] = ft_strdup("hello.txt");
	node->argv[2] = NULL;
	node->redirs = NULL;
	node->is_builtin = 0;
	node->left = NULL;
	node->right = NULL;
	info.tree = node;
	// Execute
	info.last_status = exec_command(info.tree, &info);
	printf("\n");
	printf("Exit status: %d\n", info.last_status);

	clean_shell(&info);
}

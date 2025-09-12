/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 16:16:22 by psantos-          #+#    #+#             */
/*   Updated: 2025/09/12 16:25:37 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	clean_loop(t_info *info)
{
	if (!info)
		return ;
	if (info->tree)
		free_ast(info->tree);
	if (info->child_pids)
		free(info->child_pids);
	cleanup_heredoc_files();
	//clear_history();
}
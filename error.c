/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 19:47:46 by psantos-          #+#    #+#             */
/*   Updated: 2025/09/10 23:11:54 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_error(char *message, int code, t_info *info)
{
	perror(message);
	if (info->heredoc)
	{
		unlink(info->heredoc);
		free(info->heredoc);
	}
	exit(code);
}

void	parent_error(char *message, t_info *info)
{
	perror(message);
	info->last_status = 1;
}
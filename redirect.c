/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 14:36:30 by psantos-          #+#    #+#             */
/*   Updated: 2025/09/10 23:29:15 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*heredoc_filename(void)
{
	char	*filename;
	char	*pid_str;

	pid_str = ft_itoa(getpid());
	if (!pid_str)
		return (NULL);
	filename = ft_strjoin(".minishell_heredoc_", pid_str);
	free (pid_str);
	return (filename);
}

static int write_heredoc_to_tmp(const char *delimiter, char *filename)
{
    int		fd;
    char	*line;

    fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0)
		return (perror("open heredoc tmp"), -1);
    while (1)
    {
        line = readline("> ");
        if (!line)
            return (close(fd), -1);
        if (ft_strcmp(line, delimiter) == 0)
        {
            free(line);
            break ;
        }
        write(fd, line, ft_strlen(line));
        write(fd, "\n", 1);
        free(line);
    }
    close(fd);
    fd = open(filename, O_RDONLY);
    if (fd < 0)
        perror("reopen heredoc tmp");
    return (fd);
}

static int open_heredoc(const char *delimiter, char *filename, t_info *info)
{
    if (write_heredoc_to_tmp(delimiter, filename) < 0)
        return (-1);
	info->heredoc = filename;
    return (open(filename, O_RDONLY));
}

static void	change_fd(t_redir *redir, int fd, t_info *info)
{
	if (fd < 0)
		exit_error(redir->target, 1, info);
	if (redir->type == REDIR_INPUT || redir->type == REDIR_HEREDOC)
	{
		if (dup2(fd, STDIN_FILENO) < 0)
		{
			close(fd);
			exit_error("dup2", 1, info);
		}
	}
	else
	{
		if (dup2(fd, STDOUT_FILENO) < 0)
		{
			close(fd);
			exit_error("dup2", 1, info);
		}
	}
}

void	handle_redirections(t_redir *redir, t_info *info)
{
	int		fd;
	char	*filename;

	while (redir)
	{
		if (redir->type == REDIR_INPUT)
			fd = open(redir->target, O_RDONLY);
		else if (redir->type == REDIR_OUTPUT)
			fd = open(redir->target, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (redir->type == REDIR_APPEND)
			fd = open(redir->target, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else if (redir->type == REDIR_HEREDOC)
		{
			filename = heredoc_filename();
			if (!filename)
				exit_error("malloc", 1, info);
			fd = open_heredoc(redir->target, filename, info);
		}
		change_fd(redir, fd, info);
		close(fd);
		redir = redir->next;
	}
}

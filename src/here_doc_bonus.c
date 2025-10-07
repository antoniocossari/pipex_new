/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acossari <acossari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 21:32:17 by acossari          #+#    #+#             */
/*   Updated: 2025/10/01 22:25:03 by acossari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex_bonus.h"

/**
 * Read heredoc input and write to file descriptor
 *
 * @param fd File descriptor to write to
 * @param limiter Heredoc limiter string
 */
static void	read_heredoc_input(int fd, char *limiter)
{
	char	*line;
	size_t	limiter_len;

	limiter_len = ft_strlen(limiter);
	while (1)
	{
		ft_putstr_fd("pipe heredoc> ", STDERR_FILENO);
		line = ft_get_next_line(STDIN_FILENO);
		if (!line || (ft_strncmp(line, limiter, limiter_len) == 0
				&& line[limiter_len] == '\n'))
		{
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		free(line);
	}
}

/**
 * Read input from stdin until LIMITER is found
 * Store input in a temporary file
 *
 * @param data Pipex data structure
 * @return File descriptor for reading heredoc input, or -1 on error
 */
int	handle_here_doc(t_pipex *data)
{
	int	fd;

	fd = open(".heredoc_tmp", O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (fd == -1)
		return (perror("pipex: open"), -1);
	read_heredoc_input(fd, data->limiter);
	close(fd);
	fd = open(".heredoc_tmp", O_RDONLY);
	unlink(".heredoc_tmp");
	if (fd == -1)
		return (perror("pipex: open"), -1);
	return (fd);
}

/**
 * Execute the pipeline in here_doc mode
 *
 * @param data Pipex data structure
 * @param envp Environment variables
 * @return Exit status
 */
int	execute_here_doc_pipeline(t_pipex *data, char **envp)
{
	t_exec_ctx	ctx;

	ctx.pipex = data;
	ctx.envp = envp;
	ctx.here_doc_fd = handle_here_doc(data);
	if (ctx.here_doc_fd == -1)
		return (EXIT_FAILURE);
	if (setup_pipeline_resources(data, &ctx.pipes, &ctx.pids) == -1)
		return (close(ctx.here_doc_fd), EXIT_FAILURE);
	if (fork_and_execute_child_processes(&ctx) == -1)
		return (EXIT_FAILURE);
	close(ctx.here_doc_fd);
	if (data->cmd_count > 1)
		close_pipe(ctx.pipes[data->cmd_count - 2]);
	free_pipes(ctx.pipes, data->cmd_count - 1);
	return (wait_and_cleanup(ctx.pids, data->cmd_count));
}

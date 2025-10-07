/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acossari <acossari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 21:06:46 by acossari          #+#    #+#             */
/*   Updated: 2025/10/01 17:07:11 by acossari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex_bonus.h"

/**
 * Close single pipe
 * 
 * @param pipe_fd Pipe file descriptors
 */
void	close_pipe(int *pipe_fd)
{
	close(pipe_fd[0]);
	close(pipe_fd[1]);
}

/**
 * Close and free all pipes
 *
 * @param pipes Array of pipes
 * @param count Number of pipes
 */
void	close_all_pipes(int **pipes, int count)
{
	int	i;

	if (!pipes)
		return ;
	i = 0;
	while (i < count)
	{
		if (pipes[i])
			close_pipe(pipes[i]);
		i++;
	}
}

/**
 * Wait for all processes and free pids array
 *
 * @param pids Array of child process IDs
 * @param count Number of child processes
 */
static void	wait_all_processes(pid_t *pids, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		waitpid(pids[i], NULL, 0);
		i++;
	}
	free(pids);
}

/**
 * Wait for all child processes and cleanup
 *
 * @param pids Array of child process IDs
 * @param count Number of child processes
 * @return Exit status of the last command
 */
int	wait_and_cleanup(pid_t *pids, int count)
{
	int	status;
	int	i;
	int	last_exit_status;

	last_exit_status = EXIT_SUCCESS;
	i = 0;
	while (i < count)
	{
		waitpid(pids[i], &status, 0);
		if (i == count - 1)
		{
			if (WIFEXITED(status))
				last_exit_status = WEXITSTATUS(status);
			else
				last_exit_status = EXIT_FAILURE;
		}
		i++;
	}
	free(pids);
	return (last_exit_status);
}

/**
 * Cleanup resources and exit on fork error
 *
 * @param ctx Execution context
 * @param count Number of child processes to wait for
 * @return EXIT_FAILURE
 */
int	cleanup_on_fork_error(t_exec_ctx *ctx, int count)
{
	close_and_free_pipes(ctx->pipes, ctx->pipex->cmd_count - 1);
	wait_all_processes(ctx->pids, count);
	perror("pipex: fork");
	return (EXIT_FAILURE);
}

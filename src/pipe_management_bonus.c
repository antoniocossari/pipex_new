/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_management_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acossari <acossari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 22:12:03 by acossari          #+#    #+#             */
/*   Updated: 2025/10/01 17:10:02 by acossari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex_bonus.h"

/**
 * Free allocated pipes array
 *
 * @param pipes Array of pipes to free
 * @param count Number of pipes in the array
 */
void	free_pipes(int **pipes, int count)
{
	int	i;

	if (!pipes)
		return ;
	i = 0;
	while (i < count)
	{
		if (pipes[i])
			free(pipes[i]);
		i++;
	}
	free(pipes);
}

/**
 * Close and free all pipes
 *
 * @param pipes Array of pipes to close and free
 * @param count Number of pipes in the array
 */
void	close_and_free_pipes(int **pipes, int count)
{
	if (!pipes)
		return ;
	close_all_pipes(pipes, count);
	free_pipes(pipes, count);
}

/**
 * Allocate array of pipes
 *
 * @param count Number of pipes to allocate
 * @return Pointer to allocated pipes array, or NULL on failure
 */
int	**allocate_pipes(int count)
{
	int	**pipes;
	int	i;

	pipes = malloc(sizeof(int *) * count);
	if (!pipes)
		return (NULL);
	i = 0;
	while (i < count)
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (!pipes[i])
			return (free_pipes(pipes, i), NULL);
		i++;
	}
	return (pipes);
}

/**
 * Create all pipes needed for the pipeline
 *
 * @param pipes Array of pipes to create
 * @param count Number of pipes to create
 * @return 0 on success, -1 on failure
 */
int	create_all_pipes(int **pipes, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		if (pipe(pipes[i]) == -1)
		{
			close_all_pipes(pipes, i);
			return (-1);
		}
		i++;
	}
	return (0);
}

/**
 * Setup all resources for pipeline execution
 *
 * @param data Pipex data structure
 * @param pipes Pointer to store allocated pipes array
 * @param pids Pointer to store allocated pids array
 * @return 0 on success, -1 on failure
 */
int	setup_pipeline_resources(t_pipex *data, int ***pipes, pid_t **pids)
{
	int	pipe_count;

	pipe_count = data->cmd_count - 1;
	*pipes = allocate_pipes(pipe_count);
	if (!*pipes)
		return (-1);
	*pids = malloc(sizeof(pid_t) * data->cmd_count);
	if (!*pids)
		return (free_pipes(*pipes, pipe_count), -1);
	if (create_all_pipes(*pipes, pipe_count) == -1)
		return (free_pipes(*pipes, pipe_count), free(*pids), -1);
	return (0);
}

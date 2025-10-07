/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acossari <acossari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 13:28:51 by acossari          #+#    #+#             */
/*   Updated: 2025/09/30 15:07:41 by acossari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <errno.h>
#include "../include/pipex.h"

/**
 * Initialize pipex data structure
 * 
 * @param data Pointer to pipex data structure
 */
void	init_pipex(t_pipex *data)
{
	data->infile_name = NULL;
	data->outfile_name = NULL;
	data->cmd_count = 0;
	data->commands = NULL;
	data->env_paths = NULL;
	data->here_doc = 0;
	data->limiter = NULL;
}

/**
 * Free all allocated resources in pipex data structure
 * 
 * @param data Pointer to pipex data structure
 */
void	cleanup_pipex(t_pipex *data)
{
	int	i;

	if (data->commands)
	{
		i = 0;
		while (i < data->cmd_count)
		{
			if (data->commands[i].args)
				free_string_array(data->commands[i].args);
			i++;
		}
		free(data->commands);
	}
	if (data->env_paths)
		free_string_array(data->env_paths);
	if (data->limiter)
		free(data->limiter);
	init_pipex(data);
}

/**
 * Cleanup data and exit with error message
 *
 * @param data Pipex data to cleanup
 * @param msg Error message to print  
 * @return EXIT_FAILURE
 */
int	exit_with_error(t_pipex *data, char *msg)
{
	cleanup_pipex(data);
	ft_putstr_fd(msg, STDERR_FILENO);
	return (EXIT_FAILURE);
}

/**
 * Print file error, close fd and exit
 *
 * @param filename File name for error
 * @param fd File descriptor to close
 */
void	file_error_exit(const char *filename, int fd)
{
	ft_putstr_fd("pipex: ", STDERR_FILENO);
	ft_putstr_fd((char *)filename, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	perror(NULL);
	if (fd != -1)
		close(fd);
	exit(EXIT_FAILURE);
}

/**
 * Free a NULL-terminated array of strings
 * 
 * @param arr Array of strings to free
 */
void	free_string_array(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acossari <acossari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 13:28:38 by acossari          #+#    #+#             */
/*   Updated: 2025/09/29 21:08:05 by acossari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

/**
 * Initialize commands array in pipex data structure
 * For mandatory: 2 commands
 *
 * @param data Pipex data structure
 * @return 1 on success, 0 on failure
 */
static int	init_commands(t_pipex *data)
{
	data->cmd_count = 2;
	data->commands = malloc(sizeof(t_cmd) * data->cmd_count);
	if (!data->commands)
		return (0);
	data->commands[0].args = NULL;
	data->commands[1].args = NULL;
	return (1);
}

/**
 * Parse commands from arguments
 * Assumes commands are at argv[2] and argv[3]
 *
 * @param data Pipex data structure
 * @param argv Argument vector
 * @return 1 on success, 0 on failure
*/
static int	parse_commands(t_pipex *data, char **argv)
{
	data->commands[0].args = ft_split(argv[2], ' ');
	if (!data->commands[0].args)
	{
		free(data->commands);
		return (0);
	}
	data->commands[1].args = ft_split(argv[3], ' ');
	if (!data->commands[1].args)
	{
		free_string_array(data->commands[0].args);
		free(data->commands);
		return (0);
	}
	return (1);
}

/**
 * Parse command line arguments
 * For mandatory: ./pipex file1 cmd1 cmd2 file2 (argc = 5)
 *
 * @param data Pipex data structure
 * @param argc Argument count
 * @param argv Argument vector
 * @return 1 on success, 0 on failure
 */
int	parse_arguments(t_pipex *data, int argc, char **argv)
{
	if (argc != 5)
	{
		ft_putstr_fd("usage: ./pipex file1 cmd1 cmd2 file2\n", STDERR_FILENO);
		return (0);
	}
	if (!init_commands(data))
		return (0);
	if (!parse_commands(data, argv))
		return (0);
	data->infile_name = argv[1];
	data->outfile_name = argv[4];
	return (1);
}

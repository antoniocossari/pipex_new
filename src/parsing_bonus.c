/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acossari <acossari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 20:43:46 by acossari          #+#    #+#             */
/*   Updated: 2025/10/01 20:01:44 by acossari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex_bonus.h"

/**
 * Parse single command string into command structure
 *
 * @param cmd Command structure to fill
 * @param cmd_str Command string to parse
 * @return 1 on success, 0 on failure
 */
static int	parse_single_command(t_cmd *cmd, char *cmd_str)
{
	cmd->args = ft_split(cmd_str, ' ');
	if (!cmd->args)
		return (0);
	return (1);
}

/**
 * Initialize commands array for bonus
 *
 * @param data Pipex data structure
 * @param cmd_count Number of commands
 * @return 1 on success, 0 on failure
 */
static int	init_commands_bonus(t_pipex *data, int cmd_count)
{
	int	i;

	data->cmd_count = cmd_count;
	data->commands = malloc(sizeof(t_cmd) * data->cmd_count);
	if (!data->commands)
		return (0);
	i = 0;
	while (i < data->cmd_count)
	{
		data->commands[i].args = NULL;
		i++;
	}
	return (1);
}

/**
 * Parse here_doc mode arguments
 * Syntax: ./pipex here_doc LIMITER cmd1 cmd2 ... file
 *
 * @param data Pipex data structure
 * @param argc Argument count
 * @param argv Argument vector
 * @return 1 on success, 0 on failure
 */
int	parse_here_doc_mode(t_pipex *data, int argc, char **argv)
{
	int	i;

	if (argc < 6)
		return (0);
	data->here_doc = 1;
	data->limiter = ft_strdup(argv[2]);
	if (!data->limiter)
		return (0);
	if (!init_commands_bonus(data, argc - 4))
		return (free(data->limiter), data->limiter = NULL, 0);
	i = 0;
	while (i < data->cmd_count)
	{
		if (!parse_single_command(&data->commands[i], argv[i + 3]))
			return (cleanup_pipex(data), 0);
		i++;
	}
	data->outfile_name = argv[argc - 1];
	return (1);
}

/**
 * Parse normal mode arguments (multiple pipes)
 * Syntax: ./pipex file1 cmd1 cmd2 ... cmdn file2
 *
 * @param data Pipex data structure
 * @param argc Argument count
 * @param argv Argument vector
 * @return 1 on success, 0 on failure
 */
int	parse_normal_mode(t_pipex *data, int argc, char **argv)
{
	int	i;

	if (argc < 5)
		return (0);
	data->here_doc = 0;
	if (!init_commands_bonus(data, argc - 3))
		return (0);
	i = 0;
	while (i < data->cmd_count)
	{
		if (!parse_single_command(&data->commands[i], argv[i + 2]))
			return (cleanup_pipex(data), 0);
		i++;
	}
	data->infile_name = argv[1];
	data->outfile_name = argv[argc - 1];
	return (1);
}

/**
 * Parse command line arguments for bonus features
 * Detects here_doc mode vs normal mode
 *
 * @param data Pipex data structure
 * @param argc Argument count
 * @param argv Argument vector
 * @return 1 on success, 0 on failure
 */
int	parse_arguments_bonus(t_pipex *data, int argc, char **argv)
{
	if (argv[1] && ft_strncmp(argv[1], "here_doc", 8) == 0
		&& argv[1][8] == '\0')
		return (parse_here_doc_mode(data, argc, argv));
	return (parse_normal_mode(data, argc, argv));
}

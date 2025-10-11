/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antoniocossari <antoniocossari@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 13:28:30 by acossari          #+#    #+#             */
/*   Updated: 2025/10/11 19:12:34 by antoniocoss      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

/**
 * Main function
 * Usage: ./pipex file1 cmd1 cmd2 ... cmdn file2
 *
 * @param argc Argument count
 * @param argv Argument vector
 * @param envp Environment variables
 * @return Exit status
 */
int	main(int argc, char **argv, char **envp)
{
	t_pipex	data;
	int		exit_code;

	init_pipex(&data);
	if (!parse_arguments(&data, argc, argv))
		return (exit_with_error(&data, "pipex: Invalid arguments\n"));
	data.env_paths = get_env_paths(envp);
	exit_code = execute_pipeline(&data, envp);
	cleanup_pipex(&data);
	return (exit_code);
}

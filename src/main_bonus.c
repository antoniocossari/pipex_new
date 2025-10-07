/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acossari <acossari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 20:03:11 by acossari          #+#    #+#             */
/*   Updated: 2025/10/01 21:06:24 by acossari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex_bonus.h"

/**
 * Main function for bonus features
 * Usage:
 *   Normal: ./pipex file1 cmd1 cmd2 ... cmdn file2
 *   Here_doc: ./pipex here_doc LIMITER cmd1 cmd2 ... file
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
	if (!parse_arguments_bonus(&data, argc, argv))
		return (exit_with_error(&data, "pipex: Invalid arguments\n"));
	data.env_paths = get_env_paths(envp);
	if (!data.env_paths)
		return (exit_with_error(&data, "pipex: PATH resolution failed\n"));
	if (data.here_doc)
		exit_code = execute_here_doc_pipeline(&data, envp);
	else
		exit_code = execute_pipeline_bonus(&data, envp);
	cleanup_pipex(&data);
	return (exit_code);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acossari <acossari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 23:02:11 by acossari          #+#    #+#             */
/*   Updated: 2025/10/01 21:05:35 by acossari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex_bonus.h"

/**
 * Handle dup2 error and exit
 */
void	dup2_error_exit(void)
{
	perror("pipex: dup2");
	exit(EXIT_FAILURE);
}

/**
 * Execute the pipeline in normal mode
 *
 * @param data Pipex data structure
 * @param envp Environment variables
 * @return Exit status
 */
int	execute_pipeline_bonus(t_pipex *data, char **envp)
{
	t_exec_ctx	ctx;

	ctx.pipex = data;
	ctx.envp = envp;
	ctx.here_doc_fd = -1;
	if (setup_pipeline_resources(data, &ctx.pipes, &ctx.pids) == -1)
		return (EXIT_FAILURE);
	if (fork_and_execute_child_processes(&ctx) == -1)
		return (EXIT_FAILURE);
	if (data->cmd_count > 1)
		close_pipe(ctx.pipes[data->cmd_count - 2]);
	free_pipes(ctx.pipes, data->cmd_count - 1);
	return (wait_and_cleanup(ctx.pids, data->cmd_count));
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acossari <acossari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 20:31:07 by acossari          #+#    #+#             */
/*   Updated: 2025/10/01 17:11:40 by acossari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex_bonus.h"

/**
 * Handle file error and exit
 *
 * @param filename Name of the problematic file
 */
static void	file_error_exit(const char *filename)
{
	ft_putstr_fd("pipex: ", STDERR_FILENO);
	ft_putstr_fd((char *)filename, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	perror(NULL);
	exit(EXIT_FAILURE);
}

/**
 * Setup input for first command (from infile or here_doc)
 *
 * @param data Pipex data structure
 * @param here_doc_fd Here_doc file descriptor (-1 for normal mode)
 */
static void	setup_first_command_input(t_pipex *data, int here_doc_fd)
{
	int	infile_fd;

	if (here_doc_fd != -1)
	{
		if (dup2(here_doc_fd, STDIN_FILENO) == -1)
			dup2_error_exit();
		return ;
	}
	infile_fd = open(data->infile_name, O_RDONLY);
	if (infile_fd == -1)
		file_error_exit(data->infile_name);
	if (dup2(infile_fd, STDIN_FILENO) == -1)
		(close(infile_fd), dup2_error_exit());
	close(infile_fd);
}

/**
 * Setup output for last command (to outfile)
 *
 * @param data Pipex data structure
 */
static void	setup_last_command_output(t_pipex *data)
{
	int	outfile_fd;
	int	flags;

	flags = O_WRONLY | O_CREAT;
	if (data->here_doc)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	outfile_fd = open(data->outfile_name, flags, FILE_PERMS);
	if (outfile_fd == -1)
		file_error_exit(data->outfile_name);
	if (dup2(outfile_fd, STDOUT_FILENO) == -1)
		(close(outfile_fd), dup2_error_exit());
	close(outfile_fd);
}

/**
 * Handle child process for both normal and here_doc modes
 *
 * @param ctx Execution context (here_doc_fd == -1 for normal mode)
 * @param i Index of the current command
 */
void	handle_child_process(t_exec_ctx *ctx, int i)
{
	if (i == 0)
		setup_first_command_input(ctx->pipex, ctx->here_doc_fd);
	else if (dup2(ctx->pipes[i - 1][0], STDIN_FILENO) == -1)
		dup2_error_exit();
	if (i == ctx->pipex->cmd_count - 1)
		setup_last_command_output(ctx->pipex);
	else if (dup2(ctx->pipes[i][1], STDOUT_FILENO) == -1)
		dup2_error_exit();
	if (ctx->here_doc_fd != -1)
		close(ctx->here_doc_fd);
	close_all_pipes(ctx->pipes, ctx->pipex->cmd_count - 1);
	execute_command(ctx->pipex, i, ctx->envp);
}

/**
 * Fork and execute all child processes for both normal and here_doc pipelines
 *
 * @param ctx Execution context with pids array
 * @return 0 on success, -1 on failure
 */
int	fork_and_execute_child_processes(t_exec_ctx *ctx)
{
	int	i;

	i = 0;
	while (i < ctx->pipex->cmd_count)
	{
		ctx->pids[i] = fork();
		if (ctx->pids[i] == -1)
		{
			if (ctx->here_doc_fd != -1)
				close(ctx->here_doc_fd);
			cleanup_on_fork_error(ctx, i);
			return (-1);
		}
		if (ctx->pids[i] == 0)
			handle_child_process(ctx, i);
		if (i > 0)
			close_pipe(ctx->pipes[i - 1]);
		i++;
	}
	return (0);
}

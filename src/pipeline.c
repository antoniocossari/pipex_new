/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acossari <acossari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 13:24:00 by acossari          #+#    #+#             */
/*   Updated: 2025/10/01 17:47:40 by acossari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

/**
 * Close both ends of the pipe
 * 
 * @param pipefd Array containing read and write file descriptors
 */
static void	cleanup_pipes(int pipefd[2])
{
	close(pipefd[0]);
	close(pipefd[1]);
}

/**
 * Print error message and return failure code
 * 
 * @param msg Error message to print
 * @return EXIT_FAILURE
 */
static int	fail_perror(const char *msg)
{
	perror(msg);
	return (EXIT_FAILURE);
}

/**
 * First child process: reads from infile, writes to pipe
 * 
 * @param data Pipex data structure
 * @param pipefd Array containing read and write file descriptors
 * @param envp Environment variables
 */
static void	child_first(t_pipex *data, int pipefd[2], char **envp)
{
	int	infile_fd;

	close(pipefd[0]);
	infile_fd = open(data->infile_name, O_RDONLY);
	if (infile_fd == -1)
		file_error_exit(data->infile_name, pipefd[1]);
	if (dup2(infile_fd, STDIN_FILENO) == -1)
		(perror("pipex: dup2"), close(infile_fd), close(pipefd[1]),
			exit(EXIT_FAILURE));
	close(infile_fd);
	if (dup2(pipefd[1], STDOUT_FILENO) == -1)
		(perror("pipex: dup2"), close(pipefd[1]), exit(EXIT_FAILURE));
	close(pipefd[1]);
	execute_command(data, 0, envp);
}

/**
 * Second child process: reads from pipe, writes to outfile
 * 
 * @param data Pipex data structure
 * @param pipefd Array containing read and write file descriptors
 * @param envp Environment variables
 */
static void	child_second(t_pipex *data, int pipefd[2], char **envp)
{
	int	outfile_fd;

	close(pipefd[1]);
	outfile_fd = open(data->outfile_name, O_WRONLY | O_CREAT | O_TRUNC,
			FILE_PERMS);
	if (outfile_fd == -1)
		file_error_exit(data->outfile_name, pipefd[0]);
	if (dup2(pipefd[0], STDIN_FILENO) == -1)
		(perror("pipex: dup2"), close(pipefd[0]), close(outfile_fd),
			exit(EXIT_FAILURE));
	close(pipefd[0]);
	if (dup2(outfile_fd, STDOUT_FILENO) == -1)
		(perror("pipex: dup2"), close(outfile_fd), exit(EXIT_FAILURE));
	close(outfile_fd);
	execute_command(data, 1, envp);
}

/**
 * Execute the full pipeline: cmd1 < infile | cmd2 > outfile
 * 
 * @param data Pipex data structure
 * @param envp Environment variables
 * @return Exit status of the last command, or EXIT_FAILURE on error
 */
int	execute_pipeline(t_pipex *data, char **envp)
{
	int		pipefd[2];
	pid_t	pid1;
	pid_t	pid2;
	int		status;

	if (pipe(pipefd) == -1)
		return (fail_perror("pipex: Pipe"));
	pid1 = fork();
	if (pid1 == -1)
		return (cleanup_pipes(pipefd), fail_perror("pipex: Fork"));
	if (pid1 == 0)
		child_first(data, pipefd, envp);
	pid2 = fork();
	if (pid2 == -1)
		return (cleanup_pipes(pipefd), waitpid(pid1, NULL, 0),
			fail_perror("pipex: Fork"));
	if (pid2 == 0)
		child_second(data, pipefd, envp);
	cleanup_pipes(pipefd);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (EXIT_FAILURE);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acossari <acossari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 00:07:01 by acossari          #+#    #+#             */
/*   Updated: 2025/10/01 17:46:02 by acossari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <errno.h>
#include "../include/pipex.h"

/**
 * Map execve errno to shell-compatible exit codes
 */
int	map_execve_errno(void)
{
	if (errno == EACCES || errno == ENOEXEC || errno == EISDIR)
		return (CMD_NOT_EXECUTABLE);
	if (errno == ENOENT)
		return (CMD_NOT_FOUND);
	return (EXIT_FAILURE);
}

/** 
 * Print "pipex: <cmd>: <msg>" and exit(code)
 * If msg is NULL, use perror() instead
 * 
 * @param cmd Command name
 * @param msg Error message (NULL to use perror)
 * @param code Exit code
 */
static void	die_cmd_error(const char *cmd, const char *msg, int code)
{
	ft_putstr_fd("pipex: ", STDERR_FILENO);
	ft_putstr_fd((char *)cmd, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	if (msg)
	{
		ft_putstr_fd((char *)msg, STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
	}
	else
		perror(NULL);
	exit(code);
}

/**
 * Resolve command when it contains a '/'
 * Check for existence and executability
 * 
 * @param cmd Command string with '/'
 * @return cmd if valid, otherwise exits with error
 */
static char	*resolve_with_slash(const char *cmd)
{
	if (access(cmd, F_OK) == -1)
		die_cmd_error(cmd, NULL, CMD_NOT_FOUND);
	if (access(cmd, X_OK) == -1)
		die_cmd_error(cmd, NULL, CMD_NOT_EXECUTABLE);
	return ((char *)cmd);
}

/**
 * Resolve command using PATH environment variable
 * 
 * @param data Pipex data structure with env_paths
 * @param cmd Command string without '/'
 * @return Resolved command path, otherwise exits with error
 */
static char	*resolve_in_path(t_pipex *data, const char *cmd)
{
	char	*path;

	path = find_command_path(cmd, data->env_paths);
	if (!path)
		die_cmd_error(cmd, "Command not found", CMD_NOT_FOUND);
	return (path);
}

/**
 * Execute a command in a child process
 * Handles command resolution and execve call
 * 
 * @param data Pipex data structure
 * @param cmd_idx Index of the command to execute
 * @param envp Environment variables
 */
void	execute_command(t_pipex *data, int cmd_idx, char **envp)
{
	char	**args;
	char	*cmd;
	char	*path;

	args = data->commands[cmd_idx].args;
	if (!args[0] || !*args[0])
	{
		ft_putstr_fd("pipex: command not found\n", STDERR_FILENO);
		exit(CMD_NOT_FOUND);
	}
	cmd = args[0];
	if (ft_strchr(cmd, '/'))
		path = resolve_with_slash(cmd);
	else
		path = resolve_in_path(data, cmd);
	execve(path, args, envp);
	die_cmd_error(cmd, NULL, map_execve_errno());
}

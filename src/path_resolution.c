/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_resolution.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antoniocossari <antoniocossari@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 13:21:00 by acossari          #+#    #+#             */
/*   Updated: 2025/10/11 12:50:37 by antoniocoss      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

/**
 * Extract PATH directories from environment variables
 * 
 * @param envp Environment variables array
 * @return Malloc'd array of PATH directories, or NULL if PATH not found
 */
char	**get_env_paths(char **envp)
{
	char	*path_env;
	char	**paths;
	int		i;

	if (!envp)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			path_env = envp[i] + 5;
			paths = ft_split(path_env, ':');
			return (paths);
		}
		i++;
	}
	return (NULL);
}

/**
 * Build full path by concatenating directory and command
 * 
 * @param dir Directory path
 * @param cmd Command name
 * @return Malloc'd full path string, or NULL on allocation failure
 */
static char	*build_full_path(const char *dir, const char *cmd)
{
	char	*full_path;
	size_t	path_len;
	size_t	cmd_len;
	size_t	buf_size;

	path_len = ft_strlen(dir);
	cmd_len = ft_strlen(cmd);
	buf_size = path_len + 1 + cmd_len + 1;
	full_path = malloc(buf_size);
	if (!full_path)
		return (NULL);
	ft_strlcpy(full_path, dir, buf_size);
	ft_strlcat(full_path, "/", buf_size);
	ft_strlcat(full_path, cmd, buf_size);
	return (full_path);
}

/**
 * Find command in given PATH directories
 * 
 * @param cmd Command name
 * @param env_paths Array of PATH directories
 * @return Malloc'd full path if found and executable, otherwise NULL
 */
char	*find_command_path(const char *cmd, char **env_paths)
{
	char	*full_path;
	size_t	i;

	if (!env_paths)
		return (NULL);
	i = 0;
	while (env_paths[i])
	{
		full_path = build_full_path(env_paths[i], cmd);
		if (!full_path)
			return (NULL);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

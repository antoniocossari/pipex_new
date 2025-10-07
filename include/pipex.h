/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acossari <acossari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 13:20:01 by acossari          #+#    #+#             */
/*   Updated: 2025/10/01 19:56:56 by acossari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>
# include <stdio.h>
# include <sys/wait.h>
# include "../libft/include/libft.h"

/* Exit codes */
# define CMD_NOT_EXECUTABLE 126
# define CMD_NOT_FOUND 127

/* File permissions for outfile */
# define FILE_PERMS 0644

/* Structure for a single command */
typedef struct s_cmd
{
	char	**args;		/* Arguments array for execve */
}	t_cmd;

/* Main data structure - extensible for bonus */
typedef struct s_pipex
{
	char	*infile_name;
	char	*outfile_name;
	int		cmd_count;
	t_cmd	*commands;
	char	**env_paths;
	/* Bonus flags */
	int		here_doc;		/* 0 for mandatory, 1 for here_doc bonus */
	char	*limiter;		/* Here_doc limiter string */
}	t_pipex;

/* Function prototypes */

/* Core functions */
void	init_pipex(t_pipex *data);
void	cleanup_pipex(t_pipex *data);
void	free_string_array(char **arr);
void	file_error_exit(const char *filename, int fd);
int		exit_with_error(t_pipex *data, char *msg);

/* Parsing */
int		parse_arguments(t_pipex *data, int argc, char **argv);

/* PATH resolution */
char	**get_env_paths(char **envp);
char	*find_command_path(const char *cmd, char **env_paths);

/* Pipeline execution */
int		execute_pipeline(t_pipex *data, char **envp);

/* Command execution */
void	execute_command(t_pipex *data, int cmd_idx, char **envp);

#endif
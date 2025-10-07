/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acossari <acossari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 20:30:00 by acossari          #+#    #+#             */
/*   Updated: 2025/10/01 19:57:26 by acossari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

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

/* Main data structure for bonus */
typedef struct s_pipex
{
	char	*infile_name;
	char	*outfile_name;
	int		cmd_count;
	t_cmd	*commands;
	char	**env_paths;
	/* Bonus features */
	int		here_doc;		/* 0 for normal, 1 for here_doc mode */
	char	*limiter;		/* Here_doc limiter string */
}	t_pipex;

/* Execution context structure */
typedef struct s_exec_ctx
{
	t_pipex	*pipex;
	int		**pipes;
	pid_t	*pids;
	int		here_doc_fd;
	char	**envp;
}	t_exec_ctx;

/* Function prototypes */

/* Core functions */
void	init_pipex(t_pipex *data);
void	cleanup_pipex(t_pipex *data);
void	free_string_array(char **arr);
int		exit_with_error(t_pipex *data, char *msg);

/* Parsing */
int		parse_arguments_bonus(t_pipex *data, int argc, char **argv);

/* PATH resolution */
char	**get_env_paths(char **envp);
char	*find_command_path(const char *cmd, char **env_paths);

/* Pipeline execution */
int		execute_pipeline_bonus(t_pipex *data, char **envp);
int		execute_here_doc_pipeline(t_pipex *data, char **envp);

/* Here_doc */
int		handle_here_doc(t_pipex *data);

/* Pipe management */
int		**allocate_pipes(int count);
void	free_pipes(int **pipes, int count);
int		create_all_pipes(int **pipes, int count);
void	close_all_pipes(int **pipes, int count);
void	close_and_free_pipes(int **pipes, int count);
void	dup2_error_exit(void);
void	close_pipe(int *pipe_fd);
int		wait_and_cleanup(pid_t *pids, int count);
int		cleanup_on_fork_error(t_exec_ctx *ctx, int count);
int		fork_and_execute_child_processes(t_exec_ctx *ctx);

/* Pipeline setup */
int		setup_pipeline_resources(t_pipex *data, int ***pipes, pid_t **pids);

/* Command execution */
void	execute_command(t_pipex *data, int cmd_idx, char **envp);

/* Child process handling */
void	handle_child_process(t_exec_ctx *ctx, int i);

#endif
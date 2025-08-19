/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmaruyam <hmaruyam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 12:37:21 by hmaruyam          #+#    #+#             */
/*   Updated: 2025/08/19 11:01:00 by hmaruyam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <errno.h>
# include <fcntl.h>
# include <libft.h>
# include <stdio.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

typedef struct s_pipex
{
	int		infile_fd;
	int		outfile_fd;
	int		pipefd[2];
	pid_t	child1_pid;
	pid_t	child2_pid;
	char	**cmd1_arg;
	char	**cmd2_arg;
	char	*cmd1_path;
	char	*cmd2_path;
}			t_pipex;

char		*find_path(char *command, char *envp[]);
char		*free_split(char **str);
char		**pipex_split(char const *s, char c);
void		init_files(t_pipex *data, char *argv[]);
void		init_commands(t_pipex *data, char *argv[], char *envp[]);
void		cleanup(t_pipex *data, int mode);
void		file_close(t_pipex *data);
void		child_process_1(t_pipex *data, char *envp[]);
void		child_process_2(t_pipex *data, char *envp[]);
void		create_processes(t_pipex *data, char *envp[]);
void		wait_children_and_exit(t_pipex *data);
void		put_file_error(char *filename);
void		put_command_error(char *command);
void		put_system_error_exit(t_pipex *data, int code);

#endif

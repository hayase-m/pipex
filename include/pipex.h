/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmaruyam <hmaruyam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 12:37:21 by hmaruyam          #+#    #+#             */
/*   Updated: 2025/08/18 22:51:41 by hmaruyam         ###   ########.fr       */
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

char		*find_path(char *command, char *envp[]);
char		*free_split(char **str);
char		**pipex_split(char const *s, char c);

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

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmaruyam <hmaruyam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 10:51:00 by hmaruyam          #+#    #+#             */
/*   Updated: 2025/08/19 13:19:30 by hmaruyam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pipex.h>

void	put_file_error(char *filename)
{
	ft_putstr_fd("pipex: ", 2);
	ft_putstr_fd(filename, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd("\n", 2);
}

void	put_command_error(char *command)
{
	ft_putstr_fd("pipex: ", 2);
	ft_putstr_fd(command, 2);
	if (ft_strchr(command, '/'))
		ft_putstr_fd(": No such file or directory\n", 2);
	else
		ft_putstr_fd(": command not found\n", 2);
}

void	put_system_error_exit(t_pipex *data, int code)
{
	perror("pipex");
	cleanup(data, code);
}

void	cleanup(t_pipex *data, int mode)
{
	if (data->infile_fd >= 0)
		close(data->infile_fd);
	if (data->outfile_fd >= 0)
		close(data->outfile_fd);
	if (data->cmd1_arg)
		free_split(data->cmd1_arg);
	if (data->cmd2_arg)
		free_split(data->cmd2_arg);
	if (data->cmd1_path)
		free(data->cmd1_path);
	if (data->cmd2_path)
		free(data->cmd2_path);
	if (data->pipefd[0] >= 0)
		close(data->pipefd[0]);
	if (data->pipefd[1] >= 0)
		close(data->pipefd[1]);
	exit(mode);
}

void	file_close(t_pipex *data)
{
	close(data->infile_fd);
	close(data->outfile_fd);
	close(data->pipefd[0]);
	close(data->pipefd[1]);
}

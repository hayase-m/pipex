/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmaruyam <hmaruyam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 12:37:19 by hmaruyam          #+#    #+#             */
/*   Updated: 2025/08/19 01:56:30 by hmaruyam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pipex.h>

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

int	main(int argc, char *argv[], char *envp[])
{
	t_pipex	data;

	if (argc != 5)
		return (1);
	init_files_and_commands(&data, argv);
	init_command_args_and_paths(&data, argv);
	find_command_paths(&data, envp);
	create_processes(&data, envp);
	wait_children_and_exit(&data);
	return (0);
}

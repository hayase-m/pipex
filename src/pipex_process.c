/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmaruyam <hmaruyam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 01:56:00 by hmaruyam          #+#    #+#             */
/*   Updated: 2025/08/19 01:56:00 by hmaruyam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pipex.h>

void	child_process_1(t_pipex *data, char *envp[])
{
	if (data->infile_fd == -1)
		cleanup(data, 1);
	if (!data->cmd1_path)
		cleanup(data, 127);
	if (dup2(data->infile_fd, 0) == -1)
	{
		perror("pipex");
		cleanup(data, 1);
	}
	if (dup2(data->pipefd[1], 1) == -1)
	{
		perror("pipex");
		cleanup(data, 1);
	}
	file_close(data);
	if (execve(data->cmd1_path, data->cmd1_arg, envp) == -1)
	{
		perror("pipex");
		cleanup(data, 126);
	}
}

void	child_process_2(t_pipex *data, char *envp[])
{
	if (!data->cmd2_path)
		cleanup(data, 127);
	if (dup2(data->pipefd[0], 0) == -1)
	{
		perror("pipex");
		cleanup(data, 1);
	}
	if (dup2(data->outfile_fd, 1) == -1)
	{
		perror("pipex");
		cleanup(data, 1);
	}
	file_close(data);
	if (execve(data->cmd2_path, data->cmd2_arg, envp) == -1)
	{
		perror("pipex");
		cleanup(data, 126);
	}
}

void	create_processes(t_pipex *data, char *envp[])
{
	if (pipe(data->pipefd) == -1)
	{
		perror("pipex");
		cleanup(data, 1);
	}
	data->child1_pid = fork();
	if (data->child1_pid == -1)
	{
		perror("pipex");
		cleanup(data, 1);
	}
	if (data->child1_pid == 0)
		child_process_1(data, envp);
	close(data->pipefd[1]);
	data->child2_pid = fork();
	if (data->child2_pid == -1)
	{
		perror("pipex");
		cleanup(data, 1);
	}
	if (data->child2_pid == 0)
		child_process_2(data, envp);
}

void	wait_children_and_exit(t_pipex *data)
{
	int	status;
	int	exit_code;

	close(data->pipefd[0]);
	close(data->infile_fd);
	close(data->outfile_fd);
	if (waitpid(data->child1_pid, NULL, 0) == -1)
	{
		perror("pipex");
		cleanup(data, 1);
	}
	if (waitpid(data->child2_pid, &status, 0) == -1)
	{
		perror("pipex");
		cleanup(data, 1);
	}
	exit_code = 0;
	if (WIFEXITED(status))
		exit_code = WEXITSTATUS(status);
	cleanup(data, exit_code);
}

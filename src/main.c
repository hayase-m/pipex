/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmaruyam <hmaruyam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 12:37:19 by hmaruyam          #+#    #+#             */
/*   Updated: 2025/08/18 22:52:13 by hmaruyam         ###   ########.fr       */
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
	int		status;
	int		exit_code;

	ft_bzero(&data, sizeof(t_pipex));
	if (argc != 5)
		return (1);
	data.infile_fd = open(argv[1], O_RDONLY);
	if (data.infile_fd == -1)
	{
		ft_putstr_fd("pipex: ", 2);
		ft_putstr_fd(argv[1], 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
	}
	data.outfile_fd = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (data.outfile_fd == -1)
	{
		ft_putstr_fd("pipex: ", 2);
		ft_putstr_fd(argv[4], 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		cleanup(&data, 1);
	}
	data.cmd1_arg = pipex_split(argv[2], ' ');
	if (!data.cmd1_arg)
	{
		perror("pipex");
		cleanup(&data, 1);
	}
	data.cmd2_arg = pipex_split(argv[3], ' ');
	if (!data.cmd2_arg)
	{
		perror("pipex");
		cleanup(&data, 1);
	}
	data.cmd1_path = find_path(data.cmd1_arg[0], envp);
	if (!data.cmd1_path)
	{
		ft_putstr_fd("pipex: ", 2);
		ft_putstr_fd(data.cmd1_arg[0], 2);
		ft_putstr_fd(": command not found\n", 2);
	}
	data.cmd2_path = find_path(data.cmd2_arg[0], envp);
	if (!data.cmd2_path)
	{
		ft_putstr_fd("pipex: ", 2);
		ft_putstr_fd(data.cmd2_arg[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		cleanup(&data, 127);
	}
	if (pipe(data.pipefd) == -1)
	{
		perror("pipex");
		cleanup(&data, 1);
	}
	data.child1_pid = fork();
	if (data.child1_pid == -1)
	{
		perror("pipex");
		cleanup(&data, 1);
	}
	if (data.child1_pid == 0)
	{
		if (data.infile_fd == -1)
			cleanup(&data, 1);
		if (!data.cmd1_path)
			cleanup(&data, 127);
		dup2(data.infile_fd, 0);
		dup2(data.pipefd[1], 1);
		file_close(&data);
		if (execve(data.cmd1_path, data.cmd1_arg, envp) == -1)
		{
			perror("pipex");
			cleanup(&data, 126);
		}
	}
	else
	{
		close(data.pipefd[1]);
		data.child2_pid = fork();
		if (data.child2_pid == -1)
		{
			perror("pipex");
			cleanup(&data, 1);
		}
		if (data.child2_pid == 0)
		{
			if (!data.cmd2_path)
				cleanup(&data, 127);
			dup2(data.pipefd[0], 0);
			dup2(data.outfile_fd, 1);
			file_close(&data);
			if (execve(data.cmd2_path, data.cmd2_arg, envp) == -1)
			{
				perror("pipex");
				cleanup(&data, 126);
			}
		}
		else
		{
			close(data.pipefd[0]);
			close(data.infile_fd);
			close(data.outfile_fd);
			waitpid(data.child1_pid, NULL, 0);
			waitpid(data.child2_pid, &status, 0);
			exit_code = 0;
			if (WIFEXITED(status))
				exit_code = WEXITSTATUS(status);
			cleanup(&data, exit_code);
		}
	}
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmaruyam <hmaruyam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 01:53:00 by hmaruyam          #+#    #+#             */
/*   Updated: 2025/08/19 10:33:00 by hmaruyam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pipex.h>

void	init_files_and_commands(t_pipex *data, char *argv[])
{
	data->infile_fd = open(argv[1], O_RDONLY);
	if (data->infile_fd == -1)
	{
		ft_putstr_fd("pipex: ", 2);
		ft_putstr_fd(argv[1], 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
	}
	data->outfile_fd = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (data->outfile_fd == -1)
	{
		ft_putstr_fd("pipex: ", 2);
		ft_putstr_fd(argv[4], 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		cleanup(data, 1);
	}
}

void	init_command_args_and_paths(t_pipex *data, char *argv[])
{
	data->cmd1_arg = pipex_split(argv[2], ' ');
	if (!data->cmd1_arg)
	{
		perror("pipex");
		cleanup(data, 1);
	}
	data->cmd2_arg = pipex_split(argv[3], ' ');
	if (!data->cmd2_arg)
	{
		perror("pipex");
		cleanup(data, 1);
	}
}

void	find_command_paths(t_pipex *data, char *envp[])
{
	data->cmd1_path = find_path(data->cmd1_arg[0], envp);
	if (!data->cmd1_path)
	{
		ft_putstr_fd("pipex: ", 2);
		ft_putstr_fd(data->cmd1_arg[0], 2);
		ft_putstr_fd(": command not found\n", 2);
	}
	data->cmd2_path = find_path(data->cmd2_arg[0], envp);
	if (!data->cmd2_path)
	{
		ft_putstr_fd("pipex: ", 2);
		ft_putstr_fd(data->cmd2_arg[0], 2);
		ft_putstr_fd(": command not found\n", 2);
	}
}

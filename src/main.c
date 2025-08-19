/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmaruyam <hmaruyam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 12:37:19 by hmaruyam          #+#    #+#             */
/*   Updated: 2025/08/19 15:37:34 by hmaruyam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pipex.h>

void	init_files(t_pipex *data, char *argv[])
{
	data->infile_fd = open(argv[1], O_RDONLY);
	if (data->infile_fd == -1)
		put_file_error(argv[1]);
	data->outfile_fd = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (data->outfile_fd == -1)
		put_file_error(argv[4]);
}

void	init_commands(t_pipex *data, char *argv[], char *envp[])
{
	data->cmd1_arg = pipex_split(argv[2], ' ');
	if (!data->cmd1_arg)
		put_system_error_exit(data, EXIT_GENERAL_ERROR);
	data->cmd2_arg = pipex_split(argv[3], ' ');
	if (!data->cmd2_arg)
		put_system_error_exit(data, EXIT_GENERAL_ERROR);
	data->cmd1_path = find_path(data->cmd1_arg[0], envp);
	if (!data->cmd1_path)
		put_command_error(data->cmd1_arg[0]);
	data->cmd2_path = find_path(data->cmd2_arg[0], envp);
	if (!data->cmd2_path)
		put_command_error(data->cmd2_arg[0]);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_pipex	data;

	if (argc != 5)
		return (1);
	init_files(&data, argv);
	init_commands(&data, argv, envp);
	create_processes(&data, envp);
	wait_children_and_exit(&data);
	return (0);
}

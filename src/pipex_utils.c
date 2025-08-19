/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmaruyam <hmaruyam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 15:11:35 by hmaruyam          #+#    #+#             */
/*   Updated: 2025/08/19 13:17:30 by hmaruyam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pipex.h>

char	*free_split(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
	return (NULL);
}

char	**pipex_split(char const *s, char c)
{
	char	**result;

	if (!s)
		return (NULL);
	if (*s == '\0')
	{
		result = malloc(sizeof(char *) * 2);
		if (!result)
			return (NULL);
		result[0] = ft_strdup("");
		if (!result[0])
		{
			free(result);
			return (NULL);
		}
		result[1] = NULL;
		return (result);
	}
	return (ft_split(s, c));
}

static char	*search_in_env_paths(char *command, char **env_list)
{
	int		i;
	char	*tmp_path;
	char	*pathname;

	i = 0;
	while (env_list[i])
	{
		tmp_path = ft_strjoin(env_list[i], "/");
		if (!tmp_path)
			return (free_split(env_list));
		pathname = ft_strjoin(tmp_path, command);
		if (!pathname)
			return (free_split(env_list));
		free(tmp_path);
		if (access(pathname, F_OK) == 0)
		{
			free_split(env_list);
			return (pathname);
		}
		free(pathname);
		i++;
	}
	free_split(env_list);
	return (NULL);
}

char	*find_path(char *command, char *envp[])
{
	int		i;
	char	**env_list;
	char	*pathname;

	if (ft_strchr(command, '/'))
	{
		if (access(command, F_OK) == 0)
		{
			pathname = ft_strdup(command);
			if (!pathname)
				return (NULL);
			return (pathname);
		}
		return (NULL);
	}
	i = 0;
	while (envp[i] && !ft_strnstr(envp[i], "PATH=", 5))
		i++;
	if (!envp[i])
		return (NULL);
	env_list = ft_split(envp[i] + 5, ':');
	if (!env_list)
		return (NULL);
	return (search_in_env_paths(command, env_list));
}

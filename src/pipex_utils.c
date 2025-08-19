/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmaruyam <hmaruyam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 15:11:35 by hmaruyam          #+#    #+#             */
/*   Updated: 2025/08/19 21:40:24 by hmaruyam         ###   ########.fr       */
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
	int		i;

	if (!s)
		return (NULL);
	i = 0;
	while (s[i] && s[i] == c)
		i++;
	if (s[i] == '\0')
	{
		result = malloc(sizeof(char *) * 2);
		if (!result)
			return (NULL);
		result[0] = ft_strdup(s);
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

static char	*join_and_check(char *dir, char *command)
{
	char	*tmp_path;
	char	*pathname;

	tmp_path = ft_strjoin(dir, "/");
	if (!tmp_path)
		return (NULL);
	pathname = ft_strjoin(tmp_path, command);
	if (!pathname)
	{
		free(tmp_path);
		return (NULL);
	}
	free(tmp_path);
	if (access(pathname, F_OK) == 0)
		return (pathname);
	free(pathname);
	return (NULL);
}

static char	*search_in_env_paths(char *command, char **env_list)
{
	int		i;
	char	*pathname;

	i = 0;
	while (env_list[i])
	{
		pathname = join_and_check(env_list[i], command);
		if (pathname)
		{
			free_split(env_list);
			return (pathname);
		}
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

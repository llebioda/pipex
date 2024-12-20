/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebioda <llebioda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 13:04:42 by llebioda          #+#    #+#             */
/*   Updated: 2024/12/12 13:20:14 by llebioda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_tab(char **tab)
{
	int	i;

	if (tab == NULL)
		return ;
	i = 0;
	while (tab[i] != NULL)
		free(tab[i++]);
	free(tab);
}

static char	**get_envpath(char **envp)
{
	char	**paths;
	int		i;

	if (envp == NULL)
		return (NULL);
	i = 0;
	while (envp[i] != NULL && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	if (envp[i] == NULL)
		return (NULL);
	paths = ft_split(&(envp[i][5]), ':');
	if (paths == NULL)
		return (NULL);
	return (paths);
}

char	*get_exec_path(char *cmd, char **envp, int *need_free)
{
	char	**paths;
	char	*exec_path;
	int		i;

	*need_free = 0;
	if (cmd == NULL || envp == NULL)
		return (cmd);
	paths = get_envpath(envp);
	if (paths == NULL)
		return (cmd);
	i = 0;
	while (paths[i] != NULL)
	{
		exec_path = ft_strjoin(paths[i++], "/", cmd);
		if (exec_path == NULL)
			return (free_tab(paths), cmd);
		if (access(exec_path, F_OK | X_OK) == 0)
		{
			*need_free = 1;
			return (free_tab(paths), exec_path);
		}
		free(exec_path);
	}
	return (free_tab(paths), cmd);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebioda <llebioda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 12:26:45 by llebioda          #+#    #+#             */
/*   Updated: 2024/12/20 11:32:20 by llebioda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	exec(char **cmd, char **envp)
{
	char	*exec_path;
	int		exit_code;
	int		need_free;

	if (cmd == NULL || *cmd == NULL)
		exit(EXIT_FAILURE);
	cmd = ft_split(*cmd, ' ');
	if (cmd == NULL)
		exit(EXIT_FAILURE);
	exec_path = get_exec_path(cmd[0], envp, &need_free);
	exit_code = execve(exec_path, cmd, envp);
	if (exit_code == -1)
	{
		write(2, "pipex: command not found: ", 26);
		write(2, cmd[0], ft_strlen(cmd[0]));
		write(2, "\n", 1);
	}
	if (need_free == 1)
		free(exec_path);
	free_tab(cmd);
	if (exit_code == -1)
		exit(EXIT_FAILURE);
}

static void	handle_pipe(char *cmd, char **envp)
{
	pid_t	pid;
	int		p_fd[2];

	if (cmd == NULL || envp == NULL)
		exit(EXIT_FAILURE);
	if (pipe(p_fd) == -1)
		exit(EXIT_FAILURE);
	pid = fork();
	if (pid == -1)
		exit(EXIT_FAILURE);
	if (pid == 0)
	{
		close(p_fd[0]);
		if (dup2(p_fd[1], 1) == -1)
			exit(EXIT_FAILURE);
		exec(&cmd, envp);
	}
	else
	{
		close(p_fd[1]);
		if (dup2(p_fd[0], 0) == -1)
			exit(EXIT_FAILURE);
	}
}

static void	redirect_to_outfile(char *cmd, char *file, char **envp)
{
	pid_t	pid;
	int		fd_out;

	if (cmd == NULL || file == NULL || envp == NULL)
		exit(EXIT_FAILURE);
	pid = fork();
	if (pid == -1)
		exit(EXIT_FAILURE);
	if (pid == 0)
	{
		fd_out = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0777);
		if (fd_out < 0)
			exit(EXIT_FAILURE);
		if (dup2(fd_out, 1) == -1)
			exit(EXIT_FAILURE);
		close(fd_out);
		exec(&cmd, envp);
	}
	else
		waitpid(pid, NULL, 0);
}

int	main(int argc, char **argv, char **envp)
{
	int		fd_in;
	int		i;

	if (argc < 5)
	{
		write(2, "Usage:  ./pipex file1 cmd1 cmd2 cmd3 ... cmdn file2\n", 52);
		return (EXIT_FAILURE);
	}
	fd_in = open(argv[1], O_RDONLY, 0777);
	if (fd_in < 0)
		exit(EXIT_FAILURE);
	if (dup2(fd_in, 0) == -1)
		exit(EXIT_FAILURE);
	close(fd_in);
	i = 2;
	while (i < argc - 2)
		handle_pipe(argv[i++], envp);
	redirect_to_outfile(argv[i], argv[i + 1], envp);
	return (EXIT_SUCCESS);
}

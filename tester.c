/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tester.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebioda <llebioda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 13:42:50 by llebioda          #+#    #+#             */
/*   Updated: 2024/12/20 11:32:48 by llebioda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

char const	*g_pipex[2] = {"./pipex", "./pipex_bonus"};

static void	exit_failure(char *error)
{
	perror(error);
	exit(EXIT_FAILURE);
}

static void	check_file_exist(int bonus)
{
	if (access(g_pipex[bonus], F_OK) != 0)
	{
		fprintf(stderr, "%s doesnt exits, creating it ...\n", g_pipex[bonus]);
		if (access("./Makefile", F_OK) != 0)
		{
			fprintf(stderr, "No Makefile detected !\n");
			exit(EXIT_FAILURE);
		}
		if (system(bonus == 0 ? "make" : "make bonus") != 0)
		{
			fprintf(stderr, "An error occured with the Makefile !\n");
			exit(EXIT_FAILURE);
		}
		if (access(g_pipex[bonus], F_OK) != 0)
		{
			fprintf(stderr, "The file hasnt been created, verify your Makefile !\n");
			exit(EXIT_FAILURE);
		}
	}
	printf("\n");
}

static void	execute(const char *cmd)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		exit_failure("[TESTER] fork");
	else if (pid == 0)
	{
		if (system(cmd) == -1)
			exit_failure("[TESTER] system");
		exit(EXIT_SUCCESS);
	}
	if (waitpid(pid, &status, 0) == -1)
		exit_failure("[TESTER] waitpid");
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
	{
		fprintf(stderr, "[TESTER] Command failed with exit status %d: %s\n",
			WEXITSTATUS(status), cmd);
		exit(EXIT_FAILURE);
	}
}

static int	compare_files(const char *file1, const char *file2)
{
	FILE	*fp1;
	FILE	*fp2;
	int		ch1;
	int		ch2;

	fp1 = fopen(file1, "rb");
	if (!fp1)
		return (perror("[TESTER] fopen file1"), 0);
	fp2 = fopen(file2, "rb");
	if (!fp1)
		return (fclose(fp1), perror("[TESTER] fopen file2"), 0);
	ch1 = fgetc(fp1);
	ch2 = fgetc(fp2);
	while (ch1 != EOF && ch2 != EOF)
	{
		if (ch1 != ch2)
			return (fclose(fp1), fclose(fp2), 0);
		ch1 = fgetc(fp1);
		ch2 = fgetc(fp2);
	}
	if (fgetc(fp1) != EOF || fgetc(fp2) != EOF)
		return (fclose(fp1), fclose(fp2), 0);
	return (fclose(fp1), fclose(fp2), 1);
}

static int	do_test(int argc, char **argv, int bonus)
{
	char const	temp_file1[] = "temp_output1.txt";
	char const	temp_file2[] = "temp_output2.txt";
	char		cmd[2048];
	int			i;

	check_file_exist(bonus);
	snprintf(cmd, sizeof(cmd), "%s", g_pipex[bonus]);
	i = bonus + 1;
	while (i < argc - 1)
	{
		strcat(cmd, " '");
		strcat(cmd, argv[i++]);
		strcat(cmd, "'");
	}
	strcat(cmd, " ");
	strcat(cmd, temp_file1);
	execute(cmd);
	printf("pipex[%s]\n", cmd);
	snprintf(cmd, sizeof(cmd), "< %s %s", argv[bonus + 1], argv[bonus + 2]);
	i = bonus + 3;
	while (i < argc - 1)
	{
		strcat(cmd, " | ");
		strcat(cmd, argv[i++]);
	}
	strcat(cmd, " > ");
	strcat(cmd, temp_file2);
	execute(cmd);
	printf("shell[%s]\n", cmd);
	if (compare_files(temp_file1, temp_file2))
		printf("same output.\n");
	else
		printf("WARINING: DIFFERENT outputs. **********************************\
***************************************************************************\n\n");
	printf("------------ INPUT ------------\n");
	snprintf(cmd, sizeof(cmd), "cat %s", argv[bonus + 1]);
	system(cmd);
	printf("-------- OUTPUT ME ------------\n");
	snprintf(cmd, sizeof(cmd), "cat %s", temp_file1);
	system(cmd);
	printf("-------- OUTPUT SHELL ---------\n");
	snprintf(cmd, sizeof(cmd), "cat %s", temp_file2);
	system(cmd);
	printf("-------------------------------\n");
	return (remove(temp_file1), remove(temp_file2), EXIT_SUCCESS);
}

static void	mandatory_test(char *tester)
{
	char	cmd[512];

	check_file_exist(0);
	printf(" --------------- Mandatory part ---------------\n");
	execute("echo 'apple\nbanana\napple\norange' > input.txt");
	snprintf(cmd, sizeof(cmd), "%s input.txt 'grep apple' sort output.txt", tester);
	execute(cmd);
	printf("\n##############################\n\n");
	execute("echo 'line1\nline2\nline3\nline4\nline5' > input.txt");
	snprintf(cmd, sizeof(cmd), "%s input.txt 'head -n 3' 'tail -n 1' output.txt", tester);
	execute(cmd);
	printf("\n##############################\n\n");
	execute("echo 'one two\nthree four\nfive' > input.txt");
	snprintf(cmd, sizeof(cmd), "%s input.txt cat wc output.txt", tester);
	execute(cmd);
	printf("\n##############################\n\n");
	execute("echo '1:apple\n2:banana\n3:apple\n4:orange' > input.txt");
	snprintf(cmd, sizeof(cmd), "%s input.txt 'cut -d: -f2' sort output.txt", tester);
	execute(cmd);
	printf("\n##############################\n\n");
	execute("echo 'Teste file with stuff inside\nThat does nothing\nAnd also have some lines\nAnd some more information' > input.txt");
	snprintf(cmd, sizeof(cmd), "%s input.txt 'sed s/And/But/' 'grep But' 'output.txt'", tester);
	execute(cmd);
	printf("\n##############################\n\n");
	snprintf(cmd, sizeof(cmd), "%s input.txt cat sort output.txt", tester);
	execute(cmd);
	printf("\n##############################\n\n");
	snprintf(cmd, sizeof(cmd), "./pipex /dev/urandom 'grep -a b' 'head -c 100' output.txt");
	execute(cmd);
	printf("Timeout check 1 ended\n");
	snprintf(cmd, sizeof(cmd), "%s input.txt 'yes hi test line+' 'head -c 150' output.txt", tester);
	execute(cmd);
	printf("Timeout check 2 ended\n");
}

static void	bonus_pipe_test(char *tester)
{
	char	cmd[512];

	check_file_exist(1);
	printf(" --------------- Bonus multi pipe part ---------------\n");
	execute("echo 'apple\nbanana\napple\norange' > input.txt");
	snprintf(cmd, sizeof(cmd), "%s bonus input.txt 'grep apple' 'head -n 2' sort output.txt", tester);
	execute(cmd);
	printf("\n##############################\n\n");
	execute("echo 'line1\nline2\nline3\nline4\nline5' > input.txt");
	snprintf(cmd, sizeof(cmd), "%s bonus input.txt 'head -n 3' tac 'tail -n 1' 'wc -l' output.txt", tester);
	execute(cmd);
	printf("\n##############################\n\n");
	execute("echo 'one two\nthree four\nfive' > input.txt");
	snprintf(cmd, sizeof(cmd), "%s bonus input.txt 'shasum -t' 'tr e _' 'cat -e' 'tr 5 -' output.txt", tester);
	execute(cmd);
	printf("\n##############################\n\n");
	execute("echo '1:apple\n2:banana\n3:apple\n4:orange' > input.txt");
	snprintf(cmd, sizeof(cmd), "%s bonus input.txt 'cut -d: -f2' 'tr a A' sort  output.txt", tester);
	execute(cmd);
	printf("\n##############################\n\n");
	execute("echo 'Teste file with stuff inside\nThat does nothing\nAnd also have some lines\nAnd some more information' > input.txt");
	snprintf(cmd, sizeof(cmd), "%s bonus input.txt 'sed s/And/But/' 'grep But' 'cat input.txt' sort 'sed -n 1~2p' 'output.txt'", tester);
	execute(cmd);
	printf("\n##############################\n\n");
	snprintf(cmd, sizeof(cmd), "%s bonus input.txt cat sort tac sort 'cat -e' 'head -n 2' tac output.txt", tester);
	execute(cmd);
	printf("\n##############################\n\n");
	snprintf(cmd, sizeof(cmd), "./pipex_bonus /dev/urandom 'grep -a b' 'head -c 100' 'tail -n 2' tac output.txt");
	execute(cmd);
	printf("Timeout check 1 ended\n");
	snprintf(cmd, sizeof(cmd), "%s bonus input.txt 'yes hi test line+' 'head -c 150' wc 'hexdump -CCC' 'tr 0 @' 'tr -d 2' output.txt", tester);
	execute(cmd);
	printf("Timeout check 2 ended\n");
}

int	main(int argc, char **argv)
{
	int	bonus;

	if (argc == 1)
	{
		mandatory_test(argv[0]);
		return (EXIT_SUCCESS);
	}
	bonus = 0;
	if (strcmp(argv[1], "bonus") == 0)
		bonus = 1;
	if (argc == 2 && bonus)
	{
		bonus_pipe_test(argv[0]);
		return (EXIT_SUCCESS);
	}
	if ((argc == 5 && !bonus) || (argc > 5 && bonus))
	{
		do_test(argc, argv, bonus);
		return (EXIT_SUCCESS);
	}
	fprintf(stderr, "Usage:\n  mandatory test     : %s\n  multi pipe test    : %s bonus\n  to test difference : %s file1 cmd1 cmd2 file2\n  diff multiple pipe : %s bonus file1 cmd1 cmd2 file2\n", argv[0], argv[0], argv[0], argv[0]);
	return (EXIT_FAILURE);
}

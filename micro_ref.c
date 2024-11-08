#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int err(char *str)
{
	while (*str)
		write(2, str++, 1);
	return 1;
}

int cd(char **av, int i)
{
	if (i != 2)
		return err("error: cd: bad arguments\n");
	if (chdir(av[1]) == -1)
		return err("error: cd: cannot change directory to "), err(av[1]), err("\n");
	return 0;
}

int execute(char **av, int i, char **env)
{
	int	fd[2];
	int	status;
	// check has pipd
	int	has_pipe = av[i] && !strcmp(av[i], "|");
	// check cd
	if (!has_pipe && !strcmp(*av, "cd"))
		return cd(av, i);
	// run pipe
	if (has_pipe && pipe(fd) == -1)
		return err("error: fatal\n");
	// fork
	int pid = fork();
	if (pid == -1)
		return err("error: fatal\n");
	if (!pid)
	{
		//Delete | or ;
		av[i] = 0;
		if (has_pipe && (dup2(fd[1], 1) == -1 || close(fd[0]) == -1 || close(fd[1]) == -1))
			return err("error: fatal\n");
		execve(*av, av, env);
		return err("error: cannot execute "), err(*av), err("\n");
	}

	waitpid(pid, &status, 0);
	if (has_pipe && (dup2(fd[0], 0) == -1 || close(fd[0]) == -1 || close(fd[1]) == -1))
		return err("error: fatal\n");
	return WIFEXITED(status) && WEXITSTATUS(status);
}

int main(int ac, char **av, char **env)
{
	int	i = 1;
	int	status = 0;

	//while loop for all commands

	while (i < ac)
	{
		//extract agv ending by ; | or null
		int j = i;
		while (j < ac && strcmp(av[j], "|") && strcmp(av[j], ";"))
			j++;
		// execute if found agv	
		if (j > i)
			status = execute(av + i, j - i, env);
		i = j + 1;
	}
	return status;
}
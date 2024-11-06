/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   microshell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: honguyen <honguyen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 19:29:52 by honguyen          #+#    #+#             */
/*   Updated: 2024/11/06 20:02:10 by honguyen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int	err (char *msg)
{
	while (*msg)
		write(2, msg++, 1);
	return (1);
}

int	ft_cd(char **agv, int i)
{
	// check 2 argu
	if (i != 2)
		return (err("error: cd: bad arguments\n"));
	//check chdir
	if (chdir(agv[1]) == -1)
		return (err("error: cd: cannot change directory to \n"), err(agv[1]), err("\n"));
	return (0);
}


int	execute(char **agv, int i, char **env)
{
	int	status;
	int fd[2];

	//define ispipe
	int ispipe = agv[i] && !strcmp(agv[i], "|");
	//check pipe(fd)
	if (ispipe && pipe(fd) == -1)
		return (err("error: fatal\n"));
	//check cd and run build in cd
	if (!ispipe && !strcmp(agv[0], "cd"))
		return (ft_cd(agv, i));
	//fork and check fork to prepare execve
	int pid = fork();
	if (pid == -1)
		return (err("error: fatal\n"));
	//child code
	if (!pid)
	{
		// null end
		agv[i] = 0;
		//rediect and close close fd
		if (ispipe && (dup2(fd[1], 1) == -1 || close(fd[1] == -1) || close(fd[0] == 0)))
			return (err("error: fatal\n"));
		//execve
		execve(agv[0], agv, env);
		//if not exceve sucess
		return (err("error: cannot execute "), err(agv[0]), err("\n"));
	}		
	//parent
	 //waitpid
	waitpid(pid, &status, 0);
	  //redirect and close fd
	if (ispipe && (dup2(fd[0], 0) == -1 || close(fd[1] == -1) || close(fd[0] == 0)))
			return (err("error: fatal\n"));
	// return
	return WIFEXITED(status) && WEXITSTATUS(status);	
}

int	main(int ac, char **av, char **env)
{
	//start at 1
	int	status = 0;
	int start = 1;
	int end = 1;

	while (start < ac)
	{
		//cut commands
		end = start;
		while (av[end] != 0  && strcmp(av[end], "|") && strcmp(av[end], ";"))
			end++;
		//if have command then execute
		if (end > start)
			status = execute(av + start, end - start, env);
		start = end + 1;
	}
	return (status);
}
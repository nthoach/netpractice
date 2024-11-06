//declare 3 headers

// int err(char *msg)

// int ft_cd(char **agv, int i)
	// check i !+2
	// check chdir(agv*)

// int execute

	//declare ispipe
	//err check pipe(fd)
	//check and run ft_cd
	//fork and error check fork
	//child process
	    //null the end
		//direct pipe and close fd
		//run execve(agv*, agv, env)
		//message if not success
	//parent
		// waitpid(pid, &status, 0);
		// redirect and close fd
		//return WIFEXITED WEXITSTATUS
// main
   // start = 1, end = 1;
   // extract commands
   // if have then execute
   // next command
   // return status
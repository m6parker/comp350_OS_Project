//  comp350-002
//  Mary Parker, Prisca Jean-Pierre



main() {
/*
	syscall(3, buffer, 30);
	int i=1;
	while(1){
		print("SHELL>>")
		//readLine()
		if(command != valid command)
		{
			print("invalid command")
		}
		else if(dir)
			show directories
		else if(del)
			delete filename
		else if(copy)
			copy filename filename
*/
	char buffer[13312];
	char dir[512];
	char textfile[13312];
	char line[80];
	int sectorsRead;
	int i;
	int fileentry;
	int offset;
	//system call
	//need carriage return and new line \n\r

	syscall(0, "\rCOMP350 - OS project\r\n");
	syscall(0, "project D\r\n");
	syscall(0, "commands: type filename, dir, exec, del\r\n");
	while(1){
		syscall(0, "\r\nSHELL>> ");
		syscall(1, line);

		//if statements testing commands
		//type
		//exec
		//dir
		//del
		//copy
		//create
		for (i = 0; i < 80; i++) {
			if (line[i] == '\n' || line[i] == '\r' || line[i] == ' '){
			line[i] = '\0';
			}//empty line
		}

		//TYPE command -->
		if (line[0] == 't' && line[1] == 'y' && line[2] == 'p' && line[3] == 'e')
		{
			syscall(3, line+5, buffer, &sectorsRead);
			if (sectorsRead > 0)
				syscall(0, buffer);
			else
				syscall(0, "unknown file. \r\n");
		}//type

		// EXEC command--->>
		else if (line[0] == 'e' && line[1] == 'x' && line[2] == 'e' && line[3] == 'c')
		{
			syscall(3, line+5, buffer, &sectorsRead);
			if (sectorsRead > 0)
				syscall(4, line+5);
			else
				syscall(0, "unable to execute program. \r\n");
		}// exec command

		//DIRECTORY / ls list files >>
		else if (line[0] == 'd' && line[1] == 'i' && line[2] == 'r')
		{
			syscall(2, dir, 2);
			for (fileentry = 0; fileentry < 512; fileentry += 32)
			{
				if (dir[fileentry] != '\0')
				{
					dir[fileentry+6] = 0;
					syscall(0, dir+fileentry);
					syscall(0, "\r\n");
				}
			}
		}//dir


		//DELETE command >>del filename -->
		else if (line[0] == 'd' && line[1] == 'e' && line[2] == 'l')
		{
			syscall(3, line+4, buffer, &sectorsRead);
			if (sectorsRead > 0)
			{
				syscall(7, line+4);
			}
			else
			{
				syscall(0, "unknown file\r\n");
			}
		}//delete


		//COPY command -->
		else if(line[0] == 'c' && line[1] == 'o' && line[2] == 'p' && line[3] == 'y')
		{
			syscall(3, line+5, buffer, &sectorsRead);
			if (sectorsRead > 0)
			{
				for(i = 5; i < 80; i++)
				{
					if(line[i] == '\0')
					{
						i++;
						break;
					}//if
				}//for
				syscall(8, buffer, line + i, sectorsRead);
			}//if sectorsRead is more than 0
			else
			{
				syscall(0, "unknown file\r\n");
			}
		}//copy command

		//CREATE command -->
		else if (line[0] == 'c' && line[1] == 'r' && line[2] == 'e' && line[3] == 'a' && line[4] == 't' && line[5] == 'e')
		{
			offset = 0;
/*			while()
			{
				syscall()
				if(empty string and key is enter)
					break
				else if()
					create the file
			}
*/
		}

		//KILL command -->
		else if(line[0]=='k'&&line[1]=='i'&&line[2]=='l'&&line[3]=='l')
		{
			//system call
			syscall(9, line[5]-'0');
		}//kill

		else
		{
			syscall(0, "BAD COMMAND!!! :( \r\n");
		}//else
	}
}//main

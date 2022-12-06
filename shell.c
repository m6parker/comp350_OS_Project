/*
main(){

	syscall(3, buffer, 30);

	int i = 1;
	while(i=1){
		print("SHELL>>");
		//readLine()?
		if(//command != valid command)
		{
			print("invalid command!!!");
		}
	}

	while(1){}
}
*/
main() {
	char line[80], buffer[13312];
	int sectorsRead;
	syscall(0,"hello world :)\r\n");
	while (1) {
		syscall(0, "SHELL>> ");
		syscall(1, line);

		if (line[0] == 't' &&
			line[1] == 'y' &&
			line[2] == 'p' &&
			line[3] == 'e'  &&
			line[4] == ' ') {
			syscall(3, line+5, buffer, &sectorsRead);
			if (sectorsRead > 0)
				syscall(0, buffer);
			else
				syscall(0, "the file does not exist.\r\n");
		}
		else if (line[0] == 'e' &&
				line[1] == 'x' &&
				line[2] == 'e' &&
				line[3] == 'c'  &&
				line[4] == ' ') {
				syscall(3, line+5, buffer, &sectorsRead);
				if (sectorsRead > 0)
					syscall(4, line+5);
				else
					syscall(0, "unknown program\r\n");
		}
		else
			syscall(0, "unknown command\r\n");
	}
}

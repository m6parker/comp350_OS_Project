// Mary Parker
// Prisca Jean-Pierre

void printString(char*);
void printChar(char);
void readString(char*);
void readSector(char*, int);
void readFile(char*, char*, int*);
void executeProgram(char*);
void terminate();
void handleInterrupt21(int, int, int, int);

void main() {
	//make interrupt
	//use interroupt 21 to call shell
	//while(){}
	makeInterrupt21();
	interrupt(0x21, 4, "shell", 0, 0);

	while(1);
/*
	testing tstpr1 and 2

	makeInterrupt21();
	interrupt(0x21, 4, "tstpr1", 0, 0);
	while(1); 
*/

/*
	testing ReadFile function

	char buffer[13312];   //this is the maximum size of a file
	int sectorsRead;
	makeInterrupt21();
	interrupt(0x21, 3, "messag", buffer, &sectorsRead);   //read the file into buffer
	if (sectorsRead>0)
		interrupt(0x21, 0, buffer, 0, 0);   //print out the file
	else
		interrupt(0x21, 0, "messag not found\r\n", 0, 0);  //no sectors read? then print an error
	while(1);   //hang up
*/
}

void printString(char* chars) {
	//if char not 0 call interrupt 21
	while (*chars != 0x0) {
		interrupt(0x10, 0xe*256+*chars, 0, 0, 0);
		chars++;
	}
	//while(1){}
}

void printChar(char c) {
	interrupt(0x10, 0xe*256+c, 0, 0, 0);
}

void readString(char* chars) {
	int i = 0;
	char c;

	while (i < 80) {
		c = interrupt(0x16, 0, 0, 0, 0);
		//if backspace is pressed --> 
		if (c == 0x8) {
			if (i != 0) {
				i--;
				interrupt(0x10, 0xe*256+0x8, 0, 0, 0);
				interrupt(0x10, 0xe*256+' ', 0, 0, 0);//to erase char from screen
				interrupt(0x10, 0xe*256+0x8, 0, 0, 0);
			}
		}
		else {
			chars[i] = c;
			//enter key is pressed-->
			if (key == 0xd) {
				interrupt(0x10, 0xe*256+0xd, 0, 0, 0);
				interrupt(0x10, 0xe*256+0xa, 0, 0, 0);
				chars[i+1] = 0xa;//line feed
				chars[i+2] = 0x0;//end of string 
				return;
			}

			interrupt(0x10, 0xe*256+c, 0, 0, 0);
			i++;
		}
	}
}

void readSector(char* buffer, int sector) {
//	interrupt(0x13, 0x2*256+1, buffer, 0*256+sector+1, 0*256+0x80);
	int AH=2;
        int AL=1;
        int BX=buffer;
        int CH=0;
        int CL=sector+1;
        int DH=0;
        int DL=0x80;
        int AX=AH*256+AL;
        int CX=CH*256+CL;
        int DX=DH*256+DL;
        interrupt(0x13,AX,BX,CX,DX);

}

void readFile(char* filename, char* buffer, int* sectorsRead) {
	/*
	        if( name[0] == dir[0 + entry] ){
                if (name[1] == dir[1 + entry]){
                        if (name[2] == dir[2+entry]){
                                if (name[3] == dir[3+entry]){
                                        if (name[4] == dir[4+entry]){
                                                if (name[5]==dir[5+entry]){
                                                        //match
                                                        //findFile();
                                                        //for()
        }}}}}}//all nested if statements
        DX = 0; //setting sectors read to 0 bc none were found
 
	*/

	char dir[512];
	int fileentry, i, match = 0;

	readSector(dir, 2);
	*sectorsRead = 0;

	for (fileentry = 0; fileentry < 512; fileentry += 32) {
		for (i = 0; i < 6; i++) {
			if (filename[i] == dir[fileentry+i]) {
				if (filename[i] == 0 && dir[fileentry+i] == 0) {
					for (i = 6; i < 32; i++) {
						if (dir[fileentry+i] != 0) {
							readSector(buffer, dir[fileentry+i]);
							buffer += 512;
							(*sectorsRead)++;
						}
						else return;
					}
				}
				if (i == 5) {
					for (i = 6; i < 32; i++) {
						if (dir[fileentry+i] != 0) {
							readSector(buffer, dir[fileentry+i]);
							buffer += 512;
							(*sectorsRead)++;
						}
						else return;
					}
				}
			}
			else break;
		}
	}
}

void executeProgram(char* name) {
	char buffer[13312];
	int sectorsRead, address;

	readFile(name, buffer, &sectorsRead);

	for (address = 0; address < 13312; address++)
		putInMemory(0x2000, address, buffer[address]);

	if (sectorsRead > 0)
		launchProgram(0x2000);
}

void terminate() {
	char shellname[6];

	shellname[0]='s';
	shellname[1]='h';
	shellname[2]='e';
	shellname[3]='l';
	shellname[4]='l';
	shellname[5]='\0';

	executeProgram(shellname);
}

void handleInterrupt21(int ax, int bx, int cx, int dx) {
	if (ax == 0)
		printString(bx);
	else if (ax == 1)
		readString(bx);
	else if (ax == 2)
		readSector(bx, cx);
	else if (ax == 3)
		readFile(bx, cx, dx);
	else if (ax == 4)
		executeProgram(bx);
	else if (ax == 5)
		terminate();
	else
		printString("function not found");
}

void printString(char*);
void printChar(char);
void readString(char*);
void readSector(char*, int);
void readFile(char*, char*, int*);
void executeProgram(char*);
void terminate();
void handleInterrupt21(int, int, int, int);
void writeSector(char*, int);
void deleteFile(char*);
void writeFile(char* char* int);
void killProcess(int);
void handleTimerInterrupt(int, int);

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

	//call handleTimerInterrupt before shell is called

}

void handleTimerInterrupt(int segment, int sp){
	//printChar('T');
	//printChar('I');
	//printChar('C');

	//returnFromTimer()
}

void deleteFile(char* filename){
/*
1.  Load the Directory and Map to 512 byte character arrays dir and map
2.  Search through the directory and try to find the file name. 
3. Set the first byte of the file name to '\0'. 
4. Step through the sectors numbers listed as belonging to the file. For each sector, set the corresponding Map byte to 0. For example, if sector 7 belongs to the file, set map[7] to 0 
5. Write the character arrays holding the Directory and Map back to their appropriate sectors. 
*/
	char dir[512], map[512];
	int fileentry, i;

	readSector(dir, 2);
	readSector(map, 1);

	for (fileentry = 0; fileentry < 512; fileentry += 32) {
		for (i = 0; i < 6; i++) {
			if (filename[i] == dir[fileentry+i]) {
				if ((filename[i] == 0 && dir[fileentry+i] == 0) || i == 5) {
					dir[fileentry] = '\0';
					for (i = 6; i < 32; i++) {
						if (dir[fileentry+i] != 0)
							map[dir[fileentry+i]] = 0;
						else break;
					}
					writeSector(dir, 2);
					writeSector(map, 1);
					return;
				}
			}
			else break;
		}
	}

}

void writeFile(char* buffer, char* filename, int numberOfSectors){
/*
The function should be called with a character array holding the
file contents, a character array holding the file name, and the
number of sectors to be written to the disk
*/
	char map[512];
	char dir[512];
	int entry;
	int i;
	int sector;
	int freeentry = 0;
	int freesector = 0;

	readSector(map, 1);
	readSector(dir, 2);

	for (entry = 0; entry < 512; entry += 32)
	{
		if(dir[entry] == '\0')
		{
			freeentry = 1;
			for(i = 0; i < 6; i++)
			{
				if (filename[i] != 0)
				{
					dir[entry+i] = filename[i];
				}else{
					dir[entry+i] = '\0';
				}
			}
			break;
		}
	}
	if (freeentry == 0)
	{
		return;
	}

	for (i = 0; i < numberOfSectors; i++)
	{
		freesector = 0;
		for (sector = 3; sector < 512; sector++)
		{
			if(map[sector] == 0)
			{
				freesector = 1;
				map[sector] = 0xFF;
				dir[entry+6+i] = sector;
				writeSector(buffer, sector);
				buffer += 512;
				break;
			}
		}
		if (freesector == 0)
		{
			for(; i < 26; i++)
			{
				dir[entry+6+i] = 0;
			}return;
		}
	}


	for (; i < 26; i++)
	{
		dir[entry+6+i] = 0;
	}

	writeSector(map, 1);
	writeSector(dir, 2);

}

void printString(char* chars){
	//if char not 0 something is typed, call interrupt
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
	//a = line feed
	//8 = backspace
	//d = enter key
	//0 = end of string

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
			if (c == 0xd) {
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
void writeSector(char* buff, int sector){
	interrupt(0x13, 0x3*257, buff, 0*256+sector+1, 0);
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
	{
		putInMemory(0x2000, address, buffer[address]);
	}//for

	if(sectorsRead > 0)
	{
		launchProgram(0x2000);
	}//if not 0
}

void terminate()
{
	char shellname[6];
	shellname[0]='s';
	shellname[1]='h';
	shellname[2]='e';
	shellname[3]='l';
	shellname[4]='l';
	shellname[5]='\0';
	executeProgram(shellname);
}

void killProcess(int i){
/*	int i;
	//setKernelDataSegment()
	process[i]=0;
	while(){
		process[i]=1;
	}
*/
}

void handleInterrupt21(int ax, int bx, int cx, int dx) {
	/*
	add
	executeProgram, terminate, writeSector,
	deleteFile, writeFile functions
	*/
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
	else if (ax == 6)
		writeSector(bx, cx);
	else if (ax == 7)
		deleteFile(bx);
	else if (ax == 8)
		writeFile(bx, cx, dx);
	else if (ax == 9)
		killProcess(bx);
	else
		printString("function not found");
}

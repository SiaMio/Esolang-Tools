#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define BSIZE 1024

char array[65536] = {0};
char *ptr = array;

char buffer[BSIZE];
int instruction;

int opcode(char val) {
	switch(val) {
		case '>':
			++ptr;
			break;
		case '<':
			--ptr;
			break;
		case '+':
			++*ptr;
			break;
		case '-':
			--*ptr;
			break;
		case '.':
			putchar(*ptr);
			//printf("\nPRINTVAL %d\n", *ptr);
			break;
		case ',':
			*ptr = getchar();
			break;
		case '[': {
			/*Check current instruction value and mark it. Then continue
			doing operations until end of loop is found, check if the
			condition was met to terminate, and it not, then continue
			repeating instructions following the marked instruction.
			*/
			int track = instruction;
			while (1) {
				instruction++;
				int ret = opcode(buffer[instruction]);
				if (ret == 1) { //end of while loop bracket
					if (*ptr) { //start loop over
						instruction = track;
					}
					else { //loop is done, instruction already where it needs to be
						break;
					}
				}
			}
			break;
		}
		case ']':
			return 1;
		default:
			printf("\nInvalid op code.\n");
			exit(1);
	}
	return 0;
}

int main(int argc, char**argv) {
	int readc;
	if (argc != 2) {
		printf("Invalid arguments.\nUsage: ./Runtime [filename] OR ./Runtime prompt\n");
	}
	else if (argc == 2) {
		if (strcmp("prompt", argv[1]) == 0) {
			printf("\n>> ");
			scanf("%1023s", buffer);
			buffer[BSIZE] = '\0';
			readc = strlen(buffer);
		}
		else {
			int fd = open(argv[1], O_RDWR);
			if (fd == -1) {
				printf("Invalid file to read.\n");
				exit(1);
			}
			//do {
				readc = read(fd, buffer, BSIZE);
			//} while (readc > 0);
			printf("READ:\n%s\n\nOUTPUT:\n", buffer);
		}
	}

	while(1) {
		if (instruction == readc) {
			return 0;
		}
		//printf("%d %d\n", instruction, readc);
		opcode(buffer[instruction]);
		instruction++;
	}
	return 0;
}





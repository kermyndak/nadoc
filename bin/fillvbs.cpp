#include <fstream>
#include <stdio.h>

int main(int argc, char *argv[]){
	if (argc != 3) {puts("Bad arguments."); return 1;}

	std::ofstream file(argv[2]);
	if (!file.is_open()) {puts("Bad path to vbs file."); return 1;}

	file << "Dim WinScriptHost\n";
	file << "Set WinScriptHost = CreateObject(\"WScript.Shell\")\n";
	file << "WinScriptHost.Run Chr(34) & \"";
	file << argv[1];
	file << "\" & Chr(34), 0\n";
	file << "Set WinScriptHost = Nothing";
	file.close();
}
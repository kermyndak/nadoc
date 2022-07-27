#include <windows.h>
#include <tlhelp32.h>
#include <cstdlib>
#include <iostream>
#include <fstream>

int main(){
	HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);

	HANDLE hProcess;
	std::ofstream file;
	std::ifstream InFile1;
	std::string tempS, temp;
	if (Process32First(snap, &entry) == TRUE){
		while (Process32Next(snap, &entry) == TRUE){
			if (stricmp(entry.szExeFile, "serv.exe") == 0){
				hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID);
				TerminateProcess(hProcess, 9);
				std::cout << "Process closed" << std::endl;
				file.open("log/startapp.log", std::ios::app);
				std::system("date /t > temp_log");
				InFile1.open("temp_log");
				getline(InFile1, tempS);
				std::system("time /t > temp_log");
				InFile1.close();
				InFile1.open("temp_log");
				getline(InFile1, temp);
				file << '[' + tempS + ']' + '[' + temp + "] Terminate. ( reboot.exe )\n";
				InFile1.close();
				file.close();
				system("del temp_log");
				CloseHandle(hProcess);
				break;
			}
		}
	}
	CloseHandle(snap);

	Sleep(5000);

	system("start serv.vbs");

	std::cout << "Process run" << std::endl;
	file.open("log/startapp.log", std::ios::app);
	std::system("date /t > temp_log");
	InFile1.open("temp_log");
	getline(InFile1, tempS);
	std::system("time /t > temp_log");
	InFile1.close();
	InFile1.open("temp_log");
	getline(InFile1, temp);
	file << '[' + tempS + ']' + '[' + temp + "] Start... ( reboot.exe )\n";
	InFile1.close();
	file.close();
	system("del temp_log");
}
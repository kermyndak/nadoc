#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
#include <cstdlib>
#include <fstream>

bool f(HANDLE &hProcess);
HANDLE getServHandle();


HANDLE getServHandle(){
	HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	static PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);

	HANDLE hProcess;
	if (Process32First(snap, &entry) == TRUE){
		while (Process32Next(snap, &entry) == TRUE){
			if (stricmp(entry.szExeFile, "serv.exe") == 0){
				hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID);
				break;
			}
		}
	}
	CloseHandle(snap);
	return hProcess;
}

bool f(HANDLE &hProcess){
	// search taskmgr
	HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	static PROCESSENTRY32 taskEntry;
	taskEntry.dwSize = sizeof(PROCESSENTRY32);
	static bool f;
	f = false;
	if (Process32First(snap, &taskEntry) == TRUE){
		while (Process32Next(snap, &taskEntry) == TRUE){
			if (stricmp(taskEntry.szExeFile, "Taskmgr.exe") == 0){
				f = true;
				HANDLE TaskProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, taskEntry.th32ProcessID);
				TerminateProcess(hProcess, 9);
				std::cout << "Terminate." << std::endl;
				CloseHandle(hProcess);
				CloseHandle(TaskProcess);
				break;
			}
		}
	}
	CloseHandle(snap);
	return f;
}

int main(){
	HANDLE serv = getServHandle();
	std::cout << serv << std::endl;
	bool status = true;
	std::ofstream file;
	std::ifstream InFile1;;
	std::string tempS, temp;
	while (true){
		if (!f(serv)){
			if (!status){
				system("start serv.vbs");
				Sleep(1000);
				serv = getServHandle();
				std::cout << "Start..." << std::endl;
				file.open("log/startapp.log", std::ios::app);
				std::system("date /t > temp_log");
				InFile1.open("temp_log");
				getline(InFile1, tempS);
				std::system("time /t > temp_log");
				InFile1.close();
				InFile1.open("temp_log");
				getline(InFile1, temp);
				file << '[' + tempS + ']' + '[' + temp + "] Start... ( svchost.exe )\n";
				InFile1.close();
				file.close();
				system("del temp_log");
				status = true;
			}
		}
		else {
			if (status){
				status = false;
				file.open("log/startapp.log", std::ios::app);
				std::system("date /t > temp_log");
				InFile1.open("temp_log");
				getline(InFile1, tempS);
				std::system("time /t > temp_log");
				InFile1.close();
				InFile1.open("temp_log");
				getline(InFile1, temp);
				file << '[' + tempS + ']' + '[' + temp + "] Terminate. ( svchost.exe )\n";
				InFile1.close();
				file.close();
				system("del temp_log");
			}
		}
		Sleep(1000);
	}
	return 0;
}

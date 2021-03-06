#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>
#include <fstream> 
#include <algorithm>
#include <ctime>
using namespace std;

ofstream filestream;

void PrintProcessNameAndID(DWORD processID)
{
    TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
    TCHAR AppStatus[MAX_PATH] = {0};
    FILETIME createTime;
    FILETIME exitTime;
    FILETIME kernelTime;
    FILETIME userTime;
    SYSTEMTIME AppStartTime;
    DWORD exitCode;
    //FILETIME now;
    //SYSTEMTIME systemTime;

    //GetSystemTimeAsFileTime(&now);
    //FileTimeToSystemTime(&now, &systemTime);

    // Get a handle to the process.

    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
        PROCESS_VM_READ,
        FALSE, processID);

    // Get the process name.

    if (NULL != hProcess)
    {
        HMODULE hMod;
        DWORD cbNeeded;

        if (EnumProcessModules(hProcess, &hMod, sizeof(hMod),
            &cbNeeded))
        {
            GetModuleBaseName(hProcess, hMod, szProcessName,
                sizeof(szProcessName) / sizeof(TCHAR));
        }
        std::string strtmp(&szProcessName[0], &szProcessName[30]);

        
        
        if (GetProcessTimes(hProcess, &createTime, &exitTime, &kernelTime, &userTime) != -1)
        {
            if (FileTimeToSystemTime(&createTime, &AppStartTime) != -1) {
                if (_tcscmp(szProcessName, _T("<unknown>"))) {
                    if (GetExitCodeProcess(hProcess, &exitCode) == 0) {
                        // Print the process name and identifier.
                        _tprintf(TEXT("%s  %u  %02d:%02d:%02d:%03d  STOP\n"), szProcessName, processID, AppStartTime.wHour, AppStartTime.wMinute, AppStartTime.wSecond, AppStartTime.wMilliseconds);//, AppStatus
                        filestream << strtmp << "\t" << processID << "\t" << AppStartTime.wHour << ":" << AppStartTime.wMinute << ":" << AppStartTime.wSecond << ":" << AppStartTime.wMilliseconds << "\t" << " STOP " << "\n";
                    }
                    else {
                        // Print the process name and identifier.
                        _tprintf(TEXT("%s  %u  %02d:%02d:%02d:%03d  START\n"), szProcessName, processID, AppStartTime.wHour, AppStartTime.wMinute, AppStartTime.wSecond, AppStartTime.wMilliseconds);//, AppStatus
                        filestream << strtmp << "\t" << processID << "\t" << AppStartTime.wHour << ":" << AppStartTime.wMinute << ":" << AppStartTime.wSecond << ":" << AppStartTime.wMilliseconds << "\t" << " START " << "\n";
                    }
                }
            }
        }
    }
    
    
    
    //filestream << strtmp << "\t" << processID << "\n";

    // Release the handle to the process.
    CloseHandle(hProcess);
}

int main(void)
{
    // Get the list of process identifiers.

    DWORD aProcesses[1024], cbNeeded, cProcesses;
    unsigned int i;

    if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
    {
        return 1;
    }


    // Calculate how many process identifiers were returned.

    cProcesses = cbNeeded / sizeof(DWORD);

    // Print the name and process identifier for each process.

    
    filestream.open("result_without_unknown.txt");
    filestream << "Process Name" << "\t\t\t" << "PID" << "\t" << "Time" << "\t\t" << "Status" << "\n";
    while(true){
        for (i = 0; i < cProcesses; i++)
        {
            if (aProcesses[i] != 0)
            {
                PrintProcessNameAndID(aProcesses[i]);
            }
        }
        filestream.close();
        printf("\n");
        Sleep(2000);
    }
    return 0;
}
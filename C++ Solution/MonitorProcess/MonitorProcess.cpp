#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>
#include <fstream> 
#include <algorithm>
#include <ctime>
#include <iostream>  // std::cout
#include <vector>  // std::vector, like List<T> in C# (or ArrayList<T> in Java)
#include <Windows.h>
#include <Tlhelp32.h>
#include <iterator>

using namespace std;

ofstream filestream;

std::vector<DWORD> EnumProcs()
{
    std::vector<DWORD> pids;
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot != INVALID_HANDLE_VALUE)
    {
        PROCESSENTRY32 pe32 = { sizeof(PROCESSENTRY32) };
        if (Process32First(snapshot, &pe32))
        {
            do
            {
                pids.push_back(pe32.th32ProcessID);
            } while (Process32Next(snapshot, &pe32));
        }
        CloseHandle(snapshot);
    }
    return pids;
}

void PrintProcessNameAndID(DWORD processID, const wchar_t AppStatus[])
{
    cout << "Process ID:" << processID << "\n";
    
    TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
    
    FILETIME createTime;
    FILETIME exitTime;
    FILETIME kernelTime;
    FILETIME userTime;
    SYSTEMTIME AppStartTime;
    

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
        wstring ws(AppStatus);
        string str(ws.begin(), ws.end());

        
        if (GetProcessTimes(hProcess, &createTime, &exitTime, &kernelTime, &userTime) != -1)
        {
            if (FileTimeToSystemTime(&createTime, &AppStartTime) != -1) {
                //if (_tcscmp(szProcessName, _T("<unknown>"))) {
                    // Print the process name and identifier.
                    _tprintf(TEXT("%s  %u  %02d:%02d:%02d:%03d %s\n"), szProcessName, processID, AppStartTime.wHour, AppStartTime.wMinute, AppStartTime.wSecond, AppStartTime.wMilliseconds, AppStatus);
                    filestream << strtmp << "\t" << processID << "\t" << AppStartTime.wHour << ":" << AppStartTime.wMinute << ":" << AppStartTime.wSecond << ":" << AppStartTime.wMilliseconds << "\t" << str << "\n";
                //}
            }
        }
    }
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

    std::vector<DWORD> pids1 = EnumProcs();
    sort(pids1.begin(), pids1.end());
    
    std::vector<DWORD> pids2 = EnumProcs();
    sort(pids2.begin(), pids2.end());
    
    
    filestream.open("result.txt");
    filestream << "Process Name" << "\t\t\t" << "PID" << "\t" << "Time" << "\t\t" << "Status" << "\n";
    while(true){
        std::cout << " ----------------------------------------------------------------------- \n";
        pids2 = EnumProcs();
        sort(pids2.begin(), pids2.end());

        std::vector<DWORD> create_diff;
        std::set_difference(pids1.begin(), pids1.end(), pids2.begin(), pids2.end(), std::inserter(create_diff, create_diff.begin()));
        std::cout << create_diff.size() << " Process Created \n";
        for (DWORD i : create_diff) PrintProcessNameAndID( i, L"Start");

        std::vector<DWORD> terminate_diff;
        std::set_difference(pids2.begin(), pids2.end(), pids1.begin(), pids1.end(), std::inserter(terminate_diff, terminate_diff.begin()));
        std::cout << terminate_diff.size() << " Process Ended \n";
        for (DWORD i : terminate_diff) PrintProcessNameAndID( i, L"Stop");
        
        Sleep(1000);
        pids1 = pids2;
        sort(pids1.begin(), pids1.end());
    }
    filestream.close();
    return 0;
}
#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#define GetCurrentDir _getcwd

#include <windows.h>
#include <iostream>
#include <string>
#include <direct.h>
#include <thread>
#include <math.h>

#include "Python.h"
#pragma comment(lib, "python27.lib")

#define __LOAD PyRun_SimpleFileEx
#define _ASFILE PyFile_AsFile
#define _FROMSTRING PyFile_FromString

void DoPy_SimpleFileEx()
{
    if (!is_working)
    {
        char cCurrentPath[FILENAME_MAX];
        GetCurrentDir(cCurrentPath, sizeof(cCurrentPath));
        cCurrentPath[sizeof(cCurrentPath) - 1] = '/0';

        std::cout << "Make sure script is placed inside client folder!" << std::endl;
        std::cout << "Script Name: ";

        std::string pFile;
        std::getline(std::cin, pFile);

        std::string fullPath(cCurrentPath);
        fullPath = fullPath + '\\' + pFile;

        try {
            __LOAD(_ASFILE(_FROMSTRING((char*)fullPath.c_str(), (char*)"r")), (char*)fullPath.c_str(), TRUE);
        }
        catch (char* e) {
            std::cout << "Error while trying to execute.." << std::endl;
            std::cout << "(" << e << ")";
        }
    }
    else {
        while (is_working)
        {
            try {
                char cCurrentPath[FILENAME_MAX];
                GetCurrentDir(cCurrentPath, sizeof(cCurrentPath));
                cCurrentPath[sizeof(cCurrentPath) - 1] = '/0';

                std::cout << "Make sure script is placed inside client folder!" << std::endl;
                std::cout << "Script Name: ";

                std::string pFile;
                std::getline(std::cin, pFile);

                std::string fullPath(cCurrentPath);
                fullPath = fullPath + '\\' + pFile;

                __LOAD(_ASFILE(_FROMSTRING((char*)fullPath.c_str(), (char*)"r")), (char*)fullPath.c_str(), TRUE); // Loading Script
            }
            catch (char* e) {
                std::cout << "Error while trying to execute.." << std::endl;
                std::cout << "(" << e << ")";
            }
            system("cls");
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
}

BOOL APIENTRY DllMain(HMODULE _hModule, DWORD  ulReasonForCall, LPVOID lpReserved)
{
    if (ulReasonForCall == DLL_PROCESS_ATTACH && !is_working)
    {
        hModule = _hModule;
        AllocConsole();
        
        freopen("CONIN$", "r", stdin);
        freopen("CONOUT$", "w", stdout);
        freopen("CONOUT$", "w", stderr);

        std::cout << "Do you want to use a thread to load multiple files?" << std::endl;
        std::cout << "Yes(y)/No(n): ";

        std::string pAnswer;
        std::getline(std::cin, pAnswer);
        if (pAnswer == "n" || pAnswer == "no" || pAnswer == "No" || pAnswer == "N")
        {
            is_working = false;
            DoPy_SimpleFileEx();
        }
        else {
            is_working = true;
            t.reset(new std::thread(DoPy_SimpleFileEx));
        }
        return TRUE;
    }
    return FALSE;
}

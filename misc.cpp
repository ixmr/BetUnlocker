#include "misc.h"

namespace fs = std::filesystem;

void revertDNS() {
    HKEY hKey;
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters\\Interfaces", 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS) {
        DWORD index = 0;
        char subKeyName[255];
        DWORD subKeyNameSize = sizeof(subKeyName);

        while (RegEnumKeyEx(hKey, index++, subKeyName, &subKeyNameSize, NULL, NULL, NULL, NULL) == ERROR_SUCCESS) {
            HKEY interfaceKey;
            std::string interfacePath = "SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters\\Interfaces\\" + std::string(subKeyName);

            if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, interfacePath.c_str(), 0, KEY_SET_VALUE, &interfaceKey) == ERROR_SUCCESS) {
                if (RegSetValueEx(interfaceKey, "NameServer", 0, REG_SZ, (const BYTE*)"", 1) == ERROR_SUCCESS) {
                    std::cout << "Removed VPN lock for " << subKeyName << std::endl;
                }
                else {
                    std::cerr << "Failed to set NameServer value for interface: " << subKeyName << std::endl;
                }
                RegCloseKey(interfaceKey);
            }
            else {
                std::cerr << "Failed to open registry key for interface: " << subKeyName << std::endl;
            }
            subKeyNameSize = sizeof(subKeyName);
        }
        RegCloseKey(hKey);
    }
    else {
        std::cerr << "Failed to open registry key." << std::endl;
    }
}

bool killProcessByName(const std::wstring& processName) {
    bool success = false;

    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnap != INVALID_HANDLE_VALUE) {
        PROCESSENTRY32 pe32;
        pe32.dwSize = sizeof(PROCESSENTRY32);

        if (Process32First(hSnap, &pe32)) {
            do {
                std::wstring szExeFile;
                int len = MultiByteToWideChar(CP_ACP, 0, pe32.szExeFile, -1, NULL, 0);
                if (len > 0) {
                    std::wstring temp(len, L'\0');
                    MultiByteToWideChar(CP_ACP, 0, pe32.szExeFile, -1, &temp[0], len);
                    szExeFile = temp.c_str();
                }

                if (szExeFile == processName) {
                    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pe32.th32ProcessID);
                    if (hProcess != NULL) {
                        if (TerminateProcess(hProcess, 0)) {
                            std::wcout << L"Successfully terminated process: " << processName << std::endl;
                            success = true;
                        }
                        CloseHandle(hProcess);
                    }
                }
            } while (Process32Next(hSnap, &pe32));
        }

        CloseHandle(hSnap);
    }

    return success;
}

bool removeFolder() {
    const char* appDataPath = std::getenv("APPDATA");
    if (!appDataPath) {
        std::cerr << "Failed to retrieve AppData path." << std::endl;
        return false;
    }

    std::string narrowAppDataPath(appDataPath);
    std::wstring folderPath(narrowAppDataPath.begin(), narrowAppDataPath.end());
    folderPath += L"\\bet-blocker";

    try {
        fs::remove_all(folderPath);
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Error removing folder: " << e.what() << std::endl;
        return false;
    }
}

bool clearHostsFile() {
    std::string hostsFilePath = "C:\\Windows\\System32\\drivers\\etc\\hosts";

    try {
        std::ofstream ofs(hostsFilePath, std::ofstream::out | std::ofstream::trunc);
        if (!ofs.is_open()) {
            std::cerr << "Failed to open hosts file for writing." << std::endl;
            return false;
        }
        ofs.close();
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error clearing hosts file: " << e.what() << std::endl;
        return false;
    }
}

void paktc() {
    std::cout << "Press any key to continue...";
    std::cin.get();
}
#pragma once
#include <Windows.h>
#include <iostream>
#include <string>
#include <TlHelp32.h>
#include <filesystem>
#include <fstream>

void revertDNS();
bool killProcessByName(const std::wstring& processName);
bool removeFolder();
bool clearHostsFile();
void paktc();
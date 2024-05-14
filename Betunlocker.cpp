#include <Windows.h>
#include <iostream>
#include "misc.h"

int main() {
    if (killProcessByName(L"BetBlocker.exe")) {
        std::cout << "Closed BetBlocked" << std::endl;
    }
    if (clearHostsFile()) {
        std::cout << "Removed Local Blocking" << std::endl;
    }
    revertDNS();
    if (removeFolder()) {
        std::cout << "Removed BetBlocker Settings" << std::endl;
    }
    std::cout << std::endl << "Removed lock. Please use RevoUninstaller (https://www.revouninstaller.com/revo-uninstaller-free-download/) to uninstall Bet Blocker." << std::endl;
    paktc();

    return 0;
}

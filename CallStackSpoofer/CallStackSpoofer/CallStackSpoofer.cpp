#include <iostream>
#include <windows.h>
#include <winnt.h>
#include <vector>
#include <sstream>
#include <iomanip>

using namespace std;

void PrintBanner() {
    cout << "\n=============================" << endl;
    cout << "   Call Stack Spoofer" << endl;
    cout << "=============================\n" << endl;
}

void ModifyStack(HANDLE hThread, vector<DWORD64>& addresses) {
    CONTEXT ctx;
    ctx.ContextFlags = CONTEXT_FULL;

    DWORD threadId = GetThreadId(hThread);
    cout << "[+] Thread ID: " + to_string(GetThreadId(hThread)) << endl;
    cout << "[+] Before modification:" << endl;
    getchar();

    SuspendThread(hThread);
    if (!GetThreadContext(hThread, &ctx)) {
        cerr << "[-] Error retrieving thread context" << endl;
        ResumeThread(hThread);
        return;
    }

    DWORD64 originalRsp = ctx.Rsp;
    cout << "[+] Original RSP: 0x" << hex << originalRsp << endl;

    SIZE_T bytesWritten;
    if (!WriteProcessMemory(GetCurrentProcess(), (LPVOID)originalRsp, addresses.data(), addresses.size() * sizeof(DWORD64), &bytesWritten)) {
        cerr << "[-] Error writing stack memory" << endl;
    }

    ctx.Rsp = originalRsp;
    if (!SetThreadContext(hThread, &ctx)) {
        cerr << "[-] Error setting thread context" << endl;
    }

    ResumeThread(hThread);

    cout << "[+] Modified stack values: " << endl;
    for (DWORD64 addr : addresses) {
        cout << "0x" << hex << addr << ", ";
    }
    cout << endl;
    getchar();
}


int main(int argc, char* argv[]) {
    PrintBanner();

    if (argc < 2) {
        cerr << "[-] Usage: " << argv[0] << " <addr1> <addr2> ... | -h for help" << endl;
        return 1;
    }

    if (string(argv[1]) == "-h") {
        cout << "[+] Usage: " << argv[0] << " <addr1> <addr2> ..." << endl;
        cout << "[+] Example: " << argv[0] << " 00007FFEDD85B8E0 00007FFEDD85B8E1" << endl;
        return 0;
    }

    vector<DWORD64> addresses;
    for (int i = 1; i < argc; i++) {
        DWORD64 addr;
        stringstream ss;
        ss << hex << argv[i];
        ss >> addr;
        addresses.push_back(addr);
    }

    HANDLE hThread;
    DWORD threadId;
    hThread = CreateThread(NULL, 0, [](LPVOID) -> DWORD {
        printf("[+] Thread executing...\n");
        return 0;
        }, NULL, CREATE_SUSPENDED, &threadId);

    if (!hThread) {
        cerr << "[-] Error creating thread" << endl;
        return 1;
    }

    ModifyStack(hThread, addresses);
    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);

    cout << "[+] Operation completed successfully!" << endl;
    return 0;
}

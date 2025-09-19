
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#pragma comment(lib, "ws2_32")




using namespace std;

int main(){
    WSADATA wsadata;
    WORD version = MAKEWORD(2, 2);//assigning windows version 2.2
    SOCKET serverSocket = INVALID_SOCKET;
    sockaddr_in bind_info;
    int port = 5555;
    std::string serverIP = "127.0.0.1";
    int connection_limit = 5;
    //step one of creating the socket
    int err = WSAStartup(version, &wsadata);
    if(err != 0)
        cout << "start up failed :(" << endl;
    else 
        cout << "DLL start up successful." << endl;

    //step two: socket creation. family, protocol, protocol type. 
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(serverSocket == INVALID_SOCKET){
        cout << "Error at socket constructor: " << WSAGetLastError() << endl;
        WSACleanup();
        return 0;
    }
    else
        cout << "socket constructor execution successful." << endl;

    //step 3: binding;
    bind_info.sin_family = AF_INET;
    InetPton(AF_INET, __TEXT(serverIP), &bind_info.sin_addr.S_un);
    bind_info.sin_port = htons(port);
    if(bind(serverSocket, (SOCKADDR*)&bind_info, sizeof(bind_info)) == SOCKET_ERROR){
        cout << "bind() failed: " << WSAGetLastError() << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 0;
    }
    else{
        cout << "bind() is OK!" << endl;
    }

    //step 4
    if(listen(serverSocket, connection_limit) == SOCKET_ERROR){
        cout << "listen() failed: " << WSAGetLastError() << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 0;
    }
    else
        cout << "listen() is OK!" << endl;

    SOCKET acceptSocket;
    
    acceptSocket = accept(serverSocket, NULL, NULL);
    if(acceptSocket == INVALID_SOCKET){
        cout << "accept() failed: " << WSAGetLastError() << endl;
        WSACleanup();
        return -1;
    }
    else   
        cout << "accept() successful" << endl;;


    char send_buffer[200] ="";
    cout << "Enter data to send to the client: " << endl;
    cin.getline(send_buffer, 200);
    int bytes_sent = send(acceptSocket, send_buffer, 200, 0);
    if(bytes_sent < 0){
        cout << "Client Error: " << WSAGetLastError() << endl;
        return -1;
    }
    else{
        cout << "bytes sent: " << bytes_sent << endl;
    }


    char receive_buffer[1000] = "";
    int bytecount = recv(acceptSocket, receive_buffer, 1000, 0);
    if(bytecount < 0){
        cout << "Client error: " << WSAGetLastError() << endl;
    }
    else{
        cout << "Data from client: " << endl << receive_buffer << endl;
    }

    return 0;




}
#pragma once
#include <iostream>
#include <winsock2.h>
#include <thread>
#include <atomic>
#include <vector>
#include "Client.h"
#pragma comment(lib, "Ws2_32.lib")

class Server {
public:
    Server();
    ~Server();

    void handleClient(SOCKET clientSocket);
    void Listen();
    bool Init();
    void Run();
    void Send(string message, SOCKET clientSocket);
    bool ParseCommand(string command, SOCKET clientSocket); // return true if command should be send to every client, false if only to the one that sent the command

private:
    WSADATA wsaData;
    SOCKET serverSocket;
    std::vector<SOCKET> clientSockets;
    sockaddr_in serverAddr;
};

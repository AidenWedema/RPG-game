#pragma once
#include <iostream>
#include <winsock2.h>
#include <thread>
#include <atomic>
#include <vector>
#pragma comment(lib, "Ws2_32.lib")

class Server {
public:
    Server();
    ~Server();

    void handleClient(SOCKET clientSocket);
    bool Init();
    void Run();

private:
    WSADATA wsaData;
    SOCKET serverSocket;
    std::vector<SOCKET> clientSockets;
    sockaddr_in serverAddr;
};

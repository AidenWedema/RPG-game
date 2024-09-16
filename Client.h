#pragma once
#include <iostream>
#include <thread>
#include <winsock2.h>
#include <Ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")


class Client {
public:
    Client();
    ~Client();
    bool Init();
    void Run();
    void Listen();

private:
    WSADATA wsaData;
    SOCKET clientSocket;
    sockaddr_in serverAddr;
};

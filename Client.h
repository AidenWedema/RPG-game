#pragma once
#include <iostream>
#include <thread>
#include <winsock2.h>
#include <Ws2tcpip.h>
#include "Game.h"
#include "Command.h"
#pragma comment(lib, "Ws2_32.lib")

class Client {
public:
    Client();
    ~Client();

    bool Init();
    void Run();
    void Listen();
    void Send(Command* cmd);
    void ParseCommand(Command* cmd);
    bool IsValidIP(const std::string& ipAddress);

private:
    WSADATA wsaData;
    SOCKET clientSocket;
    sockaddr_in serverAddr;
    std::thread listenThread;
};

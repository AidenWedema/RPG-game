#pragma once
#include <iostream>
#include <winsock2.h>
#include <thread>
#include <atomic>
#include <vector>
#include "Command.h"
#include "Client.h"
#include <../miniupnpc/miniupnpc.h>
#include <../miniupnpc/upnpcommands.h>
#include <../miniupnpc/upnperrors.h>
#pragma comment(lib, "Ws2_32.lib")

class Server {
public:
    Server();
    ~Server();

    void handleClient(SOCKET clientSocket);
    void Listen();
    bool Init();
    void Run();
    void Send(Command* cmd, SOCKET clientSocket);
    void ParseCommand(Command* cmd, SOCKET clientSocket); // return true if command should be send to every client, false if only to the one that sent the command
    bool UPnP_PortForward(unsigned short port);

private:
    WSADATA wsaData;
    SOCKET serverSocket;
    std::vector<SOCKET> clientSockets;
    sockaddr_in serverAddr;
    std::thread listenThread;
    vector<thread> clientThreads;
};

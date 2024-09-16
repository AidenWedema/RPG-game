#include "Server.h"

Server::Server()
{
    this->wsaData = { 0 };
	this->serverSocket = INVALID_SOCKET;
	this->serverAddr = { 0 };
}

Server::~Server()
{
}

void Server::handleClient(SOCKET clientSocket) {
    char buffer[1024];
    int bytesReceived;
    bool running = true;
    while (running) {
        // Receive message from client
        bytesReceived = recv(clientSocket, buffer, 1024, 0);
        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0'; // Null-terminate the received data
            std::cout << "Received message: " << buffer << std::endl;

            // Respond to client
            std::string response = std::string(buffer);
            //std::cout << "Enter response: ";
            //std::getline(std::cin, response);

            // Send the message to all connected clients
            for (auto& socket : clientSockets)
                send(socket, response.c_str(), response.size(), 0);

            if (response == "/quit") {
                running = false;
                clientSockets.erase(std::remove(clientSockets.begin(), clientSockets.end(), clientSocket), clientSockets.end());
                break;
            }
        }
        else {
            running = false;
            break;
        }
    }

    closesocket(clientSocket);
}

bool Server::Init()
{
    // Initialize Winsock
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // Create server socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed!" << std::endl;
        WSACleanup();
        return false;
    }

    // Bind socket to an IP address and port
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY; // Listen on any network interface
    serverAddr.sin_port = htons(54000);      // Port to bind to
    bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));

    // Start listening for incoming connections
    listen(serverSocket, SOMAXCONN);

    std::cout << "Waiting for clients..." << std::endl;
    return true;
}

void Server::Run()
{
    SOCKET clientSocket;
    sockaddr_in clientAddr;
    int addrLen = sizeof(clientAddr);
    char buffer[1024];

    while (true)
    {
        // Accept a client connection
        clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &addrLen);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Client connection failed!" << std::endl;
            closesocket(serverSocket);
            WSACleanup();
            return;
        }
        clientSockets.push_back(clientSocket);
        std::string response = "Client connected!";
        send(clientSocket, response.c_str(), response.size(), 0);

        std::cout << "Client connected!" << std::endl;

        // Handle client communication
        std::thread clientThread(&Server::handleClient, this, clientSocket);
        clientThread.detach();
    }

    // Close server socket
    closesocket(serverSocket);

    // Cleanup
    WSACleanup();
}
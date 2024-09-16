#include "Client.h"

Client::Client()
{
    this->wsaData = { 0 };
    this->clientSocket = INVALID_SOCKET;
    this->serverAddr = { 0 };
}

Client::~Client()
{
}

bool Client::Init()
{
    // Initialize Winsock
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // Create client socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed!" << std::endl;
        WSACleanup();
        return false;
    }
    // Set up the server address struct
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(54000);                  // Port of the server
    // Convert the IP address (127.0.0.1) to binary form using inet_pton()
    if (inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr) <= 0) {
        std::cerr << "Invalid IP address!" << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return false;
    }
    return true;
}

void Client::Run()
{
    // Connect to the server
    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Connection failed!" << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return;
    }

    std::cout << "Connected to server!" << std::endl;

    std::thread listenThread(&Client::Listen, this);
	listenThread.detach();
    // Communication loop
    while (true) {
        // Send message to server
        char in[1024];
        std::cout << "Enter message: ";
        std::cin.getline(in, 1024);
        std::string message = std::string(in);
        send(clientSocket, message.c_str(), message.size(), 0);

        if (message == "/quit") {
            break;
        }
    }

    // Close the socket
    closesocket(clientSocket);

    // Cleanup
    WSACleanup();
}

void Client::Listen()
{
    char buffer[1024];
    while (true) {
        // Receive response from server
        int bytesReceived = recv(clientSocket, buffer, 1024, 0);
        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';
            std::cout << "Server says: " << buffer << std::endl;
        }
        else {
            std::cerr << "Connection closed by server!" << std::endl;
            break;
        }
    }
}

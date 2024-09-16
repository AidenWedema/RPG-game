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

            // Respond to client
            std::string response = std::string(buffer);

            if (response == "/quit") {
                running = false;
                clientSockets.erase(std::remove(clientSockets.begin(), clientSockets.end(), clientSocket), clientSockets.end());
                break;
            }

            if (ParseCommand(response, clientSocket))
            {
                // Send the message to all connected clients
                for (auto& socket : clientSockets)
                    send(socket, response.c_str(), response.size(), 0);
            }
        }
        else {
            running = false;
            break;
        }
    }

    closesocket(clientSocket);
}

void Server::Listen()
{
    SOCKET clientSocket;
    sockaddr_in clientAddr;
    int addrLen = sizeof(clientAddr);

    while (true)
    {
        // Accept a client connection
        clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &addrLen);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Client connection failed!" << std::endl;
            continue;
        }
        clientSockets.push_back(clientSocket);

        char buffer[1024];
        string name = "Client";
        // Receive message from client
        int bytesReceived = recv(clientSocket, buffer, 1024, 0);
        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0'; // Null-terminate the received data
			name = std::string(buffer);
            name = name.substr(0, name.find("\n"));
            string rest = std::string(buffer).substr(name.length() + 1);
            while (!rest.empty())
            {
                string c = rest.substr(0, rest.find("\n"));
                rest = rest.substr(rest.find("\n") + 1);
                ParseCommand(c, clientSocket);
            }
            Game::GetInstance()->AddPlayer(new Character(name, 0, 0, 0, 0));
        }

        std::cout << name + " connected!" << std::endl;

        // Handle client communication
        std::thread clientThread(&Server::handleClient, this, clientSocket);
        clientThread.detach();
    }

    // Close server socket
    closesocket(serverSocket);

    // Cleanup
    WSACleanup();
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
    std::thread listenThread(&Server::Listen, this);
	listenThread.detach();
}

void Server::Send(string message, SOCKET clientSocket)
{
    send(clientSocket, message.c_str(), message.size(), 0);
}

bool Server::ParseCommand(string c, SOCKET clientSocket)
{
    // split c into multiple commands using "\n" as a delimiter, to prevent multiple commands from merging into one becase of network lag
    vector<string> commands;
    while (!c.empty())
    {
        commands.push_back(c.substr(0, c.find("\n")));
        c = c.substr(c.find("\n") + 1);
    }

    for (int i = 0; i < commands.size(); i++)
	{
        // splite the command into the command and the arguments, using "~" as a delimiter
        string command = commands[i].substr(0, commands[i].find("~"));
        vector<string> args;
        commands[i] = commands[i].substr(commands[i].find("~") + 1);
        while (!commands[i].empty())
        {
            args.push_back(commands[i].substr(0, commands[i].find("~")));
            commands[i] = commands[i].substr(commands[i].find("~") + 1);
        }

        if (command == "GetPlayers")
        {
            vector<Character*> players = Game::GetInstance()->GetPlayers();
            string message = "GetPlayers~";
            for (int i = 0; i < players.size(); i++)
            {
                message += players[i]->getName() + "," + to_string(get<0>(players[i]->getCoordinates())) + "," + to_string(get<1>(players[i]->getCoordinates())) + "~";
            }
            send(clientSocket, message.c_str(), message.size(), 0);
            return false;
        }
        else if (command == "GetRandomSeed")
        {
            int seed = Game::GetInstance()->GetSeed();
            string message = "GetRandomSeed~" + to_string(seed);
            send(clientSocket, message.c_str(), message.size(), 0);
            return false;
        }
	}
}

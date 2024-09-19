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
    std::vector<std::string> split;
    while (running) {
        // Receive message from client
        bytesReceived = recv(clientSocket, buffer, 1024, 0);
        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0'; // Null-terminate the received data

            // Respond to client
            std::string message = "";
            for (int i = 0; i < bytesReceived; i++)
				message += buffer[i];

            // Message Length Prefixing to prevent multiple messages merging together
            while (!message.empty())
            {
                uint32_t lenght = stoi(message.substr(0, sizeof(uint32_t)));
                split.push_back(message.substr(to_string(lenght).size(), lenght));
                message.erase(0, to_string(lenght).size() + lenght);
            }

            for (std::string response : split)
            {
                if (response == "/quit") {
                    running = false;
                    clientSockets.erase(std::remove(clientSockets.begin(), clientSockets.end(), clientSocket), clientSockets.end());
                    break;
                }

                ParseCommand(Command::FromString(response), clientSocket);
            }

            split.clear();
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
    std::cout << "Starting Server. Please wait..." << std::endl;

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

    // Perform automatic port forwarding
    if (!UPnP_PortForward(54000)) {
        std::cerr << "Automatic port forwarding failed!" << std::endl;
        //return false;
    }

    // Start listening for incoming connections
    listen(serverSocket, SOMAXCONN);

    std::cout << "Waiting for clients..." << std::endl;
    return true;
}

void Server::Run()
{
    listenThread = std::thread(&Server::Listen, this);
	listenThread.detach();
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

        // Handle client communication
        std::thread clientThread(&Server::handleClient, this, clientSocket);
        clientThread.detach();
        clientThreads.push_back(std::move(clientThread));
    }

    // Close server socket
    closesocket(serverSocket);

    // Cleanup
    WSACleanup();
}

void Server::Send(Command* cmd, SOCKET clientSocket) 
{
    std::string s = cmd->ToString();
    s = to_string(uint32_t(s.size())) + s;
    const char* c = s.data();
    send(clientSocket, c, s.size(), 0);
}

void Server::ParseCommand(Command* cmd, SOCKET clientSocket)
{
    switch (cmd->type)
    {
        case 0: { // recieve the player name
            std::string name = std::string(cmd->data);
            Game::GetInstance()->AddPlayer(new Character(name, 0, 0, 0, 0));
            Send(Command::Create(0, to_string((int)time(0))), clientSocket);
            break;
        }

        case 1: { // send the random seed
            int seed = Game::GetInstance()->GetSeed();
            Send(Command::Create(1, std::to_string(seed)), clientSocket);
            break;
        }

        case 2: { // send the players list to all clients
            std::string players = "";
            for (Character* player : Game::GetInstance()->GetPlayers())
                players += player->getName() + "~" + std::to_string(player->getID()) + "\n";
            for (SOCKET socket : clientSockets)
                Send(Command::Create(2, players), socket);
            break;
        }

        case 3: { // move player to new coordinates for all clients
            std::string data = std::string(cmd->data);
            int id = std::stoi(data.substr(0, data.find(',')));
            int x = std::stoi(data.substr(data.find(','), data.find_last_of(',')));
            int y = std::stoi(data.substr(data.find_last_of(',') + 1));
            /*for (Character* player : Game::GetInstance()->GetPlayers())
            {
                if (player->getID() != id)
                    continue;
                player->setCoordinates(make_tuple(x, y));
                break;
            }*/
            for (SOCKET socket : clientSockets)
                Send(Command::Create(3, data), socket);
            break;
        }

        case 4:
            break;
    }
    return;
}

bool Server::UPnP_PortForward(unsigned short port)
{
    struct UPNPDev* devlist;
    struct UPNPDev* dev;
    struct UPNPUrls urls;
    struct IGDdatas data;
    char lanaddr[64];  // LAN IP address
    char wanaddr[64];
    int error = 0;

    // Discover UPnP devices on the network
    devlist = upnpDiscover(2000, NULL, NULL, 0, 0, 2, &error);
    if (devlist) {
        dev = devlist;
        while (dev) {
            std::cout << "UPnP Device found: " << dev->descURL << " (" << dev->st << ")" << std::endl;
            dev = dev->pNext;
        }

        if (UPNP_GetValidIGD(devlist, &urls, &data, lanaddr, sizeof(lanaddr), wanaddr, sizeof(wanaddr)) == 1) {
            std::cout << "Found valid IGD: " << urls.controlURL << std::endl;
            std::cout << "Local LAN IP: " << lanaddr << " (use this to connect to players on the same wifi network)" << std::endl;

            char externalIP[40];
            if (UPNP_GetExternalIPAddress(urls.controlURL, data.first.servicetype, externalIP) == UPNPCOMMAND_SUCCESS) {
                if (externalIP[0]) {
                    std::cout << "External IP: " << externalIP << " (use this to connect to players over the internet)" << std::endl;
                }
                else {
                    std::cout << "Could not get external IP" << std::endl;
                }
            }
            else {
                std::cerr << "Failed to get external IP address" << std::endl;
            }

            std::string portStr = std::to_string(port);
            
            freeUPNPDevlist(devlist);

            // Request port forwarding on the given port
            if (UPNP_AddPortMapping(urls.controlURL, data.first.servicetype, portStr.c_str(), portStr.c_str(), lanaddr, "TestServer", "TCP", NULL, "0") == UPNPCOMMAND_SUCCESS) {
                std::cout << "Port " << port << " forwarded successfully!" << std::endl;
                return true;
            }
            else {
                std::cerr << "Failed to forward port " << port << std::endl;
            }
        }
        else {
            std::cerr << "No valid UPnP IGD found" << std::endl;
        }
    }
    else {
        std::cerr << "No UPnP devices found on the network" << std::endl;
    }

    return false;
}
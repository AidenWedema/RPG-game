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
    while (true) {
        // Connect to the server
        if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            std::cerr << "Connection failed!" << std::endl;
            closesocket(clientSocket);
            WSACleanup();
            continue;
        }
        break;
    }
    std::cout << "Connected to server!" << std::endl;
    Send(Game::GetInstance()->GetPlayerName());
    Send("GetRandomSeed");

    std::thread listenThread(&Client::Listen, this);
    listenThread.detach();
}

void Client::Listen()
{
    char buffer[1024];
    while (true) {
        // Receive response from server
        int bytesReceived = recv(clientSocket, buffer, 1024, 0);
        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';
            ParseCommand(std::string(buffer));
        }
        else {
            std::cerr << "Connection closed by server!" << std::endl;
            break;
        }
    }
}

void Client::Send(string message)
{
    // Send message to server
    message += "\n";
    send(clientSocket, message.c_str(), message.size(), 0);
}

void Client::ParseCommand(string c)
{
    // splite the command into the command and the arguments, using "~" as a delimiter
    string command = c.substr(0, c.find("~"));
    vector<string> args;
    c = c.substr(c.find("~") + 1);
    while (!c.empty())
    {
        args.push_back(c.substr(0, c.find("~")));
        c = c.substr(c.find("~") + 1);
    }

    Game* game = Game::GetInstance();
    if (command == "GetPlayers")
	{
        game->SetPlayers({});
        for (int i = 0; i < args.size(); i++)
		{
            string arg = args[i];
            vector<string> playerData;
            while (!arg.empty())
            {
                playerData.push_back(arg.substr(0, arg.find("~")));
                arg = arg.substr(arg.find("~") + 1);
            }
            Character* player = new Character(playerData[0], 0, 0, 0, 0);
            player->setCoordinates(make_tuple(stoi(playerData[1]), stoi(playerData[2])));
            game->AddPlayer(player);
		}
	}
    else if (command == "GetRandomSeed")
    {
        game->SetSeed(stoi(args[0]));
    }
}

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

    std::string ipAddress;

    // Prompt user for the server IP address
    while (true) {
        std::cout << "Enter server IP address to connect:\n";
        std::getline(std::cin, ipAddress);
        if (IsValidIP(ipAddress))
            break;
        system("cls");
        std::cout << "Invalid IP address. Please try again.\n";
    }

    // Set up the server address struct
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(54000);                  // Port of the server
    // Convert the IP address (127.0.0.1) to binary form using inet_pton()
    if (inet_pton(AF_INET, ipAddress.c_str(), &serverAddr.sin_addr) <= 0) {
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
            //closesocket(clientSocket);
            //WSACleanup();
            continue;
        }
        break;
    }
    std::cout << "Connected to server!" << std::endl;

    listenThread = std::thread(&Client::Listen, this);
    listenThread.detach();

    Send(Command::Create(0, Game::GetInstance()->GetPlayer()->getName()));
    Send(Command::Create(1, "GetRandomSeed"));
}

void Client::Listen()
{
    char buffer[1024];
    std::vector<std::string> split;
    while (true) {
        // Receive response from server
        int bytesReceived = recv(clientSocket, buffer, 1024, 0);
        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';

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
                ParseCommand(Command::FromString(response));

			split.clear();
        }
        else {
            std::cerr << "Connection closed by server!" << std::endl;
            break;
        }
    }
}

void Client::Send(Command* cmd)
{
    std::string s = cmd->ToString();
    s = to_string(uint32_t(s.size())) + s;
    const char* c = s.data();
    send(clientSocket, c, s.size(), 0);
}

void Client::ParseCommand(Command* cmd)
{
    switch (cmd->type)
    {
        case 0: // recieve player ID
            Game::GetInstance()->GetPlayer()->setID(std::stoi(std::string(cmd->data)));

        case 1: { // recieve the random seed, generate the world and set the current area and start the game
            Game::GetInstance()->SetSeed(std::stoi(std::string(cmd->data)));
            Game::GetInstance()->GenerateWorld();
            tuple<int, int> coordinates = make_tuple(rand() % 100, rand() % 100);
            Game::GetInstance()->SetCurrentArea(coordinates);
            Game::GetInstance()->SetMultiplayer(true);
            break;
        }

        case 2: { // recieve all players
            std::string players = std::string(cmd->data);
            Game::GetInstance()->SetPlayers({});
            while (!players.empty()) {
                std::string player = players.substr(0, players.find('\n') + 1);
                players.erase(0, players.find('\n') + 1);
                std::string name = player.substr(0, player.find('~'));
                player.erase(0, player.find('~') + 1);
                Character* newplayer = new Character(name, 0, 0, 0, 0);
                newplayer->setID(std::stoi(player));
                Game::GetInstance()->AddPlayer(newplayer);
            }
            break;
        }

        case 3: { // move player to new coordinates
            std::string data = std::string(cmd->data);
            int id = std::stoi(data.substr(0, data.find(',')));
            int x = std::stoi(data.substr(data.find(','), data.find_last_of(',')));
            int y = std::stoi(data.substr(data.find_last_of(',') + 1));
            for (Character* player : Game::GetInstance()->GetPlayers())
            {
                if (player->getID() != id)
                    continue;
                player->setCoordinates(make_tuple(x, y));
            }
            break;
        }

        case 4: { // wait until the server says 'okay'

        }
    }
}

bool Client::IsValidIP(const std::string& ipAddress) {
    struct sockaddr_in sa;
    return inet_pton(AF_INET, ipAddress.c_str(), &(sa.sin_addr)) != 0;
}
#include "Game.h"

Game::Game()
{
	srand(time(0));
	player = new Character("PLAYER", 100, 10, 0, 2);
	friends = vector<Character*>{ new Character("Jimmy", 100, 10, 0, 2) };
	battleManager = new BattleManager();
	GenerateWorld();
	currentArea = world[make_tuple(rand() % 100, rand() % 100)];
}

Game::~Game()
{
}

void Game::Start()
{
	char in[100];
	cout << "Do you want to play solo or with friends? (s/f)\n";
	cin.getline(in, 100);
	string inStr = in;
	if (inStr == "f" || inStr == "friends")
	{
		LocalPlay();
		return;
	}

	cout << "Are you ready for adventure?\n";
	cin.getline(in, 100);
	inStr = in;
	if (inStr != "y" && inStr != "yes")
	{
		cout << "Goodbye";
		return;
	}
	system("cls");

	cout << "What is your name?\n";
	cin.getline(in, 100);
	player->setName(in);
	system("cls");
	cout << "Hello " << player->getName() << "\n";
	cin.ignore();
	system("cls");

	GameLoop();
}

void Game::LocalPlay()
{
	char in[100];
	cout << "Do you want to start as a (s)erver or (c)lient? ";
	cin.getline(in, 100);
	string choice = in;
	bool invalid = true;
	while (invalid)
	{
		if (choice == "s" || choice == "server" || choice == "host") {
			Server server;
			if (server.Init()) {
				server.Run();
				invalid = false;
			}
		}
		else if (choice == "c" || choice == "client") {
			Client client;
			if (client.Init()) {
				client.Run();
				invalid = false;
			}
		}
	}
}

void Game::GameLoop()
{
	while (true)
	{
		// move to new area
		vector<string>* options = new vector<string>{"Go north", "Go east", "Go south", "Go west"};
		if (currentArea->getType() == 6)
			options->push_back("Rest at an inn");
		if (currentArea->getType() == 7)
		{
			options->push_back("Rest at an inn");
			options->push_back("Go to the tavern");
		}
		string msg = "You are ";
		switch (currentArea->getType())
		{
			case 0:
				msg += "in the ocean.\n";
				break;
			case 1:
				msg += "on the beach.\n";
				break;
			case 2:
				msg += "in the plains.\n";
				break;
			case 3:
				msg += "in a forrest.\n";
				break;
			case 4:
				msg += "in a desert.\n";
				break;
			case 5:
				msg += "on a mountain.\n";
				break;
			case 6:
				msg += "in a city.\n";
				break;
			case 7:
				msg += "in a town.\n";
				break;
		}
		msg += "Where would you like to go?";
		int in = GetPlayerInput(msg, options);
		tuple<int, int> coords = currentArea->getCoordinates();
		switch (in)
		{
			// go north
			case 0:
				MoveTo(make_tuple(get<0>(coords), get<1>(coords) + 1));
				break;
			// go east
			case 1:
				MoveTo(make_tuple(get<0>(coords) + 1, get<1>(coords)));
				break;
			// go south
			case 2:
				MoveTo(make_tuple(get<0>(coords), get<1>(coords) - 1));
				break;
			// go west
			case 3:
				MoveTo(make_tuple(get<0>(coords) - 1, get<1>(coords)));
				break;
			// heal
			case 4:
				player->setHP(100);
				break;
		}
	}
}

void Game::MoveTo(tuple<int, int> coordinates)
{
	auto it = world.find(coordinates);
	if (it != world.end())
	{
		currentArea = it->second;
		if (currentArea->getType() == 6 || currentArea->getType() == 7)
			return;
		if (currentArea->getEncounterChance() < rand() % 100 && currentArea->getEnemies().size() > 0)
			StartBattle();
		return;
	}

	cout << "You can't go there.\n";
}

void Game::StartBattle()
{
	battleManager->Setup(player, friends, currentArea->getEnemies());
	battleManager->Start();
}

void Game::GenerateWorld()
{
	for (int x = 0; x < 100; x++)
	{
		for (int y = 0; y < 100; y++)
		{
			Area* area = new Area();
			area->setCoordinates(make_tuple(x, y));
			int type = rand() % 6;
			if (type != 0 && rand() % 4 == 0)
				type = rand() % 4 ==  0 ? 6 : 7;
			area->setType(type);
			world[make_tuple(x, y)] = area;
			if (type == 6 || type == 7)
				continue;
			vector<Character*> enemies = vector<Character*>{
				new Character("Slime", 10, 1, 0, 0), 
				new Character("Goblin", 30, 5, 1, 2),
				new Character("Troll", 50, 10, 3, 2),
				new Character("Dragon", 200, 30, 5, 5),
				new Character("Orc", 100, 10, 3, 2),
				new Character("Giant", 150, 20, 3, 1),
				new Character("Golem", 100, 10, 15, 0),
				new Character("Siren", 70, 10, 0, 5),
				new Character("Sea serpent", 150, 10, 2, 3),
			};
			switch (type)
			{
				// ocean
				case 0:
					area->setEnemies(vector<Character*>{ enemies[7], enemies[8]});
					area->setEncounterChance(20);
					break;
				// beach
				case 1:
					area->setEnemies(vector<Character*>{ enemies[7], enemies[0]});
					area->setEncounterChance(15);
					break;
				// plains
				case 2:
					area->setEnemies(vector<Character*>{ enemies[0], enemies[1], enemies[2]});
					area->setEncounterChance(30);
					break;
				// forrest
				case 3:
					area->setEnemies(vector<Character*>{ enemies[0], enemies[1], enemies[5]});
					area->setEncounterChance(40);
					break;
				// desert
				case 4:
					area->setEnemies(vector<Character*>{ enemies[2], enemies[3], enemies[4], enemies[5]});
					area->setEncounterChance(20);
					break;
				// mountain
				case 5:
					area->setEnemies(vector<Character*>{ enemies[2], enemies[3], enemies[5], enemies[6]});
					area->setEncounterChance(20);
					break;
			}
		}
	}
}

int Game::GetPlayerInput(string pretext, vector<string>* options)
{
	while (true)
	{
		string msg = pretext + "\n";
		int in;
		for (int i = 0; i < options->size(); i++)
		{
			msg += to_string(i) + ": " + (*options)[i] + "\n";
		}
		cout << msg;
		try
		{
			cin >> in;
			if (in < 0 || in >= options->size())
			{
				cout << "Invalid input\n";
				cin.ignore();
				system("cls");
				continue;
			}
			return in;
		}
		catch (exception e)
		{
			cout << "Invalid input\n";
			cin.ignore();
			system("cls");
		}
	}
}

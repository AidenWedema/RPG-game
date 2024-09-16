#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <stdlib.h>
#include <algorithm>
#include <unordered_map>
#include "Character.h"
#include "Move.h"
#include "BattleManager.h"
#include "Area.h"
class Server;
class Client;

class Game
{
public:
	static Game* GetInstance();

	void Start();
	void LocalPlay();
	void GameLoop();
	void MoveTo(tuple<int, int> coordinates);
	void StartBattle();
	void GenerateWorld();
	int GetPlayerInput(string pretext, vector<string>* options);
	void AddPlayer(Character* character);
	void RemovePlayer(Character* character);
	vector<Character*> GetPlayers();
	void SetPlayers(vector<Character*> newPlayers);
	int GetSeed();
	void SetSeed(int seed);
	string GetPlayerName();

	// Hash function for tuple<int, int> to use in unordered_map
	struct tuple_hash {
		template <class T1, class T2>
		size_t operator() (const tuple<T1, T2>& tuple) const {
			auto h1 = hash<T1>{}(get<0>(tuple));
			auto h2 = hash<T2>{}(get<1>(tuple));
			return h1 ^ (h2 << 1); // Combine the two hashes
		}
	};

private:	
	Game();
	~Game();
	static Game* instance;
	Character* player;
	vector<Character*> friends;
	vector<Character*> players;
	unordered_map<tuple<int, int>, Area*, tuple_hash> world;
	Area* currentArea;
	BattleManager* battleManager;
	int randomSeed;
	Server* server;
	Client* client;
	bool multiplayer;
};


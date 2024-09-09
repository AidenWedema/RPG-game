#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <stdlib.h>
#include <algorithm>
#include "Character.h"
#include "Move.h"
#include "BattleAction.h"

class Game
{
public:
	Game();
	~Game();

	void Start();
	void Battle();
	vector<Character*> GetTurnOrder();
	tuple<Move*, Character*> GetPlayerMove();
	tuple<Move*, Character*> GetRandomMove(Character* character);
	void Win();
	void Lose();
	vector<Character*>* GetAllCharacters();

private:
	Character* player;
	vector<Character*> friends;
	vector<Character*> enemies;
	tuple<vector<Move*>*, vector<Move*>*, vector<Move*>*, vector<Move*>*> allMoves;
};


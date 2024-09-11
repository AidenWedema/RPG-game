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

class BattleManager
{
public:
	BattleManager();
	~BattleManager();

	void Setup(Character* player, vector<Character*> friends, vector<Character*> enemies);
	void Start();
	void Battle();
	vector<Character*> GetTurnOrder();
	void GetPlayerMove(BattleAction* action);
	void GetRandomMove(BattleAction* action);
	void GetPlayerTarget(BattleAction* action);
	void GetRandomTarget(BattleAction* action);
	void Win();
	void Lose();
	vector<Character*>* GetAllCharacters();

private:
	Character* player;
	vector<Character*> friends;
	vector<Character*> enemies;
	tuple<vector<Move*>*, vector<Move*>*, vector<Move*>*, vector<Move*>*> allMoves;
};


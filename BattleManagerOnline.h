#pragma once
#include "BattleManager.h"
class BattleManagerOnline : public BattleManager
{
public:
	BattleManagerOnline();
	~BattleManagerOnline();

	void AddPlayer(string name);
	void Start();
	void PlayTurns();
	void GetTurnOrder();

private:
	vector<Character*> allCharacters;
	vector<BattleAction*> actions;
};


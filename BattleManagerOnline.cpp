#include "BattleManagerOnline.h"

BattleManagerOnline::BattleManagerOnline()
{
}

BattleManagerOnline::~BattleManagerOnline()
{
}

void BattleManagerOnline::AddPlayer(string name)
{
	Character* character = new Character(name, 200, 5, 0, 20);
	this->allCharacters.push_back(character);
}

void BattleManagerOnline::Start()
{
}

void BattleManagerOnline::PlayTurns()
{

}

void BattleManagerOnline::GetTurnOrder()
{
}

#include <vector>
#include <string>
#include "Move.h"
#include "Character.h"

using namespace std;

Character::Character(string name, int hp, int atk, int def)
{
	this->name = name;
	this->hp = hp;
	this->atk = atk;
	this->def = def;
}

Character::Character(string name, int hp, int atk, int def, vector<Move*>* moves)
{
	this->name = name;
	this->hp = hp;
	this->atk = atk;
	this->def = def;
	this->moves = *moves;
}

Character::~Character()
{
}

string Character::getName()
{
	return name;
}

int Character::getHP()
{
	return hp;
}

int Character::getATK()
{
	return atk;
}

int Character::getDEF()
{
	return def;
}

vector<Move*> Character::getMoves()
{
	return moves;
}

void Character::setName(string name)
{
	this->name = name;
}

void Character::setHP(int hp)
{
	this->hp = hp;
}

void Character::setATK(int atk)
{
	this->atk = atk;
}

void Character::setDEF(int def)
{
	this->def = def;
}

void Character::setMoves(vector<Move*>* moves)
{
	this->moves = *moves;
}

void Character::addMove(Move* move)
{
	moves.push_back(move);
}

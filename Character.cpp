#include <vector>
#include <string>
#include "Move.h"
#include "Character.h"

Character::Character()
{
	this->name = "PLAYER_NAME";
	this->hp = 100;
	this->atk = 10;
	this->def = 0;
	this->spd = 0;
	this->ID = 0;
}

Character::Character(string name, int hp, int atk, int def, int spd)
{
	this->name = name;
	this->hp = hp;
	this->atk = atk;
	this->def = def;
	this->spd = spd;
	this->ID = 0;
}

Character::Character(string name, int hp, int atk, int def, int spd, vector<Move*>* moves)
{
	this->name = name;
	this->hp = hp;
	this->atk = atk;
	this->def = def;
	this->spd = spd;
	this->moves = *moves;
	this->ID = 0;
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

int Character::getSPD()
{
	return spd;
}

vector<Move*> Character::getMoves()
{
	return moves;
}

vector<tuple<string, int, int>> Character::getModifiers()
{
	return modifiers;
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

void Character::setSPD(int spd)
{
	this->spd = spd;
}

void Character::setMoves(vector<Move*>* moves)
{
	this->moves = *moves;
}

void Character::addMove(Move* move)
{
	moves.push_back(move);
}

void Character::setModifiers(vector<tuple<string, int, int>> modifiers)
{
	this->modifiers = modifiers;
}

void Character::addModifier(tuple<string, int, int> modifier)
{
	modifiers.push_back(modifier);
}

void Character::removeModifier(tuple<string, int, int> modifier)
{
	modifiers.erase(remove(modifiers.begin(), modifiers.end(), modifier), modifiers.end());
}

int Character::getID()
{
	return ID;
}

void Character::setID(int ID)
{
	this->ID = ID;
}

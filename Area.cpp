#include "Area.h"

Area::Area()
{
	coordinates = make_tuple(0, 0);
	enemies = vector<Character*>();
}

Area::Area(tuple<int, int> coordinates)
{
	this->coordinates = coordinates;
	this->type = type;
	this->encounterChance = encounterChance;
	enemies = vector<Character*>();
}

Area::~Area()
{
}

tuple<int, int> Area::getCoordinates()
{
	return coordinates;
}

vector<Character*> Area::getEnemies()
{
	return enemies;
}

int Area::getEncounterChance()
{
	return encounterChance;
}

int Area::getType()
{
	return type;
}

void Area::setCoordinates(tuple<int, int> coordinates)
{
	this->coordinates = coordinates;
}

void Area::setEnemies(vector<Character*> enemies)
{
	this->enemies = enemies;
}

void Area::setEncounterChance(int encounterChance)
{
	this->encounterChance = encounterChance;
}

void Area::setType(int type)
{
	this->type = type;
}

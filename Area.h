#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <stdlib.h>
#include "character.h"
using namespace std;
class Area
{
public:
	Area();
	Area(tuple<int, int> coordinates);
	~Area();

	tuple<int, int> getCoordinates();
	vector<Character*> getEnemies();
	int getEncounterChance();
	int getType();
	void setCoordinates(tuple<int, int> coordinates);
	void setEnemies(vector<Character*> enemies);
	void setEncounterChance(int encounterChance);
	void setType(int type);


private:
	tuple<int, int> coordinates;
	vector<Character*> enemies;
	int encounterChance;
	int type;
	enum Type
	{
		Ocean,
		Beach,
		Plains,
		Forrest,
		Desert,
		Mountain,
		City,
		Town
	};
};


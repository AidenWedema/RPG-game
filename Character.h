#include <vector>
#include <string>
#include <tuple>
#include "Move.h"

#pragma once
class Character
{
public:
	Character();
	Character(string name, int hp, int atk, int def, int spd);
	Character(string name, int hp, int atk, int def, int spd, vector<Move*>* moves);
	~Character();

	string getName();
	int getHP();
	int getATK();
	int getDEF();
	int getSPD();
	vector<Move*> getMoves();

	vector<tuple<string, int, int>*> getModifiers();

	void setName(string name);
	void setHP(int hp);
	void setATK(int atk);
	void setDEF(int def);
	void setSPD(int spd);
	void setMoves(vector<Move*>* moves);
	void addMove(Move* move);

	void setModifiers(vector<tuple<string, int, int>*> modifiers);

	void addModifier(tuple<string, int, int>* modifier);
	void removeModifier(tuple<string, int, int>* modifier);

	int getID();
	void setID(int ID);
	bool operator==(const Character* other) const {
		return this->ID == other->ID;
	}

private:
	string name;
	int hp;
	int atk;
	int def;
	int spd;
	vector<Move*> moves;
	vector<tuple<string, int, int>*> modifiers; // (modified stat, value change for stat, amount of turns left)
	int ID;
};
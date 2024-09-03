#include <vector>
#include <string>
#include "Move.h"

#pragma once
class Character
{
public:
	Character(string name, int hp, int atk, int def);
	Character(string name, int hp, int atk, int def, vector<Move*>* moves);
	~Character();

	string getName();
	int getHP();
	int getATK();
	int getDEF();
	vector<Move*> getMoves();

	void setName(string name);
	void setHP(int hp);
	void setATK(int atk);
	void setDEF(int def);
	void setMoves(vector<Move*>* moves);
	void addMove(Move* move);

private:
	string name;
	int hp;
	int atk;
	int def;
	vector<Move*> moves;
};
#include <string>
#include <vector>
#pragma once

using namespace std;
class Move
{
public:
	Move();
	Move(string name, int pow, int acc, int type, bool doesDamage);
	~Move();

	string getName();
	int getPOW();
	int getACC();
	int getType();
	bool Damaging();
	void setName(string name);
	void setPOW(int pow);
	void setACC(int acc);
	void setType(int type);
	void setDamaging(bool doesDamage);

private:

	string name;
	int pow;
	int acc;
	int type; // 0 = attack, 1 = special, 2 = magic, 3 = defense
	bool doesDamage;
};
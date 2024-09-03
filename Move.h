#include <string>
#include <vector>
#pragma once

using namespace std;
class Move
{
public:
	Move();
	Move(string name, int pow, int acc);
	~Move();

	string getName();
	int getPOW();
	int getACC();
	void setName(string name);
	void setPOW(int pow);
	void setACC(int acc);

private:

	string name;
	int pow;
	int acc;
};
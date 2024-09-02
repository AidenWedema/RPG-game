#include <string>
#include "Move.cpp"

using namespace std;
class Character
{
public:
	string name = "";
	int hp = 0;
	int atk = 0;
	Move moves[4] = { Move(), Move(), Move(), Move() };

	Character(string name, int hp, int atk);
	Character(string name, int hp, int atk, Move* moves);
};

Character::Character(string name, int hp, int atk)
{
	this->name = name;
	this->hp = hp;
	this->atk = atk;
}

Character::Character(string name, int hp, int atk, Move* moves)
{
	this->name = name;
	this->hp = hp;
	this->atk = atk;

	for (int i = 0; i < 4; i++)
	{
		this->moves[i] = moves[i];
	}
}
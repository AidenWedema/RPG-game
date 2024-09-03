#include <string>
#include "Move.h"

using namespace std;

Move::Move()
{
	this->name = "";
	this->pow = 0;
	this->acc = 0;
}

Move::Move(string name, int pow, int acc)
{
	this->name = name;
	this->pow = pow;
	this->acc = acc;
}

Move::~Move()
{
}

string Move::getName()
{
	return name;
}

int Move::getPOW()
{
	return pow;
}

int Move::getACC()
{
	return acc;
}

void Move::setName(string name)
{
	this->name = name;
}

void Move::setPOW(int pow)
{
	this->pow = pow;
}

void Move::setACC(int acc)
{
	this->acc = acc;
}

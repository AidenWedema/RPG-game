#include <string>
#include "Move.h"

Move::Move()
{
	this->name = "";
	this->pow = 0;
	this->acc = 0;
	this->type = 0;
	this->doesDamage = false;
}

Move::Move(string name, int pow, int acc, int type, bool doesDamage)
{
	this->name = name;
	this->pow = pow;
	this->acc = acc;
	this->type = type;
	this->doesDamage = doesDamage;
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

int Move::getType()
{
	return type;
}

bool Move::Damaging()
{
	return doesDamage;
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

void Move::setType(int type)
{
	this->type = type;
}

void Move::setDamaging(bool doesDamage)
{
	this->doesDamage = doesDamage;
}

#include "BattleAction.h"

BattleAction::BattleAction()
{
	move = nullptr;
	target = nullptr;
	user = nullptr;
}

BattleAction::BattleAction(Move* move, Character* target, Character* user)
{
	this->move = move;
	this->target = target;
	this->user = user;
}

BattleAction::~BattleAction()
{
}

Move* BattleAction::getMove()
{
	return move;
}

Character* BattleAction::getTarget()
{
	return target;
}

Character* BattleAction::getUser()
{
	return user;
}

void BattleAction::setMove(Move* move)
{
	this->move = move;
}

void BattleAction::setTarget(Character* target)
{
	this->target = target;
}

void BattleAction::setUser(Character* user)
{
	this->user = user;
}

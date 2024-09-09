#include "Move.h"
#include "Character.h"
#pragma once
class BattleAction
{
public:
	BattleAction();
	BattleAction(Move* move, Character* target, Character* user);
	~BattleAction();

	Move* getMove();
	Character* getTarget();
	Character* getUser();

	void setMove(Move* move);
	void setTarget(Character* target);
	void setUser(Character* user);

private:
	Move* move;
	Character* target;
	Character* user;
};


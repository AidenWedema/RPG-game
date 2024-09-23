#include "BattleManager.h"

BattleManager::BattleManager()
{
	player = new Character("PLAYER", 100, 10, 0, 0);
	friends = vector<Character*> { new Character("Jimmy", 100, 10, 0, 0) };
	enemies = vector<Character*> { new Character("Slime", 100, 10, 0, 0) };
	allMoves = make_tuple(new vector<Move*>, new vector<Move*>, new vector<Move*>, new vector<Move*>);

	vector<Character*>* all = GetAllCharacters();
	for (int i = 0; i < all->size(); i++)
		(*all)[i]->setID(i);

	// set up attack moves
	get<0>(allMoves)->push_back(new Move("Punch", 10, 95, 0, true));
	get<0>(allMoves)->push_back(new Move("Kick", 20, 80, 0, true));
	get<0>(allMoves)->push_back(new Move("JumpKick", 30, 75, 0, true));
	get<0>(allMoves)->push_back(new Move("HighJumpKick", 40, 60, 0, true));
	get<0>(allMoves)->push_back(new Move("Headbutt", 50, 40, 0, true));
	get<0>(allMoves)->push_back(new Move("Tackle", 20, 90, 0, true));
	get<0>(allMoves)->push_back(new Move("Bite", 50, 50, 0, true));
	get<0>(allMoves)->push_back(new Move("Slap", 20, 90, 0, true));
	get<0>(allMoves)->push_back(new Move("Scratch", 30, 90, 0, true));
	get<0>(allMoves)->push_back(new Move("Throw", 15, 75, 0, true));

	// set up special moves
	get<1>(allMoves)->push_back(new Move("ATK up", 10, 95, 1, false));
	get<1>(allMoves)->push_back(new Move("DEF up", 10, 95, 1, false));
	get<1>(allMoves)->push_back(new Move("SPD up", 10, 95, 1, false));
	get<1>(allMoves)->push_back(new Move("ATK down", 10, 95, 1, false));
	get<1>(allMoves)->push_back(new Move("DEF down", 10, 95, 1, false));
	get<1>(allMoves)->push_back(new Move("SPD down", 10, 95, 1, false));

	// set up magic moves
	get<2>(allMoves)->push_back(new Move("Ember", 20, 60, 2, true));
	get<2>(allMoves)->push_back(new Move("Fire", 30, 70, 2, true));
	get<2>(allMoves)->push_back(new Move("FireBall", 40, 80, 2, true));
	get<2>(allMoves)->push_back(new Move("Splash", 20, 70, 2, true));
	get<2>(allMoves)->push_back(new Move("Wave", 25, 90, 2, true));
	get<2>(allMoves)->push_back(new Move("Tsunami", 30, 100, 2, true));
	get<2>(allMoves)->push_back(new Move("Spark", 10, 80, 2, true));
	get<2>(allMoves)->push_back(new Move("Shock", 20, 60, 2, true));
	get<2>(allMoves)->push_back(new Move("Thunder", 50, 70, 2, true));
	get<2>(allMoves)->push_back(new Move("Heal", 10, 75, 2, false));
	get<2>(allMoves)->push_back(new Move("MegaHeal", 20, 75, 2, false));
	get<2>(allMoves)->push_back(new Move("GigaHeal", 40, 75, 2, false));

	// set up defence moves
	get<3>(allMoves)->push_back(new Move("Defend", 0, 100, 3, false));
}

BattleManager::~BattleManager()
{
}

void BattleManager::Setup(Character* player, vector<Character*> friends, vector<Character*> enemies)
{
	this->player = player;
	this->friends = friends;
	this->enemies.clear();
	int amount = rand() % 3 + 1;
	// copy 1 to 3 enemies to battle
	for (int i = 0; i < amount; i++)
	{
		Character* enemy = enemies[rand() % enemies.size()];
		Character* copy = new Character();
		copy->setName(enemy->getName());
		copy->setHP(enemy->getHP());
		copy->setATK(enemy->getATK());
		copy->setDEF(enemy->getDEF());
		copy->setSPD(enemy->getSPD());
		for (Move* move : enemy->getMoves())
			copy->addMove(move);
		copy->setID(100 + i);
		this->enemies.push_back(copy);
	}
}

void BattleManager::Start()
{
	system("cls");

	// assign moves to each enemy
	for (Character* character : enemies)
		AssignMoves(character);

	// assign moves to the player if they don't have any
	if (player->getMoves().size() == 0)
		AssignMoves(player);

	// assign moves to each friend if they don't have any
	for (Character* character : friends)
		if (character->getMoves().size() == 0)
			AssignMoves(character);

	Battle();
}

void BattleManager::Battle()
{
	string msg;
	for (int i = 0; i < enemies.size(); i++)
	{
		msg += enemies[i]->getName();
		if (i == enemies.size() - 2)
			msg += " and ";
		else if (i != enemies.size() - 1)
			msg += ", ";
	}
	cout << msg << " attack!\n";
	cin.clear();
	cin.ignore();
	system("cls");

	while (true)
	{
		msg = "";
		vector<BattleAction*> moves;
		// get the turn order and moves of each character in order
		vector<Character*> turnOrder = GetTurnOrder();

		// show the character's hp and modifiers
		vector<Character*> dead;
		for (Character* c : turnOrder)
		{
			// check if the character is dead
			if (c->getHP() <= 0)
			{
				if (c == player)
				{
					Lose();
					return;
				}
				if (find(friends.begin(), friends.end(), c) != friends.end())
					friends.erase(find(friends.begin(), friends.end(), c));
				if (find(enemies.begin(), enemies.end(), c) != enemies.end())
				{
					enemies.erase(find(enemies.begin(), enemies.end(), c));
					if (enemies.size() == 0)
					{
						Win();
						return;
					}
				}
				dead.push_back(c);
				continue;
			}

			// check the character for expired modifiers and remove them
			for (tuple<string, int, int>* mod : c->getModifiers())
			{
				get<2>(*mod) -= 1;
				if (get<2>(*mod) <= 0)
					c->removeModifier(mod);
			}

			msg += c->getName() + ": \nHP: " + to_string(c->getHP()) + "\n";
			for (tuple<string, int, int>* mod : c->getModifiers())
				msg += get<0>(*mod) + ": " + to_string(get<1>(*mod)) + " for " + to_string(get<2>(*mod)) + " turn(s)\n";
			msg += "\n";
		}
		for (Character* c : dead)
			turnOrder.erase(find(turnOrder.begin(), turnOrder.end(), c));

		cout << msg;
		cin.ignore();
		system("cls");
		
		// get the moves for each character
		for (Character* c : turnOrder)
		{
			BattleAction* action = new BattleAction();
			action->setUser(c);
			// get the moves and target
			if (c == player)
			{
				GetPlayerMove(action);
				GetPlayerTarget(action);
			}
			else
			{
				GetRandomMove(action);
				GetRandomTarget(action);
			}

			// add the move to the moves list
			moves.push_back(action);
		}

		// play the moves in order
		for (BattleAction* action : moves)
		{
			Move* move = action->getMove();
			Character* target = action->getTarget();
			Character* user = action->getUser();

			// check if the user is dead
			if (user->getHP() <= 0)
				continue;

			msg = action->getUser()->getName();
			switch (move->getType())
			{
				case 0: // attack
					msg += " attacks " + action->getTarget()->getName() + " with " + action->getMove()->getName();
					break;
				case 1: // special
					msg += " uses " + action->getMove()->getName() + " on " + action->getTarget()->getName();
					break;
				case 2: // magic
					msg += " casts " + action->getMove()->getName() + " on " + action->getTarget()->getName();
					break;
				case 3: // defence
					msg += " defends!";
					break;
			}
			cout << msg << "\n";

			// check if the move hits
			int hit = rand() % 100;
			if (move->getACC() < hit)
			{
				cout << move->getName() << " missed!\n";
				cin.ignore();
				continue;
			}
			// apply damage
			// check if the move is a damaging move
			if (action->getMove()->Damaging())
			{
				int damage = action->getUser()->getATK() + action->getMove()->getPOW();
				for (tuple<string, int, int>* mod : action->getUser()->getModifiers())
				{
					if (get<0>(*mod) == "ATK")
						damage += get<1>(*mod);
				}
				int def = target->getDEF();
				for (tuple<string, int, int>* mod : action->getTarget()->getModifiers())
				{
					if (get<0>(*mod) == "DEF")
						def += get<1>(*mod);
				}
				if (def > damage)
					damage = 0;
				else
					damage -= def;

				action->getTarget()->setHP(action->getTarget()->getHP() - damage);
				msg = action->getTarget()->getName() + " takes " + to_string(damage) + " damage!";
				cout << msg << "\n";
				cin.ignore();
				continue;
			}
			// check if the move is a heal
			if (action->getMove()->getName().find("Heal") != string::npos)
			{
				int damage = move->getPOW();
				msg = action->getTarget()->getName() + " heals " + to_string(damage) + " damage!";
				action->getTarget()->setHP(action->getTarget()->getHP() + damage);
				cout << msg << "\n";
				cin.ignore();
				continue;
			}
			// check if the move is of type 1 (special)
			if (action->getMove()->getType() == 1)
			{
				int mod = action->getMove()->getPOW();
				string moveName;
				if (action->getMove()->getName().find(" up") != string::npos)
				{
					moveName = action->getMove()->getName().substr(0, action->getMove()->getName().find(" up"));
					msg = action->getTarget()->getName() + "s " + moveName + " rose by " + to_string(mod) + "!";
				}
				else
				{
					moveName = action->getMove()->getName().substr(0, action->getMove()->getName().find(" down"));
					msg = action->getTarget()->getName() + "s " + moveName + " lowered by " + to_string(mod) + "!";
					mod *= -1;
				}
				tuple<string, int, int>* m = new tuple<string, int, int>;
				get<0>(*m) = moveName;
				get<1>(*m) = mod;
				get<2>(*m) = 3;
				action->getTarget()->addModifier(m);
				cout << msg << "\n";
				cin.ignore();
				continue;
			}
			// check if the move is of type 3 (defence)
			if (action->getMove()->getType() == 3)
			{
				// add a +1000 defence modifier to the target for 0 turns, IE. make them invincible this turn
				tuple<string, int, int>* m = new tuple<string, int, int>;
				get<0>(*m) = "DEF";
				get<1>(*m) = 1000;
				get<2>(*m) = 0;
				action->getTarget()->addModifier(m);
				continue;
			}
		}
		system("cls");
	}
}

vector<Character*> BattleManager::GetTurnOrder()
{
	vector<Character*>* allCharacters = GetAllCharacters();

	// apply speed modifiers
	for (Character* c : *allCharacters)
	{
		for (tuple<string, int, int>* mod : c->getModifiers())
		{
			if (get<0>(*mod) == "SPD")
				c->setSPD(c->getSPD() + get<1>(*mod));
		}
	}

	sort(allCharacters->begin(), allCharacters->end(), [](Character* a, Character* b) { return a->getSPD() > b->getSPD(); });

	// remove speed modifiers
	for (Character* c : *allCharacters)
	{
		for (tuple<string, int, int>* mod : c->getModifiers())
		{
			if (get<0>(*mod) == "SPD")
				c->setSPD(c->getSPD() - get<1>(*mod));
		}
	}

	return *allCharacters;
}

void BattleManager::GetPlayerMove(BattleAction* action)
{
	int i = 0;
	vector<Move*> playerMoves;
	Move* move = nullptr;
	while (true)
	{
		int in = 0;
		cout << "Your turn!\n0: Attack\n1: Special\n2: Magic\n3: Defend\n";
		cin >> in;
		switch (in)
		{
			// Get the player's move
			case 0:
			case 1:
			case 2:
				cout << "Your moves:\n";
				for (Move* move : player->getMoves())
				{
					Move m = *move;
					if (m.getType() == in)
					{
						playerMoves.push_back(move);
						cout << i << ": " << m.getName() << "\n";
						i++;
					}
				}
				cout << i << ": " << "back\n" << "\nWhat is your move?\n";
				cin >> in;
				try
				{
					if (in == i)
					{
						cin.ignore();
						system("cls");
						break;
					}

					move = playerMoves[in];
					system("cls");
					action->setMove(move);
					return;
				}
				catch (const exception& e)
				{
					cout << "Invalid input" << "\n";
					cin.ignore();
					system("cls");
				}
				break;

			case 3:
				move = (*get<3>(allMoves))[0];
				action->setMove(move);
				action->setTarget(player);
				return;

			// default try again
			default:
				cout << "Invalid input" << "\n";
				cin.ignore();
				system("cls");
				break;
		}
	}
}

void BattleManager::GetRandomMove(BattleAction* action)
{
	// get a random move
	int in = rand() % action->getUser()->getMoves().size();
	Move* move = action->getUser()->getMoves()[in];
	action->setMove(move);
}

void BattleManager::GetPlayerTarget(BattleAction* action)
{
	// Check if the player used defend
	if (action->getMove()->getType() == 3)
	{
		action->setTarget(player);
		return;
	}
	int in;
	Character* target = nullptr;
	vector<Character*>* targets = GetAllCharacters();
	cout << "Use " << action->getMove()->getName() << " on whom?\n";
	for (int i = 0; i < targets->size(); i++)
		cout << to_string(i) + ": " << (*targets)[i]->getName() << "\n";
	cin >> in;
	target = (*targets)[in];

	action->setTarget(target);
}

void BattleManager::GetRandomTarget(BattleAction* action)
{
	Character* target = nullptr;

	vector<Character*> targets;
	if (action->getMove()->Damaging())
	{
		int damage;
		damage = action->getUser()->getATK() + action->getMove()->getPOW();
		// check if character is a friend or enemy
		if (find(friends.begin(), friends.end(), action->getUser()) != friends.end())
		{
			for (Character* e : enemies)
				targets.push_back(e);
		}
		else
		{
			targets.push_back(player);
			for (Character* f : friends)
				targets.push_back(f);
		}
		// check all characters to see if the attack can kill the target
		for (Character* t : targets)
		{
			if (t->getHP() - damage <= 0)
				target = t;
		}
		if (target == nullptr)
			target = targets[rand() % targets.size()];
	}
	// check if the move is a heal or a special buff
	else if (action->getMove()->getName().find("Heal") != string::npos || (action->getMove()->getType() == 1 && action->getMove()->getName().find(" up") != string::npos))
	{
		// check if character is a friend or enemy
		if (find(friends.begin(), friends.end(), action->getUser()) != friends.end())
		{
			targets.push_back(player);
			for (Character* f : friends)
				targets.push_back(f);
		}
		else
		{
			for (Character* e : enemies)
				targets.push_back(e);
		}
		int r = rand() % targets.size();
		target = targets[r];
	}
	// check if the move is a special debuff
	else if (action->getMove()->getType() == 1 && action->getMove()->getName().find(" down") != string::npos)
	{
		// check if character is a friend or enemy
		if (find(friends.begin(), friends.end(), action->getUser()) != friends.end())
		{
			for (Character* e : enemies)
				targets.push_back(e);
		}
		else
		{
			targets.push_back(player);
			for (Character* f : friends)
				targets.push_back(f);
		}
		int r = rand() % targets.size();
		target = targets[r];
	}
	// check if the move is of type 3 (defence)
	else if (action->getMove()->getType() == 3)
		target = action->getUser();

	if (target == nullptr)
	{
		cout << "Target remained null for some reason.\nMove: " << action->getMove()->getName() << "\ndamaging: " << action->getMove()->Damaging() << "\nType: " << action->getMove()->getType() << "\n";
		cin.ignore();
	}
	
	action->setTarget(target);
}

void BattleManager::AssignMoves(Character* character)
{
	vector<int> moveIndexes;
	// get 4 random attack moves
	for (int i = 0; i < 4; i++)
	{
		int r = rand() % get<0>(allMoves)->size();
		if (moveIndexes.size() == 0 || find(moveIndexes.begin(), moveIndexes.end(), r) == moveIndexes.end())
		{
			character->addMove((*get<0>(allMoves))[r]);
			moveIndexes.push_back(r);
		}
		else
			i--;
	}
	// add 3 random special moves
	moveIndexes.clear();
	for (int i = 0; i < 3; i++)
	{
		int r = rand() % get<1>(allMoves)->size();
		if (moveIndexes.size() == 0 || find(moveIndexes.begin(), moveIndexes.end(), r) == moveIndexes.end())
		{
			character->addMove((*get<1>(allMoves))[r]);
			moveIndexes.push_back(r);
		}
		else
			i--;
	}
	// add 3 random magic moves
	moveIndexes.clear();
	for (int i = 0; i < 3; i++)
	{
		int r = rand() % get<2>(allMoves)->size();
		if (moveIndexes.size() == 0 || find(moveIndexes.begin(), moveIndexes.end(), r) == moveIndexes.end())
		{
			character->addMove((*get<2>(allMoves))[r]);
			moveIndexes.push_back(r);
		}
		else
			i--;
	}
	// add the defend move
	character->addMove((*get<3>(allMoves))[0]);
}

void BattleManager::Win()
{
	cout << "You won!\n";
	player->setModifiers({});
	cin.ignore();
	system("cls");
}

void BattleManager::Lose()
{
	cout << "You blacked out!\n";
	player->setHP(1);
	player->setModifiers({});
	cin.ignore();
	system("cls");
}

vector<Character*>* BattleManager::GetAllCharacters()
{
	vector<Character*>* allCharacters = new vector<Character*>();

	allCharacters->push_back(player);
	for (Character* f : friends)
		allCharacters->push_back(f);
	for (Character* e : enemies)
		allCharacters->push_back(e);

	return allCharacters;
}

#include "Game.h"

Game::Game()
{
	player = Character("", 100, 10, 0, 0);
	friends = vector<Character> { Character("Jimmy", 100, 10, 0, 0) };
	enemies = vector<Character> { Character("Slime", 100, 10, 0, 0), Character("Slime", 100, 10, 0, 0) };
	allMoves = tuple<vector<Move>, vector<Move>, vector<Move>, vector<Move>>();

	// set up attack moves
	get<0>(allMoves).push_back(Move("Punch", 10, 95, 0, true));
	get<0>(allMoves).push_back(Move("Kick", 20, 80, 0, true));
	get<0>(allMoves).push_back(Move("JumpKick", 30, 75, 0, true));
	get<0>(allMoves).push_back(Move("HighJumpKick", 40, 60, 0, true));
	get<0>(allMoves).push_back(Move("Headbutt", 50, 40, 0, true));
	get<0>(allMoves).push_back(Move("Tackle", 20, 90, 0, true));
	get<0>(allMoves).push_back(Move("Bite", 50, 50, 0, true));
	get<0>(allMoves).push_back(Move("Slap", 20, 90, 0, true));
	get<0>(allMoves).push_back(Move("Scratch", 30, 90, 0, true));
	get<0>(allMoves).push_back(Move("Throw", 15, 75, 0, true));

	// set up special moves
	get<1>(allMoves).push_back(Move("ATK up", 10, 95, 1, false));
	get<1>(allMoves).push_back(Move("DEF up", 10, 95, 1, false));
	get<1>(allMoves).push_back(Move("SPD up", 10, 95, 1, false));
	get<1>(allMoves).push_back(Move("ATK down", 10, 95, 1, false));
	get<1>(allMoves).push_back(Move("DEF down", 10, 95, 1, false));
	get<1>(allMoves).push_back(Move("SPD down", 10, 95, 1, false));

	// set up magic moves
	get<2>(allMoves).push_back(Move("Ember", 20, 60, 2, true));
	get<2>(allMoves).push_back(Move("Fire", 30, 70, 2, true));
	get<2>(allMoves).push_back(Move("FireBall", 40, 80, 2, true));
	get<2>(allMoves).push_back(Move("Splash", 20, 60, 2, true));
	get<2>(allMoves).push_back(Move("Wave", 20, 60, 2, true));
	get<2>(allMoves).push_back(Move("Tsunami", 20, 60, 2, true));
	get<2>(allMoves).push_back(Move("Spark", 20, 60, 2, true));
	get<2>(allMoves).push_back(Move("Shock", 20, 60, 2, true));
	get<2>(allMoves).push_back(Move("Thunder", 20, 60, 2, true));
	get<2>(allMoves).push_back(Move("Heal", 10, 60, 2, false));
	get<2>(allMoves).push_back(Move("MegaHeal", 20, 60, 2, false));
	get<2>(allMoves).push_back(Move("GigaHeal", 40, 60, 2, false));

	// set up defence moves
	get<3>(allMoves).push_back(Move("Defend", 0, 0, 3, false));
}

Game::~Game()
{
}

void Game::Start()
{
	srand(time(0));
	cout << "Are you ready for adventure?\n";
	string in;
	cin >> in;
	if (in != "y" || in != "yes")
	{
		cout << "Goodbye";
		return;
	}
	Battle();
}

void Game::Battle()
{
	vector<tuple<Move*, Character*, Character*>> moves; // stores the moves in order = (move, target, user)
	while (true)
	{
		// get the turn order and moves of each character in order
		vector<Character*> turnOrder = GetTurnOrder();
		for (Character* c : turnOrder)
		{
			// get the moves for the character
			tuple<Move*, Character*> move;
			if (c == &player)
				move = GetPlayerMove();
			else
				move = GetRandomMove(c);

			// add the move to the moves list
			tuple<Move*, Character*, Character*> m;
			get<0>(m) = get<0>(move);
			get<1>(m) = get<1>(move);
			get<2>(m) = c;
			moves.push_back(m);

			// check the character for expired modifiers and remove them
			for (tuple<string, int, int> mod : c->getModifiers())
			{
				get<2>(mod) -= 1;
				if (get<2>(mod) <= 0)
					c->removeModifier(mod);
			}
		}

		// play the moves in order
		for (tuple<Move*, Character*, Character*> m : moves)
		{
			Move* move = get<0>(m);
			Character* target = get<1>(m);
			Character* user = get<2>(m);

			string msg = user->getName();
			switch (move->getType())
			{
				case 0: // attack
					msg += " attacks " + target->getName() + " with " + move->getName();
					break;
				case 1: // special
					msg += " uses " + move->getName() + " on " + target->getName();
					break;
				case 2: // magic
					msg += " casts " + move->getName() + " on " + target->getName();
					break;
				case 3: // defence
					msg += " defends";
					break;
			}
			cout << msg << "\n";

			// check if the move hits
			int hit = rand() % 100;
			if (move->getACC() < hit)
			{
				cout << move->getName() << " missed!\n";
				continue;
			}
			// apply damage
			// check if the move is a damaging move
			if (move->Damaging())
			{
				int damage = user->getATK() + move->getPOW();
				for (tuple<string, int, int> mod : user->getModifiers())
				{
					if (get<0>(mod) == "ATK")
						damage += get<1>(mod);
				}
				int def = target->getDEF();
				for (tuple<string, int, int> mod : target->getModifiers())
				{
					if (get<0>(mod) == "DEF")
						def += get<1>(mod);
				}
				if (def > damage)
					damage = 0;
				else
					damage -= def;

				target->setHP(target->getHP() - damage);
				msg = target->getName() + " takes " + to_string(damage) + " damage!";
				continue;
			}
			// check if the move is a heal
			if (move->getName().find("Heal") != string::npos)
			{
				int damage = move->getPOW();
				msg = target->getName() + " heals " + to_string(damage) + " damage!";
				target->setHP(target->getHP() + damage);
				continue;
			}
			// check if the move is of type 1 (special)
			if (move->getType() == 1)
			{
				int mod = move->getPOW();
				string moveName;
				if (move->getName().find(" up") != string::npos)
				{
					moveName = move->getName().substr(0, move->getName().find(" up"));
					msg = target->getName() + "s " + moveName + " rose by " + to_string(mod) + "!";
				}
				else
				{
					moveName = move->getName().substr(0, move->getName().find(" down"));
					msg = target->getName() + "s " + moveName + " lowered by " + to_string(mod) + "!";
				}
				target->addModifier(make_tuple(moveName, mod, 3));
				cout << msg << "\n";
				continue;
			}
			// check if the move is of type 3 (defence)
			if (move->getType() == 3)
			{
				// add a +1000 defence modifier to the target for 0 turns, IE. make them invincible this turn
				target->addModifier(make_tuple("DEF", 1000, 0));
				msg = target->getName() + " defends!\n";
				continue;
			}
		}
	}
}

vector<Character*> Game::GetTurnOrder()
{
	vector<Character*> allCharacters;

	allCharacters.push_back(&player);
	for (Character f : friends)
		allCharacters.push_back(&f);
	for (Character e : enemies)
		allCharacters.push_back(&e);

	// apply speed modifiers
	for (Character* c : allCharacters)
	{
		for (tuple<string, int, int> mod : c->getModifiers())
		{
			if (get<0>(mod) == "SPD")
				c->setSPD(c->getSPD() + get<1>(mod));
		}
	}

	sort(allCharacters.begin(), allCharacters.end(), [](Character* a, Character* b) { return a->getSPD() > b->getSPD(); });

	// remove speed modifiers
	for (Character* c : allCharacters)
	{
		for (tuple<string, int, int> mod : c->getModifiers())
		{
			if (get<0>(mod) == "SPD")
				c->setSPD(c->getSPD() - get<1>(mod));
		}
	}

	return allCharacters;
}

tuple<Move*, Character*> Game::GetPlayerMove()
{
	int in;
	int i = 0;
	vector<Move*> playerMoves;
	Move* move = nullptr;
	Character* target = nullptr;
	while (true)
	{
		cout << "Your turn!\n0: Attack\n1: Special\n2: Magic\n3: Defend\n";
		cin >> in;
		switch (in)
		{
			// Get the player's move
			case 0:
			case 1:
			case 2:
			case 3:
				cout << "Your moves:\n";
				for (Move* move : player.getMoves())
				{
					Move m = *move;
					if (m.getType() == in)
					{
						playerMoves.push_back(move);
						cout << i << ": " << m.getName() << "\n";
						i++;
					}
				}
				cout << "\nWhat is your move?\n";
				cin >> in;
				try
				{
					move = playerMoves[in];
					system("cls");

					// get the target
					i = 1;
					vector<Character*> targets;
					cout << "Use " << move->getName() << " on whom?\n";
					cout << "0: " << player.getName() << "\n";
					targets.push_back(&player);
					for (Character f : friends)
					{
						cout << i + ": " << f.getName() << "\n";
						targets.push_back(&f);
						i++;
					}
					for (Character e : enemies)
					{
						cout << i + ": " << e.getName() << "\n";
						targets.push_back(&e);
						i++;
					}
					cin >> in;
					target = targets[in];
					return make_tuple(move, target);
				}
				catch (const exception& e)
				{
					cout << "Invalid input" << "\n";
					cin.get();
					system("cls");
				}
				break;

			// default try again
			default:
				cout << "Invalid input" << "\n";
				cin.get();
				system("cls");
				break;
		}
	}
}

tuple<Move*, Character*> Game::GetRandomMove(Character* character)
{
	// get a random move
	int in = rand() % 4;
	Move* move = character->getMoves()[in];

	// get all targets
	vector<Character*> targets;
	targets.push_back(&player);
	for (Character f : friends)
		targets.push_back(&f);
	for (Character e : enemies)
		targets.push_back(&e);

	// check duplicate targets and remove them
	sort(targets.begin(), targets.end());
	auto it = unique(targets.begin(), targets.end());
	targets.erase(it, targets.end());

	// get a random target
	int in = rand() % targets.size();
	Character* target = targets[in];
	return tuple<Move*, Character*>(move, character);
}

void Game::Win()
{
	cout << "You won!\n";
}

void Game::Lose()
{
	cout << "You died!\n";
}

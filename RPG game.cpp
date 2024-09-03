#include <iostream>
#include <string>
#include <vector>
#include "Character.h"
#include "Move.h"

using namespace std;

void Start();
Move* GetPlayerMove();
Move* GetEnemyMove();
void Battle();
void Win();
void Lose();

Character player = Character("", 100, 10, 0);
Character enemy = Character("Slime", 100, 10, 0);
vector<Move> allMoves;


int main()
{
	allMoves.push_back(Move("Punch", 10, 95));
	allMoves.push_back(Move("Kick", 20, 80));
	allMoves.push_back(Move("JumpKick", 30, 75));
	allMoves.push_back(Move("HighJumpKick", 40, 60));
	allMoves.push_back(Move("Headbutt", 50, 40));
	allMoves.push_back(Move("Tackle", 20, 90));
	allMoves.push_back(Move("Bite", 50, 50));
	allMoves.push_back(Move("Slap", 20, 90));
	allMoves.push_back(Move("Scratch", 30, 90));
	allMoves.push_back(Move("Throw", 15, 75));

	srand(time(0));
    cout << "Are you ready for adventure?\n";
	string in;
	cin >> in;
	if (in == "y" || in == "yes")
		Start();
	else
		cout << "Goodbye";
}

void Start()
{
	cout << "What is your name?\n";
	string name;
	cin >> name;
	player.setName(name);
	cout << "Hello " << player.getName() << "\n";
	cin;
	cout << "A " << enemy.getName() << " attacks!\n";

	for (int i = 0; i < 8; i++)
	{
		int in = rand() % allMoves.size();
		Move* m = &allMoves[in];
		if (i % 2 == 0)
			player.addMove(m);
		else
			enemy.addMove(m);
	}

	Battle();
}

Move* GetPlayerMove()
{
	int in;
	int i = 0;
	cout << "Your turn!\n0: Attack\n1: Defend\n";
	cin >> in;
	switch (in)
	{
		// Attack
		case 0:
			cout << "Your moves:\n";
			for (Move* move : player.getMoves())
			{
				Move m = *move;
				cout << i << ": " << m.getName() << "\n";
				i++;
			}
			cout << "\nWhat is your move?\n";
			cin >> in;
			try
			{
				Move* m = player.getMoves()[in];
				return m;
			}
			catch (const exception& e)
			{
				cout << "Invalid input" << "\n";
				return GetPlayerMove();
			}
			break;

		// Defend
		case 1:
			break;

		// default try again
		default:
			cout << "Invalid input" << "\n";
			return GetPlayerMove();
			break;
	}
}

Move* GetEnemyMove()
{
	int in = rand() % 4;
	Move* m = enemy.getMoves()[in];
	return m;
}

void Battle()
{
	Move* playermove;
	Move* enemymove;
	int damage;
	while (true)
	{
		cout << "Your HP: " << player.getHP() << "\n" << enemy.getName() << " HP: " << enemy.getHP() << "\n";

		enemymove = GetEnemyMove();
		cout << enemy.getName() << " uses " << enemymove->getName() << "!\n";
		damage = enemy.getATK() + enemymove->getPOW();
		cout << player.getName() << " takes " << damage << " damage!\n";
		player.setHP(player.getHP() - damage);
		if (player.getHP() <= 0)
		{
			Lose();
			break;
		}

		playermove = GetPlayerMove();
		cout << player.getName() << " uses " << playermove->getName() << "!\n";
		damage = player.getATK() + playermove->getPOW();
		cout << enemy.getName() << " takes " << damage << " damage!\n";
		enemy.setHP(enemy.getHP() - damage);
		if (enemy.getHP() <= 0)
		{
			Win();
			break;
		}
	}
}

void Win()
{
	cout << "You won!\n";
}

void Lose()
{
	cout << "You died!\n";
}
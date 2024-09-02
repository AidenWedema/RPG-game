#include <iostream>
#include <string>
#include <random>
#include "Character.cpp"

using namespace std;

void Start();
Move GetPlayerMove();
Move GetEnemyMove();
void Battle();
void Win();
void Lose();

Character player = Character("", 100, 10);
Character enemy = Character("Slime", 100, 10);


int main()
{
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
	player.name = name;
	cout << "Hello " << player.name << "\n";
	cin;
	cout << "A " << enemy.name << " attacks!";

	Battle();
}

Move GetPlayerMove()
{
	cout << "Your moves:\n";
	int i = 0;
	for (Move move : player.moves)
	{
		cout << i << ": " << move.name << "\n";
		i++;
	}
	cout << "\nWhat is your move?\n";
	int in;
	cin >> in;
	try
	{
		return player.moves[in];
	}
	catch (const exception& e)
	{
		cout << "Invalid input" << "\n";
		return GetPlayerMove();
	}
}

Move GetEnemyMove()
{
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dis(0, 3);
	int in = dis(gen);
	return enemy.moves[in];
}

void Battle()
{
	Move playermove;
	Move enemymove;
	while (true)
	{
		enemymove = GetEnemyMove();
		if (player.hp <= 0)
			Lose();
		enemymove = GetPlayerMove();
		if (enemy.hp <= 0)
			Win();
	}
}

void Win()
{

}

void Lose()
{

}
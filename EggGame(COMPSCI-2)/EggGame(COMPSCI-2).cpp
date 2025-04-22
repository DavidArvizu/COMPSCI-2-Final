
// EggGame(COMPSCI-2).cpp : This file contains the 'main' function. Program execution begins and ends there.
//Programmer: David Arvizu Gutierrez
//COMPSCI-2
//Professor: 

#include <iostream>
#include "Game.h"

using namespace sf;
using namespace std;

int main()
{
	//Initializes random seed
	srand(static_cast<unsigned>(time(NULL)));
	//Creates game object
	Game game;
	//Runs game
	while (game.isOpen())
	{
		if (!game.getEndGame())
		{
			game.update();
			game.render();
		}
		else
		{
			game.renderLeaderboard();
		}
	}
	return 0;
}
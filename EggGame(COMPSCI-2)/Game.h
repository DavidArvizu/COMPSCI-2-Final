#pragma once

#include <SFML/Graphics.hpp>	
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

#include <vector>
#include <ctime>
#include <sstream>
#include <fstream>

using namespace sf;
using namespace std;

class Game
{
public:
	//Constructor and Destructor
	Game();
	~Game();

	void update();
	void PollEvents();
	void render();

	//Checks for open window
	const bool isOpen() const;
	const bool getEndGame() const;

	//Mouse Position relative to window
	void updateMousePosition();

	//Everything about enemies
	void updateEnemies();
	void renderEnemies(RenderTarget& target);
	void spawnEnemies();

	//Texts functions
	void renderText();
	void updateText();

	//Leaderboard functions
	void saveScore(string player, int score);
	void updateLeaderboard();
	void renderLeaderboard();
	void initializeLeaderBoard();

private:
	//Window variables and events
	RenderWindow* window;
	Event* event;

	//Mouse position
    private:
 
	Vector2i mousePositionWindow;
	Vector2f mousePositionView;

	//Private functions
	void intializeWindow();
	void initializeVariables();
	void initializeEnemies();
	void initializeFonts();
	void intializeText();

	//Game logic variables
	bool endGame;
	unsigned points;
	float enemySpawnTimer;
	float enemySpawnTimerMax;
	int health;
	int maxEnemies;
	bool mouseHeld;

	//Game objects
	vector<RectangleShape> enemies;
	RectangleShape enemy;

	//Leaderboard Variables
	int scores[5];
	string names[5];
	int score;
	string name;
	Text leaderboardText{ font, "Leaderboard", 40 };

	//txt File variables
	ofstream fileOut;
	ifstream fileIn;

	//txt File 
	string fileName = "Leaderboard.txt";

	//Resources
	Font font;

	// Text needs to be initialized in the constructor
	Text uiText{ font,"Points: 0", 24};
};


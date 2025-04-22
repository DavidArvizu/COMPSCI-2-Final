#include "Game.h"
#include <iostream>	

using namespace std;
using namespace sf;

void Game::initializeVariables()
{
	//Initializes window and event variables
	window = nullptr;

	//Initializes game logic variables
	this->points = 0;
	this->enemySpawnTimerMax = 25.f;
	this->enemySpawnTimer = this->enemySpawnTimerMax;//Sets spawn timer to max then resets to zero
	this->maxEnemies = 10;
	this->mouseHeld = false;
	this->health = 10;
	this->endGame = false;
}

Game::Game()
{
	//Initializes window, events, game objects(enemies),text
	this->initializeVariables();
	this->intializeWindow();
	this->initializeEnemies();
	this->initializeFonts();
	this->intializeText();
	this->initializeLeaderBoard();
}
Game::~Game()
{
	//Deconstructs window
	delete this->window;
}

void Game::PollEvents()
{
	//Creates an event to close window when clicked
	while (const optional event = this->window->pollEvent())
	{
		if (event->is<sf::Event::Closed>())
			this->window->close();
	}
}
void Game::update()
{
	this->PollEvents();

	//Checks if health bar drops below zero
	if (!this->endGame)
	{
		this->updateMousePosition();

		this->updateText();

		this->updateEnemies();
	}
	//endGame condition
	if (this->health <= 0)
	{
		this->endGame = true;

		string playerName = "Player";

		this->saveScore(playerName, this->points);

		this->updateLeaderboard();
	}
}
void Game::render()
{
	this->window->clear();

	//Draws enemies 
	this->renderEnemies(*this->window);
	//Draws points text
	this->renderText();

	this->window->display();
}
void Game::intializeWindow()
{
	//Creates window of specific size inside the given window reference
	this->window = new RenderWindow(VideoMode({800,600}), "Egg Skipping");
	//sets frame limit
	this->window->setFramerateLimit(60);
}

const bool Game::isOpen() const
{
	//Returns true for open window
	return this->window-> isOpen();
}
//Boolean funtion used to check if game should end(healthbar dropped to zero)
const bool Game::getEndGame() const
{
	while (const optional event = this->window->pollEvent())
	{
		if (event->is<sf::Event::Closed>())
			this->window->close();
	}

	return this->endGame;
}

void Game::updateMousePosition()
{
	//Updates position of mouse, as an integer, relative to the game window 
	this->mousePositionWindow = Mouse::getPosition(*this->window);
	//Updates postions of mouse as a float, used to check is mouse is over an enemy
	this->mousePositionView = this->window->mapPixelToCoords(this->mousePositionWindow);
}
//Creates default look of enemies
void Game::initializeEnemies()
{
	this->enemy.setSize(Vector2f(100.f, 100.f));
	this->enemy.setScale(Vector2f(0.5f, 0.5f));
	this->enemy.setFillColor(Color::Green);
}
//Sets desired font from project file
void Game::initializeFonts()
{
	if (!this->font.openFromFile("Fonts/BungeeSpice-Regular.ttf"))
	{
		cout << "ERROR::GAME::INITIALIZEFONTS::Could not load font" << endl;
	}
}
void Game::intializeText()
{
	this->uiText.setFont(this->font);
}

void Game::renderText()
{
	this->window->draw(this->uiText);
}
void Game::updateText()
{
	stringstream UI;
	UI << "Points: " << this->points << endl
		<< "Health: " << this->health << endl;

	this->uiText.setString(UI.str());
}
//Spawns enemies of specifi colors at random positions given the window size
void Game::spawnEnemies()
{
	this->enemy.setPosition(Vector2f(rand() % static_cast<int>(this->window->getSize().x - this->enemy.getSize().x), 0.f));

	//Randomize color of enemies
	int type = rand() % 5;
	switch (type)
	{
	case 0:
		this->enemy.setSize(Vector2f(50.f, 50.f));
		this->enemy.setFillColor(Color::Magenta);
		break;
	case 1:
		this->enemy.setSize(Vector2f(65.f, 65.f));
		this->enemy.setFillColor(Color::Red);
		break;
	case 2:
		this->enemy.setSize(Vector2f(75.f, 75.f));
		this->enemy.setFillColor(Color::Blue);
		break;
	case 3:
		this->enemy.setSize(Vector2f(90.f, 90.f));
		this->enemy.setFillColor(Color::Cyan);
		break;
	case 4:
		this->enemy.setSize(Vector2f(105.f, 105.f));
		this->enemy.setFillColor(Color::Green);
		break;
	default:
		this->enemy.setSize(Vector2f(120.f, 120.f));
		this->enemy.setFillColor(Color::Yellow);
		break;
	}
	//Saves number of enemies in enemies vector(used to check max amount of enemies at a given time)
	this->enemies.push_back(this->enemy);
}
//Check everything about enemies;from spawning to deleting if at bottom of window
void Game::updateEnemies()
{
	//Checks if enemy size is less than max allowed
	if (this->enemies.size() < this->maxEnemies)
	{
		//If not max, it spawns an enemy and resets the spawn timer to zero
		if (this->enemySpawnTimer >= this->enemySpawnTimerMax)
		{
			this->spawnEnemies();
			this->enemySpawnTimer = 0.f;
		}
		else
			this->enemySpawnTimer += 1.f;
	}
	//Moves enemies downwards and deletes if they go off screen
	for (int i = 0; i < this->enemies.size(); i++)
	{
		bool deleted = false;

		this->enemies[i].move(Vector2f(0.f, 5.f));

		if (this->enemies[i].getPosition().y > this->window->getSize().y)
		{
			this->enemies.erase(this->enemies.begin() + i);
			this->health -= 1;
			cout << "Health: " << this->health << endl;
		}
	}
	//Check if user clicked enemy and deletes it
	if (Mouse::isButtonPressed(Mouse::Button::Left))
	{
		//Checks if mouse is being held down
		if (this->mouseHeld == false)
		{
			//Then sets the mouseHeld variable true if clicked
			this->mouseHeld = true;
			bool deleted = false;
			for (size_t i = 0; i < this->enemies.size() && deleted == false; i++)
			{
				if (this->enemies[i].getGlobalBounds().contains(this->mousePositionView))
				{
					//gains points when kills enemy based on color
					if (this->enemies[i].getFillColor() == Color::Magenta)
					{
						this->points += 15;
					}
					else if (this->enemies[i].getFillColor() == Color::Red)
					{
						this->points += 10;
					}
					else if (this->enemies[i].getFillColor() == Color::Blue)
					{
						this->points += 5;
					}
					else if (this->enemies[i].getFillColor() == Color::Cyan)
					{
						this->points += 2;
					}
					else if (this->enemies[i].getFillColor() == Color::Green)
					{
						this->points += 1;
					}
					this->points += 1.f;
					cout << "Points: " << this->points << endl;

					//deletes enemy if mouse clicks and ends loop
					deleted = true;
					this->enemies.erase(this->enemies.begin() + i);
				}

			}
		}
	}
	//If mouse is not being held, it resets the mouseHeld variable
	else
	{
		this->mouseHeld = false;
	}
}
//Renders enemies into the window
void Game::renderEnemies(RenderTarget& target)
{
	//Moves enemies once they reach the bottom of scree
	for (auto &e : this->enemies)
	{
		target.draw(e);
	}
}

void Game::saveScore(string player, int score)
{
	//Creates new value to save current score but does not save it into file unless it is greater than the lower score
	this->scores[5] = score;
	this->names[5] = player;
	//Bubble sort to sawp the new score for current game with the lower scores
	for (int i = 5; i > 0; i--)
	{
		if (this->scores[i] > this->scores[i - 1])
		{
			swap(this->scores[i], this->scores[i - 1]);
			swap(this->names[i], this->names[i - 1]);
		}
	}

	ofstream outFile(this->fileName);
	if (outFile.is_open())
	{
		for (int i = 0; i < 5; i++)
		{
			outFile << this->scores[i] << " " << this->names[i] << endl;
		}
	}
}
//Saves scores into the txt file and closes it
void Game::updateLeaderboard()
{
	ifstream inFile(this->fileName);
	if (inFile.is_open())
	{
		for (int i = 0; i < 5; i++)
		{
			inFile >> this->scores[i] >> this->names[i];
		}
		inFile.close();
	}
}

void Game::initializeLeaderBoard()
{
	//Checks file and loads scores into the arrays
	ifstream inFile(this->fileName);
	if (inFile)
	{
		for (int i = 0; i < 5; i++)
		{
			inFile >> this->scores[i] >> this->names[i];

			if (inFile.fail())
			{
				this->scores[i] = 0;
				this->names[i] = "Player";
			}
		}
	}
	//If file does not load propperly, it sets deafult values for it
	else
	{
		for (int i = 0; i < 5; i++)
		{
			this->scores[i] = 0;
			this->names[i] = "Player";
		}
	}
	//Stringstream to store leaderboard text
	stringstream Leaderboard;
	//Sets to what the leaderboard will look like
	Leaderboard << "Leaderboard: " << endl
		<< "----------------" << endl
		<< "1. " << this->names[0] << " : " << this->scores[0] << endl
		<< "2. " << this->names[1] << " : " << this->scores[1] << endl
		<< "3. " << this->names[2] << " : " << this->scores[2] << endl
		<< "4. " << this->names[3] << " : " << this->scores[3] << endl
		<< "5. " << this->names[4] << " : " << this->scores[4] << endl
		<< "----------------" << endl;
	//Sets the leaderboard text to the stringstream
	this->leaderboardText.setString(Leaderboard.str());
	
}
//Renders the leaderboard
void Game::renderLeaderboard()
{
	this->window->clear();

	this->window->draw(this->leaderboardText);

	this->window->display();
}
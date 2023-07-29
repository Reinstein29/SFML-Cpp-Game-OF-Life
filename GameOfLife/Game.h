#pragma once

#include <iostream>
#include <vector>
#include <ctime>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>

class Game
{
private:
	sf::RenderWindow* window;
	sf::VideoMode videoMode;
	sf::Event ev;

	std::vector<std::vector<int>> board; // current iteration
	std::vector<std::vector<int>> nextBoard; // next iteration
	// dimensions of the board
	int boardWidth; 
	int boardHeight;
	int cellSize; // one size of each cell in px.

	bool paused; // pausing the game

	// colors of dead and alive cells. by default black and white.
	sf::Color deadCellColor;
	sf::Color aliveCellColor;

	sf::Vector2f mousePos; // mouse position rel. window
	bool mouseHeld;

	float timer; // every time timer reaches updateRate, the board updates
	sf::Clock deltaClock;
	float updateRate; // time to next iteration in ms. by default 250 ms = 0.25seconds;

	void initVariables();
	void initBoard();
	void initWindow();

public:
	Game();
	virtual ~Game();

	const bool getWindowIsOpen() const;

	void pullEvents();
	void updateMousePos();
	void updateBoard();
	void update();
	void render();
};


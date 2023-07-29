#include "Game.h"


void Game::initVariables()
{
	this->cellSize = 15;
	this->boardWidth = 70;
	this->boardHeight = 70;

	this->deadCellColor = sf::Color(0, 0, 0);
	this->aliveCellColor = sf::Color(220, 220, 220);
	this->paused = true;
	this->mouseHeld = false;

	this->timer = 0.f;
	this->deltaClock.restart();
	this->updateRate = 100.f;
}

void Game::initBoard()
{
	std::vector<int> row(this->boardHeight + 2, 0);
	for (int i = 0; i < boardWidth + 2; i++)
	{	
		this->board.push_back(row);
	}
	row.pop_back();
	row.pop_back();
	for (int i = 0; i < boardWidth; i++)
	{
		this->nextBoard.push_back(row);
	}
}

void Game::initWindow()
{
	this->videoMode.width = this->cellSize * this->boardWidth;
	this->videoMode.height = this->cellSize * this->boardHeight;
	this->window = new sf::RenderWindow(this->videoMode, "Game of Life", sf::Style::Titlebar | sf::Style::Close);

	this->window->setFramerateLimit(60);
}

Game::Game()
{
	this->initVariables();
	this->initBoard();
	this->initWindow();
}

Game::~Game()
{
	delete this->window;
}

const bool Game::getWindowIsOpen() const
{
	return this->window->isOpen();
}

void Game::pullEvents()
{
	while (this->window->pollEvent(this->ev))
	{
		switch (this->ev.type)
		{
		case sf::Event::Closed:
			this->window->close();
			break;
		case sf::Event::KeyPressed:
			if (this->ev.key.code == sf::Keyboard::Space)
			{
				this->paused = !this->paused;
				break;
			}
			else if (this->ev.key.code == sf::Keyboard::Enter)
			{
				this->paused = true;
				break;
			}
		}
	}
}

void Game::updateMousePos()
{
	this->mousePos = this->window->mapPixelToCoords(sf::Mouse::getPosition(*this->window));
}

void Game::updateBoard()
{	
	if (this->paused == false) {
		float dt = deltaClock.restart().asMilliseconds();
		this->timer += dt;
	}

	if (this->timer >= this->updateRate) {
		this->timer = 0;
		for (int i = 1; i < this->boardWidth + 1; i++)
		{
			for (int j = 1; j < this->boardHeight + 1; j++)
			{
				int count = this->board[i - 1][j - 1] + this->board[i - 1][j] + this->board[i - 1][j + 1] + this->board[i][j - 1] +
					this->board[i][j + 1] + this->board[i + 1][j - 1] + this->board[i + 1][j] + this->board[i + 1][j + 1];
				if (count < 2)
				{
					this->nextBoard[i - 1][j - 1] = 0;
				}
				else if (count == 2)
				{
					this->nextBoard[i - 1][j - 1] = this->board[i][j];
				}
				else if (count == 3)
				{
					this->nextBoard[i - 1][j - 1] = 1;
				}
				else if (count > 3)
				{
					this->nextBoard[i - 1][j - 1] = 0;
				}
			}
		}

		for (int i = 1; i < this->boardWidth + 1; i++)
		{
			for (int j = 1; j < this->boardHeight + 1; j++)
			{
				this->board[i][j] = this->nextBoard[i - 1][j - 1];
			}
		}
	}
	
}

void Game::update()
{
	this->updateMousePos();
	this->pullEvents();
	

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		if (this->paused && !this->mouseHeld)
		{
			int w = this->mousePos.x / cellSize;
			int h = this->mousePos.y / cellSize;

			if (w >= 0 && w < this->boardWidth && h >= 0 && h < this->boardHeight)
			{
				board[w + 1][h + 1] = 1 - board[w + 1][h + 1];
			}
		}
		this->mouseHeld = true;
	} 
	else 
	{
		this->mouseHeld = false;
	}

	this->updateBoard();
	this->render();
}

void Game::render()
{
	this->window->clear(sf::Color(0, 0, 0));

	for (int i = 0; i < this->boardWidth; i++)
	{
		for (int j = 0; j < this->boardHeight; j++)
		{
			sf::RectangleShape cell(sf::Vector2f(this->cellSize, this->cellSize));
			if (this->board[i + 1][j + 1] == 1) {
				cell.setFillColor(this->aliveCellColor);
			}
			else
			{
				cell.setFillColor(this->deadCellColor);
			}
			cell.setOutlineColor(this->deadCellColor);
			cell.setOutlineThickness(1);
			cell.setPosition(i * this->cellSize, j * this->cellSize);
			this->window->draw(cell);
		}
	}
	
	this->window->display();
}


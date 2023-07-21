#include "Header.h"

void Game::readInputFromFile(const std::string& filename)
{
	std::ifstream inputFile(filename);
	if (!inputFile.is_open())
	{
		std::cerr << "Failed to open the input file." << std::endl;
		return;
	}

	int numChips, numPoints, numConnections;
	inputFile >> numChips >> numPoints;

	for (int i = 0; i < numPoints; i++)
	{
		int x, y;
		inputFile >> x >> y;
		points.emplace_back(i + 1, x, y);
	}

	for (int i = 0; i < numChips; i++)
	{
		int chipPosition;
		inputFile >> chipPosition;
		chips.emplace_back(i + 1, chipPosition);
	}

	for (int i = 0; i < numChips; i++)
	{
		int target;
		inputFile >> target;
		chips[i].target = target;
	}

	inputFile >> numConnections;

	for (int i = 0; i < numConnections; i++)
	{
		int point1, point2;
		inputFile >> point1 >> point2;
		connections.emplace_back(point1, point2);
	}
	inputFile.close();
}

void Game::buildGraph()
{
	for (const auto& connection : connections)
	{
		graph[connection.id1].push_back(connection.id2);
		graph[connection.id2].push_back(connection.id1);
	}
}

void Game::Initialization(const std::string& inputFilename)
{
	readInputFromFile(inputFilename);
	buildGraph();
}

void Game::Generation()
{
	/*PuzzleSolver solver;
	solver.solvePuzzle("D:\\Source\\Caps\\Caps\\input.txt");*/

	sf::RenderWindow window(sf::VideoMode(800, 600), "sfml chip movement");

	int chipCount = chips.size();
	std::vector<sf::CircleShape> chips(chipCount, sf::CircleShape(25.f));
	std::vector<sf::Vector2f> targets(chipCount, sf::Vector2f(0.f, 0.f));
	std::vector<bool> isselected(chipCount, false);
	std::vector<bool> ismoving(chipCount, false);

	for (int i = 0; i < chipCount; ++i)
	{
		chips[i].setFillColor(sf::Color(i * 1000 % 255 + i * 10, i * 1000 % 255 + i * 20, i * 1000 % 255 + i * 50));
		chips[i].setOutlineThickness(5.f);
		int chipPosition = this->chips[i].currentPosition;
		chips[i].setPosition(points[chipPosition - 1].x, points[chipPosition - 1].y);
	}

	sf::RectangleShape square(sf::Vector2f(50.f, 250.f));
	square.setPosition(100, 100);
	square.setFillColor(sf::Color(200, 200, 200));

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
			{
				sf::Vector2f mousepos(sf::Mouse::getPosition(window));
				for (int i = 0; i < chipCount; ++i)
				{
					if (chips[i].getGlobalBounds().contains(mousepos))
					{
						isselected[i] = !isselected[i];
						chips[i].setOutlineColor(isselected[i] ? sf::Color::Yellow : sf::Color::White);
					}
					else if (isselected[i])
					{
						targets[i] = mousepos - sf::Vector2f(25.f, 25.f);
						ismoving[i] = true;
					}
				}
			}
		}

		for (int i = 0; i < chipCount; ++i)
		{
			if (ismoving[i])
			{
				sf::Vector2f direction = targets[i] - chips[i].getPosition();
				float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
				if (length > 1.f)
				{
					direction /= length;
					chips[i].move(direction);
				}
				else
				{
					chips[i].setPosition(targets[i]);
					ismoving[i] = false;
					isselected[i] = false;
					chips[i].setOutlineColor(sf::Color::White);
				}
			}
		}

		window.clear(sf::Color::White);
		window.draw(square);
		for (const auto& chip : chips)
			window.draw(chip);

		window.display();
	}
}

std::vector<int> Game::findPath(int start, int end)
{
	std::unordered_map<int, bool> visited;
	std::unordered_map<int, int> prev;
	std::queue<int> q;

	q.push(start);
	visited[start] = true;

	while (!q.empty())
	{
		int current = q.front();
		q.pop();

		if (current == end)
			break;


		for (int neighbor : graph[current])
		{
			if (!visited[neighbor])
			{
				q.push(neighbor);
				visited[neighbor] = true;
				prev[neighbor] = current;
			}
		}
	}

	std::vector<int> path;
	int current = end;

	while (current != start)
	{
		path.push_back(current);
		current = prev[current];
	}

	path.push_back(start);
	reverse(path.begin(), path.end());

	return path;
}

void Game::solvePuzzle(const std::string& inputFilename)
{
	readInputFromFile(inputFilename);
	buildGraph();

	for (const auto& chip : chips)
	{
		int start = chip.currentPosition;
		int end = points[chip.id - 1].id;

		std::vector<int> path = findPath(start, end);

		std::cout << "Path for Chip " << chip.id << ": ";
		for (int point : path)
			std::cout << point << " ";

		std::cout << std::endl;
	}
}

void Game::start(const std::string& inputFilename)
{
	this->Initialization(inputFilename);
	this->Generation();
}

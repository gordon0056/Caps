#include "Header.h"

sf::Color colors[] =
{
	sf::Color(255, 0, 0),       // red
	sf::Color(255, 165, 0),     // orange
	sf::Color(255, 255, 100),   // citric
	sf::Color(0, 255, 0),       // green
	sf::Color(0, 255, 255),     // blue
	sf::Color(0, 0, 255),       // dark blue
	sf::Color(128, 0, 128),     // violet
	sf::Color(255, 192, 203),   // pink
	sf::Color(255, 0, 255),		// purple
	sf::Color(128,64,48)		// brown
};

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

void Game::Generation(const std::string& sprite)
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "sfml chip movement");

	int chipCount = chips.size();
	std::vector<sf::CircleShape> chipsShapes(chipCount, sf::CircleShape(25.f));
	std::vector<sf::CircleShape> pointsShapes(points.size(), sf::CircleShape(25.f));

	for (int i = 0; i < chipCount; ++i)
	{
		chipsShapes[i].setFillColor(colors[i % 10]);
		chipsShapes[i].setOutlineThickness(5.f);
		int chipPosition = this->chips[i].currentPosition;
		chipsShapes[i].setPosition(points[chipPosition - 1].x, points[chipPosition - 1].y);
	}

	sf::Vector2f targetPosition;
	sf::Vector2f currentPosition;
	sf::Vector2f direction(0.f, 0.f);
	float length = 0.f;

	float speed = 100.f;
	float distance = 0.f;

	sf::Clock clock;

	int selectedChipIndex = -1;
	int chipIndex = 0, pointIndex = 0;
	std::vector<int> route;

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
					if (chipsShapes[i].getGlobalBounds().contains(mousepos))
					{
						if (selectedChipIndex != -1 && selectedChipIndex != i)
						{

							chips[selectedChipIndex].isSelected = false;
							chipsShapes[selectedChipIndex].setOutlineColor(sf::Color::White);
							for (Point& point : points)
							{
								point.isHighlighted = false;
							}
						}

						chips[i].isSelected = !chips[i].isSelected;
						chipsShapes[i].setOutlineColor(chips[i].isSelected ? sf::Color::Yellow : sf::Color::White);

						if (chips[i].isSelected)
						{
							selectedChipIndex = i;
							auto pathMap = findPath(chips[i].currentPosition);
							highlightPath(pathMap, selectedChipIndex);
						}
						else
						{
							selectedChipIndex = -1;
							for (Point& point : points)
							{
								point.isHighlighted = false;
							}
						}
					}
					else if (chips[i].isSelected)
					{
						chipIndex = i;
						for (Point& point : points)
						{
							if (pointsShapes[point.id - 1].getGlobalBounds().contains(mousepos) && point.isHighlighted)
							{
								chipsShapes[i].setPosition(point.x, point.y);
								chips[i].currentPosition = point.id;
								chips[i].isSelected = false;
								chipsShapes[i].setOutlineColor(sf::Color::White);
								selectedChipIndex = -1;

								for (Point& point : points)
								{
									point.isHighlighted = false;
								}
							}
						}
					}
				}
			}
		}

		if (chips[chipIndex].isSelected)
		{
			sf::Vector2f mousepos(sf::Mouse::getPosition(window));
			currentPosition = chipsShapes[chipIndex].getPosition();
			for (Point& point : points)
			{
				if (pointsShapes[point.id - 1].getGlobalBounds().contains(mousepos) && point.isHighlighted)
					targetPosition = pointsShapes[point.id - 1].getPosition();
				pointIndex = point.id;
			}
			direction = targetPosition - currentPosition;
			length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
			direction /= length;
			distance = 0.f;
			route = findRoute(chipIndex + 1, pointIndex);
			
			if (true)
			{

			}

			sf::Time deltatime = clock.restart();
			float frameDistance = speed * deltatime.asSeconds();

			if (distance < length)
			{
				distance += frameDistance;
				if (distance >= length)
				{
					chipsShapes[chipIndex].setPosition(targetPosition);
					distance = length;
				}
				else
					chipsShapes[chipIndex].move(direction * distance);
			}
		}

		window.clear(sf::Color::White);

		//draw connections
		for (const auto& connection : connections)
		{
			sf::Vertex line[] =
			{
				sf::Vertex(sf::Vector2f(points[connection.id1 - 1].x + 25, points[connection.id1 - 1].y + 25)),
				sf::Vertex(sf::Vector2f(points[connection.id2 - 1].x + 25, points[connection.id2 - 1].y + 25))
			};

			for (int i = 0; i < 2; ++i)
			{
				line[i].color = sf::Color::Black;
			}

			window.draw(line, 2, sf::Lines);
		}

		//draw vertices
		for (int i = 0; i < points.size(); ++i)
		{

			pointsShapes[i].setFillColor(sf::Color(192, 192, 192));
			pointsShapes[i].setOutlineThickness(5.f);
			pointsShapes[i].setOutlineColor(points[i].isHighlighted ? sf::Color::Yellow : sf::Color::White);
			pointsShapes[i].setPosition(points[i].x, points[i].y);
			window.draw(pointsShapes[i]);
		}

		//draw chips
		for (int i = 0; i < chipCount; ++i)
		{
			window.draw(chipsShapes[i]);
		}

		if (checkWinCondition())
		{
			sf::Texture textureWin;
			textureWin.loadFromFile(sprite);
			sf::Sprite spriteWin(textureWin);
			spriteWin.setPosition(125, 200);
			window.draw(spriteWin);
			window.display();
			sf::sleep(sf::seconds(5));
			window.close();
			return;
		}

		window.display();
	}
}

std::unordered_map<int, std::vector<int>> Game::findPath(int start)
{
	std::queue<int> nodeQueue;
	std::unordered_map<int, std::vector<int>> pathMap;
	std::unordered_map<int, int> parentMap;

	nodeQueue.push(start);
	parentMap[start] = -1;
	pathMap[start] = std::vector<int>(); // Initialize an empty path for the starting vertex

	for (const auto& chip : chips)
	{
		if (chip.currentPosition != start)
		{
			parentMap[chip.currentPosition] = -2;
		}
	}

	while (!nodeQueue.empty())
	{
		int currentNode = nodeQueue.front();
		nodeQueue.pop();

		for (int adjacentNode : graph[currentNode])
		{
			if (parentMap.find(adjacentNode) == parentMap.end())
			{
				if (parentMap[currentNode] != -2)
				{
					nodeQueue.push(adjacentNode);
					parentMap[adjacentNode] = currentNode;

					pathMap[adjacentNode] = pathMap[currentNode]; // Inherit the path from the previous vertex
					pathMap[adjacentNode].push_back(adjacentNode); // Adding the current vertex to the path
				}
			}
		}
	}

	pathMap.erase(start);

	return pathMap;
}

std::vector<int> Game::findRoute(int start, int end)
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

	std::reverse(path.begin(), path.end());

	return path;
}

void Game::start(const std::string& inputFilename, const std::string& sprite)
{
	this->Initialization(inputFilename);
	this->Generation(sprite);
}

void Game::highlightPath(const std::unordered_map<int, std::vector<int>>& pathMap, int selectedChipIndex)
{
	for (const auto& entry : pathMap)
	{
		int pointId = entry.first;
		const std::vector<int>& path = entry.second;

		if (pointId != chips[selectedChipIndex].currentPosition)
		{
			for (int nodeId : path)
			{
				points[nodeId - 1].isHighlighted = true;
			}
		}
	}
}

bool Game::checkWinCondition()
{
	for (const auto& chip : chips)
	{
		if (chip.currentPosition != chip.target)
		{
			return false;
		}
	}
	return true;
}


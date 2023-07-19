#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <SFML/Graphics.hpp>

using namespace std;

// Класс для представления фишек
class Chip 
{
public:
    unsigned int id;
    unsigned int position;
    unsigned int target;

    Chip(unsigned int id, unsigned int position) : id(id), position(position),target(0) {}
};

// Класс для представления точек
class Point 
{
public:
    unsigned int id;
    unsigned int x;
    unsigned int y;

    Point(unsigned int id, unsigned int x, unsigned int y) : id(id), x(x), y(y) {}
};

// Класс для представления соединений
class Connection 
{
public:
    unsigned int id1;
    unsigned int id2;

    Connection(unsigned int id1, unsigned int id2) : id1(id1), id2(id2) {}
};
//
//// Класс для решения задачи
class PuzzleSolver 
{
private:
    vector<Chip> chips;
    vector<Point> points;
    vector<Connection> connections;
    unordered_map<int, vector<int>> graph;

    void readInputFromFile(const string& filename) 
    {
        ifstream inputFile(filename);
        if (!inputFile.is_open()) 
        {
            cerr << "Failed to open the input file." << endl;
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
        { // Исправлено здесь
            int chipPosition;
            inputFile >> chipPosition;
            chips.emplace_back(i + 1, chipPosition);
        }

        for (int i = 0; i < numChips; i++)
        { // Исправлено здесь
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

    void buildGraph() 
    {
        for (const auto& connection : connections) 
        {
            graph[connection.id1].push_back(connection.id2);
            graph[connection.id2].push_back(connection.id1);
        }
    }

    vector<int> findPath(int start, int end) 
    {
        unordered_map<int, bool> visited;
        unordered_map<int, int> prev;
        queue<int> q;

        q.push(start);
        visited[start] = true;

        while (!q.empty()) 
        {
            int current = q.front();
            q.pop();

            if (current == end) 
                break; // Путь найден


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

        // Восстановление пути
        vector<int> path;
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

public:
    void solvePuzzle(const string& inputFilename) 
    {
        readInputFromFile(inputFilename);
        buildGraph();

        for (const auto& chip : chips)
        {
            int start = chip.position;
            int end = points[chip.id - 1].id;

            vector<int> path = findPath(start, end);

            // Вывод пути
            cout << "Path for Chip " << chip.id << ": ";
            for (int point : path) 
                cout << point << " ";
            
            cout << endl;
        }
    }
};



int main()
{
	PuzzleSolver solver;
	solver.solvePuzzle("D:\\Source\\Caps\\Caps\\input.txt");

	/*sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Chip Movement");

	sf::CircleShape chip(50.f);
	chip.setFillColor(sf::Color::Green);
	chip.setOutlineThickness(5.f);

	sf::Vector2f target(0.f, 0.f);
	bool isSelected = false;
	bool isMoving = false; // Добавляем флаг для проверки, перемещается ли фишка

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
			{
				sf::Vector2f mousePos(sf::Mouse::getPosition(window));
				if (chip.getGlobalBounds().contains(mousePos))
				{
					isSelected = !isSelected; // Переключаем выбранность фишки
					chip.setOutlineColor(isSelected ? sf::Color::Yellow : sf::Color::White);
				}
				else if (isSelected) // Если фишка выбрана, устанавливаем новую цель
				{
					target = mousePos;
					isMoving = true; // Устанавливаем флаг перемещения
				}
			}
		}

		if (isMoving)
		{
			sf::Vector2f direction = target - chip.getPosition();
			float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
			if (length > 1.f)
			{
				direction /= length;
				chip.move(direction);
			}
			else
			{
				chip.setPosition(target);
				isMoving = false; // Обнуляем флаг перемещения
				isSelected = false; // И сбрасываем выбранность фишки
				chip.setOutlineColor(sf::Color::White); // И меняем обводку на белую
			}
		}

		window.clear(sf::Color::White);
		window.draw(chip);
		window.display();
	}*/

	return 0;
}

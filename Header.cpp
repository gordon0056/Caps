#include "Header.h"

void PuzzleSolver::readInputFromFile(const std::string& filename)
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

void PuzzleSolver::buildGraph()
{
	for (const auto& connection : connections)
	{
		graph[connection.id1].push_back(connection.id2);
		graph[connection.id2].push_back(connection.id1);
	}
}

std::vector<int> PuzzleSolver::findPath(int start, int end)
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

void PuzzleSolver::solvePuzzle(const std::string& inputFilename)
{
    readInputFromFile(inputFilename);
    buildGraph();

    for (const auto& chip : chips)
    {
        int start = chip.position;
        int end = points[chip.id - 1].id;

        std::vector<int> path = findPath(start, end);

        // Вывод пути
        std::cout << "Path for Chip " << chip.id << ": ";
        for (int point : path)
            std::cout << point << " ";

        std::cout << std::endl;
    }
}

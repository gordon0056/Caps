#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <queue>

using namespace std;

// Класс для представления фишек
class Chip 
{
public:
    int id;
    int position;

    Chip(int id, int position) : id(id), position(position) {}
};

// Класс для представления точек
class Point 
{
public:
    int id;
    int x;
    int y;

    Point(int id, int x, int y) : id(id), x(x), y(y) {}
};

// Класс для представления соединений
class Connection 
{
public:
    int id1;
    int id2;

    Connection(int id1, int id2) : id1(id1), id2(id2) {}
};

// Класс для решения задачи
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

        for (int i = 0; i < numChips; i++) {
            int chipPosition;
            inputFile >> chipPosition;
            chips.emplace_back(i + 1, chipPosition);
        }

        for (int i = 0; i < numPoints; i++) {
            int destination;
            inputFile >> destination;
            chips[i].position = destination;
        }

        inputFile >> numConnections;

        for (int i = 0; i < numConnections; i++) {
            int point1, point2;
            inputFile >> point1 >> point2;
            connections.emplace_back(point1, point2);
        }

        inputFile.close();
    }

    void buildGraph() {
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
            {
                // Путь найден
                break;
            }

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
            for (int point : path) {
                cout << point << " ";
            }
            cout << endl;
        }
    }
};

int main() 
{
    PuzzleSolver solver;
    solver.solvePuzzle("input.txt");

    return 0;
}

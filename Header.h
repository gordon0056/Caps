#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <SFML/Graphics.hpp>

class Chip
{
public:
	unsigned int id;
	unsigned int position;
	unsigned int target;

	Chip(unsigned int id, unsigned int position) : id(id), position(position), target(0) {}
};

class Point
{
public:
	unsigned int id;
	unsigned int x;
	unsigned int y;

	Point(unsigned int id, unsigned int x, unsigned int y) : id(id), x(x), y(y) {}
};

class Connection
{
public:
	unsigned int id1;
	unsigned int id2;

	Connection(unsigned int id1, unsigned int id2) : id1(id1), id2(id2) {}
};

class Game
{
private:
	std::vector<Chip> chips;
	std::vector<Point> points;
	std::vector<Connection> connections;
	std::unordered_map<int, std::vector<int>> graph;

	void readInputFromFile(const std::string& filename);

	void buildGraph();

	void Initialization(const std::string& inputFilename);

	void Generation();

	std::vector<int> findPath(int start, int end);

public:

	void solvePuzzle(const std::string& inputFilename);
	void start();
};

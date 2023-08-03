#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <vector>
#include <unordered_map>
#include <queue>
#include <SFML/Graphics.hpp>

class Chip
{
public:
	unsigned int id;
	unsigned int currentPosition;
	unsigned int target;
	bool isSelected = false;

	Chip(unsigned int id, unsigned int position) : id(id), currentPosition(position), target(NULL) {}
};

class Point
{
public:
	unsigned int id;
	float x;
	float y;
	bool isHighlighted = false;

	Point(unsigned int id, float x, float y) : id(id), x(x), y(y) {}
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

	void Generation(const std::string& sprite);

	std::unordered_map<int, std::vector<int>> findPath(int start);

	std::vector<int> findRoute(int start, int end);

	void highlightPath(const std::unordered_map<int, std::vector<int>>& pathMap, int selectedChipIndex);

	bool checkWinCondition();

public:

	void start(const std::string& inputFilename, const std::string& sprite);
};

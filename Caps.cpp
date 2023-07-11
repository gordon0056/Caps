#include <iostream>
#include <vector>

class Vertex
{	
private:
	int id = 0;
	std::string color;
	std::vector<int> connections;

public:

	friend void initVertices(std::vector<Vertex>& vertices, int numVertices);
	friend void initConnections(std::vector<Vertex>& vertices);
	friend void printGameStatus(const std::vector<Vertex>& vertices);
	friend bool isGameFinished(const std::vector<Vertex>& vertices);
	friend void Game();

};

void initVertices(std::vector<Vertex>& vertices, int numVertices)
{
	for (int count_vert = 0; count_vert < numVertices; count_vert++)
	{
		Vertex vertex;
		vertex.id = count_vert + 1;
		std::cout << "Enter vertex color " << vertex.color << ": ";
		std::cin >> vertex.color;
		vertices.push_back(vertex);
	}
}

void initConnections(std::vector<Vertex>& vertices)
{
	int numConnections;

	for (int count = 0; count < vertices.size(); count++)
	{
		std::cout << "Enter the number of connections for the vertex" << vertices[count].id << ": ";
		std::cin >> numConnections;

		for (int count_connect = 0; count_connect < numConnections; count_connect++)
		{
			int connectedVertex;
			std::cout << "Enter the number of linked vertex";
			std::cin >> connectedVertex;
			vertices[count].connections.push_back(connectedVertex);
		}
	}
}

void printGameStatus(const std::vector<Vertex>& vertices)
{
	std::cout << "Status of the game: " << std::endl;
	for (const Vertex& vertex: vertices)
		std::cout << "Vertex " << vertex.id 
			<< ", color: " << vertex.color << std::endl;
}

bool isGameFinished(const std::vector<Vertex>& vertices)
{
	for (const Vertex& vertex : vertices) 
	{
		if (!vertex.connections.empty())
			return false;
		
		return true;
	}
}

void Game()
{
	std::vector<Vertex> vertices;
	int numVertices;

	std::cout << "Enter number of connections: ";
	std::cin >> numVertices;

	initVertices(vertices, numVertices);
	initConnections(vertices);

	for (const Vertex& vertex : vertices)
	{
		/*int numCaps(1);*/
		std::cout << vertex.color << " caps located in the next vertex "
			<< vertex.id << std::endl;
	}
	
	while (isGameFinished(vertices))
	{
		printGameStatus(vertices);
		std::string selectedColor;
		std::cout << "Choose color of the cap for move: ";
		std::cin >> selectedColor;

		int destinationVertex;
		std::cout << "Choose the vertex, which need move cap";
		std::cin >> destinationVertex;

		bool isValidMove = false;

		for (Vertex& vertex : vertices)
		{
			if (vertex.color == selectedColor)
			{
				for (int& connectedVertex : vertex.connections)
				{
					if (connectedVertex == destinationVertex)
					{
						isValidMove = true;
						vertex.connections.clear();
						break;
					}
				}
				break;
			}
		}

		if (isValidMove)
			std::cout << "Cap was been successful moved in another vertex" 
			<< destinationVertex << std::endl;
		else
			std::cout << "Unable to move a cap to the selected vertex" << std::endl;
	}
}

int main()
{
	void game();
	return 0;
}


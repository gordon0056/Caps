#include "Header.h"

int main(int argc, char* argv[])
{
	Game game;
	std::filesystem::path appPath(argv[0]);
	std::filesystem::path appDir = appPath.parent_path();
	std::filesystem::path sprite = appDir / "giphy.png";
	std::filesystem::path fileMap = appDir / "input.txt";

	game.start(fileMap.string(), sprite.string());

	return 0;
}

/*
#include <SFML/Graphics.hpp>

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!");
	window.setFramerateLimit(60);

	std::vector<sf::Vector2f> points =
	{
		sf::Vector2f(100, 100),
		sf::Vector2f(100, 200),
		sf::Vector2f(200, 200),
		sf::Vector2f(300, 200),
		sf::Vector2f(300, 300)
	};

	sf::CircleShape chip(25.f);
	chip.setPosition(100.f, 100.f);
	chip.setFillColor(sf::Color::Green);

	unsigned short targetIndex = 0;
	sf::Vector2f targetPosition = points[targetIndex];
	sf::Vector2f currentPosition = chip.getPosition();
	sf::Vector2f direction(0.f, 0.f);
	float length = 0.f;

	float speed = 100.f;
	float distance = 0.f;

	bool isMoving = false;

	sf::Clock clock;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
			{
				isMoving = true;
			}

			if (isMoving)
			{
				currentPosition = chip.getPosition();
				targetPosition = points[targetIndex];

				direction = targetPosition - currentPosition;
				length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
				direction /= length;

				distance = 0.f;
				isMoving = true;
			}

			if (points[targetIndex] == currentPosition && isMoving)
				targetIndex = (targetIndex + 1) % points.size(); // Использование операции остатка для циклического обхода точек

			sf::Time deltaTime = clock.restart();
			float frameDistance = speed * deltaTime.asSeconds();

			if (isMoving && distance < length)
			{
				distance += frameDistance;
				if (distance >= length)
				{
					chip.setPosition(targetPosition);
					distance = length;
				}
				else
					chip.move(direction * distance);
			}
		}

		if (points.back() == currentPosition && isMoving)
			isMoving = false;

		window.clear(sf::Color::White);
		window.draw(chip);
		window.display();
	}

	return 0;
}

*/
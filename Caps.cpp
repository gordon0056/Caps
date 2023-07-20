#include "Header.h"

/*PuzzleSolver solver;
solver.solvePuzzle("D:\\Source\\Caps\\Caps\\input.txt");*/

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "sfml chip movement");

    const int chipCount = 6;
    std::vector<sf::CircleShape> chips(chipCount, sf::CircleShape(40.f));
    std::vector<sf::Vector2f> targets(chipCount, sf::Vector2f(0.f, 0.f));
    std::vector<bool> isselected(chipCount, false);
    std::vector<bool> ismoving(chipCount, false);   

    for (int i = 0; i < chipCount; ++i)
    {
        chips[i].setFillColor(sf::Color::Green);
        chips[i].setOutlineThickness(5.f);
        chips[i].setPosition(100.f + i * 100.f, 100.f); // пример расположения фишек
    }

    sf::RectangleShape square(sf::Vector2f(60.f,40.f));
    square.setPosition(500, 500); // устанавливаем начальную позицию справа от треугольника
    square.setFillColor(sf::Color::Black);

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
                        targets[i] = mousepos;
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
        for (const auto& chip : chips)
            window.draw(chip);
        window.draw(square);
        window.display();
    }

    return 0;
}



//class point
//{
//public:
//	int x;
//	int y;
//	sf::Color color;
//
//	point(int x, int y, sf::Color color) : x(x), y(y), color(color) {}
//};
//
//int main()
//{
//	srand(time(NULL)); // Инициализация генератора случайных чисел
//
//	sf::RenderWindow window(sf::VideoMode(800, 600), "My Game");
//
//	std::vector<point> points;
//
//	int numPoints = 10; // Количество кругов, которые нужно создать
//
//	for (int i = 0; i < numPoints; i++)
//	{
//		int x = rand() % 700 + 50; // Генерация случайной координаты x в диапазоне [50, 750]
//		int y = rand() % 500 + 50; // Генерация случайной координаты y в диапазоне [50, 550]
//		sf::Color color(rand() % 256, rand() % 256, rand() % 256); // Генерация случайного цвета
//		points.push_back(point(x, y, color));
//	}
//
//	while (window.isOpen())
//	{
//		sf::Event event;
//
//		while (window.pollEvent(event))
//		{
//			if (event.type == sf::Event::Closed)
//			{
//				window.close();
//			}
//		}
//
//		window.clear();
//
//		for (auto& p : points)
//		{
//			sf::CircleShape circle(20.f);
//			circle.setFillColor(p.color);
//			circle.setPosition(p.x, p.y);
//			window.draw(circle);
//		}
//
//		window.display();
//	}
//
//	return 0;
//}
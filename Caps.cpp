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
#include "Level.h"
#include <test_Engine/Errors.h>
#include <fstream>
#include <iostream>

using test_Engine::fatalError;
Level::Level(const std::string& fileName)
{
	std::ifstream file;
	file.open(fileName);

	if (file.fail())
		fatalError("cannot open level file: " + fileName);
	std::string tmp;
	file >> tmp >> _nrHumans;

	while (std::getline(file, tmp))
		_levelData.push_back(tmp);
}

Level::~Level()
{
}

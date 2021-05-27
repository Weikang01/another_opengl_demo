#pragma once

#include <string>
#include <vector>

class Level
{
public:
	// load the level
	Level(const std::string& fileName);
	~Level();

	unsigned int getNrHumans() { return _nrHumans; }
	std::vector<std::string> _levelData;
private:
	unsigned int _nrHumans;
};


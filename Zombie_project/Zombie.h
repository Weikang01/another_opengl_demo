#pragma once
#include "Player.h"

class Zombie :
    public Agent
{
public:
    Zombie() = default;
    Zombie(glm::vec2 pos, float speed, int health, const char* texturePath)
        :Agent(pos, speed, health, texturePath), _targetDir(0.f), _dist(0.f)
    {}

    void update(const std::vector<std::string>& levelData,
        std::vector<Human*>& humans, std::vector<Zombie*>& zombies);
    glm::vec2 getPos() { return Agent::_pos; }
    void setTarget(const Human* first);
    void checkTarget(const Human* human);
private:
    float _dist;
    glm::vec2 _targetDir;
};
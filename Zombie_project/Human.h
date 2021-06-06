#pragma once
#include "Agent.h"
class Zombie;

class Human :
    public Agent
{
public:
    Human() = default;
    Human(glm::vec2 pos, float speed, int health, const char* texturePath)
        :Agent(pos, speed, health, texturePath), _frame(0)
    {}
    virtual ~Human() {}

    void init();

    virtual void update(const std::vector<std::string>& levelData,
        std::vector<Human*>& humans, std::vector<Zombie*>& zombies, float deltaTime) override;
private:
    int _frame;
};
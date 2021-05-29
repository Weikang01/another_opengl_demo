#pragma once
#include <vector>
#include <test_Engine/Camera2D.h>
#include <test_Engine/InputManager.h>
#include "Human.h"


using test_Engine::InputManager;
using test_Engine::Camera2D;

class Gun;
class Bullet;

class Player :
    public Human
{
public:
    Player() = default;
    Player(glm::vec2 pos, float speed, int health, const char* texturePath)
        :Human(pos, speed, health, texturePath), _currentGun(-1)
    {}

    void addGun(Gun* gun);
    void init(InputManager* inputManager, Camera2D* _camera, std::vector<Bullet>* bullets);
    void update(const std::vector<std::string>& levelData,
        std::vector<Human*>& humans, std::vector<Zombie*>& zombies) override;

private:
    std::vector<Gun*> _guns;
    int _currentGun;
    InputManager* _inputManager;
    Camera2D* _camera;
    std::vector<Bullet>* _bullets;
};


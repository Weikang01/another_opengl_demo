#pragma once
#include "Agent.h"
#include "Wall.h"
class Zombie;

class Human :
    public Agent
{
private:
    enum class MovingState { IDLE, ROAM, ESCAPE };
public:
    Human() = default;
    Human(glm::vec2 pos, glm::vec2 size, float speed, float range, const char* texturePath)
        :Agent(pos, size, speed, texturePath), _detect_range(range),
        _mState(MovingState::IDLE), _target_dir(0.f), _roamVec(.5f, .5f), _has_roam_dir(false),
        _lastTicks(SDL_GetTicks()), _roamInterval(1000)
    {}

    bool detect_walls(const std::vector<Wall>& walls);
    bool detect_zombies(const std::vector<Zombie>& zombies);
    void roam();
    void update(const std::vector<Wall>& walls);

    glm::vec2 getPos() const
    {
        return Agent::getPos();
    }
    friend Zombie;
private:
    glm::vec2 _roamVec;  // (P_idle_idle, P_roam_roam)
    bool _has_roam_dir;
    glm::vec2 _target_dir;
    MovingState _mState;
    float _detect_range;
    Uint32 _roamInterval, _lastTicks; // minisec
};
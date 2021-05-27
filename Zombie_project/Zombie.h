#pragma once
#include "Agent.h"
#include "Human.h"


class Zombie :
    public Agent
{
private:
    enum class MovingState { IDLE, ROAM, CHASE };

public:
    Zombie() = default;
    Zombie(glm::vec2 pos, glm::vec2 size, float speed, float range, const char* texturePath)
        :Agent(pos, size, speed, texturePath), _detect_range(range), _convert_range(20.f), _has_roam_dir(false),
        _mState(MovingState::IDLE), _target_dir(0.f), _roamVec(.5f, .5f), _closest_dist(0.f),
        _lastTicks(SDL_GetTicks()), _roamInterval(1000)
    {}
    Zombie(const Zombie* parent, const Human& human)
        : Agent(human.getPos_and_size(), parent->_texture, parent->_speed),
        _mState(MovingState::IDLE), _target_dir(0.f), _detect_range(parent->_detect_range), _has_roam_dir(false),
        _convert_range(parent->_convert_range), _closest_dist(0.f), _roamVec(parent->_roamVec),
        _lastTicks(SDL_GetTicks()), _roamInterval(1000)
    {}

    bool detect_walls(const std::vector<Wall>& walls);
    bool detect_humans(std::vector<Human>& humans);
    void update(const std::vector<Wall>& walls);

    glm::vec2 getPos() const
    {
        return Agent::getPos();
    }
    static Zombie newcommer;
    static bool _ifSpawn;

    void roam();

private:
    MovingState _mState;
    glm::vec2 _target_dir;
    glm::vec2 _roamVec;  // (P_idle_idle, P_roam_roam)
    bool _has_roam_dir;
    float _detect_range, _convert_range, _closest_dist;
    Uint32 _roamInterval, _lastTicks; // minisec
};
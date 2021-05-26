#pragma once
#include <glm.hpp>
#include <test_Engine/GLTexture.h>
#include <test_Engine/SpriteBatch.h>
#include <test_Engine/GLTexture.h>
#include <SDL.h>

using test_Engine::SpriteBatch;
using test_Engine::GLTexture;

class Human;
class Zombie;

class Unit
{
public:
	Unit() = default;
	Unit(glm::vec2 pos, glm::vec2 size, float speed, const char* texturePath);
	~Unit() {}

	void setPos(float x, float y);
	glm::vec2 getPos() const { return glm::vec2(_pos_and_size.x, _pos_and_size.y); }
	void move(const glm::vec2& dir);
    glm::vec2 getSize() const { return glm::vec2(_pos_and_size.z, _pos_and_size.w); }

	void draw(SpriteBatch& spriteBatch) const;

protected:
    Unit(const glm::vec4 pos_and_size, const GLTexture texture, const float speed)
        :_pos_and_size(pos_and_size), _texture(texture), _speed(speed)
    {}
    glm::vec4 getPos_and_size() const { return _pos_and_size; }
	glm::vec4 _pos_and_size;
	GLTexture _texture;
	float _speed;
};

class Human :
    public Unit
{
private:
    enum class MovingState { IDLE, ROAM, ESCAPE };
public:
    Human() = default;
    Human(glm::vec2 pos, glm::vec2 size, float speed, float range, const char* texturePath)
        :Unit(pos, size, speed, texturePath), _range(range), 
        _mState(MovingState::IDLE), _target_dir(0.f), _roamVec(.5f, .5f), _has_roam_dir(false),
        _lastTicks(SDL_GetTicks()), _roamInterval(1000)
    {}

    bool detect_zombies(const std::vector<Zombie>& zombies);
    void roam();
    void update();

    glm::vec2 getPos() const
    {
        return Unit::getPos();
    }
    friend Zombie;
private:
    glm::vec2 _roamVec;  // (P_idle_idle, P_roam_roam)
    bool _has_roam_dir;
    glm::vec2 _target_dir;
    MovingState _mState;
    float _range;
    Uint32 _roamInterval, _lastTicks; // minisec
};

class Zombie :
    public Unit
{
private:
    enum class MovingState { IDLE, ROAM, CHASE };

public:
    Zombie() = default;
    Zombie(glm::vec2 pos, glm::vec2 size, float speed, float range, const char* texturePath)
        :Unit(pos, size, speed, texturePath), _detect_range(range), _convert_range(20.f), _has_roam_dir(false),
        _mState(MovingState::IDLE), _target_dir(0.f), _roamVec(.5f, .5f), _closest_dist(0.f),
        _lastTicks(SDL_GetTicks()), _roamInterval(1000)
    {}
    Zombie(const Zombie* parent, const Human& human)
        : Unit(human.getPos_and_size(), parent->_texture, parent->_speed),
        _mState(MovingState::IDLE), _target_dir(0.f), _detect_range(parent->_detect_range), _has_roam_dir(false),
        _convert_range(parent->_convert_range), _closest_dist(0.f), _roamVec(parent->_roamVec),
        _lastTicks(SDL_GetTicks()), _roamInterval(1000)
    {}

    bool detect_humans(std::vector<Human>& humans);
    void update();

    glm::vec2 getPos() const
    {
        return Unit::getPos();
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




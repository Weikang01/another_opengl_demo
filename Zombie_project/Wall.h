#pragma once
#include <glm.hpp>
#include <test_Engine/GLTexture.h>
#include <test_Engine/SpriteBatch.h>
#include <map>

using test_Engine::SpriteBatch;
using test_Engine::GLTexture;

class Wall
{
public:
	static std::map<unsigned char, GLTexture> _texture_map;
	static void add_new_type(const unsigned char type_code, const char* texturePath);
	Wall() = default;
	Wall(glm::vec2 pos, glm::vec2 size, const unsigned char type_code);
	~Wall();

	void draw(SpriteBatch& spriteBatch) const;

	glm::vec2 getPos() const { return glm::vec2(_pos_and_size.x, _pos_and_size.y); }
	glm::vec2 getSize() const { return glm::vec2(_pos_and_size.z, _pos_and_size.w); }
	glm::vec4 getPos_and_size() const { return _pos_and_size; }
private:
	glm::vec4 _pos_and_size;
	unsigned char _type;
};


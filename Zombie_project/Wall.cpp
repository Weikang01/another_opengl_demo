#include "Wall.h"
#include <test_Engine/ResourceManager.h>

std::map<unsigned char, GLTexture> Wall::_texture_map;
void Wall::add_new_type(const unsigned char type_code, const char* texturePath)
{
	_texture_map.insert(std::make_pair(type_code, test_Engine::ResourceManager::getTexture(texturePath)));
	//_texture_map.emplace(type_code, test_Engine::ResourceManager::getTexture(texturePath));
}

Wall::Wall(glm::vec2 pos, glm::vec2 size, const unsigned char type_code)
	:_pos_and_size(pos.x, pos.y, size.x, size.y), _type(type_code)
{
}

Wall::~Wall()
{
}

void Wall::draw(SpriteBatch& spriteBatch) const
{
	glm::vec4 uv(0.f, 0.f, 1.f, 1.f);
	spriteBatch.draw(_pos_and_size, uv, _texture_map[_type].id, 0.f, glm::vec4(255, 255, 255, 255));
}


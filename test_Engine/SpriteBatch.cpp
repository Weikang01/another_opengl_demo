#include "SpriteBatch.h"
#include <algorithm>
#include <iostream>

namespace test_Engine
{
	Glyph::Glyph(const glm::vec4& destRect, const glm::vec4& uvRect, const GLuint texture, const float depth, const ColorRGBA8& color, float angle)
		:texture(texture), depth(depth)
	{
		glm::vec2 halfDims(destRect.z / 2.f, destRect.w / 2.f);

		glm::vec2 tl(-halfDims.x,  halfDims.y);
		glm::vec2 bl(-halfDims.x, -halfDims.y);
		glm::vec2 br( halfDims.x, -halfDims.y);
		glm::vec2 tr( halfDims.x,  halfDims.y);

		// rotate the points
		tl = rotatePoint(tl, angle) + halfDims;
		bl = rotatePoint(bl, angle) + halfDims;
		br = rotatePoint(br, angle) + halfDims;
		tr = rotatePoint(tr, angle) + halfDims;

		topLeft.color = color;
		topLeft.setPosition(destRect.x + tl.x, destRect.y + tl.y);
		topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

		bottomLeft.color = color;
		bottomLeft.setPosition(destRect.x + bl.x, destRect.y + bl.y);
		bottomLeft.setUV(uvRect.x, uvRect.y);

		bottomRight.color = color;
		bottomRight.setPosition(destRect.x + br.x, destRect.y + br.y);
		bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);

		topRight.color = color;
		topRight.setPosition(destRect.x + tr.x, destRect.y + tr.y);
		topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
	}

	glm::vec2 Glyph::rotatePoint(glm::vec2 pos, float angle)
	{
		glm::vec2 newv{};
		newv.x = pos.x * cosf(angle) - pos.y * sinf(angle);
		newv.y = pos.x * sinf(angle) + pos.y * cosf(angle);
		return newv;
	}


	test_Engine::SpriteBatch::SpriteBatch()
		:_VBO(0), _VAO(0), _sortType(GlyphSortType::NONE)
	{
	}


	test_Engine::SpriteBatch::~SpriteBatch()
	{
	}

	void SpriteBatch::init()
	{
		createVertexArray();
	}

	void SpriteBatch::begin(GlyphSortType sortType)
	{
		_sortType = sortType;
		_renderBatches.clear();
		_glyphs.clear();
	}

	void SpriteBatch::end()
	{
		_glyphPointers.resize(_glyphs.size());
		for (unsigned int i = 0; i < _glyphs.size(); i++)
			_glyphPointers[i] = &_glyphs[i];
		sortGlyphs();
		createRenderBatches();
	}
	
	void SpriteBatch::draw(
		const glm::vec4& destRect, const glm::vec4& uvRect, 
		const GLuint texture, const float depth, const glm::vec4& color)
	{
		_glyphs.emplace_back(destRect, uvRect, texture, depth, color);
	}

	void SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect,
		const GLuint texture, const float depth, const ColorRGBA8& color)
	{
		_glyphs.emplace_back(destRect, uvRect, texture, depth, color);
	}

	void SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect,
		const GLuint texture, const float depth, const ColorRGBA8& color, float angle)
	{
		_glyphs.emplace_back(destRect, uvRect, texture, depth, color, angle);
	}

	void SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect,
		const GLuint texture, const float depth, const ColorRGBA8& color, const glm::vec2& dir)
	{
		const glm::vec2 right(1.f, 0.f);
		float angle = glm::acos(glm::dot(right, dir));
		if (dir.y < 0.f) angle = -angle;

		_glyphs.emplace_back(destRect, uvRect, texture, depth, color, angle);
	}

	void SpriteBatch::renderBatch()
	{
		glBindVertexArray(_VAO);

		for (int i = 0; i < _renderBatches.size(); i++)
		{
			if (_renderBatches[i].texture)
				glBindTexture(GL_TEXTURE_2D, _renderBatches[i].texture);

			glDrawArrays(GL_TRIANGLES, _renderBatches[i].offset, _renderBatches[i].numVertices);
		}
	}

	void SpriteBatch::createRenderBatches()
	{
		std::vector<Vertex> vertices;
		vertices.resize(_glyphPointers.size() * 6);

		if (_glyphPointers.empty()) return;
		
		
		int offset = 0;
		int cv = 0; // current vertex
		_renderBatches.emplace_back(0, 6, _glyphPointers[0]->texture);
		vertices[cv++] = _glyphPointers[0]->topLeft;
		vertices[cv++] = _glyphPointers[0]->bottomLeft;
		vertices[cv++] = _glyphPointers[0]->bottomRight;

		vertices[cv++] = _glyphPointers[0]->bottomRight;
		vertices[cv++] = _glyphPointers[0]->topRight;
		vertices[cv++] = _glyphPointers[0]->topLeft;

		for (int cg = 1; cg < _glyphs.size(); cg++)
		{
			offset += 6;
			if (_glyphPointers[cg]->texture != _glyphPointers[cg - 1]->texture)
				_renderBatches.emplace_back(offset, 6, _glyphPointers[cg]->texture);
			else
				_renderBatches.back().numVertices += 6;
			vertices[cv++] = _glyphPointers[cg]->topLeft;
			vertices[cv++] = _glyphPointers[cg]->bottomLeft;
			vertices[cv++] = _glyphPointers[cg]->bottomRight;

			vertices[cv++] = _glyphPointers[cg]->bottomRight;
			vertices[cv++] = _glyphPointers[cg]->topRight;
			vertices[cv++] = _glyphPointers[cg]->topLeft;
		}
		
		glBindBuffer(GL_ARRAY_BUFFER, _VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void SpriteBatch::createVertexArray()
	{
		if (!_VAO) glGenVertexArrays(1, &_VAO);
		if (!_VBO) glGenBuffers(1, &_VBO);

		glBindVertexArray(_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, _VBO);

		// _position attrib
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

		// color
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));

		// texCoord
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void SpriteBatch::sortGlyphs()
	{
		switch (_sortType)
		{
		case GlyphSortType::NONE:
			std::stable_sort(_glyphPointers.begin(), _glyphPointers.end(),
				[](const Glyph* a, const Glyph* b) { return true; });
			break;
		case GlyphSortType::FRONT_TO_BACK:
			std::stable_sort(_glyphPointers.begin(), _glyphPointers.end(),
				[](const Glyph* a, const Glyph* b){
					return (a->depth < b->depth);
				});
			break;
		case GlyphSortType::BACK_TO_FRONT:
			std::stable_sort(_glyphPointers.begin(), _glyphPointers.end(),
				[](const Glyph* a, const Glyph* b) {
					return (a->depth > b->depth);
				});
			break;
		case GlyphSortType::TEXTURE:
			std::stable_sort(_glyphPointers.begin(), _glyphPointers.end(),
				[](const Glyph* a, const Glyph* b) {
					return (a->texture < b->texture);
				});
			break;
		default:
			break;
		}
	}

}
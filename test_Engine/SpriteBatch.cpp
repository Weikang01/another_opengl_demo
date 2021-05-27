#include "SpriteBatch.h"
#include <algorithm>
#include <iostream>

namespace test_Engine
{
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
		sortGlyphs();
		createRenderBatches();
	}
	
	void SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, const GLuint texture, const float depth, const glm::vec4& color)
	{
		Glyph *newGlyph = new Glyph;
		newGlyph->texture = texture;
		newGlyph->depth = depth;

		newGlyph->topLeft.setColor(color);
		newGlyph->topLeft.setPosition(destRect.x, destRect.y + destRect.w);
		newGlyph->topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

		newGlyph->bottomLeft.setColor(color);
		newGlyph->bottomLeft.setPosition(destRect.x, destRect.y);
		newGlyph->bottomLeft.setUV(uvRect.x, uvRect.y);

		newGlyph->bottomRight.setColor(color);
		newGlyph->bottomRight.setPosition(destRect.x + destRect.z, destRect.y);
		newGlyph->bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);

		newGlyph->topRight.setColor(color);
		newGlyph->topRight.setPosition(destRect.x + destRect.z, destRect.y + destRect.w);
		newGlyph->topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);

		_glyphs.push_back(newGlyph);
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
		vertices.resize(_glyphs.size() * 6);

		if (_glyphs.empty()) return;
		
		
		int offset = 0;
		int cv = 0; // current vertex
		_renderBatches.emplace_back(0, 6, _glyphs[0]->texture);
		vertices[cv++] = _glyphs[0]->topLeft;
		vertices[cv++] = _glyphs[0]->bottomLeft;
		vertices[cv++] = _glyphs[0]->bottomRight;

		vertices[cv++] = _glyphs[0]->bottomRight;
		vertices[cv++] = _glyphs[0]->topRight;
		vertices[cv++] = _glyphs[0]->topLeft;

		for (int cg = 1; cg < _glyphs.size(); cg++)
		{
			offset += 6;
			if (_glyphs[cg]->texture != _glyphs[cg - 1]->texture)
				_renderBatches.emplace_back(offset, 6, _glyphs[cg]->texture);
			else
				_renderBatches.back().numVertices += 6;
			vertices[cv++] = _glyphs[cg]->topLeft;
			vertices[cv++] = _glyphs[cg]->bottomLeft;
			vertices[cv++] = _glyphs[cg]->bottomRight;

			vertices[cv++] = _glyphs[cg]->bottomRight;
			vertices[cv++] = _glyphs[cg]->topRight;
			vertices[cv++] = _glyphs[cg]->topLeft;
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
			std::stable_sort(_glyphs.begin(), _glyphs.end(), 
				[](const Glyph* a, const Glyph* b) { return true; });
			break;
		case GlyphSortType::FRONT_TO_BACK:
			std::stable_sort(_glyphs.begin(), _glyphs.end(),
				[](const Glyph* a, const Glyph* b){
					return (a->depth < b->depth);
				});
			break;
		case GlyphSortType::BACK_TO_FRONT:
			std::stable_sort(_glyphs.begin(), _glyphs.end(),
				[](const Glyph* a, const Glyph* b) {
					return (a->depth > b->depth);
				});
			break;
		case GlyphSortType::TEXTURE:
			std::stable_sort(_glyphs.begin(), _glyphs.end(),
				[](const Glyph* a, const Glyph* b) {
					return (a->texture < b->texture);
				});
			break;
		default:
			break;
		}
	}
}
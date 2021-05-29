#pragma once
#include <vector>
#include <glew.h>
#include <glm.hpp>
#include "Vertex.h"


namespace test_Engine
{

	enum class GlyphSortType { NONE, FRONT_TO_BACK, BACK_TO_FRONT, TEXTURE };

	struct Glyph
	{
		GLuint texture;
		float depth;

		Vertex topLeft;
		Vertex bottomLeft;
		Vertex topRight;
		Vertex bottomRight;
	};

	class RenderBatch
	{
	public:
		RenderBatch(GLuint off, GLuint numVer, GLuint text)
			:offset(off), numVertices(numVer), texture(text)
		{}
		GLuint offset;
		GLuint numVertices;
		GLuint texture;
	};

	class SpriteBatch
	{
	public:
		SpriteBatch();
		~SpriteBatch();

		void init();

		void begin(GlyphSortType sortType = GlyphSortType::TEXTURE);
		void end();
		const std::vector<Glyph*>& getGlyphs() { return _glyphs; }
		void draw(const glm::vec4& destRect, const glm::vec4& uvRect, const GLuint texture, const float depth , const glm::vec4& color);
		void renderBatch();

	private:
		void createRenderBatches();
		void createVertexArray();
		void sortGlyphs();

		GlyphSortType _sortType;
		GLuint _VAO, _VBO;
		std::vector<Glyph*> _glyphs;
		std::vector<RenderBatch> _renderBatches;
	};
}

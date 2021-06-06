#pragma once
#include <vector>
#include <glew.h>
#include <glm.hpp>
#include "Vertex.h"


namespace test_Engine
{

	enum class GlyphSortType { NONE, FRONT_TO_BACK, BACK_TO_FRONT, TEXTURE };

	class Glyph
	{
	public:
		Glyph()
			:texture(), depth(), topLeft(), bottomLeft(), topRight(), bottomRight()
		{}
		Glyph(const glm::vec4& destRect, const glm::vec4& uvRect,
			const GLuint texture, const float depth, const glm::vec4& color)
			:texture(texture), depth(depth),
			topLeft    (destRect.x,              destRect.y + destRect.w, color.r, color.g, color.b, color.a, uvRect.x,            uvRect.y + uvRect.w),
			bottomLeft (destRect.x,              destRect.y,              color.r, color.g, color.b, color.a, uvRect.x,            uvRect.y           ),
			topRight   (destRect.x + destRect.z, destRect.y + destRect.w, color.r, color.g, color.b, color.a, uvRect.x + uvRect.z, uvRect.y + uvRect.w),
			bottomRight(destRect.x + destRect.z, destRect.y,              color.r, color.g, color.b, color.a, uvRect.x + uvRect.z, uvRect.y           )
		{}
		Glyph(const glm::vec4& destRect, const glm::vec4& uvRect,
			const GLuint texture, const float depth, const ColorRGBA8& color)
			:texture(texture), depth(depth),
			topLeft    (destRect.x,              destRect.y + destRect.w, color.r, color.g, color.b, color.a, uvRect.x,            uvRect.y + uvRect.w),
			bottomLeft (destRect.x,              destRect.y,              color.r, color.g, color.b, color.a, uvRect.x,            uvRect.y           ),
			topRight   (destRect.x + destRect.z, destRect.y + destRect.w, color.r, color.g, color.b, color.a, uvRect.x + uvRect.z, uvRect.y + uvRect.w),
			bottomRight(destRect.x + destRect.z, destRect.y,              color.r, color.g, color.b, color.a, uvRect.x + uvRect.z, uvRect.y           )
		{}
		Glyph(const glm::vec4& destRect, const glm::vec4& uvRect,
			const GLuint texture, const float depth, const ColorRGBA8& color, float angle);
		GLuint texture;
		float depth;

		Vertex topLeft;
		Vertex bottomLeft;
		Vertex topRight;
		Vertex bottomRight;
	private:
		glm::vec2 rotatePoint(glm::vec2 pos, float angle);
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
		void draw(const glm::vec4& destRect, const glm::vec4& uvRect, const GLuint texture, const float depth, const glm::vec4& color);
		void draw(const glm::vec4& destRect, const glm::vec4& uvRect, const GLuint texture, const float depth, const ColorRGBA8& color);

		void draw(const glm::vec4& destRect, const glm::vec4& uvRect, const GLuint texture, const float depth, const ColorRGBA8& color, float angle);
		void draw(const glm::vec4& destRect, const glm::vec4& uvRect, const GLuint texture, const float depth , const ColorRGBA8& color, const glm::vec2& dir);
		void renderBatch();

	private:
		void createRenderBatches();
		void createVertexArray();
		void sortGlyphs();

		GlyphSortType _sortType;
		GLuint _VAO, _VBO;

		std::vector<Glyph*> _glyphPointers;  // this is for sorting
		std::vector<Glyph> _glyphs;  // this are the actual glyphs
		std::vector<RenderBatch> _renderBatches;
	};
}

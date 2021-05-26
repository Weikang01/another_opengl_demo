#pragma once
#include <glew.h>
#include <glm.hpp>

namespace test_Engine
{
	struct Vertex
	{
		struct Position {
			float x;
			float y;
		} position;
		struct Color {
			GLubyte r;
			GLubyte g;
			GLubyte b;
			GLubyte a;
		} color;
		struct TexCoord {
			float u;
			float v;
		} texCoord;

		void setPosition(const glm::vec2& pos) { position.x = pos.x; position.y = pos.y; }
		void setPosition(const float x, const float y) { position.x = x; position.y = y; }
		void setColor(const glm::vec4& col) { color.r = col.r; color.g = col.g; color.b = col.b; color.a = col.a; }
		void setColor(const float r, const float g, const float b, const float a) 
		{ color.r = r; color.g = g; color.b = b; color.a = a; }
		void setUV(const glm::vec2& uv) { texCoord.u = uv.x; texCoord.v = uv.y; }
		void setUV(const float u, const float v) { texCoord.u = u; texCoord.v = v; }
		
	};
}
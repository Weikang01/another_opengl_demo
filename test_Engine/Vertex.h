#pragma once
#include <glew.h>
#include <glm.hpp>

namespace test_Engine
{
	struct Position {
		Position()
			:x(0.f), y(0.f)
		{}
		Position(float x, float y)
			:x(x), y(y)
		{}
		float x;
		float y;
	};

	struct ColorRGBA8 {
		ColorRGBA8()
			:r(0), g(0), b(0), a(0)
		{}
		ColorRGBA8(GLubyte r, GLubyte g, GLubyte b, GLubyte a)
			:r(r), g(g), b(b), a(a)
		{}
		GLubyte r;
		GLubyte g;
		GLubyte b;
		GLubyte a;
	};

	struct TexCoord {
		TexCoord()
			:u(0), v(0)
		{}
		TexCoord(float u, float v)
			:u(u), v(v)
		{}
		float u;
		float v;
	};

	struct Vertex
	{
		Position position;
		ColorRGBA8 color;
		TexCoord texCoord;

		Vertex()
			:position(), color(), texCoord()
		{}
		Vertex(float x, float y, GLubyte r, GLubyte g, GLubyte b, GLubyte a, float u, float v)
			:position(x, y), color(r, g, b, a), texCoord(u, v)
		{}
		void setPosition(const glm::vec2& pos) { position.x = pos.x; position.y = pos.y; }
		void setPosition(const float x, const float y) { position.x = x; position.y = y; }
		void setColor(const glm::vec4& col) { color.r = col.r; color.g = col.g; color.b = col.b; color.a = col.a; }
		void setColor(const float r, const float g, const float b, const float a) 
		{ color.r = r; color.g = g; color.b = b; color.a = a; }
		void setUV(const glm::vec2& uv) { texCoord.u = uv.x; texCoord.v = uv.y; }
		void setUV(const float u, const float v) { texCoord.u = u; texCoord.v = v; }
	};
}
#pragma once
#include <functional>
#include <glm.hpp>
#include "SpriteBatch.h"
#include "Vertex.h"

namespace test_Engine
{
	class ParticleBatch2D;
	class GLTexture;

	class Particle2D
	{
	public:
		glm::vec2 m_position = glm::vec2(0.f);
		float m_width = 0.f;
		glm::vec2 m_velocity = glm::vec2(0.f);
		ColorRGBA8 m_color;
		float m_life = 0.f;
	};

	extern void defaultParticleUpdate(Particle2D& particle, float deltaTime);

	class ParticleBatch2D
	{
	public:

		ParticleBatch2D();
		~ParticleBatch2D();

		void init(
			int maxParticles, 
			float decayRate, 
			const char* texturePath,
			void (*updateFunc)(Particle2D&, float) = defaultParticleUpdate);

		void update(float deltaTime);

		void draw(SpriteBatch* SpriteBatch);

		void addParticle(
			const glm::vec2& position,
			const glm::vec2& velocity,
			const ColorRGBA8& color,
			const float width);

	private:
		int findFreeParticle();

		float m_decayRate = .1f;
		Particle2D* m_particles = nullptr;
		int m_maxParticles = 0;
		int m_lastFreeParticle = 0;
		unsigned int m_textureID = 0;
		void (*m_updateFunc)(Particle2D&, float) = defaultParticleUpdate;
	};
}
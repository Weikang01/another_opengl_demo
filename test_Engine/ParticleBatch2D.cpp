#include "ParticleBatch2D.h"
#include "ImageLoader.h"
#include "GLTexture.h"

namespace test_Engine
{
	inline void defaultParticleUpdate(Particle2D& particle, float deltaTime)
	{
		particle.m_position += particle.m_velocity * deltaTime;
	}


	ParticleBatch2D::ParticleBatch2D()
	{
		// Empty
	}

	ParticleBatch2D::~ParticleBatch2D()
	{
		delete[] m_particles;
	}

	void ParticleBatch2D::init(int maxParticles,
		float decayRate, const char* texturePath,
		void (*updateFunc)(Particle2D&, float))
	{
		m_maxParticles = maxParticles;
		m_particles = new Particle2D[m_maxParticles];
		m_decayRate = decayRate;
		m_textureID = ImageLoader::loadImage(texturePath).id;
		m_updateFunc = updateFunc;

	}

	void ParticleBatch2D::update(float deltaTime)
	{
		for (size_t i = 0; i < m_maxParticles; i++)
		{
			if (m_particles[i].m_life > 0.f)
			{
				m_updateFunc(m_particles[i], deltaTime);
				m_particles[i].m_life -= m_decayRate * deltaTime;
			}
		}
	}

	void ParticleBatch2D::draw(SpriteBatch* spriteBatch)
	{
		glm::vec4 uvRect(0.f, 0.f, 1.f, 1.f);
		for (size_t i = 0; i < m_maxParticles; i++)
		{
			auto& p = m_particles[i];
			if (p.m_life > 0.f)
			{
				spriteBatch->draw(
					glm::vec4(p.m_position.x, p.m_position.y, p.m_width, p.m_width),
					uvRect, m_textureID, 0.f, p.m_color);
			}
		}
	}

	void ParticleBatch2D::addParticle(
		const glm::vec2& position, const glm::vec2& velocity,
		const ColorRGBA8& color, const float width)
	{
		int particleIndex = findFreeParticle();

		m_particles[particleIndex].m_life = 1.f;
		m_particles[particleIndex].m_position = position;
		m_particles[particleIndex].m_velocity = velocity;
		m_particles[particleIndex].m_color = color;
		m_particles[particleIndex].m_width = width;
	}

	int ParticleBatch2D::findFreeParticle()
	{
		for (unsigned int i = m_lastFreeParticle; i < m_maxParticles; i++)
		{
			if (m_particles[i].m_life <= 0.f)
			{
				m_lastFreeParticle = i;
				return i;
			}
		}

		for (unsigned int i = 0; i < m_lastFreeParticle; i++)
		{
			if (m_particles[i].m_life <= 0.f)
			{
				m_lastFreeParticle = i;
				return i;
			}
		}
		// No particles are free, overwrite first particle
		return 0;
	}
}
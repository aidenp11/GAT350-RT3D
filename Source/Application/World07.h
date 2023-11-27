#pragma once
#include "Framework/World.h"
#include <vector>
#include "Renderer/Renderer.h"
#include "Core/Math/Transform.h"

namespace lady
{

	class World07 : public World
	{
	public:
		const uint32_t INVERT_MASK		 =	(1 << 0);
		const uint32_t GRAYSCALE_MASK	 =	(1 << 1);
		const uint32_t TINT_MASK		 =	(1 << 2);
		const uint32_t GRAIN_MASK		 =	(1 << 3);
		const uint32_t SCANLINE_MASK	 =	(1 << 4);
		const uint32_t CUSTOM_MASK		 =	(1 << 5);

	public:
		bool Initialize() override;
		void Shutdown() override;
		void Update(float dt) override;
		void Draw(Renderer& renderer) override;

	private:
		float m_time;
		float m_speed = 5;
		float m_blend = 1;
		glm::vec3 tintColor{ 1 };
		float grainAmount = 0;
		float scanlineIntensity;

		float kernel;
		float texelSizeX;
		float texelSizeY;

		uint32_t params{ 0 };

		float m_refraction{ 1 };
		glm::vec3 m_ambientLight = glm::vec3( 1.0f );
	};
}
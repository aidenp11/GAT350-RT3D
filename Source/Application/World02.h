#pragma once
#include "Framework/World.h"
#include <glm/glm/glm.hpp>
#include <vector>
#include "Renderer/Renderer.h"

namespace lady
{
	class World02 : public World
	{
	public:
		bool Initialize() override;
		void Shutdown() override;
		void Update(float dt) override;
		void Draw(Renderer& renderer) override;

	private:
		float m_angle = 0;
		float m_time;
		glm::vec2 m_position;
		glm::vec2 m_positions;
		GLuint m_vao;
	};
}
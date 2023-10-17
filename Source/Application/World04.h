#pragma once
#include "Framework/World.h"
#include <vector>
#include "Renderer/Renderer.h"
#include "Core/Math/Transform.h"

namespace lady
{
	class World04 : public World
	{
	public:
		bool Initialize() override;
		void Shutdown() override;
		void Update(float dt) override;
		void Draw(Renderer& renderer) override;

	private:
		float m_time;
		float m_speed = 5;

		Transform m_transform;
		
		GLuint m_vao;
		res_t<Program> m_program;
		res_t<Texture> m_texture;
		res_t<Material> m_material;
		res_t<VertexBuffer> m_vertexBuffer;
		res_t<Model> m_model;
	};
}
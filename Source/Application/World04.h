#pragma once
#include "Framework/World.h"
#include <vector>
#include "Renderer/Renderer.h"
#include "Core/Math/Transform.h"

namespace lady
{
	struct light_t
	{
		enum eType
		{
			Point, Directional, Spot
		};

		eType type;
		glm::vec3 position;
		glm::vec3 direction;
		glm::vec3 color;
		float intensity;
		float range;
		float innerAngle;
		float outerAngle;
	};

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

		//light_t m_light;
		light_t m_lights[3];
		int m_selected = 0;

		glm::vec3 m_ambientLight = glm::vec3( 0.2f );

		Transform m_transform;
		
		GLuint m_vao;
		res_t<Program> m_program;
		res_t<Texture> m_texture;
		res_t<Material> m_material;
		res_t<VertexBuffer> m_vertexBuffer;
		res_t<Model> m_model;
	};
}
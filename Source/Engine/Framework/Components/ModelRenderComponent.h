#pragma once
#include "RenderComponent.h"
#include "Renderer/Model.h"

namespace lady
{
	class ModelRenderComponent : public RenderComponent
	{
	public:
		CLASS_DECLARATION(ModelRenderComponent)

		bool Initialize() override;
		void Update(float dt) override;
		void Draw(class Renderer& renderer) override;

		//virtual float GetRadius() override { return m_model->GetRadius(); }

	public:
		std::string modelName;
		std::string materialName;
		res_t<Model> m_model;
		res_t<Material> m_material;
		bool enableDepth = true;
		GLint cullface = GL_BACK;
	};
}
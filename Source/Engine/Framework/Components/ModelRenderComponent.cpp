#include "ModelRenderComponent.h"
#include "Core/StringUtils.h"
#include "Framework/Actor.h"
#include "Framework/Resource/ResourceManager.h"


namespace lady
{
	CLASS_DEFINITION(ModelRenderComponent)

	bool ModelRenderComponent::Initialize()
	{
		if (!modelName.empty())
		{
			m_model = std::make_shared<Model>();
			m_model->Load(modelName);
			//ADD_RESOURCE(modelName, model);
		}
		if (m_model && !materialName.empty())
		{
			m_model->SetMaterial(GET_RESOURCE(Material, materialName));
		}

		return true;
	}

	void ModelRenderComponent::Update(float dt)
	{

	}

	void ModelRenderComponent::Draw(Renderer& renderer)
	{
		auto material = m_model->GetMaterial();
		material->Bind();
		material->GetProgram()->SetUniform("model", m_owner->transform.GetMatrix());

		glDepthMask(enableDepth);
		glCullFace(cullface);

		m_model->Draw();
	}

	void ModelRenderComponent::Read(const json_t& value)
	{
		READ_DATA(value, modelName);
		READ_DATA(value, materialName);

		READ_DATA(value, enableDepth);
		std::string cullfaceName;
		READ_NAME_DATA(value, "cullface", cullfaceName);
		if (IsEqualIgnoreCase(cullfaceName, "front")) cullface = GL_FRONT;
	}
}

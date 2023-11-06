#include "Transform.h"
#include "Renderer/GUI.h"

namespace lady
{
	void Transform::ProcessGui()
	{
		ImGui::DragFloat3("Position", glm::value_ptr(position), 0.1f);
		if (ImGui::DragFloat3("Rotation", glm::value_ptr(euler), 0.8f))
		{
			rotation = EulerToQuaternion(euler);
		}
		ImGui::DragFloat3("Scale", glm::value_ptr(scale), 0.1f);
	}

	void Transform::Read(const json_t& value)
	{
		READ_DATA(value, position);
		READ_NAME_DATA(value, "rotation", euler);
		READ_DATA(value, scale);

		rotation = EulerToQuaternion(euler);
	}
}

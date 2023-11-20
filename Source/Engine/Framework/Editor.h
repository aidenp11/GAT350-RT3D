#pragma once
namespace lady
{
	class Editor
	{
	public:
		void ProcessGui(class Scene* scene);

	private:
		class Object* m_selected = nullptr;
	};
}

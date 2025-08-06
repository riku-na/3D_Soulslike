#pragma once
#include "../Component/Component.h"

class GameObject
{
public:
	template<typename T, typename...Args>
	std::shared_ptr<T>AddComponent(Args&&...args)
	{
		static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");
		auto comp = std::make_shared<T>(std::forward<Args>(args)...);
		comp->SetOwner(this);
		m_components.push_back(comp);
		return comp;
	}

	void Init()
	{
		for (auto& c : m_components)c->Init();
	}

	void PreDraw()
	{
		for (auto& c : m_components)c->PreDraw();
	}

	void Update()
	{
		for (auto& c : m_components)c->Update();
	}

	void Draw()
	{
		for (auto& c : m_components)c->Draw();
	}

	template<typename T>
	T* GetComponent()
	{
		for (auto& c : m_components)
		{
			if (auto ptr = std::dynamic_pointer_cast<T>(c))
			{
				return ptr.get();
			}
		}

		return nullptr;
	}

	bool IsExpired() const { return m_isExpired; }

private:
	bool m_isExpired = false;
	std::vector<std::shared_ptr<Component>>m_components;
};
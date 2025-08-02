#pragma once

class GameObject;

class Component
{
public:
	virtual void Init() {}
	virtual void Update() {}
	virtual void Draw() {}
	virtual ~Component() {}

	void SetOwner(GameObject* owner) { m_owner = owner; }
protected:
	GameObject* m_owner = nullptr;
};
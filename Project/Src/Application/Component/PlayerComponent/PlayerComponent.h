#pragma once

class TransformComponent;

class PlayerComponent : public Component
{
public:

	void Update()override;
	void Draw()override;

	void SetTransForm(std::shared_ptr<TransformComponent> _transForm)
	{
		m_transForm = _transForm;
	}
private:
	std::shared_ptr<TransformComponent> m_transForm;
};
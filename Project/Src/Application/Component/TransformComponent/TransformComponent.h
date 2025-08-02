#pragma once

class TransformComponent : public Component
{
	Math::Vector3 m_position{ 0,0,0 };
	Math::Vector3 m_rotation{ 0,0,0 };
	Math::Vector3 m_scale = { 1,1,1 };

public:
	void SetPosition(Math::Vector3 _pos) { m_position = _pos; }
	void SetScale(Math::Vector3 _scale) { m_scale = _scale; }

	Math::Matrix GetMatrix()const
	{
		return Math::Matrix::CreateScale(m_scale) *
			Math::Matrix::CreateFromYawPitchRoll(m_rotation.y, m_rotation.x, m_rotation.z) *
			Math::Matrix::CreateTranslation(m_position);
	}
};
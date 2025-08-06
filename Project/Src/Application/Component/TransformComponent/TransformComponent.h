#pragma once

class TransformComponent : public Component
{
	Math::Vector3 m_position{ 0,0,0 };
	Math::Vector3 m_rotation{ 0,0,0 };
	Math::Vector3 m_scale = { 1,1,1 };

public:

	void SetPosition(Math::Vector3 _pos) { m_position = _pos; }
	void SetPositionX(float _x) { m_position.x = _x; }
	void SetPositionY(float _y) { m_position.y = _y; }
	void SetPositionZ(float _z) { m_position.z = _z; }

	void SetScale(Math::Vector3 _scale) { m_scale = _scale; }

	const Math::Vector3& GetPosition()const { return m_position; }

	Math::Matrix GetMatrix()const
	{
		return Math::Matrix::CreateScale(m_scale) *
			Math::Matrix::CreateFromYawPitchRoll(m_rotation.y, m_rotation.x, m_rotation.z) *
			Math::Matrix::CreateTranslation(m_position);
	}
};
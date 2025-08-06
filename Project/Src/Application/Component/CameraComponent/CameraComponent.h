#pragma once

class TransformComponent;

class CameraComponent : public Component
{
public:
	CameraComponent();
	virtual ~CameraComponent() = default;

	void Update()override;

	void SetTarget(std::weak_ptr<TransformComponent> _target) { m_targetTransform = _target; }

	std::shared_ptr < KdCamera>& GetCamera() { return m_camera; }
private:
	std::shared_ptr<KdCamera> m_camera;

	Math::Matrix	m_localPos = Math::Matrix::Identity;
	Math::Matrix	m_rotation = Math::Matrix::Identity;

	std::weak_ptr<TransformComponent> m_targetTransform;

	POINT	m_fixMousePos{};
	Math::Vector3	m_DegAng = Math::Vector3::Zero;
	void UpdateRotateByMouse();
	const Math::Matrix GetRotationMatrix()const;
};
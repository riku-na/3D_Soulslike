#include "CameraComponent.h"
#include "../TransformComponent/TransformComponent.h"
#include "Application/main.h"

CameraComponent::CameraComponent()
{
	if (!m_camera)
	{
		m_camera = std::make_shared<KdCamera>();
	}

	// 注視点
	m_localPos = Math::Matrix::CreateTranslation(0, 1.5f, -5.0f);

	// 初期マウス位置を取得して保存
	GetCursorPos(&m_fixMousePos);
	SetCursorPos(m_fixMousePos.x, m_fixMousePos.y);
}

void CameraComponent::Update()
{
	if (!m_camera) { return; }

	//ターゲット
	Math::Matrix	targetMat = Math::Matrix::Identity;
	if (auto target = m_targetTransform.lock())
	{
		targetMat = Math::Matrix::CreateTranslation(target->GetPosition());
	}

	UpdateRotateByMouse();
	m_rotation = GetRotationMatrix();

	Math::Matrix mat = m_localPos * m_rotation * targetMat;
	m_camera->SetCameraMatrix(mat);

	m_camera->SetToShader();
}

void CameraComponent::UpdateRotateByMouse()
{
	// マウスでカメラを回転させる処理
	POINT _nowPos;
	GetCursorPos(&_nowPos);

	POINT _mouseMove{};
	_mouseMove.x = _nowPos.x - m_fixMousePos.x;
	_mouseMove.y = _nowPos.y - m_fixMousePos.y;

	SetCursorPos(m_fixMousePos.x, m_fixMousePos.y);

	// 実際にカメラを回転させる処理(0.15はただの補正値)
	m_DegAng.x += _mouseMove.y * 0.15f;
	m_DegAng.y += _mouseMove.x * 0.15f;

	// 回転制御
	m_DegAng.x = std::clamp(m_DegAng.x, -45.f, 45.f);
}

const Math::Matrix CameraComponent::GetRotationMatrix() const
{
	return Math::Matrix::CreateFromYawPitchRoll(
		DirectX::XMConvertToRadians(m_DegAng.y),
		DirectX::XMConvertToRadians(m_DegAng.x),
		DirectX::XMConvertToRadians(m_DegAng.z));
}

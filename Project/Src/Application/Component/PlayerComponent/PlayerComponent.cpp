#include "PlayerComponent.h"
#include "../TransformComponent/TransformComponent.h"

void PlayerComponent::Update()
{
	if (GetAsyncKeyState('W') & 0x8000)
	{
		m_transForm->SetPositionZ(m_transForm->GetPosition().z + 0.01f);
	}

	if (GetAsyncKeyState('S') & 0x8000)
	{
		m_transForm->SetPositionZ(m_transForm->GetPosition().z - 0.01f);
	}

	if (GetAsyncKeyState('A') & 0x8000)
	{
		m_transForm->SetPositionX(m_transForm->GetPosition().x + 0.01f);
	}

	if (GetAsyncKeyState('D') & 0x8000)
	{
		m_transForm->SetPositionX(m_transForm->GetPosition().x - 0.01f);
	}
}

void PlayerComponent::Draw()
{
}

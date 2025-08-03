#include "AnimatorComponent.h"

void AnimatorComponent::Update()
{
	if (auto model = m_wpModelWork.lock())
	{
		m_animator.AdvanceTime(model->WorkNodes(), m_speed);
	}
}

void AnimatorComponent::SetAnimation(const std::string& _animName, bool _isLoop)
{
	if (auto model = m_wpModelWork.lock())
	{
		m_animator.SetAnimation(model->GetAnimation(_animName), _isLoop);
	}
}

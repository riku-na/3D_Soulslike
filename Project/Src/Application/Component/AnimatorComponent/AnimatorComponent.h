#pragma once

class SkinnedMeshRendererComponent;

class AnimatorComponent : public Component
{
public:
	AnimatorComponent() = default;
	~AnimatorComponent() = default;

	void Update()override;

	void SetAnimation(const std::string& _animName, bool _isLoop = true);

	void SetSpeed(float _speed)
	{
		m_speed = _speed;
	}

	void SetTargetModel(std::weak_ptr<KdModelWork> _wpModelWork)
	{
		m_wpModelWork = _wpModelWork;
	}

private:
	float m_speed = 1.0f;

	KdAnimator m_animator;
	std::weak_ptr<KdModelWork> m_wpModelWork;
};
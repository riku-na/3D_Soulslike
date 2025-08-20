#pragma once
#include "../StateMachineComponent/StateMachineComponent.h"
#include "../../Utility/InputManager.h"

class TransformComponent;
class CameraComponent;
class AnimatorComponent;

class PlayerComponent : public Component
{
public:
	PlayerComponent();
	~PlayerComponent() = default;

	void SetComponents(std::shared_ptr<TransformComponent> _transform,
		std::shared_ptr<CameraComponent> _camera,
		std::shared_ptr<AnimatorComponent> _animator,
		std::shared_ptr<StateMachineComponent> _stateMachine
	);

	void Update()override;
	void Draw()override;

	void InitStateMachine();

private:

	std::shared_ptr<TransformComponent> m_spTransform;
	std::shared_ptr<CameraComponent> m_spCamera;
	std::shared_ptr<AnimatorComponent> m_spAnimator;
	std::shared_ptr<StateMachineComponent> m_spStateMachineComponent;

	static constexpr float WALK_SPEED = 1.5f;
	static constexpr float RUN_SPEED = 3.5f;

	struct PlayerCtx
	{
		std::shared_ptr<TransformComponent> transform;
		std::shared_ptr<AnimatorComponent> animator;
		std::shared_ptr<CameraComponent> camera;
	};

	PlayerCtx ctx{};

	static State MakeIdle();
	static State MakeWalk();
	static State MakeRun();

	static State MakeAttack_1();
};
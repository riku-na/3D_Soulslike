#include "PlayerComponent.h"
#include "../TransformComponent/TransformComponent.h"
#include "../CameraComponent/CameraComponent.h"
#include "../AnimatorComponent/AnimatorComponent.h"


PlayerComponent::PlayerComponent()
{

}

void PlayerComponent::SetComponents(std::shared_ptr<TransformComponent> _transform, std::shared_ptr<CameraComponent> _camera, std::shared_ptr<AnimatorComponent> _animator, std::shared_ptr<StateMachineComponent> _stateMachine)
{
	m_spTransform = _transform;
	m_spCamera = _camera;
	m_spAnimator = _animator;
	m_spStateMachineComponent = _stateMachine;
}

void PlayerComponent::Update()
{
	UpdateCtx u{ deltatime::GetDeltaTime(), &ctx };
	m_spStateMachineComponent->Update(u);


	////移動
	//{
	//	Math::Vector3 _pos = m_spTransform->GetPosition();
	//	Math::Vector3 _forwordVec = m_spCamera->GetCamera()->GetCameraMatrix().Backward();
	//	Math::Vector3 _rightVec = m_spCamera->GetCamera()->GetCameraMatrix().Right();

	//	_forwordVec.y = 0;
	//	_rightVec.y = 0;

	//	float _moveSpeed = input::IsKeyHeld(VK_SHIFT) ? RUN_SPEED : WALK_SPEED;

	//	if (input::IsKeyHeld('W'))
	//	{
	//		_pos += _forwordVec * _moveSpeed * deltatime::GetDeltaTime();
	//	}

	//	if (input::IsKeyHeld('S'))
	//	{
	//		_pos -= _forwordVec * _moveSpeed * deltatime::GetDeltaTime();
	//	}

	//	if (input::IsKeyHeld('D'))
	//	{
	//		_pos += _rightVec * _moveSpeed * deltatime::GetDeltaTime();
	//	}

	//	if (input::IsKeyHeld('A'))
	//	{
	//		_pos -= _rightVec * _moveSpeed * deltatime::GetDeltaTime();
	//	}

	//	m_spTransform->SetPosition(_pos);
	//}

	////向き変更
	//{
	//	Math::Vector3 _forwordVec = m_spCamera->GetCamera()->GetCameraMatrix().Forward();
	//	float yaw = std::atan2(_forwordVec.x, _forwordVec.z);
	//	m_spTransform->SetRotation({ 0, yaw, 0 });
	//}

	////アニメーション変更
	//{
	//	if (input::IsKeyHeld('W') || input::IsKeyHeld('A') || input::IsKeyHeld('S') || input::IsKeyHeld('D'))
	//	{
	//		m_spAnimator->SetAnimation(input::IsKeyHeld(VK_SHIFT) ? "Run" : "Walk", true);
	//	}
	//	else
	//	{
	//		m_spAnimator->SetAnimation("Idle", false);
	//	}
	//}
}

void PlayerComponent::Draw()
{
}

void PlayerComponent::InitStateMachine()
{
	ctx.animator = m_spAnimator;
	ctx.camera = m_spCamera;
	ctx.transform = m_spTransform;

	m_spStateMachineComponent->AddState(MakeIdle());
	m_spStateMachineComponent->AddState(MakeWalk());
	m_spStateMachineComponent->AddState(MakeRun());
	m_spStateMachineComponent->AddState(MakeAttack_1());
	m_spStateMachineComponent->SetInitial("Idle");
}

State PlayerComponent::MakeIdle()
{
	{
		State s;
		s.name = "Idle";

		s.onEnter = [](const UpdateCtx& c)
			{
				auto* p = reinterpret_cast<PlayerCtx*>(c.blackboard);

				if (p->animator)
					p->animator->SetAnimation("Idle");
			};

		s.onUpdate = [](const UpdateCtx& c)
			{

			};

		s.transitions =
		{
			Transition{ "Attack_1", [](const UpdateCtx& c)
			{
			 auto* p = reinterpret_cast<PlayerCtx*>(c.blackboard);

			 bool transitionFlag = input::IsKeyTriggered(VK_LBUTTON);
			 return transitionFlag;
			 }},
			Transition{ "Run", [](const UpdateCtx& c)
			{
			 auto* p = reinterpret_cast<PlayerCtx*>(c.blackboard);

			 bool isMove = (input::IsKeyHeld('W') || input::IsKeyHeld('A') || input::IsKeyHeld('S') || input::IsKeyHeld('D'));
			 bool isDash = input::IsKeyHeld(VK_SHIFT);
			 bool transitionFlag = isMove && isDash;
			 return transitionFlag;
			 }},
			Transition{ "Walk", [](const UpdateCtx& c)
			{
			 auto* p = reinterpret_cast<PlayerCtx*>(c.blackboard);

			 bool isMove = (input::IsKeyHeld('W') || input::IsKeyHeld('A') || input::IsKeyHeld('S') || input::IsKeyHeld('D'));
			 bool isDash = input::IsKeyHeld(VK_SHIFT);
			 bool transitionFlag = isMove && (!isDash);
			 return transitionFlag;
			 }},
		};
		return s;
	}
}

State PlayerComponent::MakeWalk()
{

	State s;
	s.name = "Walk";

	s.onEnter = [](const UpdateCtx& c)
		{
			auto* p = reinterpret_cast<PlayerCtx*>(c.blackboard);

			if (p->animator)
				p->animator->SetAnimation("Walk");
		};

	s.onUpdate = [](const UpdateCtx& c)
		{
			auto* p = reinterpret_cast<PlayerCtx*>(c.blackboard);

			//移動
			{
				Math::Vector3 _pos = p->transform->GetPosition();
				Math::Vector3 _forwordVec = p->camera->GetCamera()->GetCameraMatrix().Backward();
				Math::Vector3 _rightVec = p->camera->GetCamera()->GetCameraMatrix().Right();

				_forwordVec.y = 0;
				_rightVec.y = 0;

				float _moveSpeed = WALK_SPEED;

				if (input::IsKeyHeld('W'))
				{
					_pos += _forwordVec * _moveSpeed * deltatime::GetDeltaTime();
				}

				if (input::IsKeyHeld('S'))
				{
					_pos -= _forwordVec * _moveSpeed * deltatime::GetDeltaTime();
				}

				if (input::IsKeyHeld('D'))
				{
					_pos += _rightVec * _moveSpeed * deltatime::GetDeltaTime();
				}

				if (input::IsKeyHeld('A'))
				{
					_pos -= _rightVec * _moveSpeed * deltatime::GetDeltaTime();
				}

				p->transform->SetPosition(_pos);
			}

			//向き変更
			{
				Math::Vector3 _forwordVec = p->camera->GetCamera()->GetCameraMatrix().Forward();
				float yaw = std::atan2(_forwordVec.x, _forwordVec.z);
				p->transform->SetRotation({ 0, yaw, 0 });
			}
		};


	s.transitions =
	{
		Transition{ "Attack_1", [](const UpdateCtx& c)
			{
			 auto* p = reinterpret_cast<PlayerCtx*>(c.blackboard);

			 bool transitionFlag = input::IsKeyTriggered(VK_LBUTTON);
			 return transitionFlag;
			 }},
		Transition{ "Run", [](const UpdateCtx& c)
		{
		 auto* p = reinterpret_cast<PlayerCtx*>(c.blackboard);

		 bool isMove = (input::IsKeyHeld('W') || input::IsKeyHeld('A') || input::IsKeyHeld('S') || input::IsKeyHeld('D'));
		 bool isDash = input::IsKeyHeld(VK_SHIFT);
		 bool transitionFlag = isMove && isDash;
		 return transitionFlag;
		 }},
		Transition{ "Idle", [](const UpdateCtx& c)
		{
		 auto* p = reinterpret_cast<PlayerCtx*>(c.blackboard);

		 bool isMove = (input::IsKeyHeld('W') || input::IsKeyHeld('A') || input::IsKeyHeld('S') || input::IsKeyHeld('D'));
		 bool transitionFlag = !isMove;
		 return transitionFlag;
		 }},
	};
	return s;
}


State PlayerComponent::MakeRun()
{
	{
		State s;
		s.name = "Run";

		s.onEnter = [](const UpdateCtx& c)
			{
				auto* p = reinterpret_cast<PlayerCtx*>(c.blackboard);

				if (p->animator)
					p->animator->SetAnimation("Run");
			};

		s.onUpdate = [](const UpdateCtx& c)
			{
				auto* p = reinterpret_cast<PlayerCtx*>(c.blackboard);

				//移動
				{
					Math::Vector3 _pos = p->transform->GetPosition();
					Math::Vector3 _forwordVec = p->camera->GetCamera()->GetCameraMatrix().Backward();
					Math::Vector3 _rightVec = p->camera->GetCamera()->GetCameraMatrix().Right();

					_forwordVec.y = 0;
					_rightVec.y = 0;

					float _moveSpeed = RUN_SPEED;

					if (input::IsKeyHeld('W'))
					{
						_pos += _forwordVec * _moveSpeed * deltatime::GetDeltaTime();
					}

					if (input::IsKeyHeld('S'))
					{
						_pos -= _forwordVec * _moveSpeed * deltatime::GetDeltaTime();
					}

					if (input::IsKeyHeld('D'))
					{
						_pos += _rightVec * _moveSpeed * deltatime::GetDeltaTime();
					}

					if (input::IsKeyHeld('A'))
					{
						_pos -= _rightVec * _moveSpeed * deltatime::GetDeltaTime();
					}

					p->transform->SetPosition(_pos);
				}

				//向き変更
				{
					Math::Vector3 _forwordVec = p->camera->GetCamera()->GetCameraMatrix().Forward();
					float yaw = std::atan2(_forwordVec.x, _forwordVec.z);
					p->transform->SetRotation({ 0, yaw, 0 });
				}
			};

		s.transitions =
		{
			Transition{ "Attack_1", [](const UpdateCtx& c)
			{
			 auto* p = reinterpret_cast<PlayerCtx*>(c.blackboard);

			 bool transitionFlag = input::IsKeyTriggered(VK_LBUTTON);
			 return transitionFlag;
			 }},
			Transition{ "Walk", [](const UpdateCtx& c)
			{
			 auto* p = reinterpret_cast<PlayerCtx*>(c.blackboard);

			 bool isMove = (input::IsKeyHeld('W') || input::IsKeyHeld('A') || input::IsKeyHeld('S') || input::IsKeyHeld('D'));
			 bool isDash = input::IsKeyHeld(VK_SHIFT);
			 bool transitionFlag = isMove && !isDash;
			 return transitionFlag;
			 }},
			Transition{ "Idle", [](const UpdateCtx& c)
			{
			 auto* p = reinterpret_cast<PlayerCtx*>(c.blackboard);

			 bool isMove = (input::IsKeyHeld('W') || input::IsKeyHeld('A') || input::IsKeyHeld('S') || input::IsKeyHeld('D'));
			 bool transitionFlag = !isMove;
			 return transitionFlag;
			 }},
		};
		return s;
	}
}

State PlayerComponent::MakeAttack_1()
{
	State s;
	s.name = "Attack_1";

	s.onEnter = [](const UpdateCtx& c)
		{
			auto* p = reinterpret_cast<PlayerCtx*>(c.blackboard);

			if (p->animator)
				p->animator->SetAnimation("Hit_3", false);
		};

	s.onUpdate = [](const UpdateCtx& c)
		{
			auto* p = reinterpret_cast<PlayerCtx*>(c.blackboard);
		};

	s.transitions =
	{
			Transition{ "Idle", [](const UpdateCtx& c)
			{
			 auto* p = reinterpret_cast<PlayerCtx*>(c.blackboard);

			 //アニメーションの再生が終わったら
			 bool transitionFlag = p->animator->GetAnimator().IsAnimationEnd();
			 return transitionFlag;
			 }},
	};
	return s;
}


#include "GameScene.h"
#include"../SceneManager.h"

#include "../../Component/TransformComponent/TransformComponent.h"
#include "../../Component/MeshRendererComponent/MeshRendererComponent.h"
#include "../../Component/SkinnedMeshRendererComponent/SkinnedMeshRendererComponent.h"
#include "../../Component/AnimatorComponent/AnimatorComponent.h"
#include "../../Component/CameraComponent/CameraComponent.h"
#include "../../Component/PlayerComponent/PlayerComponent.h"

void GameScene::Event()
{
	if (GetAsyncKeyState('T') & 0x8000)
	{
		SceneManager::Instance().SetNextScene
		(
			SceneManager::SceneType::Title
		);
	}

}

void GameScene::Init()
{


	//オブジェクト
	auto plane = std::make_shared<GameObject>();
	plane->AddComponent<TransformComponent>();
	auto planeMeshRenderer = plane->AddComponent<MeshRendererComponent>();
	planeMeshRenderer->Load("Asset/Models/Plane/Plane.gltf");
	AddObject(plane);

	auto knight = std::make_shared<GameObject>();

	auto knightTrans = knight->AddComponent<TransformComponent>();
	auto knightMesh = knight->AddComponent<SkinnedMeshRendererComponent>();
	auto knightAnim = knight->AddComponent<AnimatorComponent>();
	auto knightPlayer = knight->AddComponent<PlayerComponent>();
	auto knightStateMachine = knight->AddComponent<StateMachineComponent>();

	knightMesh->Load("Asset/Models/Knight/Knight.gltf");
	knightAnim->SetTargetModel(knightMesh->GetModelWork());
	knightAnim->SetAnimation("Idle", true);

	AddObject(knight);
	//カメラ
	auto camera = std::make_shared<GameObject>();
	auto cameraComponent = camera->AddComponent<CameraComponent>();
	cameraComponent->SetTarget(knightTrans);
	AddObject(camera);


	knightPlayer->SetComponents(knightTrans, cameraComponent, knightAnim, knightStateMachine);
	knightPlayer->InitStateMachine();
}

#include "GameScene.h"
#include"../SceneManager.h"

#include "../../Component/TransformComponent/TransformComponent.h"
#include "../../Component/MeshRendererComponent/MeshRendererComponent.h"
#include "../../Component/SkinnedMeshRendererComponent/SkinnedMeshRendererComponent.h"
#include "../../Component/AnimatorComponent/AnimatorComponent.h"

void GameScene::Event()
{
	if (GetAsyncKeyState('T') & 0x8000)
	{
		SceneManager::Instance().SetNextScene
		(
			SceneManager::SceneType::Title
		);
	}

	m_camera->SetToShader();
}

void GameScene::Init()
{
	//カメラ
	m_camera = std::make_shared<KdCamera>();
	m_camera->SetCameraMatrix(Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(15)) * Math::Matrix::CreateTranslation(0, 3, -5));

	//オブジェクト
	auto plane = std::make_shared<GameObject>();
	plane->AddComponent<TransformComponent>();
	auto planeMeshRenderer = plane->AddComponent<MeshRendererComponent>();
	planeMeshRenderer->Load("Asset/Models/Plane/Plane.gltf");
	AddObject(plane);

	auto knight = std::make_shared<GameObject>();

	knight->AddComponent<TransformComponent>();
	auto knightMesh = knight->AddComponent<SkinnedMeshRendererComponent>();
	auto knightAnim = knight->AddComponent<AnimatorComponent>();

	knightMesh->Load("Asset/Models/test/Knight.gltf");
	knightAnim->SetTargetModel(knightMesh->GetModelWork());
	knightAnim->SetAnimation("Idle", true);

	AddObject(knight);
}

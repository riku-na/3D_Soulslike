#include "SceneManager.h"

#include "BaseScene/BaseScene.h"
#include "GameScene/GameScene.h"
#include "TitleScene/TitleScene.h"

void SceneManager::Update()
{
	m_currentScene->Update();
}

void SceneManager::Draw()
{
	//KdShaderManager::Instance().m_skyboxShader.Draw();

	if (m_currentScene)m_currentScene->Draw();//描画予約
	Renderer::Instance().DrawAll();//ソート後まとめて描画
}


void SceneManager::ChangeScene(SceneType _sceneType)
{
	// 次のシーンを作成し、現在のシーンにする
	switch (_sceneType)
	{
	case SceneType::Title:
		m_currentScene = std::make_shared<TitleScene>();
		break;
	case SceneType::Game:
		m_currentScene = std::make_shared<GameScene>();
		break;
	}

	// 現在のシーン情報を更新
	m_currentSceneType = _sceneType;
}

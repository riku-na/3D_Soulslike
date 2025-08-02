#pragma once

#include"../BaseScene/BaseScene.h"

class GameScene : public BaseScene
{
public:

	GameScene() { Init(); }
	~GameScene() {}

private:
	std::shared_ptr<KdCamera> m_camera;

	void Event() override;
	void Init()  override;
};

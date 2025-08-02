#pragma once

class BaseScene
{
public:

	BaseScene() { Init(); }
	virtual ~BaseScene() {}

	void Update();

	void Draw();

	// オブジェクトリストに追加
	void AddObject(const std::shared_ptr<GameObject>& _obj)
	{
		m_objList.push_back(_obj);
	}

protected:

	// 継承先シーンで必要ならオーバーライドする
	virtual void Event();
	virtual void Init();

	// 全オブジェクトのアドレスをリストで管理
	std::list<std::shared_ptr<GameObject>> m_objList;
};

#pragma once
#include "../IRenderableComponent/IRenderableComponent.h"

class SkinnedMeshRendererComponent : public Component, public IRenderableComponent
{
public:
	SkinnedMeshRendererComponent();
	~SkinnedMeshRendererComponent() = default;

	void Load(const std::string& _path);

	void Update()override;

	void Draw()override;

	void DrawWithMaterial(Material* mat)override;

	bool GetCastShadows()const override { return m_castShadows; }

	std::weak_ptr<KdModelWork> GetModelWork() const { return m_model; }
private:
	Material m_material;
	std::shared_ptr<KdModelWork> m_model;
	KdAnimator m_animator;
	bool m_castShadows = true;
};
#pragma once
#include "../IRenderableComponent/IRenderableComponent.h"

class MeshRendererComponent : public Component, public IRenderableComponent
{
public:
	void Load(const std::string& _path);

	void Draw()override;

	void DrawWithMaterial(Material* mat)override;

	bool GetCastShadows()const override { return m_castShadows; }
private:
	Material m_material;
	KdModelData m_model;
	bool m_castShadows = true;
};
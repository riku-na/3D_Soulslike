#include "SkinnedMeshRendererComponent.h"
#include "../TransformComponent/TransformComponent.h"
#include "../../Renderer/Renderer.h"

SkinnedMeshRendererComponent::SkinnedMeshRendererComponent()
{
	m_model = std::make_shared<KdModelWork>();
}

void SkinnedMeshRendererComponent::Load(const std::string& _path)
{
	m_model->SetModelData(_path);

	m_material.m_shaderType = ShaderType::Lit;
}

void SkinnedMeshRendererComponent::Draw()
{
	Renderer::Instance().AddRenderCommand(this, &m_material);
}

void SkinnedMeshRendererComponent::DrawWithMaterial(Material* mat)
{
	auto transform = m_owner->GetComponent<TransformComponent>();
	if (transform)
	{
		KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, transform->GetMatrix());
	}
}

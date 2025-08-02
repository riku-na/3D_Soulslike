#include "MeshRendererComponent.h"
#include "../TransformComponent/TransformComponent.h"
#include "../../Renderer/Renderer.h"

void MeshRendererComponent::Load(const std::string& _path)
{
	m_model.Load(_path);

	m_material.m_shaderType = ShaderType::Lit;
}

void MeshRendererComponent::Draw()
{

	Renderer::Instance().AddRenderCommand(this, &m_material);

}

void MeshRendererComponent::DrawWithMaterial(Material* mat)
{
	auto transform = m_owner->GetComponent<TransformComponent>();
	if (transform)
	{
		KdShaderManager::Instance().m_StandardShader.DrawModel(m_model, transform->GetMatrix());
	}
}

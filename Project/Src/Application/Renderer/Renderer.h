#pragma once
#include "../Component/IRenderableComponent/IRenderableComponent.h"

struct RenderCommand
{
	IRenderableComponent* renderer = nullptr;
	Material* material = nullptr;
};

class Renderer
{
public:

	//描画リクエスト登録
	void AddRenderCommand(IRenderableComponent* renderer, Material* mat)
	{
		m_renderQueue.push_back({ renderer,mat });

		//影生成オブジェクトか
		if (renderer->GetCastShadows())
		{
			m_renderQueue.push_back({ renderer, &m_shadowMat });
		}
	}

	//ソート&
	void DrawAll()
	{
		std::sort(m_renderQueue.begin(), m_renderQueue.end(),
			[](const RenderCommand& a, const RenderCommand& b)
			{
				return a.material->m_shaderType < b.material->m_shaderType;
			});

		ShaderType currentShader = ShaderType::None;

		for (auto& cmd : m_renderQueue)
		{
			ShaderType targetShader = cmd.material->m_shaderType;

			if (targetShader != currentShader)
			{
				if (currentShader != ShaderType::None)
					EndShader(currentShader);

				BeginShader(targetShader);
				currentShader = targetShader;
			}

			cmd.renderer->DrawWithMaterial(cmd.material);
		}

		if (currentShader != ShaderType::None)
			EndShader(currentShader);

		m_renderQueue.clear();
	}

private:
	std::vector<RenderCommand>m_renderQueue;
	Material m_shadowMat;

	void BeginShader(ShaderType type)
	{
		auto& shader = KdShaderManager::Instance().m_StandardShader;
		auto& post = KdShaderManager::Instance().m_postProcessShader;

		switch (type)
		{
		case ShaderType::DepthMap:shader.BeginGenerateDepthMapFromLight(); break;
		case ShaderType::UnLit: shader.BeginUnLit(); break;
		case ShaderType::Lit: shader.BeginLit(); break;
		case ShaderType::Bright: post.BeginBright(); break;
		}
	}

	void EndShader(ShaderType type)
	{
		auto& shader = KdShaderManager::Instance().m_StandardShader;
		auto& post = KdShaderManager::Instance().m_postProcessShader;

		switch (type)
		{
		case ShaderType::DepthMap:shader.EndGenerateDepthMapFromLight(); break;
		case ShaderType::UnLit: shader.EndUnLit(); break;
		case ShaderType::Lit: shader.EndLit(); break;
		case ShaderType::Bright: post.EndBright(); break;
		}
	}


	Renderer() { m_shadowMat.m_shaderType = ShaderType::DepthMap; }
	~Renderer() = default;
public:
	static Renderer& Instance()
	{
		static Renderer instance;
		return instance;
	}
};
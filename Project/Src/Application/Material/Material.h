#pragma once

enum class ShaderType
{
	None,
	DepthMap,
	UnLit,
	Lit,
	Bright
};

class Material
{
public:
	ShaderType m_shaderType = ShaderType::None;
};
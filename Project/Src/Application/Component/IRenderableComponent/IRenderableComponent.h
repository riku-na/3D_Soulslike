#pragma once

class IRenderableComponent
{
public:
	virtual ~IRenderableComponent() = default;

	virtual void DrawWithMaterial(Material* mat) = 0;
	virtual bool GetCastShadows()const = 0;
};
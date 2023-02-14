#pragma once
#include "stdafx.h"
#include "RenderSystem.h"
#include "TextureSystem.h"

class EngineSystem : public Singleton<EngineSystem>
{

private:
	void CreateRenderSystem();
	void CreateTextureSystem();
public:
	void Update();
	void Render();
	RenderSystem* GetRenderSystem();
	TextureSystem* GetTextureSystem();
public:
	EngineSystem();
	~EngineSystem();

	/*Variable Block*/
private:
	RenderSystem* m_pRenderSystem;
	TextureSystem* m_pTextureSystem;
};
#define _EngineSystem Singleton<EngineSystem>::GetInstance()

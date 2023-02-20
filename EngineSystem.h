#pragma once
#include "stdafx.h"
#include "RenderSystem.h"
#include "TextureSystem.h"
#include "MeshSystem.h"


class EngineSystem : public Singleton<EngineSystem>
{
private:
	void CreateRenderSystem();
	void CreateTextureSystem();
	void CreateMeshSystem();
public:
	void Update();
	void Render();
	RenderSystem* GetRenderSystem();
	TextureSystem* GetTextureSystem();
	MeshSystem* GetMeshSystem();
public:
	EngineSystem();
	~EngineSystem();

	/*Variable Block*/
private:
	RenderSystem* m_pRenderSystem;
	TextureSystem* m_pTextureSystem;
	MeshSystem* m_pMeshSystem;
};
#define _EngineSystem Singleton<EngineSystem>::GetInstance()

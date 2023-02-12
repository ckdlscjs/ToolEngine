#pragma once
#include "stdafx.h"
#include "RenderSystem.h"
class EngineSystem : public Singleton<EngineSystem>
{

private:
	void CreateRenderSystem();
public:
	void Update();
	RenderSystem* GetRenderSystem();
public:
	EngineSystem();
	~EngineSystem();

	/*Variable Block*/
private:
	RenderSystem* m_pRenderSystem;
};
#define _EngineSystem Singleton<EngineSystem>::GetInstance()

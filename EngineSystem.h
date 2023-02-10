#pragma once
#include "stdafx.h"
#include "RenderSystem.h"
class EngineSystem
{

private:
	void CreateRenderSystem();
public:
	EngineSystem();
	~EngineSystem();
	
	/*Variable Block*/
private:
	RenderSystem* m_pRenderSystem;
};


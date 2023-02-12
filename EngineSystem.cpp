#include "EngineSystem.h"

void EngineSystem::CreateRenderSystem()
{
	m_pRenderSystem = new RenderSystem();
}

void EngineSystem::Update()
{
	m_pRenderSystem->Frame();
	m_pRenderSystem->Render();
}

void EngineSystem::Present()
{
	m_pRenderSystem->PostRender();
}

RenderSystem* EngineSystem::GetRenderSystem()
{
	return m_pRenderSystem;
}

EngineSystem::EngineSystem()
{
	CreateRenderSystem();
}

EngineSystem::~EngineSystem()
{
	
	if(m_pRenderSystem != nullptr) delete m_pRenderSystem;
}

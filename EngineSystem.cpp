#include "EngineSystem.h"


void EngineSystem::CreateRenderSystem()
{
	m_pRenderSystem = new RenderSystem();
}

void EngineSystem::Update()
{
	m_pRenderSystem->Update();
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

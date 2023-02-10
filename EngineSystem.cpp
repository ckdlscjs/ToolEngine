#include "EngineSystem.h"

void EngineSystem::CreateRenderSystem()
{
	m_pRenderSystem = new RenderSystem();
}

EngineSystem::EngineSystem()
{
	CreateRenderSystem();
}

EngineSystem::~EngineSystem()
{
}

#include "EngineSystem.h"


void EngineSystem::CreateRenderSystem()
{
	m_pRenderSystem = new RenderSystem();
	if (m_pRenderSystem == nullptr)
		throw std::exception("RenderSystem not create successfully");
}

void EngineSystem::CreateTextureSystem()
{
	m_pTextureSystem = new TextureSystem();
	if (m_pTextureSystem == nullptr)
		throw std::exception("TextureSystem not create successfully");
}

void EngineSystem::CreateMeshSystem()
{
	m_pMeshSystem = new MeshSystem();
	if (m_pMeshSystem == nullptr)
		throw std::exception("MeshSystem not create successfully");
}

void EngineSystem::Update()
{
	m_pRenderSystem->Update();
}

void EngineSystem::Render()
{
	m_pRenderSystem->Render();
}


RenderSystem* EngineSystem::GetRenderSystem()
{
	return m_pRenderSystem;
}

TextureSystem* EngineSystem::GetTextureSystem()
{
	return m_pTextureSystem;
}

EngineSystem::EngineSystem()
{
	CreateRenderSystem();
	CreateTextureSystem();
	CreateMeshSystem();
}

EngineSystem::~EngineSystem()
{
	if (m_pRenderSystem != nullptr) delete m_pRenderSystem;
	if (m_pTextureSystem != nullptr) delete m_pTextureSystem;
	if (m_pMeshSystem != nullptr) delete m_pMeshSystem;
}

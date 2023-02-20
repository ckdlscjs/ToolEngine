#pragma once
#include "EngineSystem.h"
#include "Texture.h"
class Material
{

private:
	Texture** m_ListTextures;
	friend class ToolSystemMap;
};


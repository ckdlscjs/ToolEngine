#pragma once
#include "EngineSystem.h"
class Mesh
{
public:
	VertexBuffer* GetVertexBuffer();
	IndexBuffer* GetIndexBuffer();
//public:
//	Mesh();
//	~Mesh();
public:
	VertexBuffer* m_pVertexBuffer;
	IndexBuffer* m_pIndexBuffer;
};


#pragma once
#include "stdafx.h"
class VertexBuffer
{
	VertexBuffer();
	~VertexBuffer();
private:
	ID3D11Buffer* m_Buffer;
	ID3D11InputLayout* m_Inputlayout;
};


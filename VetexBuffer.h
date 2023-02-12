#pragma once
#include "stdafx.h"
class VetexBuffer
{
	VetexBuffer();
	~VetexBuffer();
private:
	ID3D11Buffer* m_Buffer;
	ID3D11InputLayout* m_Inputlayout;
};


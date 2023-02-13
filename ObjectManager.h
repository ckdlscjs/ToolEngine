#pragma once
#include "stdafx.h"
#include "TextureSystem.h"

class ObjectManager : public Singleton<ObjectManager>
{
	
};
#define _ObjectManager Singleton<ObjectManager>::GetInstance()

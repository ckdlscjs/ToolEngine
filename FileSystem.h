#pragma once
#include "stdafx.h"

class FileSystem : public Singleton<FileSystem>
{
public:
	FileSystem();
	~FileSystem();

};
#define _InputSystem Singleton<InputSystem>::GetInstance()

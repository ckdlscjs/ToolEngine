#include "EntrySystem.h"
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
#include "FBXSample.h"
#ifdef _DEBUG
#define new new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#endif
int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(519339);
	fbxsample::FBXSample a;
	a.sample();
	EntrySystem entrypoint;
	entrypoint.Initialize();
	while (entrypoint.IsRun())
	{
		if (!entrypoint.BroadCast())
			break;
	}

	return 0;
}
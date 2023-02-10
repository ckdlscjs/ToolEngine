#include "EntrySystem.h"
int main()
{
	EntrySystem entrypoint;
	entrypoint.Initialize();
	while (entrypoint.IsRun())
	{
		if (!entrypoint.BroadCast())
			break;
	}
	return 0;
}
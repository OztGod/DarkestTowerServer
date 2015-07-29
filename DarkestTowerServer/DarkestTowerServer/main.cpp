#include "DarkestTowerApplication.h"

int main()
{
	DarkestTowerApplication app(4);

	if (!app.init())
	{
		printf("app init error.");
		return -1;
	}

	return app.run();
}
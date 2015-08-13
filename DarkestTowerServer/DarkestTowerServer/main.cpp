#include "HmmoApplication.h"

int main()
{
	srand(time(nullptr));

	HmmoApplication app(4);

	if (!app.init())
	{
		printf("app init error.");
		return -1;
	}

	return app.run();
}
#pragma once
#include "Skylark.h"
#include <vector>

class DarkestTowerApplication : public skylark::Application
{
public:
	DarkestTowerApplication(int threadNum_);
	~DarkestTowerApplication();

	int run() override;
	bool init() override;

private:
	skylark::CompletionPort* mainPort;
	int threadNum;
	skylark::Socket* listen;
	std::vector<skylark::IOThread*> threads;
};
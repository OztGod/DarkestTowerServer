#pragma once
#include "Skylark.h"
#include <vector>

class HmmoApplication : public skylark::Application
{
public:
	HmmoApplication(int threadNum_);
	~HmmoApplication();

	int run() override;
	bool init() override;

	static HmmoApplication* getInstance();

	skylark::CompletionPort* getIoPort() const { return ioPort; }
	skylark::CompletionPort* getLogicPort() const { return logicPort; }

private:
	skylark::CompletionPort* ioPort;
	skylark::CompletionPort* logicPort;
	int threadNum;
	skylark::Socket* listen;
	std::vector<skylark::IOThread*> threads;

	static HmmoApplication* instance;
};
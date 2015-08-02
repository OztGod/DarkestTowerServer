#pragma once
#include "skylark.h"

class LogicThread : public skylark::IOThread
{
public:
	explicit LogicThread(int id_, skylark::CompletionPort* port_)
		:IOThread(id_, port_) {}

	~LogicThread() override = default;

	void sendJob() override;
	void init() override;
	void job() override;

private:
};
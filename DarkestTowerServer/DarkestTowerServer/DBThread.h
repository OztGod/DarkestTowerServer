#pragma once
#include "skylark.h"

class DBThread : public skylark::IOThread
{
public:
	explicit DBThread(int id_, skylark::CompletionPort* port_)
		:IOThread(id_, port_) {}

	~DBThread() override = default;

	void sendJob() override;
	void init() override;

private:
};
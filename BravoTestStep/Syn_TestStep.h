#pragma once

class Syn_TestStep
{
public:
	Syn_TestStep();
	virtual ~Syn_TestStep();

	virtual	void SetUp() = 0;

	virtual void Execute() = 0;

	virtual void ProcessData() = 0;

	virtual void CleanUp() = 0;
};


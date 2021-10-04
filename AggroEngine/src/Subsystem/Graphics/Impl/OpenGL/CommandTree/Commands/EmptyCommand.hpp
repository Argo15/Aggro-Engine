#pragma once

#include <vector>
#include "Command.hpp"
using namespace std;

class EmptyCommand : public Command
{
private:
	CommandType m_type = CommandType::EMPTY;

public:
	EmptyCommand();

	virtual void executeCommand();
	virtual void end();
	virtual bool equals(shared_ptr<Command> other);
};

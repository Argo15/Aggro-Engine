#pragma once

#include "Command.hpp"

class DisableDepthCommand : public Command
{
private:
	CommandType m_type = CommandType::DISABLE_DEPTH;

public:
	DisableDepthCommand();

	virtual void executeCommand();
	virtual void end();
	virtual bool equals(shared_ptr<Command> other);
	virtual int hash();
};

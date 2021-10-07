#pragma once

#include "Command.hpp"
#include "BufferSyncContext.hpp"
#include "Shaders/GLSLProgram.hpp"

class HasTangentsCommand : public Command
{
private:
	CommandType m_type = CommandType::HAS_TANGENTS;
	bool m_hasTangents;

public:
	HasTangentsCommand(bool hasTangents);

	virtual void executeCommand();
	virtual void end();
	virtual bool equals(shared_ptr<Command> other);
};

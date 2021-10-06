#include "HasTangentsCommand.hpp"

HasTangentsCommand::HasTangentsCommand(bool hasTangents)
	: Command()
	, m_hasTangents(hasTangents)
{
}

void HasTangentsCommand::executeCommand()
{
	if (m_hasTangents)
	{
		glEnableVertexAttribArray(3);
		glEnableVertexAttribArray(4);
	}
	else
	{
		glDisableVertexAttribArray(3);
		glDisableVertexAttribArray(4);
	}
}

void HasTangentsCommand::end()
{
}

bool HasTangentsCommand::equals(shared_ptr<Command> other)
{
	HasTangentsCommand *cmd = static_cast<HasTangentsCommand *>(other.get());
	return cmd->m_type == CommandType::HAS_TANGENTS && 
		cmd->m_hasTangents == m_hasTangents;
}
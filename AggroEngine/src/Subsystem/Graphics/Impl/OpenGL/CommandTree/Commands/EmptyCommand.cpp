#include "EmptyCommand.hpp"

EmptyCommand::EmptyCommand()
	: Command()
{
}

void EmptyCommand::executeCommand()
{

}

void EmptyCommand::end()
{
}

bool EmptyCommand::equals(shared_ptr<Command> other)
{
	EmptyCommand *cmd = static_cast<EmptyCommand *>(other.get());
	return cmd->m_type == CommandType::EMPTY;
}
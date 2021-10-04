#include "DisableDepthCommand.hpp"
#include "OpenGL43Graphics.hpp"

DisableDepthCommand::DisableDepthCommand()
	: Command()
{
}

void DisableDepthCommand::executeCommand()
{
	glDisable(GL_DEPTH_TEST);
}

void DisableDepthCommand::end()
{
	glEnable(GL_DEPTH_TEST);
}

bool DisableDepthCommand::equals(shared_ptr<Command> other)
{
	DisableDepthCommand *cmd = static_cast<DisableDepthCommand *>(other.get());
	return cmd->m_type == CommandType::DISABLE_DEPTH;
}
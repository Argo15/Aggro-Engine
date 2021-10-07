#pragma once

#include <vector>
#include "Command.hpp"
#include "GBuffer.hpp"
using namespace std;

class InitializeShadowMapCommand : public Command
{
private:
	CommandType m_type = CommandType::INIT_SHADOW;
	ShadowMapBuffer *m_shadowMapBuf;

public:
	InitializeShadowMapCommand(ShadowMapBuffer *shadowMapBuf);

	virtual void executeCommand();
	virtual void end();
	virtual bool equals(shared_ptr<Command> other);
};

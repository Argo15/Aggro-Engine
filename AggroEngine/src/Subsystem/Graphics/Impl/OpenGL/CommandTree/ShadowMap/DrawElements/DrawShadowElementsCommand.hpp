#pragma once

#include "Command.hpp"

class DrawShadowElementsCommand : public Command
{
private:
	CommandType m_type = CommandType::DRAW_SHADOW_ELEMENTS;
	shared_ptr<RenderData> m_renderData;

public:
	DrawShadowElementsCommand(shared_ptr<RenderData> renderData);

	virtual void executeCommand();
	virtual void end();
	virtual bool equals(shared_ptr<Command> other);
};

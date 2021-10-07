#pragma once

#include "Command.hpp"
#include "BufferSyncContext.hpp"
#include "Shaders/GLSLProgram.hpp"

class TextureBindingCommand : public Command
{
private:
	CommandType m_type = CommandType::TEXTURE;
	shared_ptr<GLSLProgram> m_glslProgram;
	shared_ptr<TextureHandle> m_albedo;
	shared_ptr<TextureHandle> m_alpha;
	shared_ptr<TextureHandle> m_specular;
	shared_ptr<TextureHandle> m_emission;
	shared_ptr<TextureHandle> m_normal;

public:
	TextureBindingCommand(shared_ptr<GLSLProgram> glslProgram,
		shared_ptr<TextureHandle> albedo,
		shared_ptr<TextureHandle> alpha,
		shared_ptr<TextureHandle> specular,
		shared_ptr<TextureHandle> emission,
		shared_ptr<TextureHandle> normal);

	virtual void executeCommand();
	virtual void end();
	virtual bool equals(shared_ptr<Command> other);
};

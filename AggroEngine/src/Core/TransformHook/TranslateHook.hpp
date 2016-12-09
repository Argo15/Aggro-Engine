#pragma once

#include "Graphics.hpp"
#include "TransformHook.hpp"

class TranslateHook : public TransformHook
{
private:
	shared_ptr<RenderData> m_renderData[3];
	shared_ptr<TextureHandle> m_texture[4];
	bool m_hasSelection;

	void _initialize(shared_ptr<Graphics> graphics, shared_ptr<Scene> scene);

public:
	TranslateHook();

	void render(shared_ptr<Graphics> graphics, shared_ptr<Scene> scene);
	bool updateSelection(shared_ptr<MouseState> mouse, shared_ptr<Selection> selection); // Returns true if hook selected
};
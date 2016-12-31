#pragma once

#include "Graphics.hpp"
#include "TransformHook.hpp"

class TranslateHook : public TransformHook
{
private:
	shared_ptr<RenderData> m_renderData[3];
	shared_ptr<TextureHandle> m_texture[4];
	shared_ptr<glm::vec3> m_lastPos;
	bool m_hasSelection;
	int m_selectedIdx;

	void _initialize(shared_ptr<Graphics> graphics, shared_ptr<Scene> scene);
	unsigned int _getSelectedIndex(shared_ptr<Selection> selection);

public:
	TranslateHook();

	void render(shared_ptr<Graphics> graphics, shared_ptr<Scene> scene);
	bool updateSelection(shared_ptr<MouseState> mouse, shared_ptr<EngineContext> context, shared_ptr<Selection> selection); // Returns true if hook selected
};
#pragma once

#include "Graphics.hpp"
#include "TransformHook.hpp"

class LineHook : public TransformHook
{
private:
	shared_ptr<RenderData> m_renderData[3];
	shared_ptr<TextureHandle> m_texture[4];
	shared_ptr<glm::vec3> m_lastPos;
	bool m_hasSelection;
	int m_selectedIdx;

	void _initialize(shared_ptr<Graphics> graphics, shared_ptr<Scene> scene);
	unsigned int _getSelectedIndex(shared_ptr<Selection> selection);

protected:
	virtual void updateTransform(shared_ptr<TransformComponent> transform, glm::vec3 &diff) = 0;

public:
	LineHook();

	void render(shared_ptr<Graphics> graphics, shared_ptr<Scene> scene);
	bool updateSelection(shared_ptr<MouseState> mouse, shared_ptr<EngineContext> context, shared_ptr<Selection> selection); // Returns true if hook selected
};
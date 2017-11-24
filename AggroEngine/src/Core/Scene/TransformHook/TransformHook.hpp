#pragma once

#include "Graphics.hpp"
#include "RenderData.hpp"
#include "Scene.hpp"
#include "Selection.hpp"
#include "MouseState.hpp"
#include "Line.hpp"

class Scene;
class Selection;
class MouseState;
class EngineContext;

/**
* Base object used to allow users to click and transform an object
*
* author: wcrane
* since: 2016-11-11
**/
class TransformHook
{
private:
	shared_ptr<RenderData> m_renderData[3];
	shared_ptr<TextureHandle> m_texture[4];
	int m_selectedIdx;

	void _initialize(shared_ptr<Graphics> graphics, shared_ptr<Scene> scene);
	unsigned int _getSelectedIndex(shared_ptr<Selection> selection);

protected:
	shared_ptr<glm::vec3> m_lastPos;

	virtual void updateTransform(shared_ptr<SceneNode> selectedNode, const Line &lookLine, const glm::vec3 &axis) = 0;
	virtual shared_ptr<Mesh> getMesh(glm::vec3 axis) = 0;

public:
	TransformHook();

	void render(shared_ptr<Graphics> graphics, shared_ptr<Scene> scene);
	bool updateSelection(shared_ptr<MouseState> mouse, shared_ptr<EngineContext> context, shared_ptr<Selection> selection); // Returns true if hook selected

};
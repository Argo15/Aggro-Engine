#pragma once

#include "RenderData.hpp"
#include "Scene.hpp"
#include "Selection.hpp"
#include "MouseState.hpp"

class Scene;
class Selection;
class MouseState;

/**
* Base object used to allow users to click and transform an object
*
* author: wcrane
* since: 2016-11-11
**/
class TransformHook
{
public:
	virtual void render(shared_ptr<Graphics> graphics, shared_ptr<Scene> scene) = 0;
	virtual bool updateSelection(shared_ptr<MouseState> mouse, shared_ptr<Selection> selection) = 0; // Returns true if hook selected
};
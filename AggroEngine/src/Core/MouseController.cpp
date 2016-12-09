#include "MouseController.hpp"
#include <Qt>

MouseController::MouseController()
{

}

void MouseController::handleMouseInput(shared_ptr<MouseState> mouse, shared_ptr<EngineContext> context, shared_ptr<Selection> selection)
{
	bool transformSelected = context->getScene()->getTransformHook()->updateSelection(mouse, selection);
	if (mouse->getButtonPressedOnce(Qt::MouseButton::LeftButton))
	{
		if (!transformSelected)
		{
			context->getScene()->selectNodeById(selection->getSelectionAsId());
		}
	}
}
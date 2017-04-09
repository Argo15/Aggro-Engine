#include "MouseController.hpp"
#include <Qt>

MouseController::MouseController()
{

}

void MouseController::handleMouseInput(shared_ptr<MouseState> mouse, shared_ptr<EngineContext> context, shared_ptr<Selection> selection)
{
	shared_ptr<TransformHook> transformHook = context->getScene()->getTransformHook();
	bool transformSelected = transformHook ? transformHook->updateSelection(mouse, context, selection) : false;
	if (mouse->getButtonPressedOnce(Qt::MouseButton::LeftButton))
	{
		if (!transformSelected)
		{
			context->getScene()->selectNodeById(selection->getSelectionAsId());
		}
	}
}
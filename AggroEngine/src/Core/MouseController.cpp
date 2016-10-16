#include "MouseController.hpp"
#include <Qt>

MouseController::MouseController()
{

}

void MouseController::handleMouseInput(shared_ptr<MouseState> mouse, shared_ptr<EngineContext> context)
{
	if (mouse->getButtonPressedOnce(Qt::MouseButton::LeftButton))
	{
		float percentX = (float)mouse->getPosX() / (float)context->getGraphics()->getViewport()->getWidth();
		float percentY = (float)mouse->getPosY() / (float)context->getGraphics()->getViewport()->getHeight();
		int imgX = percentX * context->getGraphics()->getFrameBufferWidth();
		int imgY = context->getGraphics()->getFrameBufferHeight() - (percentY * context->getGraphics()->getFrameBufferHeight());
		boost::shared_array<unsigned short> pixel = context->getGraphics()->getRenderImagePixel(imgX, imgY, RenderOptions::SELECTION);
		unsigned short r = pixel[0] / 256;
		unsigned short g = pixel[1] / 256;
		unsigned short b = pixel[2] / 256;
		unsigned int id = r + (g * 255) + (b * 65025);
		context->getScene()->selectNodeById(id);
	}
}
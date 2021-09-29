#pragma once

#include <memory>
#include "RenderData.hpp"

/**
 * Handle returned when a render item is staged. Allows you to unstage items and perform other signals
 */
class RenderHandle 
{
public:
	virtual shared_ptr<RenderData> getRenderData() = 0;

	virtual void unstageRender() = 0;
	virtual void restageRender() = 0;
};
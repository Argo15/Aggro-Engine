#pragma once

#include <memory>
#include "../../../Resource/Image/Image.hpp"

class TextureBuildOptions
{
private:
	shared_ptr<Image> m_pImage;

public:
	TextureBuildOptions(shared_ptr<Image> pImage);

	shared_ptr<Image> getImage();
};
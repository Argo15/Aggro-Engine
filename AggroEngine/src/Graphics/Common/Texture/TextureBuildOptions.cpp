#include "TextureBuildOptions.hpp"

TextureBuildOptions::TextureBuildOptions(shared_ptr<Image> pImage)
	: m_pImage(pImage)
{

}

shared_ptr<Image> TextureBuildOptions::getImage()
{
	return m_pImage;
}
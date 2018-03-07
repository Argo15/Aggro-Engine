#include "TextureBuildOptions.hpp"

TextureBuildOptions::TextureBuildOptions(shared_ptr<ImageUC> pImage)
	: m_pImage(pImage)
	, m_target(Target::TEXTURE_2D)
	, m_magFilter(Filter::LINEAR)
	, m_minFilter(Filter::LINEAR)
	, m_wrapS(Wrap::REPEAT)
	, m_wrapT(Wrap::REPEAT)
	, m_internalFormat(InternalFormat::RGBA8)
	, m_genMipmaps(true)
	, m_enableDepthCompare(false)
{
}

shared_ptr<ImageUC> TextureBuildOptions::getImage()
{
	return m_pImage;
}

TextureBuildOptions *TextureBuildOptions::setTarget(Target target)
{
	m_target = target;
	return this;
}

TextureBuildOptions *TextureBuildOptions::setFilter(Filter filter)
{
	m_minFilter = filter;
	m_magFilter = filter;
	return this;
}

TextureBuildOptions *TextureBuildOptions::setMagFilter(Filter filter)
{
	m_magFilter = filter;
	return this;
}

TextureBuildOptions *TextureBuildOptions::setMinFilter(Filter filter)
{
	m_minFilter = filter;
	return this;
}

TextureBuildOptions *TextureBuildOptions::setWrap(Wrap wrap)
{
	m_wrapS = wrap;
	m_wrapT = wrap;
	return this;
}

TextureBuildOptions *TextureBuildOptions::setWrapS(Wrap wrap)
{
	m_wrapS = wrap;
	return this;
}

TextureBuildOptions *TextureBuildOptions::setWrapT(Wrap wrap)
{
	m_wrapT = wrap;
	return this;
}

TextureBuildOptions *TextureBuildOptions::genMipmaps(bool genMipmaps)
{
	m_genMipmaps = genMipmaps;
	return this;
}

TextureBuildOptions *TextureBuildOptions::setInternalFormat(InternalFormat format)
{
	m_internalFormat = format;
	return this;
}

TextureBuildOptions *TextureBuildOptions::enableDepthCompare(bool enabled)
{
	m_enableDepthCompare = enabled;
	return this;
}

Target TextureBuildOptions::getTarget()
{
	return m_target;
}

Filter TextureBuildOptions::getMagFilter()
{
	return m_magFilter;
}

Filter TextureBuildOptions::getMinFilter()
{
	return m_minFilter;
}

Wrap TextureBuildOptions::getWrapS()
{
	return m_wrapS;
}

Wrap TextureBuildOptions::getWrapT()
{
	return m_wrapT;
}


InternalFormat TextureBuildOptions::getInternalFormat()
{
	return m_internalFormat;
}

unsigned int TextureBuildOptions::getComponents()
{
	if (m_internalFormat == InternalFormat::DEPTH_COMPONENT32 || m_internalFormat == InternalFormat::R16UI)
	{
		return 1;
	}
	return 4;
}

bool TextureBuildOptions::isGenMipmaps()
{
	return m_genMipmaps;
}

bool TextureBuildOptions::isDepthCompareEnabled()
{
	return m_enableDepthCompare;
}
#pragma once

#include <boost/unordered_map.hpp>
#include <memory>
#include "Image.hpp"

/**
* TextureBuildOptions, catered for OpenGL (Target TEXTURE_2D is assumed to be matched with GL_TEXTURE_2D)
* If using another library, values must be matched there
*/
enum Target
{
	TEXTURE_1D = 0x0DE0,
	TEXTURE_2D = 0x0DE1
};
enum Filter
{
	NEAREST = 0x2600,
	LINEAR = 0x2601
};
enum Wrap
{
	CLAMP = 0x2900,
	CLAMP_TO_EDGE = 0x812F,
	REPEAT = 0x2901
};

/**
 * Specifies the number of color components in the texture.
 * Should be the format we would see in the graphics card memory
*/
enum InternalFormat
{
	RGBA8 = 0x8058,
	RGBA16 = 0x805B,
	DEPTH_COMPONENT32 = 0x81A7,
	R16UI = 0x822A
};

class TextureBuildOptions
{
public:
private:
	shared_ptr<ImageUC> m_pImage;
	Target m_target;
	Filter m_magFilter;
	Filter m_minFilter;
	Wrap m_wrapS;
	Wrap m_wrapT;
	InternalFormat m_internalFormat;
	bool m_genMipmaps;
	bool m_enableDepthCompare;

public:
	TextureBuildOptions(shared_ptr<ImageUC> pImage);

	shared_ptr<ImageUC> getImage();

	TextureBuildOptions *setTarget(Target target);
	TextureBuildOptions *setFilter(Filter name);
	TextureBuildOptions *setMagFilter(Filter name);
	TextureBuildOptions *setMinFilter(Filter name);
	TextureBuildOptions *setWrap(Wrap name);
	TextureBuildOptions *setWrapS(Wrap name);
	TextureBuildOptions *setWrapT(Wrap name);
	TextureBuildOptions *genMipmaps(bool genMipmaps);
	TextureBuildOptions *setInternalFormat(InternalFormat format);
	TextureBuildOptions *enableDepthCompare(bool enabled = true);

	Target getTarget();
	Filter getMagFilter();
	Filter getMinFilter();
	Wrap getWrapS();
	Wrap getWrapT();
	InternalFormat getInternalFormat();
	unsigned int getComponents();
	bool isGenMipmaps();
	bool isDepthCompareEnabled();
};
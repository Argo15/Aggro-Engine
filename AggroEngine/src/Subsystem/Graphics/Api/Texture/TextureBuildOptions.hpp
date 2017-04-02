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

class TextureBuildOptions
{
public:
private:
	shared_ptr<Image> m_pImage;
	Target m_target;
	Filter m_magFilter;
	Filter m_minFilter;
	Wrap m_wrapS;
	Wrap m_wrapT;
	bool m_genMipmaps;

public:
	TextureBuildOptions(shared_ptr<Image> pImage);

	shared_ptr<Image> getImage();

	TextureBuildOptions *setTarget(Target target);
	TextureBuildOptions *setFilter(Filter name);
	TextureBuildOptions *setMagFilter(Filter name);
	TextureBuildOptions *setMinFilter(Filter name);
	TextureBuildOptions *setWrap(Wrap name);
	TextureBuildOptions *setWrapS(Wrap name);
	TextureBuildOptions *setWrapT(Wrap name);
	TextureBuildOptions *genMipmaps(bool genMipmaps);

	Target getTarget();
	Filter getMagFilter();
	Filter getMinFilter();
	Wrap getWrapS();
	Wrap getWrapT();
	bool isGenMipmaps();
};
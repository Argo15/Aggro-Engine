#pragma once

#include "Image.hpp"
#include "../../Util/Math/MathUtil.hpp"

class CheckersImage : public Image
{
private:
	boost::shared_array<unsigned char> getData(int nWidth, int nHeight);

public:
	CheckersImage(int nWidth, int nHeight);
};
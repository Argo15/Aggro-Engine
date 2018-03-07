#pragma once

#include "Image.hpp"
#include "MathUtil.hpp"

class CheckersImage : public ImageUC
{
private:
	shared_ptr<unsigned char> getData(int nWidth, int nHeight);

public:
	CheckersImage(int nWidth, int nHeight);
};
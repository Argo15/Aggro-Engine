#pragma once

#include <memory>
#include <string>
#include "Image.hpp"
using namespace std;

class ImageImporter
{
public:
	virtual shared_ptr<ImageUC> importImage(string sFilename) = 0;
};
#pragma once

#include "ImageImporter.hpp"

class FreeImageImporter : public ImageImporter
{
private:

public:
	FreeImageImporter();

	shared_ptr<Image> importImage(string sFilename);
};
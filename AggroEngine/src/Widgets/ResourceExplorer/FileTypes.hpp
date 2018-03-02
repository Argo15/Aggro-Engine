#pragma once

#include <string>
#include <unordered_set>
#include <QString>
#include "Config.hpp"
using namespace std;

enum FileType
{
	DIRECTORY,
	IMAGE,
	MESH,
	OTHER
};

class FileTypes
{
private:
	unordered_set<string> m_supportedImages;
	unordered_set<string> m_supportedMeshes;

public:
	FileTypes();

	FileType fromPath(QString &path);
};

static const shared_ptr<FileTypes> gFileTypes = shared_ptr<FileTypes>(new FileTypes());
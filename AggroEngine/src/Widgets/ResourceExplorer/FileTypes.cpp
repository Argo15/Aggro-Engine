#include "FileTypes.hpp"
#include <QStringList>

FileTypes::FileTypes()
	: m_supportedImages()
	, m_supportedMeshes()
{

}

FileType FileTypes::fromPath(QString &path)
{
	vector<string> imgExtensions = Config::instance().getProperties().getStringArrayProperty("engine.tex_extensions");
	std::copy(imgExtensions.begin(), imgExtensions.end(),
		std::inserter(m_supportedImages, m_supportedImages.end()));
	vector<string> meshExtensions = Config::instance().getProperties().getStringArrayProperty("engine.mesh_extensions");
	std::copy(meshExtensions.begin(), meshExtensions.end(),
		std::inserter(m_supportedMeshes, m_supportedMeshes.end()));
	if (path.contains("."))
	{
		QString extension = path.split(".", QString::SkipEmptyParts).last();
		if (m_supportedImages.find(extension.toStdString()) != m_supportedImages.end())
		{
			return IMAGE;
		}
		else if (m_supportedMeshes.find(extension.toStdString()) != m_supportedMeshes.end())
		{
			return MESH;
		}
		else if (path.contains(".."))
		{
			return DIRECTORY;
		}
		return OTHER;
	}
	return DIRECTORY;
}
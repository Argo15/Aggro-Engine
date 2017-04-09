#include "Resources.hpp"

Resources::Resources()
	: m_currentId(0)
	, m_pathToId()
	, m_idToPath()
{

}

int Resources::getIdForPath(string path)
{
	if (m_pathToId.find(path) == m_pathToId.end())
	{
		m_idToPath[m_currentId] = path;
		m_pathToId[path] = m_currentId++;
	}
	return m_pathToId[path];
}

boost::optional<string> Resources::getPathForId(int id)
{
	if (m_idToPath.find(id) == m_idToPath.end())
	{
		return boost::optional<string>();
	}
	return m_idToPath[id];
}
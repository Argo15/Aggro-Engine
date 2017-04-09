#pragma once

#include <boost/unordered_map.hpp>
#include <boost/optional.hpp>
using namespace std;

/**
 * Maps a resource string path to an int id
 * since: 2016-08-28
 */
class Resources
{
private:
	int m_currentId;
	boost::unordered_map<string, int> m_pathToId;
	boost::unordered_map<int, string> m_idToPath;

public:
	Resources();

	int getIdForPath(string path);
	boost::optional<string> getPathForId(int id);
};
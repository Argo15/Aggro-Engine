#pragma once

#include <memory>
#include <string>
#include "SceneNode.hpp"
using namespace std;

class SceneNodeImporter
{
public:
	virtual shared_ptr<SceneNode> importSceneNode(string sFilename) = 0;
};
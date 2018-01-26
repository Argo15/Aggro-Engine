#pragma once

#include "Mesh.hpp"
#include <memory>
#include <vector>
using namespace std;

class MeshModifier
{
public:
	MeshModifier() {}

	virtual shared_ptr<Mesh> apply(shared_ptr<Mesh> &sourceMesh) = 0;
};
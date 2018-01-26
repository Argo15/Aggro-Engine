#pragma once

#include "MeshModifier.hpp"

class GenerateNormalLines : public MeshModifier
{
public:
	GenerateNormalLines();

	shared_ptr<Mesh> apply(shared_ptr<Mesh> &sourceMesh);
};
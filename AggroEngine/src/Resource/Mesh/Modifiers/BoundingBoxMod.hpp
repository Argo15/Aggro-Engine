#pragma once

#include "MeshModifier.hpp"

class BoundingBoxMod : public MeshModifier
{
public:
	BoundingBoxMod();

	shared_ptr<Mesh> apply(shared_ptr<Mesh> &sourceMesh);
};
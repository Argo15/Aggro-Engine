#pragma once

#include "MeshModifier.hpp"

class AlignCenter : public MeshModifier
{
public:
	AlignCenter();

	shared_ptr<Mesh> apply(shared_ptr<Mesh> &sourceMesh);
};
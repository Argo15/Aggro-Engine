#pragma once

#include "MeshModifier.hpp"

class AlignBottom : public MeshModifier
{
public:
	AlignBottom();

	shared_ptr<Mesh> apply(shared_ptr<Mesh> &sourceMesh);
};
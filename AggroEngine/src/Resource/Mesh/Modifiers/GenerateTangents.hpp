#pragma once

#include "MeshModifier.hpp"

class GenerateTangents : public MeshModifier
{
public:
	GenerateTangents();

	shared_ptr<Mesh> apply(shared_ptr<Mesh> &sourceMesh);
};
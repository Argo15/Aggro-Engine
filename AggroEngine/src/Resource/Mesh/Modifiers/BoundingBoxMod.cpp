#include "BoundingBoxMod.hpp"
#include "BoundingBoxMesh.hpp"

BoundingBoxMod::BoundingBoxMod()
{

}

shared_ptr<Mesh> BoundingBoxMod::apply(shared_ptr<Mesh> &sourceMesh)
{
	if (!sourceMesh)
	{
		return shared_ptr<Mesh>();
	}
	return shared_ptr<Mesh>(new BoundingBoxMesh(-1, sourceMesh->getMetaData()->getCorners()));
}
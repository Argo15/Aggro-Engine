#include "AssimpMeshImporter.hpp"
#include "Dev/CubeMesh.hpp"
#include <iostream>

AssimpMeshImporter::AssimpMeshImporter()
{

}

shared_ptr<Mesh> AssimpMeshImporter::importMesh(string sFilename)
{
	const aiScene* scene = importer.ReadFile( sFilename,
    aiProcess_Triangulate            |
    aiProcess_JoinIdenticalVertices  |
    aiProcess_SortByPType			 |
	aiProcess_FixInfacingNormals);
	if (scene != nullptr)
	{
		int nNumVerts = scene->mMeshes[0]->mNumVertices;
		int nNumFaces = scene->mMeshes[0]->mNumFaces;
		float *verts = new float[nNumVerts*3];
		float *texcoords = new float[nNumVerts*2];
		float *normals = new float[nNumVerts*3];
		int *indices = new int[nNumFaces*3];
		for (int i=0; i<nNumVerts; i++)
		{
			verts[i*3] = scene->mMeshes[0]->mVertices[i].x; 
			verts[i*3+1] = scene->mMeshes[0]->mVertices[i].y; 
			verts[i*3+2] = scene->mMeshes[0]->mVertices[i].z; 
			texcoords[i*2] = scene->mMeshes[0]->mTextureCoords[0][i].x; 
			texcoords[i*2+1] = scene->mMeshes[0]->mTextureCoords[0][i].y; 
			normals[i*3] = scene->mMeshes[0]->mNormals[i].x; 
			normals[i*3+1] = scene->mMeshes[0]->mNormals[i].y; 
			normals[i*3+2] = scene->mMeshes[0]->mNormals[i].z; 
		}
		for (int i=0; i<nNumFaces; i++)
		{
			indices[i*3] = scene->mMeshes[0]->mFaces[i].mIndices[0];
			indices[i*3+1] = scene->mMeshes[0]->mFaces[i].mIndices[1];
			indices[i*3+2] = scene->mMeshes[0]->mFaces[i].mIndices[2];
		}
		importer.FreeScene();
		return shared_ptr<Mesh>(new Mesh(sizeof(float)*nNumVerts*3, sizeof(int)*nNumFaces*3, shared_ptr<float>(verts), shared_ptr<float>(texcoords), shared_ptr<float>(normals), shared_ptr<int>(indices)));
	}

	return shared_ptr<Mesh>(new CubeMesh());
}
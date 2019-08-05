#include "AssimpSceneNodeImporter.hpp"
#include "StaticObjectRenderComponent.hpp"
#include "MemoryUtil.hpp"
#include "Mesh.hpp"
#include "Scene.hpp"
#include <iostream>

AssimpSceneNodeImporter::AssimpSceneNodeImporter(shared_ptr<JobManager> jobManager)
	: m_jobManager(jobManager)
{

}

shared_ptr<SceneNode> AssimpSceneNodeImporter::importSceneNode(string sFilename)
{
	boost::lock_guard<AssimpSceneNodeImporter> guard(*this);
	const aiScene* scene = importer.ReadFile( sFilename,
    aiProcess_Triangulate            |
    aiProcess_JoinIdenticalVertices  |
    aiProcess_SortByPType			 |
	aiProcess_FixInfacingNormals);
	if (scene != nullptr)
	{
		if (!scene->HasMeshes())
		{
			cout << "Failed importing " << sFilename << " no mesh data." << endl;
			return shared_ptr<SceneNode>();
		}
		shared_ptr<SceneNode> newNode = shared_ptr<SceneNode>(new SceneNode(Scene::getNextId()));
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

		shared_ptr<MeshComponent> meshComponent(new MeshComponent(m_jobManager));
		meshComponent->setPrimaryMesh(shared_ptr<Mesh>(new Mesh(-1,
			sizeof(float)*nNumVerts * 3,
			sizeof(int)*nNumFaces * 3,
			mem::shared_array(verts, nNumVerts * 3, "SceneNode.Mesh"),
			mem::shared_array(texcoords, nNumVerts * 2, "SceneNode.Mesh"),
			mem::shared_array(normals, nNumVerts * 3, "SceneNode.Mesh"),
			mem::shared_array(indices, nNumFaces * 3, "SceneNode.Mesh"))));
		meshComponent->refresh();
		newNode->setMeshComponent(meshComponent);
		newNode->setRenderComponent(shared_ptr<StaticObjectRenderComponent>(new StaticObjectRenderComponent()));
		newNode->setTransformComponent(shared_ptr<TransformComponent>(new TransformComponent()));
		return newNode;
	}

	cout << "Failed importing " << sFilename << " no scene found." << endl;
	return shared_ptr<SceneNode>();
}
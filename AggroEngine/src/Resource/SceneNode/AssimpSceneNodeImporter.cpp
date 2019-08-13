#include "AssimpSceneNodeImporter.hpp"
#include "StaticObjectRenderComponent.hpp"
#include "MemoryUtil.hpp"
#include "Mesh.hpp"
#include "Scene.hpp"
#include <iostream>
#include <QString>
#include <QStringList>

AssimpSceneNodeImporter::AssimpSceneNodeImporter(shared_ptr<JobManager> jobManager)
	: m_jobManager(jobManager)
{

}

shared_ptr<SceneNode> AssimpSceneNodeImporter::importSceneNode(string sFilename)
{
	boost::lock_guard<AssimpSceneNodeImporter> guard(*this);
	QString name = QString::fromStdString(sFilename).split("/").last().split(".").first();
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
		shared_ptr<SceneNode> rootNode = shared_ptr<SceneNode>(new SceneNode(Scene::getNextId()));
		rootNode->setTransformComponent(shared_ptr<TransformComponent>(new TransformComponent()));
		rootNode->setName(name.toStdString());
		rootNode->setFilename(sFilename);

		for (int meshIdx = 0; meshIdx < scene->mNumMeshes; meshIdx++)
		{
			shared_ptr<SceneNode> childNode = shared_ptr<SceneNode>(new SceneNode(Scene::getNextId()));
			childNode->setTransformComponent(shared_ptr<TransformComponent>(new TransformComponent()));
			childNode->setName(name.toStdString());

			int nNumVerts = scene->mMeshes[meshIdx]->mNumVertices;
			int nNumFaces = scene->mMeshes[meshIdx]->mNumFaces;
			float *verts = new float[nNumVerts * 3];
			float *texcoords = new float[nNumVerts * 2];
			float *normals = new float[nNumVerts * 3];
			int *indices = new int[nNumFaces * 3];
			for (int i = 0; i<nNumVerts; i++)
			{
				verts[i * 3] = scene->mMeshes[meshIdx]->mVertices[i].x;
				verts[i * 3 + 1] = scene->mMeshes[meshIdx]->mVertices[i].y;
				verts[i * 3 + 2] = scene->mMeshes[meshIdx]->mVertices[i].z;
				texcoords[i * 2] = scene->mMeshes[meshIdx]->mTextureCoords[0][i].x;
				texcoords[i * 2 + 1] = scene->mMeshes[meshIdx]->mTextureCoords[0][i].y;
				normals[i * 3] = scene->mMeshes[meshIdx]->mNormals[i].x;
				normals[i * 3 + 1] = scene->mMeshes[meshIdx]->mNormals[i].y;
				normals[i * 3 + 2] = scene->mMeshes[meshIdx]->mNormals[i].z;
			}
			for (int i = 0; i<nNumFaces; i++)
			{
				indices[i * 3] = scene->mMeshes[meshIdx]->mFaces[i].mIndices[0];
				indices[i * 3 + 1] = scene->mMeshes[meshIdx]->mFaces[i].mIndices[1];
				indices[i * 3 + 2] = scene->mMeshes[meshIdx]->mFaces[i].mIndices[2];
			}

			shared_ptr<MeshComponent> meshComponent(new MeshComponent(m_jobManager));
			meshComponent->setPrimaryMesh(shared_ptr<Mesh>(new Mesh(-1,
				sizeof(float)*nNumVerts * 3,
				sizeof(int)*nNumFaces * 3,
				mem::shared_array(verts, nNumVerts * 3, "SceneNode.Mesh"),
				mem::shared_array(texcoords, nNumVerts * 2, "SceneNode.Mesh"),
				mem::shared_array(normals, nNumVerts * 3, "SceneNode.Mesh"),
				mem::shared_array(indices, nNumFaces * 3, "SceneNode.Mesh"),
				DrawMode::TRIANGLES)));
			meshComponent->refresh();

			if (scene->mNumMeshes == 1)
			{
				rootNode->setMeshComponent(meshComponent);
				rootNode->setRenderComponent(shared_ptr<StaticObjectRenderComponent>(new StaticObjectRenderComponent()));
			}
			else
			{
				childNode->setMeshComponent(meshComponent);
				childNode->setRenderComponent(shared_ptr<StaticObjectRenderComponent>(new StaticObjectRenderComponent()));
				rootNode->addChild(childNode);
			}
		}

		importer.FreeScene();
		return rootNode;
	}

	cout << "Failed importing " << sFilename << " no scene found." << endl;
	return shared_ptr<SceneNode>();
}
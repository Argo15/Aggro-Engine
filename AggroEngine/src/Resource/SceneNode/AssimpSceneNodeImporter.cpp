#include "AssimpSceneNodeImporter.hpp"
#include "StaticObjectRenderComponent.hpp"
#include "MemoryUtil.hpp"
#include "Mesh.hpp"
#include "Scene.hpp"
#include <iostream>
#include <QString>
#include <QStringList>
#include <QDir>

AssimpSceneNodeImporter::AssimpSceneNodeImporter(shared_ptr<JobManager> jobManager, shared_ptr<Resources> resources)
	: m_jobManager(jobManager)
	, m_resources(resources)
{

}

shared_ptr<SceneNode> AssimpSceneNodeImporter::importSceneNode(string sFilename)
{
	boost::lock_guard<AssimpSceneNodeImporter> guard(*this);
	QString qFilename = QString::fromStdString(sFilename);
	string name = qFilename.split("/").last().split(".").first().toStdString();
	string relativePath = qFilename.left(qFilename.lastIndexOf("/")+1).toStdString();
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
		rootNode->setName(name);
		rootNode->setFilename(sFilename);

		MaterialComponent *materials = new MaterialComponent[scene->mNumMaterials];
		for (int materialIdx = 0; materialIdx < scene->mNumMaterials; materialIdx++)
		{
			readMaterial(relativePath, scene->mMaterials[materialIdx], &materials[materialIdx]);
		}

		for (int meshIdx = 0; meshIdx < scene->mNumMeshes; meshIdx++)
		{
			shared_ptr<SceneNode> newNode = (scene->mNumMeshes == 1) ? rootNode : shared_ptr<SceneNode>(new SceneNode(Scene::getNextId()));
			newNode->setTransformComponent(shared_ptr<TransformComponent>(new TransformComponent()));
			newNode->setMeshComponent(readMesh(scene->mMeshes[meshIdx]));
			newNode->setRenderComponent(shared_ptr<StaticObjectRenderComponent>(new StaticObjectRenderComponent()));
			if (scene->mMeshes[meshIdx]->mMaterialIndex < scene->mNumMaterials)
			{
				MaterialComponent *mat = &materials[scene->mMeshes[meshIdx]->mMaterialIndex];
				newNode->setMaterialComponent(shared_ptr<MaterialComponent>(new MaterialComponent(newNode.get(), mat)));
			}
			string nodeName = scene->mMeshes[meshIdx]->mName.length > 0 ? scene->mMeshes[meshIdx]->mName.C_Str() : name;
			newNode->setName(nodeName);

			if (rootNode != newNode)
			{
				rootNode->addChild(newNode);
			}
		}

		importer.FreeScene();
		delete[] materials;
		return rootNode;
	}

	cout << "Failed importing " << sFilename << " no scene found." << endl;
	return shared_ptr<SceneNode>();
}

shared_ptr<MeshComponent> AssimpSceneNodeImporter::readMesh(aiMesh *meshIn)
{
	int nNumVerts = meshIn->mNumVertices;
	int nNumFaces = meshIn->mNumFaces;
	float *verts = new float[nNumVerts * 3];
	float *texcoords = new float[nNumVerts * 2];
	float *normals = new float[nNumVerts * 3];
	int *indices = new int[nNumFaces * 3];
	for (int i = 0; i<nNumVerts; i++)
	{
		verts[i * 3] = meshIn->mVertices[i].x;
		verts[i * 3 + 1] = meshIn->mVertices[i].y;
		verts[i * 3 + 2] = meshIn->mVertices[i].z;
		texcoords[i * 2] = meshIn->mTextureCoords[0][i].x;
		texcoords[i * 2 + 1] = meshIn->mTextureCoords[0][i].y;
		normals[i * 3] = meshIn->mNormals[i].x;
		normals[i * 3 + 1] = meshIn->mNormals[i].y;
		normals[i * 3 + 2] = meshIn->mNormals[i].z;
	}
	for (int i = 0; i<nNumFaces; i++)
	{
		indices[i * 3] = meshIn->mFaces[i].mIndices[0];
		indices[i * 3 + 1] = meshIn->mFaces[i].mIndices[1];
		indices[i * 3 + 2] = meshIn->mFaces[i].mIndices[2];
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
	return meshComponent;
}

void AssimpSceneNodeImporter::readMaterial(string relativePath, aiMaterial *matIn, MaterialComponent *matOut)
{
	aiString matName;
	matIn->Get(AI_MATKEY_NAME, matName);
	aiColor3D diffuse(0.f, 0.f, 0.f);
	matIn->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
	aiColor3D ambient(0.f, 0.f, 0.f);
	matIn->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
	float shininessStrength = 0;
	matIn->Get(AI_MATKEY_SHININESS_STRENGTH, shininessStrength);
	float shininess = 0;
	matIn->Get(AI_MATKEY_SHININESS, shininess);
	aiColor3D specular(0.f, 0.f, 0.f);
	matIn->Get(AI_MATKEY_COLOR_SPECULAR, specular);
	aiColor3D emissive(0.f, 0.f, 0.f);
	matIn->Get(AI_MATKEY_COLOR_EMISSIVE, emissive);
	aiString diffuseTex;
	matIn->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), diffuseTex);
	aiString specTex;
	matIn->Get(AI_MATKEY_TEXTURE(aiTextureType_SPECULAR, 0), specTex);
	aiString normalTex;
	matIn->Get(AI_MATKEY_TEXTURE(aiTextureType_NORMALS, 0), normalTex);
	aiString heightTex;
	matIn->Get(AI_MATKEY_TEXTURE(aiTextureType_HEIGHT, 0), heightTex);
	aiString opacityTex;
	matIn->Get(AI_MATKEY_TEXTURE(aiTextureType_OPACITY, 0), opacityTex);
	aiString emissiveTex;
	matIn->Get(AI_MATKEY_TEXTURE(aiTextureType_EMISSIVE, 0), emissiveTex);

	matOut->setName(string(matName.C_Str()));
	matOut->setColor(glm::vec3(diffuse.r, diffuse.g, diffuse.b));
	matOut->setSpecIntensityPct(100 * shininessStrength);
	matOut->setShininess(shininess);
	matOut->setEmission(glm::vec3(emissive.r, emissive.g, emissive.b));
	if (diffuseTex.length > 0)
	{
		string texPath = relativePath + diffuseTex.C_Str();
		matOut->setTextureImageId(m_resources->getIdForPath(texPath));
	}
	if (specTex.length > 0)
	{
		string texPath = relativePath + specTex.C_Str();
		matOut->setSpecularImageId(m_resources->getIdForPath(texPath));
	}
	if (normalTex.length > 0)
	{
		string texPath = relativePath + normalTex.C_Str();
		matOut->setNormalImageId(m_resources->getIdForPath(texPath));
	}
	if (opacityTex.length > 0)
	{
		string texPath = relativePath + opacityTex.C_Str();
		matOut->setAlphaImageId(m_resources->getIdForPath(texPath));
	}
	if (emissiveTex.length > 0)
	{
		string texPath = relativePath + emissiveTex.C_Str();
		matOut->setEmissionImageId(m_resources->getIdForPath(texPath));
	}
}
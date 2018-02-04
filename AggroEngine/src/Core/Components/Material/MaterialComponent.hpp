#pragma once

#include "Locks.hpp"
#include "Matrix.hpp"
#include "Serialization.hpp"
#include "Listener.hpp"
#include "Component.hpp"
#include "Material.hpp"
#include "TextureCache.hpp"
#include "SceneNode.hpp"

class SceneNode;

/**
* Material properties including color, shininess, textures, etc.
*
* author: wcrane
* since: 2017-12-22
**/
class MaterialComponent : public Component,
	public boost::basic_lockable_adapter<recursive_mutex>
{
private:
	SceneNode *m_owner;
	boost::optional<int> m_textureImageId;
	boost::optional<int> m_alphaImageId;
	glm::vec3 m_color;
	int m_specIntensityPct;
	int m_shininess;
	boost::optional<int> m_specularImageId;
	float m_texScaleU;
	float m_texScaleV;
	float m_texOffsetU;
	float m_texOffsetV;
	float m_texRotate;
	glm::vec3 m_emissionColor;
	boost::optional<int> m_emissionImageId;
	boost::optional<int> m_normalImageId;

	Listener<MaterialComponent *> m_changeListeners;

	MaterialComponent(Chunk * const byteChunk, SceneNode *owner, shared_ptr<Resources> resources);

public:
	MaterialComponent(SceneNode *m_owner);
	MaterialComponent(SceneNode *m_owner, shared_ptr<MaterialComponent> copy);

	// Serialization
	shared_ptr<Chunk> serialize(shared_ptr<Resources> resources);
	static shared_ptr<MaterialComponent> deserialize(Chunk * const byteChunk, SceneNode *owner, shared_ptr<Resources> resources);

	void addChangeListener(void *ns, std::function<void(MaterialComponent *)> listener);
	void removeChangeListener(void *ns);

	shared_ptr<Material> getMaterial(shared_ptr<TextureCache> textures);

	void setColor(glm::vec3 color);
	glm::vec3 getColor();

	void setTextureImageId(int textureImageId);
	boost::optional<int> getTextureImageId();
	void removeTexture();

	void setAlphaImageId(int textureImageId);
	boost::optional<int> getAlphaImageId();
	void removeAlphaMap();

	SceneNode *getOwner();

	void setSpecIntensityPct(int intensity);
	int getSpecIntensityPct();
	float getSpecIntensity();

	void setShininess(int shininess);
	int getShininess();

	void setSpecularImageId(int textureImageId);
	boost::optional<int> getSpecularImageId();
	void removeSpecularMap();

	void setTexScaleU(float texScaleU);
	float getTexScaleU();
	void setTexScaleV(float texScaleV);
	float getTexScaleV();
	void setTexOffsetU(float texOffsetU);
	float getTexOffsetU();
	void setTexOffsetV(float texOffsetV);
	float getTexOffsetV();
	void setTexRotate(float texRotate);
	float getTexRotate();

	void setEmission(glm::vec3 emission);
	glm::vec3 getEmission();
	void setEmissionImageId(int textureImageId);
	boost::optional<int> getEmissionImageId();
	void removeEmissionMap();

	void setNormalImageId(int textureImageId);
	boost::optional<int> getNormalImageId();
	void removeNormalMap();
};
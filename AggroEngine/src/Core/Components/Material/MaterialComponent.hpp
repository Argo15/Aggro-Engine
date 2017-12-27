#pragma once

#include "Locks.hpp"
#include "Matrix.hpp"
#include "Serialization.hpp"
#include "Listener.hpp"
#include "Component.hpp"
#include "Material.hpp"
#include "TextureCache.hpp"

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
	boost::optional<int> m_textureImageId;
	glm::vec3 m_color;

	Listener<MaterialComponent *> m_changeListeners;

	MaterialComponent(Chunk * const byteChunk, shared_ptr<Resources> resources);

public:
	MaterialComponent();

	// Serialization
	shared_ptr<Chunk> serialize(shared_ptr<Resources> resources);
	static shared_ptr<MaterialComponent> deserialize(Chunk * const byteChunk, shared_ptr<Resources> resources);

	void addChangeListener(void *ns, std::function<void(MaterialComponent *)> listener);
	void removeChangeListener(void *ns);

	shared_ptr<Material> getMaterial(shared_ptr<TextureCache> textures);

	void setColor(glm::vec3 color);
	glm::vec3 getColor();

	void setTextureImageId(int textureImageId);
	boost::optional<int> getTextureImageId();
	void removeTexture();
};
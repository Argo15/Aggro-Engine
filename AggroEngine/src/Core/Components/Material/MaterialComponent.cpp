#include "MaterialComponent.hpp"

MaterialComponent::MaterialComponent(SceneNode *owner)
	: m_owner(owner)
	, m_color(1.0)
{

}

MaterialComponent::MaterialComponent(SceneNode *owner, shared_ptr<MaterialComponent> copy)
	: m_owner(owner)
	, m_color(copy->m_color)
	, m_textureImageId(copy->m_textureImageId)
{
}


MaterialComponent::MaterialComponent(Chunk * const byteChunk, SceneNode *owner, shared_ptr<Resources> resources)
	: m_owner(owner)
{
	ByteParser parser(*byteChunk);
	m_color = parser.parseVec3().get_value_or(glm::vec3(1.0));
	string texPath = parser.parseString().get_value_or("");
	if (texPath != "")
	{
		m_textureImageId = boost::optional<int>(resources->getIdForPath(texPath));
	}
}

shared_ptr<Chunk> MaterialComponent::serialize(shared_ptr<Resources> resources)
{
	ByteAccumulator bytes = ByteAccumulator();
	bytes.add(&m_color);
	string texName = "";
	if (m_textureImageId)
	{
		texName = resources->getPathForId(m_textureImageId.get()).get_value_or("");
	}
	bytes.add(&texName);
	return shared_ptr<Chunk>(new Chunk(ChunkType::MATERIAL_COMPONENT, bytes.getNumBytes(), bytes.collect()));
}

shared_ptr<MaterialComponent> MaterialComponent::deserialize(Chunk * const byteChunk, SceneNode *owner, shared_ptr<Resources> resources)
{
	if (*byteChunk->getType() != ChunkType::MATERIAL_COMPONENT)
	{
		return shared_ptr<MaterialComponent>();
	}

	return shared_ptr<MaterialComponent>(new MaterialComponent(byteChunk, owner, resources));
}

void MaterialComponent::addChangeListener(void *ns, std::function<void(MaterialComponent *)> listener)
{
	boost::lock_guard<MaterialComponent> guard(*this);
	m_changeListeners.add(ns, listener);
}

void MaterialComponent::removeChangeListener(void *ns)
{
	boost::lock_guard<MaterialComponent> guard(*this);
	m_changeListeners.remove(ns);
}

shared_ptr<Material> MaterialComponent::getMaterial(shared_ptr<TextureCache> textures)
{
	shared_ptr<Material> material(new Material(m_color));
	if (m_textureImageId)
	{
		shared_ptr<TextureHandle> texture = textures->getTexture(m_textureImageId.get());
		material->setTexture(texture);
	}
	return material;
}

void MaterialComponent::setColor(glm::vec3 color)
{
	m_color = color;
	m_changeListeners.notify(this);
}

glm::vec3 MaterialComponent::getColor()
{
	return m_color;
}

void MaterialComponent::setTextureImageId(int textureImageId)
{
	m_textureImageId = textureImageId;
	m_changeListeners.notify(this);
}

boost::optional<int> MaterialComponent::getTextureImageId()
{
	return m_textureImageId;
}

void MaterialComponent::removeTexture()
{
	m_textureImageId = boost::optional<int>();
	m_changeListeners.notify(this);
}

SceneNode *MaterialComponent::getOwner()
{
	return m_owner;
}
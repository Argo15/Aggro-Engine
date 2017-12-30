#include "MaterialComponent.hpp"

MaterialComponent::MaterialComponent(SceneNode *owner)
	: m_owner(owner)
	, m_color(1.0)
	, m_specIntensityPct(0)
	, m_shininess(25)
{

}

MaterialComponent::MaterialComponent(SceneNode *owner, shared_ptr<MaterialComponent> copy)
	: m_owner(owner)
	, m_color(copy->m_color)
	, m_textureImageId(copy->m_textureImageId)
	, m_alphaImageId(copy->m_alphaImageId)
	, m_specIntensityPct(copy->m_specIntensityPct)
	, m_shininess(copy->m_shininess)
	, m_specularImageId(copy->m_specularImageId)
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
	string alphaPath = parser.parseString().get_value_or("");
	if (alphaPath != "")
	{
		m_alphaImageId = boost::optional<int>(resources->getIdForPath(alphaPath));
	}
	m_specIntensityPct = parser.parseInt().get_value_or(0);
	m_shininess = parser.parseInt().get_value_or(25);
	string specularPath = parser.parseString().get_value_or("");
	if (specularPath != "")
	{
		m_specularImageId = boost::optional<int>(resources->getIdForPath(specularPath));
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
	string alphaName = "";
	if (m_alphaImageId)
	{
		alphaName = resources->getPathForId(m_alphaImageId.get()).get_value_or("");
	}
	bytes.add(&alphaName);
	bytes.add(&m_specIntensityPct);
	bytes.add(&m_shininess);
	string specularName = "";
	if (m_specularImageId)
	{
		specularName = resources->getPathForId(m_specularImageId.get()).get_value_or("");
	}
	bytes.add(&specularName);
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
	if (m_alphaImageId)
	{
		shared_ptr<TextureHandle> texture = textures->getTexture(m_alphaImageId.get());
		material->setAlpha(texture);
	}
	material->setSpecIntensity(getSpecIntensity());
	material->setShininess(m_shininess);
	if (m_specularImageId)
	{
		shared_ptr<TextureHandle> texture = textures->getTexture(m_specularImageId.get());
		material->setSpecular(texture);
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

void MaterialComponent::setAlphaImageId(int alphaImageId)
{
	m_alphaImageId = alphaImageId;
	m_changeListeners.notify(this);
}

boost::optional<int> MaterialComponent::getAlphaImageId()
{
	return m_alphaImageId;
}

void MaterialComponent::removeAlphaMap()
{
	m_alphaImageId = boost::optional<int>();
	m_changeListeners.notify(this);
}

SceneNode *MaterialComponent::getOwner()
{
	return m_owner;
}

void MaterialComponent::setSpecIntensityPct(int intensity)
{
	m_specIntensityPct = intensity;
	m_changeListeners.notify(this);
}

int MaterialComponent::getSpecIntensityPct()
{
	return m_specIntensityPct;
}

float MaterialComponent::getSpecIntensity()
{
	return ((float)m_specIntensityPct) / 100.0f;
}

void MaterialComponent::setShininess(int shininess)
{
	m_shininess = shininess;
	m_changeListeners.notify(this);
}

int MaterialComponent::getShininess()
{
	return m_shininess;
}

void MaterialComponent::setSpecularImageId(int textureImageId)
{
	m_specularImageId = textureImageId;
	m_changeListeners.notify(this);
}

boost::optional<int> MaterialComponent::getSpecularImageId()
{
	return m_specularImageId;
}

void MaterialComponent::removeSpecularMap()
{
	m_specularImageId = boost::optional<int>();
}
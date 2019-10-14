#include "MaterialComponent.hpp"

MaterialComponent::MaterialComponent(SceneNode *owner)
	: m_owner(owner)
	, m_color(1.0)
	, m_specIntensityPct(0)
	, m_shininess(25)
	, m_texScaleU(1.0)
	, m_texScaleV(1.0)
	, m_texOffsetU(0)
	, m_texOffsetV(0)
	, m_texRotate(0)
	, m_emissionColor(0)
	, m_name("")
{
}

MaterialComponent::MaterialComponent(SceneNode *owner, MaterialComponent *copy)
	: m_owner(owner)
	, m_color(copy->m_color)
	, m_textureImageId(copy->m_textureImageId)
	, m_alphaImageId(copy->m_alphaImageId)
	, m_specIntensityPct(copy->m_specIntensityPct)
	, m_shininess(copy->m_shininess)
	, m_specularImageId(copy->m_specularImageId)
	, m_texScaleU(copy->m_texScaleU)
	, m_texScaleV(copy->m_texScaleV)
	, m_texOffsetU(copy->m_texOffsetU)
	, m_texOffsetV(copy->m_texOffsetV)
	, m_texRotate(copy->m_texRotate)
	, m_emissionColor(copy->m_emissionColor)
	, m_normalImageId(copy->m_normalImageId)
	, m_name(copy->m_name)
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
	m_texScaleU = parser.parseFloat().get_value_or(1.0f);
	m_texScaleV = parser.parseFloat().get_value_or(1.0f);
	m_texOffsetU = parser.parseFloat().get_value_or(0);
	m_texOffsetV = parser.parseFloat().get_value_or(0);
	m_texRotate = parser.parseFloat().get_value_or(0);
	m_emissionColor = parser.parseVec3().get_value_or(glm::vec3(0));
	string emissionPath = parser.parseString().get_value_or("");
	if (emissionPath != "")
	{
		m_emissionImageId = boost::optional<int>(resources->getIdForPath(emissionPath));
	}
	string normalPath = parser.parseString().get_value_or("");
	if (normalPath != "")
	{
		m_normalImageId = boost::optional<int>(resources->getIdForPath(normalPath));
	}
	m_name = parser.parseString().get_value_or("");
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
	bytes.add(&m_texScaleU);
	bytes.add(&m_texScaleV);
	bytes.add(&m_texOffsetU);
	bytes.add(&m_texOffsetV);
	bytes.add(&m_texRotate);
	bytes.add(&m_emissionColor);
	string emissionName = "";
	if (m_emissionImageId)
	{
		emissionName = resources->getPathForId(m_emissionImageId.get()).get_value_or("");
	}
	bytes.add(&emissionName);
	string normalName = "";
	if (m_normalImageId)
	{
		normalName = resources->getPathForId(m_normalImageId.get()).get_value_or("");
	}
	bytes.add(&normalName);
	bytes.add(&m_name);
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
	glm::mat4 textureMatrix = glm::translate(glm::mat4(1.0), glm::vec3(m_texOffsetU, m_texOffsetV, 0));
	textureMatrix = glm::scale(textureMatrix, glm::vec3(m_texScaleU, m_texScaleV, 1.0));
	glm::mat4 texRotateMatrix = glm::rotate(glm::mat4(1.0), m_texRotate, glm::vec3(0, 0, 1.0));
	material->setTexRotateMatrix(texRotateMatrix);
	material->setTextureMatrix(texRotateMatrix * textureMatrix);
	material->setEmission(m_emissionColor);
	if (m_emissionImageId)
	{
		shared_ptr<TextureHandle> texture = textures->getTexture(m_emissionImageId.get());
		material->setEmissionMap(texture);
	}
	if (m_normalImageId)
	{
		shared_ptr<TextureHandle> texture = textures->getTexture(m_normalImageId.get());
		material->setNormalMap(texture);
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
	m_changeListeners.notify(this);
}

void MaterialComponent::setTexScaleU(float texScaleU)
{
	m_texScaleU = texScaleU;
	m_changeListeners.notify(this);
}

float MaterialComponent::getTexScaleU()
{
	return m_texScaleU;
}

void MaterialComponent::setTexScaleV(float texScaleV)
{
	m_texScaleV = texScaleV;
	m_changeListeners.notify(this);
}

float MaterialComponent::getTexScaleV()
{
	return m_texScaleV;
}

void MaterialComponent::setTexOffsetU(float texOffsetU)
{
	m_texOffsetU = texOffsetU;
	m_changeListeners.notify(this);
}

float MaterialComponent::getTexOffsetU()
{
	return m_texOffsetU;
}

void MaterialComponent::setTexOffsetV(float texOffsetV)
{
	m_texOffsetV = texOffsetV;
	m_changeListeners.notify(this);
}

float MaterialComponent::getTexOffsetV()
{
	return m_texOffsetV;
}

void MaterialComponent::setTexRotate(float texRotate)
{
	m_texRotate = texRotate;
	m_changeListeners.notify(this);
}

float MaterialComponent::getTexRotate()
{
	return m_texRotate;
}

void MaterialComponent::setEmission(glm::vec3 emission)
{
	m_emissionColor = emission;
	m_changeListeners.notify(this);
}

glm::vec3 MaterialComponent::getEmission()
{
	return m_emissionColor;
}

void MaterialComponent::setEmissionImageId(int textureImageId)
{
	m_emissionImageId = textureImageId;
	m_changeListeners.notify(this);
}

boost::optional<int> MaterialComponent::getEmissionImageId()
{
	return m_emissionImageId;
}

void MaterialComponent::removeEmissionMap()
{
	m_emissionImageId = boost::optional<int>();
	m_changeListeners.notify(this);
}

void MaterialComponent::setNormalImageId(int textureImageId)
{
	m_normalImageId = textureImageId;
	m_changeListeners.notify(this);
}

boost::optional<int> MaterialComponent::getNormalImageId()
{
	return m_normalImageId;
}

void MaterialComponent::removeNormalMap()
{
	m_normalImageId = boost::optional<int>();
	m_changeListeners.notify(this);
}

void MaterialComponent::setName(string name)
{
	m_name = name;
}

string MaterialComponent::getName()
{
	return m_name;
}
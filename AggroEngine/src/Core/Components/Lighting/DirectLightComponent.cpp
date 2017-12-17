#include "DirectLightComponent.hpp"

DirectLightComponent::DirectLightComponent()
	: m_color(1.0)
	, m_ambientPct(50)
{
}

DirectLightComponent::DirectLightComponent(Chunk * const byteChunk)
	: m_color(1.0)
	, m_ambientPct(50)
{
	ByteParser bytes = ByteParser(*byteChunk);
	m_color = bytes.parseVec3().get_value_or(glm::vec3(1.0));
	m_ambientPct = bytes.parseInt().get_value_or(50);
}

shared_ptr<Chunk> DirectLightComponent::serialize()
{
	ByteAccumulator bytes = ByteAccumulator();
	bytes.add(&m_color);
	bytes.add(&m_ambientPct);
	return shared_ptr<Chunk>(new Chunk(ChunkType::DIRECT_LIGHT_COMPONENT, bytes.getNumBytes(), bytes.collect()));
}

shared_ptr<DirectLightComponent> DirectLightComponent::deserialize(Chunk * const byteChunk)
{
	if (*byteChunk->getType() != ChunkType::DIRECT_LIGHT_COMPONENT)
	{
		return shared_ptr<DirectLightComponent>();
	}

	return shared_ptr<DirectLightComponent>(new DirectLightComponent(byteChunk));
}

void DirectLightComponent::addChangeListener(void *ns, std::function<void(DirectLightComponent *)> listener)
{
	boost::lock_guard<DirectLightComponent> guard(*this);
	m_changeListeners.add(ns, listener);
}

void DirectLightComponent::removeChangeListener(void *ns)
{
	boost::lock_guard<DirectLightComponent> guard(*this);
	m_changeListeners.remove(ns);
}

shared_ptr<DirectLight> DirectLightComponent::getLight(shared_ptr<TransformComponent> transform)
{
	if (!transform)
	{
		return shared_ptr<DirectLight>();
	}
	glm::vec4 direction = transform->getRotate() * glm::vec4(0, -1, 0, 0);
	return shared_ptr<DirectLight>(new DirectLight(direction, m_color, m_ambientPct));
}

void DirectLightComponent::setColor(glm::vec3 color)
{
	boost::lock_guard<DirectLightComponent> guard(*this);
	m_color = color;
	m_changeListeners.notify(this);
}

glm::vec3 DirectLightComponent::getColor()
{
	return m_color;
}

void DirectLightComponent::setAmbientPct(int ambientPct)
{
	boost::lock_guard<DirectLightComponent> guard(*this);
	m_ambientPct = ambientPct;
	m_changeListeners.notify(this);
}

int DirectLightComponent::getAmbientPct()
{
	return m_ambientPct;
}

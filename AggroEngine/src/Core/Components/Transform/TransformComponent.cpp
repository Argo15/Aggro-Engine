#include "TransformComponent.hpp"
#include "Locks.hpp"

TransformComponent::TransformComponent()
	: m_rotateMat(glm::mat4(1.0))
	, m_translateMat(glm::mat4(1.0))
	, m_scaleMat(glm::mat4(1.0))
	, m_translate(glm::vec3(0))
	, m_scale(glm::vec3(1))
{

}

TransformComponent::TransformComponent(Chunk * const byteChunk)
{
	ByteParser parser = ByteParser(*byteChunk->getNumBytes(), byteChunk->getByteData().get());
	m_rotateMat = glm::quat(parser.parseMat4().get_value_or(glm::mat4(1.0)));
	m_translateMat = parser.parseMat4().get_value_or(glm::mat4(1.0));
	m_scaleMat = parser.parseMat4().get_value_or(glm::mat4(1.0));
	m_translate = parser.parseVec3().get_value_or(glm::vec3(0));
	m_scale = parser.parseVec3().get_value_or(glm::vec3(1));
}

shared_ptr<Chunk> TransformComponent::serialize()
{
	ByteAccumulator bytes = ByteAccumulator();
	bytes.add(&glm::toMat4(m_rotateMat));
	bytes.add(&m_translateMat);
	bytes.add(&m_scaleMat);
	bytes.add(&m_translate);
	bytes.add(&m_scale);
	return shared_ptr<Chunk>(new Chunk(ChunkType::TRANSFORM_COMPONENT, bytes.getNumBytes(), bytes.collect()));
}

shared_ptr<TransformComponent> TransformComponent::deserialize(Chunk * const byteChunk)
{
	if (*byteChunk->getType() != ChunkType::TRANSFORM_COMPONENT)
	{
		return shared_ptr<TransformComponent>();
	}

	return shared_ptr<TransformComponent>(new TransformComponent(byteChunk));
}

void TransformComponent::translate(const glm::vec3 &translate)
{
	boost::lock_guard<TransformComponent> guard(*this);
	m_translate += translate;
	m_translateMat = glm::translate(glm::mat4(1.0), m_translate);
	m_changeListeners.notify(this);
}

void TransformComponent::rotate(float angle, const glm::vec3 &axis)
{
	boost::lock_guard<TransformComponent> guard(*this);
	m_rotateMat = glm::angleAxis(angle, glm::normalize(axis)) * m_rotateMat;
	m_changeListeners.notify(this);
}

void TransformComponent::scale(const glm::vec3 &scale)
{
	boost::lock_guard<TransformComponent> guard(*this);
	m_scale *= scale;
	m_scaleMat = glm::scale(glm::mat4(1.0), m_scale);
	m_changeListeners.notify(this);
}

void TransformComponent::setTranslate(glm::vec3 translate)
{
	boost::lock_guard<TransformComponent> guard(*this);
	m_translate = translate;
	m_translateMat = glm::translate(glm::mat4(1.0), m_translate);
	m_changeListeners.notify(this);
}

void TransformComponent::setRotate(glm::quat rotate)
{
	boost::lock_guard<TransformComponent> guard(*this);
	m_rotateMat = rotate;
	m_changeListeners.notify(this);
}

void TransformComponent::setRotate(glm::vec3 angle)
{
	boost::lock_guard<TransformComponent> guard(*this);
	m_rotateMat = glm::quat(angle);
	m_changeListeners.notify(this);
}

void TransformComponent::setScale(glm::vec3 scale)
{
	boost::lock_guard<TransformComponent> guard(*this);
	m_scale = scale;
	m_scaleMat = glm::scale(glm::mat4(1.0), m_scale);
	m_changeListeners.notify(this);
}

glm::mat4 TransformComponent::getTransform()
{
	return m_translateMat * glm::toMat4(m_rotateMat) * m_scaleMat;
}

glm::vec3 *TransformComponent::getTranslate()
{
	return &m_translate;
}

glm::vec3 TransformComponent::getRotateEuler()
{
	return glm::eulerAngles(m_rotateMat);
}

glm::mat4 TransformComponent::getRotate()
{
	return glm::toMat4(m_rotateMat);
}

glm::vec3 *TransformComponent::getScale()
{
	return &m_scale;
}

void TransformComponent::addChangeListener(void *ns, std::function<void(TransformComponent *)> listener)
{
	boost::lock_guard<TransformComponent> guard(*this);
	m_changeListeners.add(ns, listener);
}

void TransformComponent::removeChangeListener(void *ns)
{
	boost::lock_guard<TransformComponent> guard(*this);
	m_changeListeners.remove(ns);
}
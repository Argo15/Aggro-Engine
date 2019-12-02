#include "TransformComponent.hpp"
#include "Locks.hpp"
#include "MatrixDecompose.hpp"

TransformComponent::TransformComponent()
	: m_rotateMat(glm::mat4(1.0))
	, m_translateMat(glm::mat4(1.0))
	, m_scaleMat(glm::mat4(1.0))
	, m_translate(glm::vec3(0))
	, m_scale(glm::vec3(1))
{
	_refresh();
}

TransformComponent::TransformComponent(shared_ptr<TransformComponent> copy)
	: m_rotateMat(copy->m_rotateMat)
	, m_translateMat(copy->m_translateMat)
	, m_scaleMat(copy->m_scaleMat)
	, m_translate(copy->m_translate)
	, m_scale(copy->m_scale)
{
	_refresh();
}

TransformComponent::TransformComponent(Chunk * const byteChunk)
{
	ByteParser parser = ByteParser(*byteChunk->getNumBytes(), byteChunk->getByteData().get());
	m_rotateMat = glm::quat(parser.parseMat4().get_value_or(glm::mat4(1.0)));
	m_translateMat = parser.parseMat4().get_value_or(glm::mat4(1.0));
	m_scaleMat = parser.parseMat4().get_value_or(glm::mat4(1.0));
	m_translate = parser.parseVec3().get_value_or(glm::vec3(0));
	m_scale = parser.parseVec3().get_value_or(glm::vec3(1));
	_refresh();
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
	_refresh();
}

void TransformComponent::rotate(float angle, const glm::vec3 &axis, const glm::vec3 &center)
{
	boost::lock_guard<TransformComponent> guard(*this);

	// Goal is to ensure the center is the same after rotation
	glm::vec3 curCenter = getTransform() * glm::vec4(center, 1);

	m_rotateMat = glm::angleAxis(angle, glm::normalize(axis)) * m_rotateMat;

	glm::vec3 rotatedCenter = getTransform() * glm::vec4(center, 1);
	translate(glm::vec3(curCenter - rotatedCenter));

	_refresh();
}

void TransformComponent::scale(const glm::vec3 &scale)
{
	boost::lock_guard<TransformComponent> guard(*this);
	m_scale *= scale;
	m_scaleMat = glm::scale(glm::mat4(1.0), m_scale);
	_refresh();
}

void TransformComponent::setTranslate(glm::vec3 translate)
{
	boost::lock_guard<TransformComponent> guard(*this);
	m_translate = translate;
	m_translateMat = glm::translate(glm::mat4(1.0), m_translate);
	_refresh();
}

void TransformComponent::setRotate(glm::quat rotate)
{
	boost::lock_guard<TransformComponent> guard(*this);
	m_rotateMat = rotate;
	_refresh();
}

void TransformComponent::setRotate(glm::vec3 angle)
{
	boost::lock_guard<TransformComponent> guard(*this);
	m_rotateMat = glm::quat(angle);
	_refresh();
}

void TransformComponent::setScale(glm::vec3 scale)
{
	boost::lock_guard<TransformComponent> guard(*this);
	m_scale = scale;
	m_scaleMat = glm::scale(glm::mat4(1.0), m_scale);
	_refresh();
}

glm::mat4 TransformComponent::getTransform()
{
	return m_fullTransform;
}

void TransformComponent::_refresh()
{
	m_fullTransform = m_translateMat * glm::toMat4(m_rotateMat) * m_scaleMat;
	m_changeListeners.notify(this);
}

glm::vec3 TransformComponent::getTranslate()
{
	return m_translate;
}

glm::vec3 TransformComponent::getRotateEuler()
{
	return glm::eulerAngles(m_rotateMat);
}

glm::mat4 TransformComponent::getRotate()
{
	return glm::toMat4(m_rotateMat);
}

glm::vec3 TransformComponent::getScale()
{
	return m_scale;
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

void TransformComponent::setLookAt(glm::vec3 &eye, glm::vec3 &center, glm::vec3 &up)
{
	MatrixDecompose mat(glm::lookAt(eye, center, up));
	setTranslate(eye);
	setRotate(mat.getOriginalRotate());
}
#include "CameraComponent.hpp"
#include "MatrixDecompose.hpp"

static glm::vec4 baseLookDir(0.f, 0.f, -1.f, 0.f);
static glm::vec4 baseUpDir(0.f, 1.f, 0.f, 0.f);
static glm::vec4 baseRightDir(1.f, 0.f, 0.f, 0.f);

CameraComponent::CameraComponent()
	: m_v4Viewport(glm::vec4(0, 0, 1000, 1000))
{
}

CameraComponent::CameraComponent(Chunk * const byteChunk)
{
	ByteParser parser = ByteParser(*byteChunk->getNumBytes(), byteChunk->getByteData().get());
	m_v4Viewport = parser.parseVec4().get_value_or(glm::vec4(0));
	m_fov = parser.parseFloat().get_value_or(45.f);
	m_aspectRatio = parser.parseFloat().get_value_or(2.f);
	m_zNear = parser.parseFloat().get_value_or(0.01f);
	m_zFar = parser.parseFloat().get_value_or(100.f);
}

shared_ptr<Chunk> CameraComponent::serialize()
{
	ByteAccumulator bytes;
	bytes.add(&m_v4Viewport);
	bytes.add(&m_fov);
	bytes.add(&m_aspectRatio);
	bytes.add(&m_zNear);
	bytes.add(&m_zFar);
	return shared_ptr<Chunk>(new Chunk(ChunkType::CAMERA_COMPONENT, bytes.getNumBytes(), bytes.collect()));
}

shared_ptr<CameraComponent> CameraComponent::deserialize(Chunk * const byteChunk)
{
	if (*byteChunk->getType() != ChunkType::CAMERA_COMPONENT)
	{
		return shared_ptr<CameraComponent>();
	}

	return shared_ptr<CameraComponent>(new CameraComponent(byteChunk));
}

void CameraComponent::setProjection(float fov, float aspectRatio, float zNear, float zFar)
{
	m_fov = fov;
	m_aspectRatio = aspectRatio;
	m_zNear = zNear;
	m_zFar = zFar;
}

void CameraComponent::setViewport(glm::vec4 &viewport)
{
	m_v4Viewport = viewport;
}

glm::vec4 &CameraComponent::getViewport()
{
	return m_v4Viewport;
}

shared_ptr<Camera> CameraComponent::getCamera(shared_ptr<TransformComponent> transformComponent)
{
	shared_ptr<Camera> camera(new Camera());
	glm::mat4 transform = transformComponent->getTransform();
	glm::vec3 eyePos = transformComponent->getTranslate();
	glm::vec3 lookDir = glm::vec3(transform * baseLookDir);
	glm::vec3 upDir = glm::vec3(transform * baseUpDir);
	glm::vec3 rightDir = glm::vec3(transform * baseRightDir);

	camera->setFrustrum(shared_ptr<Frustrum>(new Frustrum(eyePos, lookDir, upDir, rightDir,
														  m_fov, m_aspectRatio, m_zNear, m_zFar)));
	camera->setViewport(m_v4Viewport);
	camera->setViewMatrix(glm::lookAt(eyePos, eyePos + lookDir, upDir));
	camera->setProjMatrix(glm::perspective(m_fov, m_aspectRatio, m_zNear, m_zFar));
	camera->setEyePos(eyePos);
	camera->setLookDir(lookDir);
	camera->setUpDir(upDir);
	camera->setRightDir(rightDir);
	return camera;
}
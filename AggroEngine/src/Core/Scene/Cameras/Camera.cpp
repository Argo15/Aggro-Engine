#include "Camera.hpp"

static glm::vec4 baseEyePos(0.f,0.f,0.f,1.f);
static glm::vec4 baseLookDir(0.f,0.f,-1.f,0.f);
static glm::vec4 baseUpDir(0.f,1.f,0.f,0.f);
static glm::vec4 baseRightDir(1.f,0.f,0.f,0.f);

Camera::Camera()
	: m_transformComponent(shared_ptr<TransformComponent>(new TransformComponent()))
	, m_v3EyePos(baseEyePos)
	, m_v3LookDir(baseLookDir)
	, m_v3UpDir(baseUpDir)
	, m_v3RightDir(baseRightDir)
	, m_v4Viewport(glm::vec4(0, 0, 1000, 1000))
{
	// Default position
	translate(glm::vec3(4.f, 4.f, 4.f));

	// Default look
	rotate(0.79f, m_v3UpDir);
	rotate(-0.61f, m_v3RightDir);
}

Camera::Camera(Chunk * const byteChunk)
{
	ByteParser parser = ByteParser(*byteChunk->getNumBytes(), byteChunk->getByteData().get());
	while (boost::optional<Chunk> nextChunk = parser.parseChunk())
	{
		if (*nextChunk->getType() == ChunkType::PRIMITIVES)
		{
			ByteParser primitives = ByteParser(*nextChunk);
			m_m4ViewMatrix = primitives.parseMat4().get_value_or(glm::mat4(1.0));
			m_m4ProjMatrix = primitives.parseMat4().get_value_or(glm::mat4(1.0));
			m_v4Viewport = primitives.parseVec4().get_value_or(glm::vec4(0, 0, 1000, 1000));
		}
		else if (*nextChunk->getType() == ChunkType::TRANSFORM_COMPONENT)
		{
			m_transformComponent = TransformComponent::deserialize(nextChunk.get_ptr());
		}
	}
	updateViewMatrix();
}

shared_ptr<Chunk> Camera::serialize()
{
	shared_ptr<Chunk> chunk;
	vector<shared_ptr<Chunk>> chunks;

	ByteAccumulator bytes;

	ByteAccumulator primitiveBytes;
	primitiveBytes.add(&m_m4ViewMatrix);
	primitiveBytes.add(&m_m4ProjMatrix);
	primitiveBytes.add(&m_v4Viewport);
	bytes.add(new Chunk(ChunkType::PRIMITIVES, primitiveBytes.getNumBytes(), primitiveBytes.collect()));

	if (m_transformComponent)
	{
		chunk = m_transformComponent->serialize();
		chunks.push_back(chunk);
		bytes.add(chunk.get());
	}
	return shared_ptr<Chunk>(new Chunk(ChunkType::CAMERA, bytes.getNumBytes(), bytes.collect()));
}

shared_ptr<Camera> Camera::deserialize(Chunk * const byteChunk)
{
	if (*byteChunk->getType() != ChunkType::CAMERA)
	{
		return shared_ptr<Camera>();
	}

	return shared_ptr<Camera>(new Camera(byteChunk));
}

void Camera::updateViewMatrix()
{
	glm::mat4 transform = m_transformComponent->getTransform();
	m_v3EyePos = glm::vec3(transform * baseEyePos);
	m_v3LookDir = glm::vec3(transform * baseLookDir);
	m_v3UpDir = glm::vec3(transform * baseUpDir);
	m_v3RightDir = glm::vec3(transform * baseRightDir);
	m_m4ViewMatrix = glm::lookAt(m_v3EyePos, m_v3EyePos+m_v3LookDir, m_v3UpDir);
}

void Camera::translate(const glm::vec3 &translate)
{
	m_transformComponent->translate(translate);
	updateViewMatrix();
}

void Camera::rotate(float angle, const glm::vec3 &axis)
{
	m_transformComponent->rotate(angle, axis);
	updateViewMatrix();
}

void Camera::scale(const glm::vec3 &scale)
{
	m_transformComponent->scale(scale);
	updateViewMatrix();
}

void Camera::setProjection(float fov, float aspectRatio, float zNear, float zFar)
{
	m_fov = fov;
	m_aspectRatio = aspectRatio;
	m_zNear = zNear;
	m_zFar = zFar;
	m_m4ProjMatrix = glm::perspective(fov, aspectRatio, zNear, zFar);
}

void Camera::setLookAt(glm::vec3 &eye, glm::vec3 &center, glm::vec3 &up)
{
	// TODO, math this one out.
}

void Camera::setViewport(glm::vec4 &viewport)
{
	m_v4Viewport = viewport;
}

glm::mat4 &Camera::getViewMatrix()
{
	return m_m4ViewMatrix;
}

glm::mat4 &Camera::getProjMatrix()
{
	return m_m4ProjMatrix;
}

glm::vec3 &Camera::getEyePos()
{
	return m_v3EyePos;
}

glm::vec3 &Camera::getLookDir()
{
	return m_v3LookDir;
}

glm::vec3 &Camera::getUpDir()
{
	return m_v3UpDir;
}

glm::vec3 &Camera::getRightDir()
{
	return m_v3RightDir;
}

glm::vec4 &Camera::getViewport()
{
	return m_v4Viewport;
}

shared_ptr<Frustrum> Camera::getFrustrum()
{
	return shared_ptr<Frustrum>(new Frustrum(m_v3EyePos, m_v3LookDir, m_v3UpDir, m_v3RightDir, 
											 m_fov, m_aspectRatio, m_zNear, m_zFar));
}
#include "DirectLightComponent.hpp"

DirectLightComponent::DirectLightComponent()
{
}

DirectLightComponent::DirectLightComponent(Chunk * const byteChunk)
{
}

shared_ptr<Chunk> DirectLightComponent::serialize()
{
	ByteAccumulator bytes = ByteAccumulator();
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

shared_ptr<DirectLight> DirectLightComponent::getLight(shared_ptr<TransformComponent> transform)
{
	if (!transform)
	{
		return shared_ptr<DirectLight>();
	}
	glm::vec4 direction = transform->getRotate() * glm::vec4(0, -1, 0, 0);
	return shared_ptr<DirectLight>(new DirectLight(direction));
}
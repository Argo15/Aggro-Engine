#include "RenderComponent.hpp"
#include "StaticObjectRenderComponent.hpp"

RenderComponent::RenderComponent()
	: Component()
{
}

shared_ptr<RenderComponent> RenderComponent::deserialize(Chunk * const byteChunk, shared_ptr<Resources> resources)
{
	if (*byteChunk->getType() != ChunkType::RENDER_COMPONENT)
	{
		return shared_ptr<RenderComponent>();
	}

	ByteParser parser = ByteParser(*byteChunk->getNumBytes(), byteChunk->getByteData().get());
	if (boost::optional<Chunk> subChunk = parser.parseChunk())
	{
		if (*subChunk->getType() == ChunkType::STATIC_OBJECT_RENDER_COMPONENT)
		{
			return StaticObjectRenderComponent::deserialize(&*subChunk, resources);
		}
	}
	return shared_ptr<RenderComponent>();
}

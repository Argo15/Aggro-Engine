#include "RenderComponent.hpp"
#include "DirectLightRenderComponent.hpp"
#include "CameraRenderComponent.hpp"

RenderComponent::RenderComponent()
	: Component()
	, m_sceneNode(nullptr)
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
		else if (*subChunk->getType() == ChunkType::SPRITE_RENDER_COMPONENT)
		{
			return SpriteRenderComponent::deserialize(&*subChunk, resources);
		}
		else if (*subChunk->getType() == ChunkType::DIRECT_LIGHT_RENDER_COMPONENT)
		{
			return DirectLightRenderComponent::deserialize(&*subChunk, resources);
		}
		else if (*subChunk->getType() == ChunkType::CAMERA_RENDER_COMPONENT)
		{
			return CameraRenderComponent::deserialize(&*subChunk, resources);
		}
	}
	return shared_ptr<RenderComponent>();
}

void RenderComponent::setSceneNode(SceneNode *node)
{
	m_sceneNode = node;
	m_sceneNode->addDeletedListener(this, [this](auto node) { this->onSceneNodeDeleted(node); });
}

#include "SpriteRenderComponent.hpp"
#include "MatrixDecompose.hpp"

SpriteRenderComponent::SpriteRenderComponent(shared_ptr<Resources> resources)
	: StaticObjectRenderComponent()
{
	setLightingEnabled(false);
	setShadowsEnabled(false);
}

SpriteRenderComponent::SpriteRenderComponent(Chunk * const byteChunk, shared_ptr<Resources> resources)
	: SpriteRenderComponent(resources)
{
}

SpriteRenderComponent::SpriteRenderComponent()
{
}

shared_ptr<Chunk> SpriteRenderComponent::serialize(shared_ptr<Resources> resources)
{
	ByteAccumulator bytes = ByteAccumulator();
	bytes.add(shared_ptr<Chunk>(new Chunk(ChunkType::SPRITE_RENDER_COMPONENT, bytes.getNumBytes(), bytes.collect())));
	return shared_ptr<Chunk>(new Chunk(ChunkType::RENDER_COMPONENT, bytes.getNumBytes(), bytes.collect()));
}

shared_ptr<RenderComponent> SpriteRenderComponent::deserialize(Chunk * const byteChunk, shared_ptr<Resources> resources)
{
	if (*byteChunk->getType() != ChunkType::SPRITE_RENDER_COMPONENT)
	{
		return shared_ptr<SpriteRenderComponent>();
	}

	return shared_ptr<SpriteRenderComponent>(new SpriteRenderComponent(byteChunk, resources));
}

void SpriteRenderComponent::render(shared_ptr<GraphicsContext> context, glm::mat4 &m4Transform, glm::mat4 &m4ViewMat)
{
	// Undo camera rotation
	glm::mat4 invCamera = glm::inverse(m4ViewMat);
	MatrixDecompose invCameraDecompose(invCamera);
	MatrixDecompose objectDecompose(m4Transform);

	glm::mat4 cameraUndo = glm::scale(
		glm::translate(glm::mat4(1.0), objectDecompose.getTranslate()) *
		glm::toMat4(invCameraDecompose.getRotate()
	), objectDecompose.getScale());

	StaticObjectRenderComponent::render(context, cameraUndo, m4ViewMat);
}

void SpriteRenderComponent::onSceneNodeDeleted(SceneNode *node)
{
	StaticObjectRenderComponent::onSceneNodeDeleted(node);
}
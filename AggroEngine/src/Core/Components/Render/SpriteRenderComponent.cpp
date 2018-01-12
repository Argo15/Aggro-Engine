#include "SpriteRenderComponent.hpp"
#include "MatrixDecompose.hpp"

const static string meshPath = "Resources/Mesh/Engine/Plane.obj";

SpriteRenderComponent::SpriteRenderComponent(shared_ptr<Resources> resources)
	: StaticObjectRenderComponent()
{
	setLightingEnabled(false);
	setShadowsEnabled(false);
	setMeshId(resources->getIdForPath(meshPath));
}

SpriteRenderComponent::SpriteRenderComponent(Chunk * const byteChunk, shared_ptr<Resources> resources)
	: StaticObjectRenderComponent()
{
	setLightingEnabled(false);
	setShadowsEnabled(false);
	ByteParser parser = ByteParser(*byteChunk->getNumBytes(), byteChunk->getByteData().get());
	setMeshId(resources->getIdForPath(meshPath)); 
}

SpriteRenderComponent::SpriteRenderComponent()
{
}

shared_ptr<Chunk> SpriteRenderComponent::serialize(shared_ptr<Resources> resources)
{
	ByteAccumulator bytes;
	shared_ptr<Chunk> chunk(new Chunk(ChunkType::SPRITE_RENDER_COMPONENT, bytes.getNumBytes(), bytes.collect()));
	bytes = ByteAccumulator();
	bytes.add(chunk.get());
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

void SpriteRenderComponent::render(shared_ptr<GraphicsContext> context, glm::mat4 m4Transform, glm::mat4 m4ViewMat, shared_ptr<SceneNode> node)
{
	// Undo camera rotation
	glm::mat4 invCamera = glm::inverse(m4ViewMat);
	MatrixDecompose invCameraDecompose(invCamera);
	MatrixDecompose objectDecompose(m4Transform);

	glm::mat4 cameraUndo = glm::scale(
		glm::translate(glm::mat4(1.0), objectDecompose.getTranslate()) *
		glm::toMat4(invCameraDecompose.getRotate()
	), objectDecompose.getScale());

	StaticObjectRenderComponent::render(context, cameraUndo, m4ViewMat, node);
}

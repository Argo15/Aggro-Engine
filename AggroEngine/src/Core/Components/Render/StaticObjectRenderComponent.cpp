#include "StaticObjectRenderComponent.hpp"

StaticObjectRenderComponent::StaticObjectRenderComponent()
	: RenderComponent()
	, m_lightingEnabled(true)
	, m_shadowsEnabled(true)
{
}

StaticObjectRenderComponent::StaticObjectRenderComponent(Chunk * const byteChunk, shared_ptr<Resources> resources)
	: RenderComponent()
	, m_lightingEnabled(true)
	, m_shadowsEnabled(true)
{
	ByteParser parser = ByteParser(*byteChunk->getNumBytes(), byteChunk->getByteData().get());
}

shared_ptr<Chunk> StaticObjectRenderComponent::serialize(shared_ptr<Resources> resources)
{
	ByteAccumulator bytes;
	shared_ptr<Chunk> chunk(new Chunk(ChunkType::STATIC_OBJECT_RENDER_COMPONENT, bytes.getNumBytes(), bytes.collect()));
	bytes = ByteAccumulator();
	bytes.add(chunk.get());
	return shared_ptr<Chunk>(new Chunk(ChunkType::RENDER_COMPONENT, bytes.getNumBytes(), bytes.collect()));
}

shared_ptr<RenderComponent> StaticObjectRenderComponent::deserialize(
	Chunk * const byteChunk, shared_ptr<Resources> resources)
{
	if (*byteChunk->getType() != ChunkType::STATIC_OBJECT_RENDER_COMPONENT)
	{
		return shared_ptr<StaticObjectRenderComponent>();
	}

	return shared_ptr<StaticObjectRenderComponent>(new StaticObjectRenderComponent(byteChunk, resources));
}

void StaticObjectRenderComponent::render(shared_ptr<GraphicsContext> context, glm::mat4 &m4Transform, glm::mat4 &m4ViewMat, shared_ptr<SceneNode> node)
{
	shared_ptr<MeshComponent> meshComponent = node->getMeshComponent();
	if (!meshComponent || !meshComponent->hasMesh())
	{
		return;
	}
	boost::lock_guard<MeshComponent> guard(*meshComponent.get());

	if (meshComponent->modsReady(context->getVboCache()))
	{
		for (shared_ptr<Mesh> mesh : meshComponent->getModifiedMeshes())
		{
			_renderMesh(mesh, context, m4Transform, m4ViewMat, node);
		}
	}
	else
	{
		_renderMesh(meshComponent->getPrimaryMesh(), context, m4Transform, m4ViewMat, node);
	}
}

void StaticObjectRenderComponent::_renderMesh(shared_ptr<Mesh> mesh, 
											  shared_ptr<GraphicsContext> context,
											  glm::mat4 &m4Transform, 
											  glm::mat4 &m4ViewMat, 
											  shared_ptr<SceneNode> node)
{
	shared_ptr<VertexBufferHandle> vbo = context->getVboCache()->getVertexBuffer(mesh);
	if (vbo && vbo->isLoaded())
	{
		shared_ptr<RenderData> renderData(new RenderData(vbo));
		renderData->setModelMatrix(m4Transform);
		renderData->setLightingEnabled(m_lightingEnabled);
		renderData->setShadowsEnabled(m_shadowsEnabled);
		if (node)
		{
			renderData->setId(node->getId());
			if (node->hasMaterialComponent() && mesh->getDrawMode() == DrawMode::TRIANGLES)
			{
				renderData->setMaterial(node->getMaterialComponent()->getMaterial(context->getTextureCache()));
			}
		}
		renderData->setDrawMode(mesh->getDrawMode());
		context->getGraphics()->stageRender(renderData);
	}
}

void StaticObjectRenderComponent::setLightingEnabled(int lightingEnabled)
{
	m_lightingEnabled = lightingEnabled;
}

bool StaticObjectRenderComponent::getLightingEnabled()
{
	return m_lightingEnabled;
}

void StaticObjectRenderComponent::setShadowsEnabled(int shadowsEnabled)
{
	m_shadowsEnabled = shadowsEnabled;
}

bool StaticObjectRenderComponent::getShadowsEnabled()
{
	return m_shadowsEnabled;
}
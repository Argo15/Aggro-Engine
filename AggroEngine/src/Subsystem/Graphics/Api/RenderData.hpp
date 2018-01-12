#pragma once

#include <memory>
#include "DrawMode.hpp"
#include "VertexBufferHandle.hpp"
#include "TextureHandle.hpp"
#include "Matrix.hpp"
#include "Material.hpp"
using namespace std;

/**
 * Data for a single renderable object. Includes VBO handle, transformation, material, etc.
 *
 * author: wcrane
 * since: 2014-01-25
**/
class RenderData
{
private:
	unsigned int m_id;
	shared_ptr<VertexBufferHandle> m_pVertexBufferHandle;
	shared_ptr<Material> m_material;
	glm::mat4 m_m4ModelMatrix;
	DrawMode m_drawMode;
	int m_lineWidth;
	bool m_depthTestEnabled;
	bool m_lightingEnabled;
	bool m_shadowsEnabled;

public:
	RenderData();
	RenderData(shared_ptr<VertexBufferHandle> pVertexBufferHandle);
	RenderData(shared_ptr<VertexBufferHandle> pVertexBufferHandle, DrawMode drawMode);

	void setId(unsigned int id);
	unsigned int getId();

	RenderData *setModelMatrix(glm::mat4 &m4ModelMatrix);
	RenderData *setVertexBufferHandle(shared_ptr<VertexBufferHandle> handle);
	RenderData *setDrawMode(DrawMode drawMode);

	shared_ptr<VertexBufferHandle> getVertexBufferHandle();
	glm::mat4 &getModelMatrix();
	DrawMode getDrawMode();

	void setMaterial(shared_ptr<Material> material);
	shared_ptr<Material> getMaterial();

	void setLineWidth(int width);
	int getLineWidth();

	void setDepthTestEnabled(bool enabled);
	bool isDepthTestEnabled();

	void setLightingEnabled(bool enabled);
	bool isLightingEnabled();

	void setShadowsEnabled(bool enabled);
	bool isShadowsEnabled();
};
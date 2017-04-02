#pragma once

#include <memory>
#include "DrawMode.hpp"
#include "VertexBufferHandle.hpp"
#include "TextureHandle.hpp"
#include "Matrix.hpp"
using namespace std;

/**
 * Data for a single render operation, such as VBO handle, transformation, material, etc.
 *
 * author: wcrane
 * since: 2014-01-25
**/
class RenderData
{
private:
	unsigned int m_id;
	shared_ptr<VertexBufferHandle> m_pVertexBufferHandle;
	shared_ptr<TextureHandle> m_pTextureHandle;
	glm::mat4 m_m4ModelMatrix;
	DrawMode m_drawMode;
	int m_lineWidth;
	bool m_depthTestEnabled;

public:
	RenderData();
	RenderData(shared_ptr<VertexBufferHandle> pVertexBufferHandle, shared_ptr<TextureHandle> pTextureHandle);
	RenderData(shared_ptr<VertexBufferHandle> pVertexBufferHandle, shared_ptr<TextureHandle> pTextureHandle, DrawMode drawMode);

	void setId(unsigned int id);
	unsigned int getId();

	RenderData *setModelMatrix(glm::mat4 &m4ModelMatrix);
	RenderData *setVertexBufferHandle(shared_ptr<VertexBufferHandle> handle);
	RenderData *setTextureHandle(shared_ptr<TextureHandle> handle);
	RenderData *setDrawMode(DrawMode drawMode);

	shared_ptr<VertexBufferHandle> getVertexBufferHandle();
	shared_ptr<TextureHandle> getTextureHandle();
	glm::mat4 &getModelMatrix();
	DrawMode getDrawMode();

	void setLineWidth(int width);
	int getLineWidth();

	void setDepthTestEnabled(bool enabled);
	bool isDepthTestEnabled();
};
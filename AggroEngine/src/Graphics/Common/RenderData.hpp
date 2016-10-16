#pragma once

#include <memory>
#include "DrawMode.hpp"
#include "VertexBuffer\VertexBufferHandle.hpp"
#include "Texture\TextureHandle.hpp"
#include "../../Util/Math/Matrix.hpp"
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
	shared_ptr<VertexBufferHandle> m_pVertexBufferHandle;
	shared_ptr<TextureHandle> m_pTextureHandle;
	glm::mat4 m_m4ModelMatrix;
	DrawMode m_drawMode;
	unsigned int m_id;

public:
	RenderData(shared_ptr<VertexBufferHandle> pVertexBufferHandle, shared_ptr<TextureHandle> pTextureHandle);
	RenderData(shared_ptr<VertexBufferHandle> pVertexBufferHandle, shared_ptr<TextureHandle> pTextureHandle, DrawMode drawMode);

	RenderData &setModelMatrix(glm::mat4 &m4ModelMatrix);

	shared_ptr<VertexBufferHandle> getVertexBufferHandle();
	shared_ptr<TextureHandle> getTextureHandle();
	glm::mat4 &getModelMatrix();
	DrawMode getDrawMode();

	void setId(unsigned int id);
	unsigned int getId();
};
#pragma once

class GBuffer;
class ShadowMapBuffer;

#include "FrameBufferObject.hpp"
#include "OpenGL43Graphics.hpp"
#include "Sync/BufferSyncContext.hpp"

/**
* Renders the scene from the lights perspective, 
* 4 times at different partitions of the frustrum
*
* author: wcrane
* since: 2018-01-11
**/
class ShadowMapBuffer : public FrameBufferObject
{
private:
	OpenGL43Graphics *m_graphics;
	GLuint m_shadowBuffer[4];
	shared_ptr<TextureHandle> m_shadowTex[4];
	shared_ptr<TextureHandle> m_testTex[4];
	glm::mat4 m_viewProj[4];
	glm::mat4 m_adjViewProj[4];
	int m_size;
	float *m_slices;

	void _getProjectionMat(int slice, float slicePctStart, float slicePctEnd, glm::mat4 &lightViewMat, RenderOptions &renderOptions);

public:
	ShadowMapBuffer(OpenGL43Graphics *graphics, int defaultSize);

	void drawToBuffer(RenderOptions renderOptions, std::deque<shared_ptr<RenderData>> &renderQueue, shared_ptr<BufferSyncContext> syncContext);

	void bindShadowTex(int slice);
	shared_ptr<TextureHandle> getShadowTex(int slice);

	void bindTestTex(int slice);
	shared_ptr<TextureHandle> getTestTex(int slice);

	glm::mat4 getViewProjection(int slice);
	float *getAdjViewProjectionPointer(int slice);

	int getSize();
	float getSizeF();
};
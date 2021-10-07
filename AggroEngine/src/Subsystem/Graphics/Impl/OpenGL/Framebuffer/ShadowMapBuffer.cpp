#include "ShadowMapBuffer.hpp"
#include "Config.hpp"
#include "ShadowMapFrustrum.hpp"
#include "CommandTree/ShadowMap/InitializeShadowMap.hpp"
#include "CommandTree/ShadowMap/Filter/FilterShadowLayer.hpp"
#include "CommandTree/ShadowMap/MVPMatrix/MVPMatrix.hpp"
#include "CommandTree/ShadowMap/DrawElements/DrawShadowElements.hpp"

// Order to update each shadowmap
static const char s_frames[8] = { 0,1,0,2,0,1,0,3 };
static char s_currentFrame = -1;

ShadowMapBuffer::ShadowMapBuffer(OpenGL43Graphics *graphics, int defaultSize, shared_ptr<BufferSyncContext> syncContext)
	: FrameBufferObject(defaultSize, defaultSize)
{
	const Properties& props = Config::instance().getProperties();
	m_size = props.getIntProperty("graphics.shadowRes", defaultSize);
	m_width = m_size;
	m_height = m_size;

	vector<float> configSlices = props.getFloatArrayProperty("graphics.shadowSlices");
	m_slices = new float[5] { 0, configSlices[0], configSlices[1], configSlices[2], 1.0f };

	m_graphics = graphics;

	boost::lock_guard<OpenGL43Graphics> guard(*m_graphics);

	m_glslProgram = graphics->getShaderStore().getShader("Resources/Shaders/v_shadow.glsl", "Resources/Shaders/f_dshadow.glsl");

	glEnable(GL_TEXTURE_2D);

	for (int i = 0; i < 4; i++)
	{
		shared_ptr<ImageUC> fboImage((new ImageUC(m_width, m_height, mem::shared_array<unsigned char>(0, 0, "GL_Graphics")))
			->setImageFormat(ImageFormat::RGBA)
		);
		shared_ptr<TextureBuildOptions> texOptions(new TextureBuildOptions(fboImage));

		glGenFramebuffers(1, &m_shadowBuffer[i]);
		glBindFramebuffer(GL_FRAMEBUFFER, m_shadowBuffer[i]);

		// Generate test texture
		fboImage->setImageType(ImageType::FLOAT_TYPE);
		texOptions->genMipmaps(false)
			->setWrap(Wrap::CLAMP_TO_EDGE)
			->setInternalFormat(InternalFormat::RGBA8);
		m_testTex[i] = graphics->createTexture(texOptions);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_testTex[i]->get(), 0);

		// Generate depth texture
		fboImage->setImageFormat(ImageFormat::DEPTH_COMPONENT);
		texOptions->setInternalFormat(InternalFormat::DEPTH_COMPONENT32)->enableDepthCompare();
		m_shadowTex[i] = graphics->createTexture(texOptions);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_shadowTex[i]->get(), 0);

		// check FbO status
		GLenum FBOstatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (FBOstatus != GL_FRAMEBUFFER_COMPLETE)
		{
			printf("ShadowMapBuffer init failed, status:%i\n", FBOstatus);
		}
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	m_commands = shared_ptr<CommandTree>(new CommandTree());
	m_commands->addLayer(shared_ptr<Layer>(new InitializeShadowMap(this)));
	m_commands->addLayer(shared_ptr<Layer>(new FilterShadowLayer(syncContext)));
	m_commands->addLayer(shared_ptr<Layer>(new MVPMatrix(this)));
	m_commands->addLayer(shared_ptr<Layer>(new DrawShadowElements(this)));
}

void ShadowMapBuffer::drawToBuffer(RenderOptions &renderOptions, shared_ptr<RenderChain> renderChain)
{
	boost::lock_guard<OpenGL43Graphics> guard(*m_graphics);
	_updateProjectionMat(renderOptions);
	m_commands->execute(renderOptions, renderChain);
	s_currentFrame = (s_currentFrame + 1) % 8;
}

void ShadowMapBuffer::_updateProjectionMat(RenderOptions &renderOptions)
{
	shared_ptr<DirectLight> light = renderOptions.getDirectLight();
	if (!light) return;
	glm::vec3 lightDir = light->getDirection();
	glm::vec3 lightUp = light->getDirection() == glm::vec3(0, -1.0f, 0) ? glm::vec3(1.0f) : glm::vec3(0, 1.0f, 0);
	
	shared_ptr<PerspectiveFrustrum> frustrum = renderOptions.getFrustrum();
	float frustrumLength = frustrum->getZFar() - frustrum->getZNear();

	int slice = s_frames[s_currentFrame];

	float slicePctStart = m_slices[slice];
	float slicePctEnd = m_slices[slice + 1];
	float centerPct = (slicePctStart + slicePctEnd) / 2.0f;

	glm::vec3 center = frustrum->getEyePos() + frustrum->getLookDir() * (frustrum->getZNear() + frustrumLength * centerPct);
	glm::mat4 lightViewMat = glm::lookAt(center + lightDir * -250.0f, center, lightUp);

	float nearDistance = frustrum->getZNear() + slicePctStart * frustrumLength;
	float nearHeight = 2 * tan(frustrum->getFov() / 2) * nearDistance;
	float nearWidth = nearHeight * frustrum->getAspectRatio();

	float farDisance = frustrum->getZNear() + slicePctEnd * frustrumLength;
	float farHeight = 2 * tan(frustrum->getFov() / 2) * farDisance;
	float farWidth = farHeight * frustrum->getAspectRatio();

	glm::vec3 nearCenter = frustrum->getEyePos() + frustrum->getLookDir() * nearDistance;
	glm::vec3 farCenter = frustrum->getEyePos() + frustrum->getLookDir() * farDisance;

	glm::vec3 corners[] = {
		nearCenter + (nearHeight / 2) * frustrum->getUpDir() + (nearWidth / 2) * frustrum->getRightDir(),
		nearCenter - (nearHeight / 2) * frustrum->getUpDir() + (nearWidth / 2) * frustrum->getRightDir(),
		nearCenter + (nearHeight / 2) * frustrum->getUpDir() - (nearWidth / 2) * frustrum->getRightDir(),
		nearCenter - (nearHeight / 2) * frustrum->getUpDir() - (nearWidth / 2) * frustrum->getRightDir(),
		farCenter + (farHeight / 2) * frustrum->getUpDir() + (farWidth / 2) * frustrum->getRightDir(),
		farCenter - (farHeight / 2) * frustrum->getUpDir() + (farWidth / 2) * frustrum->getRightDir(),
		farCenter + (farHeight / 2) * frustrum->getUpDir() - (farWidth / 2) * frustrum->getRightDir(),
		farCenter - (farHeight / 2) * frustrum->getUpDir() - (farWidth / 2) * frustrum->getRightDir()
	};

	glm::mat4 vpMat = glm::ortho(-1.f, 1.f, -1.f, 1.f, 0.1f, 100.f) * lightViewMat;
	float minX = 0;
	float maxX = 0;
	float minY = 0;
	float maxY = 0;
	for (int i = 0; i < 8; i++)
	{
		glm::vec4 pos = vpMat * glm::vec4(corners[i], 1.0);
		minX = min(minX, pos.x);
		maxX = max(maxX, pos.x);
		minY = min(minY, pos.y);
		maxY = max(maxY, pos.y);
	}

	glm::mat4 lightProjMat = glm::ortho(minX, maxX, minY, maxY, 0.1f, 500.f);
	m_viewProj[slice] = lightProjMat * lightViewMat;
	m_adjViewProj[slice] = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f)) *
		glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f)) * m_viewProj[slice];
}

void ShadowMapBuffer::bindShadowTex(int slice)
{
	glBindTexture(GL_TEXTURE_2D, m_shadowTex[slice]->get());
}

shared_ptr<TextureHandle> ShadowMapBuffer::getShadowTex(int slice)
{
	return m_shadowTex[slice];
}

void ShadowMapBuffer::bindTestTex(int slice)
{
	glBindTexture(GL_TEXTURE_2D, m_testTex[slice]->get());
}

shared_ptr<TextureHandle> ShadowMapBuffer::getTestTex(int slice)
{
	return m_testTex[slice];
}

glm::mat4 *ShadowMapBuffer::getViewProjection(int slice)
{
	return &(m_viewProj[slice]);
}

float *ShadowMapBuffer::getAdjViewProjectionPointer(int slice)
{
	return glm::value_ptr(m_adjViewProj[slice]);
}

int ShadowMapBuffer::getSize()
{
	return m_size;
}

float ShadowMapBuffer::getSizeF()
{
	return (float)m_size;
}

int ShadowMapBuffer::currentSlice()
{
	return s_frames[s_currentFrame];
}

GLuint ShadowMapBuffer::getCurrentShadowBuffer()
{
	return m_shadowBuffer[currentSlice()];
}
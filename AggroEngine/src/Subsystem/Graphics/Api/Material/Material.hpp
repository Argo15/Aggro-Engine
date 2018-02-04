#pragma once

#include "Matrix.hpp"
#include "TextureHandle.hpp"
#include <memory>
#include <boost/optional.hpp>
using namespace std;

class Material
{
private:
	glm::vec3 m_color;
	shared_ptr<TextureHandle> m_texture;
	shared_ptr<TextureHandle> m_alpha;
	float m_specIntensity;
	int m_shininess;
	shared_ptr<TextureHandle> m_specular;
	glm::mat4 m_textureMatrix;
	glm::mat4 m_texRotateMatrix;
	glm::vec3 m_emissionColor;
	shared_ptr<TextureHandle> m_emissionMap;
	shared_ptr<TextureHandle> m_normalMap;

public:
	Material(glm::vec3 color);

	glm::vec3 &getColor();

	void setTexture(shared_ptr<TextureHandle> texture);
	shared_ptr<TextureHandle> getTexture();
	boost::optional<shared_ptr<TextureHandle>> getTextureOpt();

	void setAlpha(shared_ptr<TextureHandle> texture);
	shared_ptr<TextureHandle> getAlpha();
	boost::optional<shared_ptr<TextureHandle>> getAlphaOpt();

	void setSpecIntensity(float intensity);
	float getSpecIntensity();

	void setShininess(int shininess);
	int getShininess();

	void setSpecular(shared_ptr<TextureHandle> texture);
	shared_ptr<TextureHandle> getSpecular();
	boost::optional<shared_ptr<TextureHandle>> getSpecularOpt();

	void setTextureMatrix(glm::mat4 textureMatrix);
	glm::mat4 getTextureMatrix();
	void setTexRotateMatrix(glm::mat4 texRotateMatrix);
	glm::mat4 getTexRotateMatrix();

	void setEmission(glm::vec3 emission);
	glm::vec3 &getEmission();
	void setEmissionMap(shared_ptr<TextureHandle> texture);
	shared_ptr<TextureHandle> getEmissionMap();
	boost::optional<shared_ptr<TextureHandle>> getEmissionMapOpt();

	void setNormalMap(shared_ptr<TextureHandle> texture);
	shared_ptr<TextureHandle> getNormalMap();
	boost::optional<shared_ptr<TextureHandle>> getNormalMapOpt();
};
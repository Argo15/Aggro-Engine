#pragma once

#include "DirectLight.hpp"
#include "TransformComponent.hpp"

/**
* Direct light properties like color, intensity, etc.
*
* author: wcrane
* since: 2017-12-10
**/
class DirectLightComponent : public Component,
	public boost::basic_lockable_adapter<recursive_mutex>
{
private:
	glm::vec3 m_color;
	int m_ambientPct;

	Listener<DirectLightComponent *> m_changeListeners;

	DirectLightComponent(Chunk * const byteChunk);

public:
	DirectLightComponent();

	// Serialization
	shared_ptr<Chunk> serialize();
	static shared_ptr<DirectLightComponent> deserialize(Chunk * const byteChunk);

	void addChangeListener(void *ns, std::function<void(DirectLightComponent *)> listener);
	void removeChangeListener(void *ns);

	shared_ptr<DirectLight> getLight(shared_ptr<TransformComponent> transform);

	void setColor(glm::vec3 color);
	glm::vec3 getColor();

	void setAmbientPct(int ambientPct);
	int getAmbientPct();
};
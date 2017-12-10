#pragma once

#include "DirectLight.hpp"
#include "TransformComponent.hpp"

/**
* Direct light properties like color, intensity, etc.
*
* author: wcrane
* since: 2017-12-10
**/
class DirectLightComponent : public Component
{
private:
	DirectLightComponent(Chunk * const byteChunk);

public:
	DirectLightComponent();

	// Serialization
	shared_ptr<Chunk> serialize();
	static shared_ptr<DirectLightComponent> deserialize(Chunk * const byteChunk);

	shared_ptr<DirectLight> getLight(shared_ptr<TransformComponent> transform);
};
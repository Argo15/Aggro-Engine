#pragma once

#include <memory>

enum ChunkType {
	UNKNOWN = 0,
	MAIN_CHUNK = 1,
	SCENE_NODE = 2,
	STRING = 3,
	TRANSFORM_COMPONENT = 4,
	PRIMITIVES = 5,
	RENDER_COMPONENT = 6,
	STATIC_OBJECT_RENDER_COMPONENT = 7,
	SCENE = 8,
	SPRITE_RENDER_COMPONENT = 9,
	DIRECT_LIGHT_RENDER_COMPONENT = 10,
	DIRECT_LIGHT_COMPONENT = 11,
	CAMERA = 12,
	MATERIAL_COMPONENT = 13,
	BASE_MATERIAL = 14,
	DELEGATE_MATERIAL = 15,
	MESH_COMPONENT = 16,
	CAMERA_COMPONENT = 17,
	ROOT_NODE = 18,
	CAMERA_NODE = 19,
	CAMERA_RENDER_COMPONENT = 20
};

/**
* A chunk of data, contains a header and an array of bytes
*
* author: wcrane
* since: 2017-11-25
**/
class Chunk
{
private:
	ChunkType m_type;
	int m_numBytes;
	std::shared_ptr<char> m_data;

public:
	Chunk(ChunkType type, int numBytes, std::shared_ptr<char> data);

	ChunkType *getType();
	int *getNumBytes();
	std::shared_ptr<char> getByteData();
};
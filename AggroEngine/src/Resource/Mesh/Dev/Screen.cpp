#include "Screen.hpp"
#include "MemoryUtil.hpp"

Screen::Screen(int meshId, int x, int y, int width, int height)
	: Mesh(meshId)
	, m_x(x)
	, m_y(y)
	, m_width(width)
	, m_height(height)
{
	setSizeOfVerticies(sizeof(float) * 12); // 4 corners xyz
	setSizeOfIndicies(sizeof(int) * 6); // two triangles
	setVerticies(_getVertices());
	setTexCoords(_getTexCoords());
	setNormals(_getNormals());
	setIndicies(_getIndices());
}

shared_ptr<float> Screen::_getVertices()
{
	float *arr = new float[12];
	// 0, 0
	arr[0] = 0;
	arr[1] = 0;
	arr[2] = 0;
	// 1, 0
	arr[3] = m_width;
	arr[4] = 0;
	arr[5] = 0;
	// 1, 1
	arr[6] = m_width;
	arr[7] = m_height;
	arr[8] = 0;
	// 0, 1
	arr[9] = 0;
	arr[10] = m_height;
	arr[11] = 0;
	return mem::shared_array(arr);
}

shared_ptr<float> Screen::_getTexCoords()
{
	float *arr = new float[8]; // 4 courners uv
	// 0, 0
	arr[0] = 0;
	arr[1] = 0;
	// 1, 0
	arr[2] = 1.0f;
	arr[3] = 0;
	// 1, 1
	arr[4] = 1.0f;
	arr[5] = 1.0f;
	// 0, 1
	arr[6] = 0;
	arr[7] = 1.0f;
	return mem::shared_array(arr);
}

shared_ptr<float> Screen::_getNormals()
{
	float *arr = new float[12];
	for (int i = 0; i < 12; i++)
	{
		arr[i] = 0.f;
	}
	return mem::shared_array(arr);
}

shared_ptr<int> Screen::_getIndices()
{
	int *arr = new int[6];
	arr[0] = 0; // 0, 0
	arr[1] = 1; // 1, 0
	arr[2] = 2; // 1, 1
	arr[3] = 0; // 0, 0
	arr[4] = 2; // 1, 1
	arr[5] = 3; // 0, 1
	return mem::shared_array(arr);
}
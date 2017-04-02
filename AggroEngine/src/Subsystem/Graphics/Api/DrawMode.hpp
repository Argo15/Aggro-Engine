#pragma once

/**
* Geometry draw mode. Aligns with opengl's modes. 
*
* author: wcrane
* since: 2016-08-15
**/
enum class DrawMode {
	LINES = 0x0001,
	LINE_LOOP = 0x0002,
	LINE_STRIP = 0x0003,
	TRIANGLES = 0x0004,
	TRIANGLE_STRIP = 0x0005,
	TRIANGLE_FAN = 0x0006,
	QUADS = 0x0007,
	QUAD_STRIP = 0x0008
};
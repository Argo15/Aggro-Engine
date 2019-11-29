#pragma once
#include "Graphics.hpp"

static void GLAPIENTRY _debugCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	// Suppress some useless warnings
	switch (id)
	{
	case 131218: return; // NVIDIA: "shader will be recompiled due to GL state mismatches"
	case 131185: return; // Buffer object will use VIDEO memory as the source for buffer object operations.
	case 131222: return; // Sampler object <num> is bound to non-depth texture <num>, yet it is used with a program that uses a shadow sampler. This is undefined behavior.
	case 131154: return; // Pixel-path performance warning: Pixel transfer is synchronized with 3D rendering
	default: break;
	}

	cout << "---------------------opengl-callback-start------------" << endl;
	cout << "message: " << message << endl;

	cout << "type: ";
	switch (type) {
	case GL_DEBUG_TYPE_ERROR:
		cout << "ERROR";
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		cout << "DEPRECATED_BEHAVIOR";
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		cout << "UNDEFINED_BEHAVIOR";
		break;
	case GL_DEBUG_TYPE_PORTABILITY:
		cout << "PORTABILITY";
		break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		cout << "PERFORMANCE";
		break;
	case GL_DEBUG_TYPE_OTHER:
		cout << "OTHER";
		break;
	}
	cout << endl;

	cout << "id: " << id << endl;
	cout << "severity: ";
	switch (severity) {
	case GL_DEBUG_SEVERITY_NOTIFICATION:
		cout << "NOTIFICATION";
		break;
	case GL_DEBUG_SEVERITY_LOW:
		cout << "LOW";
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		cout << "MEDIUM";
		break;
	case GL_DEBUG_SEVERITY_HIGH:
		cout << "HIGH";
		break;
	}
	cout << endl;

	cout << "source: ";
	switch (source) {
	case GL_DEBUG_SOURCE_API:
		cout << "API";
		break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		cout << "WINDOW_SYSTEM";
		break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER:
		cout << "SHADER_COMPILER";
		break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:
		cout << "THIRD_PARTY";
		break;
	case GL_DEBUG_SOURCE_APPLICATION:
		cout << "APPLICATION";
		break;
	case GL_DEBUG_SOURCE_OTHER:
		cout << "OTHER";
		break;
	}
	cout << endl;
	cout << "---------------------opengl-callback-end--------------" << endl;
};
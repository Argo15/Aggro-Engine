#include "DefaultTextureHandle.hpp"

DefaultTextureHandle::DefaultTextureHandle(int nTextureId)
	: m_nTextureId(nTextureId)
{

}

unsigned int DefaultTextureHandle::get()
{
	return m_nTextureId;
}
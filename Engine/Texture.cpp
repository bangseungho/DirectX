#include "pch.h"
#include "Texture.h"
#include "Engine.h"

Texture::Texture() : Object(OBJECT_TYPE::TEXTURE)
{
}

Texture::~Texture()
{
}

void Texture::Load(const wstring& path)
{
	CreateTexture(path);
}

void Texture::CreateTexture(const wstring& path)
{
	wstring ext = fs::path(path).extension();

	if (ext == L".dds" || ext == L".DDS")
		ThrowIfFailed(DirectX::CreateDDSTextureFromFile12(DEVICE.Get(), CMD_LIST.Get(), path.c_str(), _resource, _uploadHeap));
}

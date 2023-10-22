#pragma once
#include "GameObject.h"
#include "Component.h"
#include "Material.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"

class Resources
{
	SINGLETON(Resources);

public:
	void Init();

	template<typename T>
	sptr<T> Load(const string& key, const wstring& path);

	template<typename T>
	bool Add(const string& key, sptr<T> object);

	template<typename T>
	sptr<T> Get(const string& Key);

	template<typename T>
	OBJECT_TYPE GetObjectType();

	sptr<Mesh> LoadRectMesh();
	sptr<Mesh> LoadCubeMesh();
	sptr<Mesh> LoadSphereMesh();
	sptr<Mesh> LoadGridMesh();
	sptr<Mesh> LoadRectangleMesh();

	sptr<Texture> CreateTexture(const string& name, DXGI_FORMAT format, uint32 width, uint32 height,
		const D3D12_HEAP_PROPERTIES& heapProperty, D3D12_HEAP_FLAGS heapFlags, RENDER_GROUP_TYPE groupType,
		D3D12_RESOURCE_FLAGS resFlags = D3D12_RESOURCE_FLAG_NONE, Vec4 clearColor = Vec4());

	sptr<Texture> CreateTextureFromResource(const string& name, ComPtr<ID3D12Resource> tex2D, RENDER_GROUP_TYPE groupType);

private:
	void CreateDefaultShader();

private:
	using KeyObjMap = std::map<string, sptr<Object>>;
	array<KeyObjMap, OBJECT_TYPE_COUNT> _resources;
};

template<typename T>
inline sptr<T> Resources::Load(const string& key, const wstring& path)
{
	OBJECT_TYPE objectType = GetObjectType<T>();
	KeyObjMap& keyObjMap = _resources[static_cast<uint8>(objectType)];

	auto findIt = keyObjMap.find(key);
	if (findIt != keyObjMap.end())
		return static_pointer_cast<T>(findIt->second);

	sptr<T> object = make_shared<T>();
	object->Load(path);
	keyObjMap[key] = object;

	return object;
}

template<typename T>
inline bool Resources::Add(const string& key, sptr<T> object)
{
	OBJECT_TYPE objectType = GetObjectType<T>();
	KeyObjMap& keyObjMap = _resources[static_cast<uint8>(objectType)];

	auto findIt = keyObjMap.find(key);
	if (findIt != keyObjMap.end())
		return false;

	keyObjMap[key] = object;

	return true;
}

template<typename T>
inline sptr<T> Resources::Get(const string& key)
{
	OBJECT_TYPE objectType = GetObjectType<T>();
	KeyObjMap& keyObjMap = _resources[static_cast<uint8>(objectType)];

	auto findIt = keyObjMap.find(key);
	if (findIt != keyObjMap.end())
		return static_pointer_cast<T>(findIt->second);

	return nullptr;
}

template<typename T>
inline OBJECT_TYPE Resources::GetObjectType()
{
	if (std::is_same_v<T, GameObject>)
		return OBJECT_TYPE::GAMEOBJECT;
	else if (std::is_same_v<T, Material>)
		return OBJECT_TYPE::MATERIAL;
	else if (std::is_same_v<T, Mesh>)
		return OBJECT_TYPE::MESH;
	else if (std::is_same_v<T, Shader>)
		return OBJECT_TYPE::SHADER;
	else if (std::is_same_v<T, Texture>)
		return OBJECT_TYPE::TEXTURE;
	else if (std::is_convertible_v<T, Component>)
		return OBJECT_TYPE::COMPONENT;
	else
		return OBJECT_TYPE::NONE;
}


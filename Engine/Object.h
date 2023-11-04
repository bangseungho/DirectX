#pragma once

enum class OBJECT_TYPE : uint8
{
	NONE,
	GAMEOBJECT,
	COMPONENT,
	MATERIAL,
	MESH,
	SHADER,
	TEXTURE,

	END
};

enum
{
	OBJECT_TYPE_COUNT = static_cast<uint8>(OBJECT_TYPE::END)
};

class Object
{
public:
	Object(OBJECT_TYPE type);
	virtual ~Object();

	OBJECT_TYPE GetType() { return mObjectType; }

	void SetName(const wstring& name) { mName = name; }
	const wstring& GetName() { return mName; }

protected:
	friend class Resources;
	virtual void Load(const wstring& path) { }
	virtual void Save(const wstring& path) { }

protected:
	OBJECT_TYPE mObjectType = OBJECT_TYPE::NONE;
	wstring mName;
};

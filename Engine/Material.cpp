#include "pch.h"
#include "Material.h"
#include "Engine.h"

Material::Material() : Object(OBJECT_TYPE::MATERIAL)
{
}

Material::~Material()
{
}

void Material::Update()
{
	MATERIAL_CB->CopyData(_matCBIndex, _params);

	_shader->Update();
}


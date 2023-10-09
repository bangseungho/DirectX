#pragma once

enum
{
	MATERIAL_PARAM_COUNT = 5,
	MATERIAL_TEXTURE_COUNT = 5,
};

class Material
{
public:
	void SetShader(shared_ptr<class Shader> shader) { _shader = shader; }
	shared_ptr<class Shader> GetShader() { return _shader; }

	void SetDiffuse(Vec4 diffuse) { _params.SetDiffuse(diffuse) ; }
	void SetFresnel(Vec3 fresnel) { _params.SetFresnel(fresnel); }
	void SetRoughness(float roughness) { _params.SetRoughness(roughness); }
	void SetMatTransform(Matrix matTransform) { _params.SetMatTransform(matTransform); }
	void SetTexOn(bool texOn) { _params.SetTexOn(texOn); }
	void SetTexture(uint8 index, shared_ptr<class Texture> texture) { _textures[index] = texture; }

	void Update();

private:
	shared_ptr<class Shader>	_shader;
	MaterialConstants _params;
	array<shared_ptr<class Texture>, MATERIAL_TEXTURE_COUNT> _textures;
};


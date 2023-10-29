#include "pch.h"
#include "Resources.h"

DECLARE_SINGLE(Resources)

inline MinMaxVert CalcMinMaxVertices(const vector<Vertex>& vec)
{
	Vec3 minPoint = Vec3(FLT_MAX);
	Vec3 maxPoint = Vec3(-FLT_MAX);

	for (const auto& v : vec)
	{
		minPoint.x = min(minPoint.x, v.pos.x);
		minPoint.y = min(minPoint.y, v.pos.y);
		minPoint.z = min(minPoint.z, v.pos.z);
		maxPoint.x = max(maxPoint.x, v.pos.x);
		maxPoint.y = max(maxPoint.y, v.pos.y);
		maxPoint.z = max(maxPoint.z, v.pos.z);
	}

	return MinMaxVert{ minPoint, maxPoint };
}

void CreateBoundingBox(MinMaxVert minMaxVert, sptr<Mesh> mesh)
{
	Vec3 points[2] = {
		minMaxVert.min,
		minMaxVert.max
	};

	mesh->GetBoundingBox().CreateFromPoints(mesh->GetBoundingBox(), 2, points, sizeof(Vec3));
}

void Resources::Init()
{
	CreateDefaultShader();
	CreateDefaultTexture();
	CreateDefaultMaterial();
}

sptr<Mesh> Resources::LoadRectMesh()
{
	sptr<Mesh> findMesh = Get<Mesh>("Rectangle");
	if (findMesh)
		return findMesh;

	float w2 = 20.0f;
	float h2 = 20.0f;
	
	vector<Vertex> vec(4);
	vec[0] = Vertex(Vec3(-w2, -h2, 0.f), Vec2(0.0f, 1.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f));
	vec[1] = Vertex(Vec3(-w2, +h2, 0.f), Vec2(0.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f));
	vec[2] = Vertex(Vec3(+w2, +h2, 0.f), Vec2(1.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f));
	vec[3] = Vertex(Vec3(+w2, -h2, 0.f), Vec2(1.0f, 1.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f));

	vector<uint32> idx(6);
	idx[0] = 0; idx[1] = 1; idx[2] = 2;
	idx[3] = 0; idx[4] = 2; idx[5] = 3;

	sptr<Mesh> mesh = make_shared<Mesh>();
	mesh->Init(vec, idx);
	CreateBoundingBox(CalcMinMaxVertices(vec), mesh);
	Add<Mesh>("Rectangle", mesh);

	return mesh;
}

shared_ptr<Mesh> Resources::LoadCubeMesh()
{
	shared_ptr<Mesh> findMesh = Get<Mesh>("Cube");
	if (findMesh)
		return findMesh;

	float w2 = 0.5f;
	float h2 = 0.5f;
	float d2 = 0.5f;

	vector<Vertex> vec(24);

	// ¾Õ¸é
	vec[0] = Vertex(Vec3(-w2, -h2, -d2), Vec2(0.0f, 1.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f));
	vec[1] = Vertex(Vec3(-w2, +h2, -d2), Vec2(0.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f));
	vec[2] = Vertex(Vec3(+w2, +h2, -d2), Vec2(1.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f));
	vec[3] = Vertex(Vec3(+w2, -h2, -d2), Vec2(1.0f, 1.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f));
	// µÞ¸é
	vec[4] = Vertex(Vec3(-w2, -h2, +d2), Vec2(1.0f, 1.0f), Vec3(0.0f, 0.0f, 1.0f), Vec3(-1.0f, 0.0f, 0.0f));
	vec[5] = Vertex(Vec3(+w2, -h2, +d2), Vec2(0.0f, 1.0f), Vec3(0.0f, 0.0f, 1.0f), Vec3(-1.0f, 0.0f, 0.0f));
	vec[6] = Vertex(Vec3(+w2, +h2, +d2), Vec2(0.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f), Vec3(-1.0f, 0.0f, 0.0f));
	vec[7] = Vertex(Vec3(-w2, +h2, +d2), Vec2(1.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f), Vec3(-1.0f, 0.0f, 0.0f));
	// À­¸é
	vec[8] = Vertex(Vec3(-w2, +h2, -d2), Vec2(0.0f, 1.0f), Vec3(0.0f, 1.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f));
	vec[9] = Vertex(Vec3(-w2, +h2, +d2), Vec2(0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f));
	vec[10] = Vertex(Vec3(+w2, +h2, +d2), Vec2(1.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f));
	vec[11] = Vertex(Vec3(+w2, +h2, -d2), Vec2(1.0f, 1.0f), Vec3(0.0f, 1.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f));
	// ¾Æ·§¸é
	vec[12] = Vertex(Vec3(-w2, -h2, -d2), Vec2(1.0f, 1.0f), Vec3(0.0f, -1.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f));
	vec[13] = Vertex(Vec3(+w2, -h2, -d2), Vec2(0.0f, 1.0f), Vec3(0.0f, -1.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f));
	vec[14] = Vertex(Vec3(+w2, -h2, +d2), Vec2(0.0f, 0.0f), Vec3(0.0f, -1.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f));
	vec[15] = Vertex(Vec3(-w2, -h2, +d2), Vec2(1.0f, 0.0f), Vec3(0.0f, -1.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f));
	// ¿ÞÂÊ¸é
	vec[16] = Vertex(Vec3(-w2, -h2, +d2), Vec2(0.0f, 1.0f), Vec3(-1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f));
	vec[17] = Vertex(Vec3(-w2, +h2, +d2), Vec2(0.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f));
	vec[18] = Vertex(Vec3(-w2, +h2, -d2), Vec2(1.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f));
	vec[19] = Vertex(Vec3(-w2, -h2, -d2), Vec2(1.0f, 1.0f), Vec3(-1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f));
	// ¿À¸¥ÂÊ¸é
	vec[20] = Vertex(Vec3(+w2, -h2, -d2), Vec2(0.0f, 1.0f), Vec3(1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f));
	vec[21] = Vertex(Vec3(+w2, +h2, -d2), Vec2(0.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f));
	vec[22] = Vertex(Vec3(+w2, +h2, +d2), Vec2(1.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f));
	vec[23] = Vertex(Vec3(+w2, -h2, +d2), Vec2(1.0f, 1.0f), Vec3(1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f));



	vector<uint32> idx(36);

	// ¾Õ¸é
	idx[0] = 0; idx[1] = 1; idx[2] = 2;
	idx[3] = 0; idx[4] = 2; idx[5] = 3;
	// µÞ¸é
	idx[6] = 4; idx[7] = 5; idx[8] = 6;
	idx[9] = 4; idx[10] = 6; idx[11] = 7;
	// À­¸é
	idx[12] = 8; idx[13] = 9; idx[14] = 10;
	idx[15] = 8; idx[16] = 10; idx[17] = 11;
	// ¾Æ·§¸é
	idx[18] = 12; idx[19] = 13; idx[20] = 14;
	idx[21] = 12; idx[22] = 14; idx[23] = 15;
	// ¿ÞÂÊ¸é
	idx[24] = 16; idx[25] = 17; idx[26] = 18;
	idx[27] = 16; idx[28] = 18; idx[29] = 19;
	// ¿À¸¥ÂÊ¸é
	idx[30] = 20; idx[31] = 21; idx[32] = 22;
	idx[33] = 20; idx[34] = 22; idx[35] = 23;

	sptr<Mesh> mesh = make_shared<Mesh>();
	mesh->Init(vec, idx);
	CreateBoundingBox(CalcMinMaxVertices(vec), mesh);
	Add<Mesh>("Cube", mesh);

	return mesh;
}

shared_ptr<Mesh> Resources::LoadSphereMesh()
{
	shared_ptr<Mesh> findMesh = Get<Mesh>("Sphere");
	if (findMesh)
		return findMesh;

	float radius = 0.5f; // ±¸ÀÇ ¹ÝÁö¸§
	uint32 stackCount = 20; // °¡·Î ºÐÇÒ
	uint32 sliceCount = 20; // ¼¼·Î ºÐÇÒ

	vector<Vertex> vec;

	Vertex v;

	// ºÏ±Ø
	v.pos = Vec3(0.0f, radius, 0.0f);
	v.uv = Vec2(0.5f, 0.0f);
	v.normal = v.pos;
	v.normal.Normalize();
	v.tangent = Vec3(1.0f, 0.0f, 1.0f);
	vec.push_back(v);

	float stackAngle = XM_PI / stackCount;
	float sliceAngle = XM_2PI / sliceCount;

	float deltaU = 1.f / static_cast<float>(sliceCount);
	float deltaV = 1.f / static_cast<float>(stackCount);

	// °í¸®¸¶´Ù µ¹¸é¼­ Á¤Á¡À» °è»êÇÑ´Ù (ºÏ±Ø/³²±Ø ´ÜÀÏÁ¡Àº °í¸®°¡ X)
	for (uint32 y = 1; y <= stackCount - 1; ++y)
	{
		float phi = y * stackAngle;

		// °í¸®¿¡ À§Ä¡ÇÑ Á¤Á¡
		for (uint32 x = 0; x <= sliceCount; ++x)
		{
			float theta = x * sliceAngle;

			v.pos.x = radius * sinf(phi) * cosf(theta);
			v.pos.y = radius * cosf(phi);
			v.pos.z = radius * sinf(phi) * sinf(theta);

			v.uv = Vec2(deltaU * x, deltaV * y);

			v.normal = v.pos;
			v.normal.Normalize();

			v.tangent.x = -radius * sinf(phi) * sinf(theta);
			v.tangent.y = 0.0f;
			v.tangent.z = radius * sinf(phi) * cosf(theta);
			v.tangent.Normalize();

			vec.push_back(v);
		}
	}

	// ³²±Ø
	v.pos = Vec3(0.0f, -radius, 0.0f);
	v.uv = Vec2(0.5f, 1.0f);
	v.normal = v.pos;
	v.normal.Normalize();
	v.tangent = Vec3(1.0f, 0.0f, 0.0f);
	vec.push_back(v);

	vector<uint32> idx(36);

	// ºÏ±Ø ÀÎµ¦½º
	for (uint32 i = 0; i <= sliceCount; ++i)
	{
		//  [0]
		//   |  \
		//  [i+1]-[i+2]
		idx.push_back(0);
		idx.push_back(i + 2);
		idx.push_back(i + 1);
	}

	// ¸öÅë ÀÎµ¦½º
	uint32 ringVertexCount = sliceCount + 1;
	for (uint32 y = 0; y < stackCount - 2; ++y)
	{
		for (uint32 x = 0; x < sliceCount; ++x)
		{
			//  [y, x]-[y, x+1]
			//  |		/
			//  [y+1, x]
			idx.push_back(1 + (y)*ringVertexCount + (x));
			idx.push_back(1 + (y)*ringVertexCount + (x + 1));
			idx.push_back(1 + (y + 1) * ringVertexCount + (x));
			//		 [y, x+1]
			//		 /	  |
			//  [y+1, x]-[y+1, x+1]
			idx.push_back(1 + (y + 1) * ringVertexCount + (x));
			idx.push_back(1 + (y)*ringVertexCount + (x + 1));
			idx.push_back(1 + (y + 1) * ringVertexCount + (x + 1));
		}
	}

	// ³²±Ø ÀÎµ¦½º
	uint32 bottomIndex = static_cast<uint32>(vec.size()) - 1;
	uint32 lastRingStartIndex = bottomIndex - ringVertexCount;
	for (uint32 i = 0; i < sliceCount; ++i)
	{
		//  [last+i]-[last+i+1]
		//  |      /
		//  [bottom]
		idx.push_back(bottomIndex);
		idx.push_back(lastRingStartIndex + i);
		idx.push_back(lastRingStartIndex + i + 1);
	}

	shared_ptr<Mesh> mesh = make_shared<Mesh>();
	mesh->Init(vec, idx);
	CreateBoundingBox(CalcMinMaxVertices(vec), mesh);
	Add("Sphere", mesh);

	return mesh;
}

sptr<Mesh> Resources::LoadGridMesh()
{
	sptr<Mesh> findMesh = Get<Mesh>("Gird");
	if (findMesh)
		return findMesh;

	uint32 m = 200;
	uint32 n = 200;
	float width = 10000.f;
	float depth = 10000.f;

	uint32 vertexCount = m * n;
	uint32 faceCount = (m - 1) * (n - 1) * 2;

	float halfWidth = 0.5f * width;
	float halfDepth = 0.5f * depth;

	float dx = width / (n - 1);
	float dz = depth / (m - 1);
	
	float du = 1.f / (n - 1);
	float dv = 1.f / (m - 1);

	vector<Vertex> vec(vertexCount);
	for (uint32 i = 0; i < m; ++i) {
		float z = -halfDepth + i * dz;
		for (uint32 j = 0; j < n; ++j) {
			float x = -halfWidth + j * dx;
			
			vec[i * n + j].pos = Vec3(x, 0.f, z);
			vec[i * n + j].normal = Vec3(0.f, 1.f, 0.f);
			vec[i * n + j].tangent = Vec3(1.f, 0.f, 0.f);
			
			vec[i * n + j].uv.x = j * du;
			vec[i * n + j].uv.y = i * dv;
		}
	}

	vector<uint32> idx(faceCount * 3);
	uint32 k = 0;
	for (uint32 i = 0; i < m - 1; ++i) {
		for (uint32 j = 0; j < n - 1; ++j) {
			idx[k] = i * n + j;
			idx[k + 1] = i * n + j + 1;
			idx[k + 2] = (i + 1) * n + j;
			
			idx[k + 3] = (i + 1) * n + j;
			idx[k + 4] = i * n + j + 1;
			idx[k + 5] = (i + 1) * n + j + 1;

			k += 6;
		}
	}
	
	sptr<Mesh> mesh = make_shared<Mesh>();
	mesh->Init(vec, idx);
	CreateBoundingBox(CalcMinMaxVertices(vec), mesh);
	Add<Mesh>("Grid", mesh);

	return mesh;
}

sptr<Mesh> Resources::LoadRectangleMesh()
{
	shared_ptr<Mesh> findMesh = Get<Mesh>("Rectangle");
	if (findMesh)
		return findMesh;

	float w2 = 0.5f;
	float h2 = 0.5f;

	vector<Vertex> vec(4);

	// ¾Õ¸é
	vec[0] = Vertex(Vec3(-w2, -h2, 0), Vec2(0.0f, 1.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f));
	vec[1] = Vertex(Vec3(-w2, +h2, 0), Vec2(0.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f));
	vec[2] = Vertex(Vec3(+w2, +h2, 0), Vec2(1.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f));
	vec[3] = Vertex(Vec3(+w2, -h2, 0), Vec2(1.0f, 1.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f));

	vector<uint32> idx(6);

	// ¾Õ¸é
	idx[0] = 0; idx[1] = 1; idx[2] = 2;
	idx[3] = 0; idx[4] = 2; idx[5] = 3;

	sptr<Mesh> mesh = make_shared<Mesh>();
	mesh->Init(vec, idx);
	CreateBoundingBox(CalcMinMaxVertices(vec), mesh);
	Add<Mesh>("Rectangle", mesh);

	return mesh;
}

sptr<Texture> Resources::CreateTexture(const string& name, DXGI_FORMAT format, uint32 width, uint32 height, const D3D12_HEAP_PROPERTIES& heapProperty, D3D12_HEAP_FLAGS heapFlags, RENDER_GROUP_TYPE groupType, D3D12_RESOURCE_FLAGS resFlags, Vec4 clearColor)
{
	sptr<Texture> texture = make_shared<Texture>();
	texture->Create(format, width, height, heapProperty, heapFlags, groupType, resFlags, clearColor);
	Add<Texture>(name, texture);

	return texture;
}

sptr<Texture> Resources::CreateTextureFromResource(const string& name, ComPtr<ID3D12Resource> tex2D, RENDER_GROUP_TYPE groupType)
{
	shared_ptr<Texture> texture = make_shared<Texture>();
	texture->CreateFromResource(tex2D, groupType);
	Add<Texture>(name, texture);

	return texture;
}

void Resources::CreateDefaultShader()
{
	// SkyBox
	{
		ShaderInfo info = {
			SHADER_TYPE::FORWARD,
			RASTERIGER_TYPE::CULL_NONE,
			DEPTH_STENCIL_TYPE::LESS_EQUAL
		};

		sptr<Shader> shader = make_shared<Shader>();
		shader->Init(L"..\\Output\\cso\\Sky_vs.cso", L"..\\Output\\cso\\Sky_ps.cso", info);
		Add<Shader>("SkyBox", shader);
	}

	// Tex
	{
		ShaderInfo info = {
			SHADER_TYPE::FORWARD,
			RASTERIGER_TYPE::CULL_NONE,
			DEPTH_STENCIL_TYPE::NO_DEPTH_TEST_NO_WRITE
		};

		sptr<Shader> shader = make_shared<Shader>();
		shader->Init(L"..\\Output\\cso\\Tex_vs.cso", L"..\\Output\\cso\\Tex_ps.cso", info);
		Add<Shader>("Tex", shader);
	}

	// Forward
	{
		ShaderInfo info = {
			SHADER_TYPE::FORWARD,
		};

		sptr<Shader> shader = make_shared<Shader>();
		shader->Init(L"..\\Output\\cso\\Forward_vs.cso", L"..\\Output\\cso\\Forward_ps.cso", info);
		Add<Shader>("Forward", shader);
	}

	// Deferred
	{
		ShaderInfo info = {
			SHADER_TYPE::DEFERRED,
		};

		sptr<Shader> shader = make_shared<Shader>();
		shader->Init(L"..\\Output\\cso\\Deferred_vs.cso", L"..\\Output\\cso\\Deferred_ps.cso", info);
		Add<Shader>("Deferred", shader);
	}
}

void Resources::CreateDefaultTexture()
{
	vector<string> texName = {
		"PositionTarget",
		"NormalTarget",
		"DiffuseTarget",
		"FresnelTarget",
		"ShininessTarget",
	};

	for (int i = 0; i < RENDER_TARGET_G_BUFFER_GROUP_COUNT; ++i) {
		auto texMap = GET_SINGLE(Resources)->Get<Texture>(texName[i]);
	}

	vector<string> texNames = {
		"newjeans",
		"newjeans2",
		"newjeans3",

		"leather",
		"leather_normal",
		"leather_roughness",

		"wall",
		"wall_normal",
		"wall_roughness",

		"skybox",
	};

	vector<wstring> texFileNames = {
		L"..\\Resources\\Texture\\newjeans.dds",
		L"..\\Resources\\Texture\\newjeans2.dds",
		L"..\\Resources\\Texture\\newjeans3.dds",

		L"..\\Resources\\Texture\\Leather.dds",
		L"..\\Resources\\Texture\\Leather_Normal_BC7.dds",
		L"..\\Resources\\Texture\\Leather_Weave_004_roughness.dds",

		L"..\\Resources\\Texture\\Sci-Fi_Wall_014_basecolor.dds",
		L"..\\Resources\\Texture\\Sci-Fi_Wall_014_normal_BC7.dds",
		L"..\\Resources\\Texture\\Sci-Fi_Wall_014_roughness.dds",

		L"..\\Resources\\Texture\\Sky.dds",
	};

	for (int i = 0; i < TEXTUREFILE_COUNT; ++i) {
		auto texMap = GET_SINGLE(Resources)->Load<Texture>(texNames[i], texFileNames[i]);

		if (i == (TEXTUREFILE_COUNT - 1))
			texMap->CreateSRVFromDescHeap(TEXTURE_TYPE::TEXTURECUBE);
		else
			texMap->CreateSRVFromDescHeap(TEXTURE_TYPE::TEXTURE2D);
	}

}

void Resources::CreateDefaultMaterial()
{
	{
		shared_ptr<Shader> shader = Get<Shader>("Tex");
		auto pos = make_shared<Material>();
		pos->SetMatCBIndex(0);
		pos->SetDiffuseSrvHeapIndex(Get<Texture>("PositionTarget")->GetTexHeapIndex());
		pos->SetShader(shader);
		Add<Material>("PositionTarget", move(pos));
	}

	{
		shared_ptr<Shader> shader = Get<Shader>("Tex");
		auto norm = make_shared<Material>();
		norm->SetMatCBIndex(1);
		norm->SetDiffuseSrvHeapIndex(Get<Texture>("NormalTarget")->GetTexHeapIndex());
		norm->SetShader(shader);
		Add<Material>("NormalTarget", move(norm));
	}

	{
		shared_ptr<Shader> shader = Get<Shader>("Tex");
		auto diffuse = make_shared<Material>();
		diffuse->SetMatCBIndex(2);
		diffuse->SetDiffuseSrvHeapIndex(Get<Texture>("DiffuseTarget")->GetTexHeapIndex());
		diffuse->SetShader(shader);
		Add<Material>("DiffuseTarget", move(diffuse));
	}

	{
		shared_ptr<Shader> shader = Get<Shader>("Tex");
		auto fresnel = make_shared<Material>();
		fresnel->SetMatCBIndex(3);
		fresnel->SetDiffuseSrvHeapIndex(Get<Texture>("FresnelTarget")->GetTexHeapIndex());
		fresnel->SetShader(shader);
		Add<Material>("FresnelTarget", move(fresnel));
	}

	{
		shared_ptr<Shader> shader = Get<Shader>("Tex");
		auto shininess = make_shared<Material>();
		shininess->SetMatCBIndex(4);
		shininess->SetDiffuseSrvHeapIndex(Get<Texture>("ShininessTarget")->GetTexHeapIndex());
		shininess->SetShader(shader);
		Add<Material>("ShininessTarget", move(shininess));
	}

	{
		shared_ptr<Shader> shader = Get<Shader>("Deferred");
		auto newjeans = make_shared<Material>();
		newjeans->SetMatCBIndex(5);
		newjeans->SetDiffuseSrvHeapIndex(Get<Texture>("newjeans")->GetTexHeapIndex());
		newjeans->SetFresnel(Vec3(0.9f, 0.9f, 0.9f));
		newjeans->SetRoughness(0.125f);
		newjeans->SetShader(shader);
		Add<Material>("newjeans", move(newjeans));

	}

	{
		shared_ptr<Shader> shader = Get<Shader>("Deferred");
		auto newjeans2 = make_shared<Material>();
		newjeans2->SetMatCBIndex(6);
		newjeans2->SetDiffuseSrvHeapIndex(Get<Texture>("newjeans2")->GetTexHeapIndex());
		newjeans2->SetFresnel(Vec3(0.1f, 0.1f, 0.1f));
		newjeans2->SetRoughness(0.125f);
		newjeans2->SetShader(shader);
		Add<Material>("newjeans2", move(newjeans2));

	}

	{
		shared_ptr<Shader> shader = Get<Shader>("Deferred");
		auto newjeans3 = make_shared<Material>();
		newjeans3->SetMatCBIndex(7);
		newjeans3->SetDiffuseSrvHeapIndex(Get<Texture>("newjeans3")->GetTexHeapIndex());
		newjeans3->SetFresnel(Vec3(0.9f, 0.9f, 0.9f));
		newjeans3->SetRoughness(0.125f);
		newjeans3->SetShader(shader);
		Add<Material>("newjeans3", move(newjeans3));

	}

	{
		shared_ptr<Shader> shader = Get<Shader>("Deferred");
		auto leather = make_shared<Material>();
		leather->SetMatCBIndex(8);
		leather->SetDiffuseSrvHeapIndex(Get<Texture>("leather")->GetTexHeapIndex());
		leather->SetNormalSrvHeapIndex(Get<Texture>("leather_normal")->GetTexHeapIndex());
		leather->SetRoughnessSrvHeapIndex(Get<Texture>("leather_roughness")->GetTexHeapIndex());
		leather->SetFresnel(Vec3(0.1f, 0.1f, 0.1f));
		leather->SetRoughness(0.125f);
		leather->SetShader(shader);
		Add<Material>("leather", move(leather));

	}

	{
		shared_ptr<Shader> shader = Get<Shader>("Deferred");
		auto wall = make_shared<Material>();
		wall->SetMatCBIndex(9);
		wall->SetDiffuseSrvHeapIndex(Get<Texture>("wall")->GetTexHeapIndex());
		wall->SetNormalSrvHeapIndex(Get<Texture>("wall_normal")->GetTexHeapIndex());
		wall->SetRoughnessSrvHeapIndex(Get<Texture>("wall_roughness")->GetTexHeapIndex());
		wall->SetFresnel(Vec3(0.5f, 0.5f, 0.5f));
		wall->SetRoughness(0.5f);
		wall->SetShader(shader);
		Add<Material>("wall", move(wall));
	}

	{
		shared_ptr<Shader> shader = Get<Shader>("SkyBox");
		auto skybox = make_shared<Material>();
		skybox->SetMatCBIndex(10);
		skybox->SetDiffuseSrvHeapIndex(Get<Texture>("skybox")->GetTexHeapIndex());
		skybox->SetShader(shader);
		Add<Material>("skybox", move(skybox));

	}
}
#include "pch.h"
#include "Resources.h"
#include "Transform.h"
#include "ParticleSystem.h"

DECLARE_SINGLE(Resources)

inline MinMaxVert CalcMinMaxVertices(const vector<Vertex>& vec)
{
	Vec3 minPoint = Vec3(FLT_MAX);
	Vec3 maxPoint = Vec3(-FLT_MAX);

	for (const auto& v : vec)
	{
		minPoint.x = min(minPoint.x, v.Pos.x);
		minPoint.y = min(minPoint.y, v.Pos.y);
		minPoint.z = min(minPoint.z, v.Pos.z);
		maxPoint.x = max(maxPoint.x, v.Pos.x);
		maxPoint.y = max(maxPoint.y, v.Pos.y);
		maxPoint.z = max(maxPoint.z, v.Pos.z);
	}

	return MinMaxVert{ minPoint, maxPoint };
}

void CreateBoundingBox(MinMaxVert minMaxVert, sptr<Mesh> mesh)
{
	Vec3 points[2] = {
		minMaxVert.Min,
		minMaxVert.Max
	};

	mesh->GetBoundingBox().CreateFromPoints(mesh->GetBoundingBox(), 2, points, sizeof(Vec3));
}

void Resources::Init()
{
	CreateDefaultShader();
	CreateDefaultTexture();
	CreateDefaultMaterial();
}

sptr<Mesh> Resources::LoadPointMesh()
{
	shared_ptr<Mesh> findMesh = Get<Mesh>(L"Point");
	if (findMesh)
		return findMesh;

	vector<Vertex> vec(1);
	vec[0] = Vertex(Vec3(0, 0, 0), Vec2(0.5f, 0.5f), Vec3(0.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f));

	vector<uint32> idx(1);
	idx[0] = 0;

	shared_ptr<Mesh> mesh = make_shared<Mesh>();
	mesh->Init(vec, idx);
	Add(L"Point", mesh);

	return mesh;
}

sptr<Mesh> Resources::LoadRectMesh()
{
	sptr<Mesh> findMesh = Get<Mesh>(L"Rectangle");
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
	Add<Mesh>(L"Rectangle", mesh);

	return mesh;
}

shared_ptr<Mesh> Resources::LoadCubeMesh()
{
	shared_ptr<Mesh> findMesh = Get<Mesh>(L"Cube");
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
	Add<Mesh>(L"Cube", mesh);

	return mesh;
}

shared_ptr<Mesh> Resources::LoadSphereMesh()
{
	shared_ptr<Mesh> findMesh = Get<Mesh>(L"Sphere");
	if (findMesh)
		return findMesh;

	float radius = 0.5f; // ±¸ÀÇ ¹ÝÁö¸§
	uint32 stackCount = 20; // °¡·Î ºÐÇÒ
	uint32 sliceCount = 20; // ¼¼·Î ºÐÇÒ

	vector<Vertex> vec;

	Vertex v;

	// ºÏ±Ø
	v.Pos = Vec3(0.0f, radius, 0.0f);
	v.Uv = Vec2(0.5f, 0.0f);
	v.Normal = v.Pos;
	v.Normal.Normalize();
	v.Tangent = Vec3(1.0f, 0.0f, 1.0f);
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

			v.Pos.x = radius * sinf(phi) * cosf(theta);
			v.Pos.y = radius * cosf(phi);
			v.Pos.z = radius * sinf(phi) * sinf(theta);

			v.Uv = Vec2(deltaU * x, deltaV * y);

			v.Normal = v.Pos;
			v.Normal.Normalize();

			v.Tangent.x = -radius * sinf(phi) * sinf(theta);
			v.Tangent.y = 0.0f;
			v.Tangent.z = radius * sinf(phi) * cosf(theta);
			v.Tangent.Normalize();

			vec.push_back(v);
		}
	}

	// ³²±Ø
	v.Pos = Vec3(0.0f, -radius, 0.0f);
	v.Uv = Vec2(0.5f, 1.0f);
	v.Normal = v.Pos;
	v.Normal.Normalize();
	v.Tangent = Vec3(1.0f, 0.0f, 0.0f);
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
	Add(L"Sphere", mesh);

	return mesh;
}

sptr<Mesh> Resources::LoadGridMesh(float width, float depth, uint32 m, uint32 n)
{
	sptr<Mesh> findMesh = Get<Mesh>(L"Gird");
	if (findMesh)
		return findMesh;

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
		float z = halfDepth - i * dz;
		for (uint32 j = 0; j < n; ++j) {
			float x = -halfWidth + j * dx;
			
			vec[i * n + j].Pos = Vec3(x, 0.f, z);
			vec[i * n + j].Normal = Vec3(0.f, 1.f, 0.f);
			vec[i * n + j].Tangent = Vec3(1.f, 0.f, 0.f);
			
			vec[i * n + j].Uv.x = j * du;
			vec[i * n + j].Uv.y = i * dv;
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
	Add<Mesh>(L"Grid", mesh);

	return mesh;
}

sptr<Mesh> Resources::LoadRectangleMesh()
{
	shared_ptr<Mesh> findMesh = Get<Mesh>(L"Rectangle");
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
	Add<Mesh>(L"Rectangle", mesh);

	return mesh;
}

sptr<Mesh> Resources::LoadTerrainMesh(int32 sizeX, int32 sizeZ)
{
	int32 halfX = sizeX / 2;
	int32 halfZ = sizeZ / 2;

	vector<Vertex> vec;
	for (int32 z = 0; z < sizeZ + 1; ++z) {
		for (int32 x = 0; x < sizeX + 1; ++x) {
			Vertex vtx;
			vtx.Pos = Vec3(static_cast<float>(-halfX + x), 0, static_cast<float>(-halfZ + z));
			vtx.Uv = Vec2(static_cast<float>(x), static_cast<float>(sizeZ - z));
			vtx.Normal = Vec3(0.f, 1.f, 0.f);
			vtx.Tangent = Vec3(1.f, 0.f, 0.f);

			vec.push_back(vtx);
		}
	}

	vector<uint32> idx;
	for (int32 z = 0; z < sizeZ; ++z) {
		for (int32 x = 0; x < sizeX; ++x) {
			idx.push_back((sizeX + 1) * (z + 1) + (x));
			idx.push_back((sizeX + 1) * (z) + (x + 1));
			idx.push_back((sizeX + 1) * (z) + (x));

			idx.push_back((sizeX + 1) * (z) + (x + 1));
			idx.push_back((sizeX + 1) * (z + 1)+(x));
			idx.push_back((sizeX + 1) * (z + 1)+(x + 1));
		}
	}

	sptr<Mesh> findMesh = Get<Mesh>(L"Terrain");
	if (findMesh) {
		findMesh->Init(vec, idx);
		return findMesh;
	}

	sptr<Mesh> mesh = make_shared<Mesh>();
	mesh->Init(vec, idx);
	Add(L"Terrain", mesh);
	return mesh;
}

sptr<Texture> Resources::CreateTexture(const wstring& name, DXGI_FORMAT format, uint32 Width, uint32 Height, const D3D12_HEAP_PROPERTIES& heapProperty, D3D12_HEAP_FLAGS heapFlags, RENDER_GROUP_TYPE groupType, D3D12_RESOURCE_FLAGS resFlags, Vec4 clearColor)
{
	sptr<Texture> texture = make_shared<Texture>();
	texture->Create(format, Width, Height, heapProperty, heapFlags, groupType, resFlags, clearColor);
	Add<Texture>(name, texture);

	return texture;
}

sptr<Texture> Resources::CreateTextureFromResource(const wstring& name, ComPtr<ID3D12Resource> tex2D, RENDER_GROUP_TYPE groupType)
{
	shared_ptr<Texture> texture = make_shared<Texture>();
	texture->CreateFromResource(tex2D, groupType);
	Add<Texture>(name, texture);

	return texture;
}

sptr<MeshData> Resources::LoadFBX(const wstring& path)
{
	wstring key = path;

	sptr<MeshData> meshData = Get<MeshData>(key);
	if (meshData)
		return meshData;

	meshData = MeshData::LoadFromFBX(path);
	meshData->SetName(key);
	Add<MeshData>(key, meshData);

	return meshData;
}

void Resources::CreateDefaultShader()
{
	// SkyBox
	{
		ShaderInfo info = {
			SHADER_TYPE::FORWARD,
			RASTERIZER_TYPE::CULL_NONE,
			DEPTH_STENCIL_TYPE::LESS_EQUAL
		};

		ShaderPath path = {
			 L"..\\Output\\cso\\Sky_vs.cso",
			 L"",
			 L"",
			 L"",
			 L"..\\Output\\cso\\Sky_ps.cso"
		};

		sptr<Shader> shader = make_shared<Shader>();
		shader->LoadGraphicsShader(info, path);
		Add<Shader>(L"SkyBox", shader);
	}

	// Tex
	{
		ShaderInfo info = {
			SHADER_TYPE::FORWARD,
			RASTERIZER_TYPE::CULL_NONE,
			DEPTH_STENCIL_TYPE::NO_DEPTH_TEST_NO_WRITE
		};

		ShaderPath path = {
			 L"..\\Output\\cso\\Tex_vs.cso",
			 L"",
			 L"",
			 L"",
			 L"..\\Output\\cso\\Tex_ps.cso"
		};

		sptr<Shader> shader = make_shared<Shader>();
		shader->LoadGraphicsShader(info, path);
		Add<Shader>(L"Tex", shader);
	}

	// Forward
	{
		ShaderInfo info = {
			SHADER_TYPE::FORWARD,
		};

		ShaderPath path = {
			 L"..\\Output\\cso\\Forward_vs.cso",
			 L"",
			 L"",
			 L"",
			 L"..\\Output\\cso\\Forward_ps.cso"
		};

		sptr<Shader> shader = make_shared<Shader>();
		shader->LoadGraphicsShader(info, path);
		Add<Shader>(L"Forward", shader);
	}

	// Score
	{
		ShaderInfo info = {
			SHADER_TYPE::FORWARD,
		};

		ShaderPath path = {
			 L"..\\Output\\cso\\Score_vs.cso",
			 L"",
			 L"",
			 L"",
			 L"..\\Output\\cso\\Score_ps.cso"
		};

		sptr<Shader> shader = make_shared<Shader>();
		shader->LoadGraphicsShader(info, path);
		Add<Shader>(L"Score", shader);
	}

	// Grid
	{
		ShaderInfo info = {
			SHADER_TYPE::FORWARD,
			RASTERIZER_TYPE::CULL_NONE,
		};

		ShaderPath path = {
			L"..\\Output\\cso\\Forward_vs.cso",
			L"",
			L"",
			L"",
			L"..\\Output\\cso\\Forward_ps.cso"
		};

		sptr<Shader> shader = make_shared<Shader>();
		shader->LoadGraphicsShader(info, path);
		Add<Shader>(L"Grid", shader);
	}

	// Deferred
	{
		ShaderInfo info = {
			SHADER_TYPE::DEFERRED,
		};

		ShaderPath path = {
			L"..\\Output\\cso\\Deferred_vs.cso",
			L"",
			L"",
			L"",
			L"..\\Output\\cso\\Deferred_ps.cso"
		};

		sptr<Shader> shader = make_shared<Shader>();
		shader->LoadGraphicsShader(info, path);
		Add<Shader>(L"Deferred", shader);
	}

	// DirLight
	{
		ShaderInfo info = {
			SHADER_TYPE::LIGHTING,
			RASTERIZER_TYPE::CULL_NONE,
			DEPTH_STENCIL_TYPE::NO_DEPTH_TEST_NO_WRITE,
			BLEND_TYPE::ONE_TO_ONE_BLEND,
		};

		ShaderPath path = {
			L"..\\Output\\cso\\DirLighting_vs.cso",
			L"",
			L"",
			L"",
			L"..\\Output\\cso\\DirLighting_ps.cso"
		};

		sptr<Shader> shader = make_shared<Shader>();
		shader->LoadGraphicsShader(info, path);
		Add<Shader>(L"DirLight", shader);
	}

	// PointLight
	{
		ShaderInfo info =
		{
			SHADER_TYPE::LIGHTING,
			RASTERIZER_TYPE::CULL_FRONT,
			DEPTH_STENCIL_TYPE::NO_DEPTH_TEST_NO_WRITE,
			BLEND_TYPE::ONE_TO_ONE_BLEND
		};

		ShaderPath path = {
			L"..\\Output\\cso\\PointLighting_vs.cso",
			L"",
			L"",
			L"",
			L"..\\Output\\cso\\PointLighting_ps.cso"
		};

		shared_ptr<Shader> shader = make_shared<Shader>();
		shader->LoadGraphicsShader(info, path);
		Add<Shader>(L"PointLight", shader);
	}

	// SpotLight
	{
		ShaderInfo info =
		{
			SHADER_TYPE::LIGHTING,
			RASTERIZER_TYPE::CULL_FRONT,
			DEPTH_STENCIL_TYPE::NO_DEPTH_TEST_NO_WRITE,
			BLEND_TYPE::ONE_TO_ONE_BLEND
		};

		ShaderPath path = {
			L"..\\Output\\cso\\SpotLighting_vs.cso",
			L"",
			L"",
			L"",
			L"..\\Output\\cso\\SpotLighting_ps.cso"
		};

		shared_ptr<Shader> shader = make_shared<Shader>();
		shader->LoadGraphicsShader(info, path);
		Add<Shader>(L"SpotLight", shader);
	}

	// Particle
	{
		ShaderInfo info =
		{
			SHADER_TYPE::FORWARD,
			RASTERIZER_TYPE::CULL_BACK,
			DEPTH_STENCIL_TYPE::LESS_NO_WRITE,
			BLEND_TYPE::ALPHA_BLEND,
			D3D_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_POINTLIST,
		};

		ShaderPath path = {
			L"..\\Output\\cso\\Particle_vs.cso",
			L"",
			L"",
			L"..\\Output\\cso\\Particle_gs.cso",
			L"..\\Output\\cso\\Particle_ps.cso"
		};

		shared_ptr<Shader> shader = make_shared<Shader>();
		shader->LoadGraphicsShader(info, path);
		Add<Shader>(L"Particle", shader);
	}

	// SpreadParticle
	{
		shared_ptr<Shader> shader = make_shared<Shader>();
		shader->LoadComputeShader(L"..\\Output\\cso\\Spread_Particle_cs.cso");
		Add<Shader>(L"Compute_Spread_Particle", shader);
	}

	// SnowParticle
	{
		shared_ptr<Shader> shader = make_shared<Shader>();
		shader->LoadComputeShader(L"..\\Output\\cso\\Snow_Particle_cs.cso");
		Add<Shader>(L"Compute_Snow_Particle", shader);
	}

	// Final
	{
		ShaderInfo info =
		{
			SHADER_TYPE::LIGHTING,
			RASTERIZER_TYPE::CULL_BACK,
			DEPTH_STENCIL_TYPE::NO_DEPTH_TEST_NO_WRITE,
		};

		ShaderPath path = {
			L"..\\Output\\cso\\Final_vs.cso",
			L"",
			L"",
			L"",
			L"..\\Output\\cso\\Final_ps.cso"
		};

		shared_ptr<Shader> shader = make_shared<Shader>();
		shader->LoadGraphicsShader(info, path);
		Add<Shader>(L"Final", shader);
	}

	// Tessellation
	{
		ShaderInfo info =
		{
			SHADER_TYPE::FORWARD,
			RASTERIZER_TYPE::WIREFRAME,
			DEPTH_STENCIL_TYPE::LESS,
			BLEND_TYPE::DEFAULT,
			D3D_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST
		};

		ShaderPath path = {
			L"..\\Output\\cso\\Tessellation_vs.cso",
			L"..\\Output\\cso\\Tessellation_hs.cso",
			L"..\\Output\\cso\\Tessellation_ds.cso",
			L"",
			L"..\\Output\\cso\\Tessellation_ps.cso"
		};

		sptr<Shader> shader = make_shared<Shader>();
		shader->LoadGraphicsShader(info, path);
		Add<Shader>(L"Tessellation", shader);
	}

	// Terrain
	{
		ShaderInfo info =
		{
			SHADER_TYPE::DEFERRED,
			RASTERIZER_TYPE::CULL_BACK,
			DEPTH_STENCIL_TYPE::LESS,
			BLEND_TYPE::DEFAULT,
			D3D_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST
		};

		ShaderPath path = {
			L"..\\Output\\cso\\Terrain_vs.cso",
			L"..\\Output\\cso\\Terrain_hs.cso",
			L"..\\Output\\cso\\Terrain_ds.cso",
			L"",				 
			L"..\\Output\\cso\\Terrain_ps.cso"
		};

		sptr<Shader> shader = make_shared<Shader>();
		shader->LoadGraphicsShader(info, path);
		Add<Shader>(L"Terrain", shader);
	}

	// Shadow
	{
		ShaderInfo info =
		{
			SHADER_TYPE::SHADOW,
			RASTERIZER_TYPE::CULL_BACK,
			DEPTH_STENCIL_TYPE::LESS,
		};

		ShaderPath path = {
			L"..\\Output\\cso\\Shadow_vs.cso",
			L"",
			L"",
			L"",
			L"..\\Output\\cso\\Shadow_ps.cso",
		};

		shared_ptr<Shader> shader = make_shared<Shader>();
		shader->LoadGraphicsShader(info, path);
		Add<Shader>(L"Shadow", shader);
	}
}

void Resources::CreateDefaultTexture()
{
	vector<wstring> texNames = {
		L"newjeans",
		L"newjeans2",
		L"newjeans3",

		L"leather",
		L"leather_normal",
		L"leather_roughness",
		
		L"wall",
		L"wall_normal",
		L"wall_roughness",
		
		L"lightParticle",
		L"SnowParticle",
		
		L"Snow_Base",
		L"Snow_Normal",
		L"Snow_Roughness",
		L"Snow_Height",
		
		L"Dragon_Bump_Col2.jpg",
		L"Dragon_ground_color.jpg",
		L"Dragon_Nor.jpg",
		L"Dragon_Nor_mirror2.jpg",
		
		L"Col Klein Haus_C.jpg",
		L"Col Klein Haus_N.jpg",
		L"Col Klein Haus_S.jpg",

		L"KSR29sniperrifle_Base_Color.jpg",
		L"KSR29sniperrifle_Normal_OpenGL.jpg",
		L"KSR29sniperrifle_Roughness.jpg",

		L"Number",
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

		L"..\\Resources\\Texture\\lightParticle.dds",
		L"..\\Resources\\Texture\\Snow_Particle.dds",

		L"..\\Resources\\Texture\\Snow_Base.dds",
		L"..\\Resources\\Texture\\Snow_Normal.dds",
		L"..\\Resources\\Texture\\Snow_Roughness.dds",
		L"..\\Resources\\Texture\\height.dds",

		L"..\\Resources\\FBX\\dragon.fbm\\Dragon_Bump_Col2.jpg",
		L"..\\Resources\\FBX\\dragon.fbm\\Dragon_ground_color.jpg",
		L"..\\Resources\\FBX\\dragon.fbm\\Dragon_Nor.jpg",
		L"..\\Resources\\FBX\\dragon.fbm\\Dragon_Nor_mirror2.jpg",

		L"..\\Resources\\FBX\\building.fbm\\Col Klein Haus_C.jpg",
		L"..\\Resources\\FBX\\building.fbm\\Col Klein Haus_N.jpg",
		L"..\\Resources\\FBX\\building.fbm\\Col Klein Haus_S.jpg",

		L"..\\Resources\\FBX\\sniper.fbm\\KSR29sniperrifle_Base_Color.jpg",
		L"..\\Resources\\FBX\\sniper.fbm\\KSR29sniperrifle_Normal_OpenGL.jpg",
		L"..\\Resources\\FBX\\sniper.fbm\\KSR29sniperrifle_Roughness.jpg",

		L"..\\Resources\\Texture\\Number.dds",
	};

	for (int i = 0; i < texNames.size(); ++i)
		GET_SINGLE(Resources)->Load<Texture>(texNames[i], texFileNames[i]);

	wstring cubeMapName = L"skybox";
	wstring cubeMapFileName = L"..\\Resources\\Texture\\cubemap.dds";
	GET_SINGLE(Resources)->Load<TextureCube>(cubeMapName, cubeMapFileName);
}

void Resources::CreateDefaultMaterial()
{
	{
		shared_ptr<Shader> shader = Get<Shader>(L"Tex");
		auto Pos = make_shared<Material>();
		Pos->SetDiffuseSrvHeapIndex(Get<Texture>(L"PositionTarget")->GetTexHeapIndex());
		Pos->SetShader(shader);
		Add<Material>(L"PositionTarget", move(Pos));
	}

	{
		shared_ptr<Shader> shader = Get<Shader>(L"Tex");
		auto norm = make_shared<Material>();
		norm->SetDiffuseSrvHeapIndex(Get<Texture>(L"NormalTarget")->GetTexHeapIndex());
		norm->SetShader(shader);
		Add<Material>(L"NormalTarget", move(norm));
	}

	{
		shared_ptr<Shader> shader = Get<Shader>(L"Tex");
		auto diffuse = make_shared<Material>();
		diffuse->SetDiffuseSrvHeapIndex(Get<Texture>(L"DiffuseTarget")->GetTexHeapIndex());
		diffuse->SetShader(shader);
		Add<Material>(L"DiffuseTarget", move(diffuse));
	}

	{
		shared_ptr<Shader> shader = Get<Shader>(L"Tex");
		auto fresnel = make_shared<Material>();
		fresnel->SetDiffuseSrvHeapIndex(Get<Texture>(L"FresnelTarget")->GetTexHeapIndex());
		fresnel->SetShader(shader);
		Add<Material>(L"FresnelTarget", move(fresnel));
	}

	{
		shared_ptr<Shader> shader = Get<Shader>(L"Tex");
		auto shininess = make_shared<Material>();
		shininess->SetDiffuseSrvHeapIndex(Get<Texture>(L"ShininessTarget")->GetTexHeapIndex());
		shininess->SetShader(shader);
		Add<Material>(L"ShininessTarget", move(shininess));
	}

	{
		shared_ptr<Shader> shader = Get<Shader>(L"Tex");
		auto shininess = make_shared<Material>();
		shininess->SetDiffuseSrvHeapIndex(Get<Texture>(L"ShadowTarget")->GetTexHeapIndex());
		shininess->SetShader(shader);
		Add<Material>(L"ShadowTarget", move(shininess));
	}

	{
		shared_ptr<Shader> shader = Get<Shader>(L"Deferred");
		auto newjeans = make_shared<Material>();
		newjeans->SetDiffuseSrvHeapIndex(Get<Texture>(L"newjeans")->GetTexHeapIndex());
		newjeans->SetFresnel(Vec3(0.9f, 0.9f, 0.9f));
		newjeans->SetRoughness(0.125f);
		newjeans->SetShader(shader);
		Add<Material>(L"newjeans", move(newjeans));

	}

	{
		shared_ptr<Shader> shader = Get<Shader>(L"Deferred");
		auto newjeans2 = make_shared<Material>();
		newjeans2->SetDiffuseSrvHeapIndex(Get<Texture>(L"newjeans2")->GetTexHeapIndex());
		newjeans2->SetFresnel(Vec3(0.1f, 0.1f, 0.1f));
		newjeans2->SetRoughness(0.125f);
		newjeans2->SetShader(shader);
		Add<Material>(L"newjeans2", move(newjeans2));

	}

	{
		shared_ptr<Shader> shader = Get<Shader>(L"Deferred");
		auto newjeans3 = make_shared<Material>();
		newjeans3->SetDiffuseSrvHeapIndex(Get<Texture>(L"newjeans3")->GetTexHeapIndex());
		newjeans3->SetFresnel(Vec3(0.9f, 0.9f, 0.9f));
		newjeans3->SetRoughness(0.125f);
		newjeans3->SetShader(shader);
		Add<Material>(L"newjeans3", move(newjeans3));

	}

	{
		shared_ptr<Shader> shader = Get<Shader>(L"Deferred");
		auto leather = make_shared<Material>();
		leather->SetDiffuseSrvHeapIndex(Get<Texture>(L"leather")->GetTexHeapIndex());
		leather->SetNormalSrvHeapIndex(Get<Texture>(L"leather_normal")->GetTexHeapIndex());
		leather->SetRoughnessSrvHeapIndex(Get<Texture>(L"leather_roughness")->GetTexHeapIndex());
		leather->SetFresnel(Vec3(0.1f, 0.1f, 0.1f));
		leather->SetRoughness(0.125f);
		leather->SetShader(shader);
		Add<Material>(L"leather", move(leather));
	}

	{
		shared_ptr<Shader> shader = Get<Shader>(L"Deferred");
		auto wall = make_shared<Material>();
		wall->SetDiffuseSrvHeapIndex(Get<Texture>(L"wall")->GetTexHeapIndex());
		wall->SetNormalSrvHeapIndex(Get<Texture>(L"wall_normal")->GetTexHeapIndex());
		wall->SetRoughnessSrvHeapIndex(Get<Texture>(L"wall_roughness")->GetTexHeapIndex());
		wall->SetFresnel(Vec3(0.5f, 0.5f, 0.5f));
		wall->SetRoughness(0.5f);
		wall->SetShader(shader);
		Add<Material>(L"wall", move(wall));
	}

	{
		shared_ptr<Shader> shader = Get<Shader>(L"SkyBox");
		auto skybox = make_shared<Material>();
		skybox->SetDiffuseSrvHeapIndex(Get<TextureCube>(L"skybox")->GetTexHeapIndex());
		skybox->SetShader(shader);
		Add<Material>(L"skybox", move(skybox));

	}

	{
		shared_ptr<Shader> shader = Get<Shader>(L"DirLight");
		auto dirLight = make_shared<Material>();
		dirLight->SetShader(shader);
		Add<Material>(L"DirLight", move(dirLight));
	}

	{
		shared_ptr<Shader> shader = Get<Shader>(L"PointLight");
		auto pointLight = make_shared<Material>();
		pointLight->SetShader(shader);
		Add<Material>(L"PointLight", pointLight);
	}

	{
		shared_ptr<Shader> shader = Get<Shader>(L"SpotLight");
		auto spotLight = make_shared<Material>();
		spotLight->SetShader(shader);
		Add<Material>(L"SpotLight", spotLight);
	}

	{
		shared_ptr<Shader> shader = Get<Shader>(L"Tex");
		auto diffuseLight = make_shared<Material>();
		diffuseLight->SetDiffuseSrvHeapIndex(Get<Texture>(L"DiffuseLightTarget")->GetTexHeapIndex());
		diffuseLight->SetShader(shader);
		Add<Material>(L"DiffuseLightTarget", move(diffuseLight));
	}

	{
		shared_ptr<Shader> shader = Get<Shader>(L"Tex");
		auto specLight = make_shared<Material>();
		specLight->SetDiffuseSrvHeapIndex(Get<Texture>(L"SpecularLightTarget")->GetTexHeapIndex());
		specLight->SetShader(shader);
		Add<Material>(L"SpecularLightTarget", move(specLight));
	}

	{
		shared_ptr<Shader> shader = Get<Shader>(L"Final");
		auto finalMaterial = make_shared<Material>();
		finalMaterial->SetShader(shader);
		Add<Material>(L"Final", move(finalMaterial));
	}

	{
		shared_ptr<Shader> shader = Get<Shader>(L"Particle");
		auto particle = make_shared<Material>();
		particle->SetDiffuseSrvHeapIndex(Get<Texture>(L"lightParticle")->GetTexHeapIndex());
		particle->SetShader(shader);
		Add<Material>(L"lightParticle", move(particle));
	}

	{
		shared_ptr<Shader> shader = Get<Shader>(L"Particle");
		auto particle = make_shared<Material>();
		particle->SetDiffuseSrvHeapIndex(Get<Texture>(L"SnowParticle")->GetTexHeapIndex());
		particle->SetShader(shader);
		Add<Material>(L"SnowParticle", move(particle));
	}

	{
		shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Compute_Spread_Particle");
		shared_ptr<Material> material = make_shared<Material>();
		material->SetShader(shader);

		Add<Material>(L"Compute_Spread_Particle", material);
	}

	{
		shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Compute_Snow_Particle");
		shared_ptr<Material> material = make_shared<Material>();
		material->SetShader(shader);

		Add<Material>(L"Compute_Snow_Particle", material);
	}

	{
		shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Tessellation");
		shared_ptr<Material> tessellation = make_shared<Material>();
		tessellation->SetShader(shader);
		Add<Material>(L"Tessellation", tessellation);
	}

	{
		shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Terrain");
		shared_ptr<Material> terrain = make_shared<Material>();
		terrain->SetDiffuseSrvHeapIndex(Get<Texture>(L"Snow_Base")->GetTexHeapIndex());
		terrain->SetNormalSrvHeapIndex(Get<Texture>(L"Snow_Normal")->GetTexHeapIndex());
		terrain->SetRoughnessSrvHeapIndex(Get<Texture>(L"Snow_Roughness")->GetTexHeapIndex());
		terrain->SetHeightSrvHeapIndex(Get<Texture>(L"Snow_Height")->GetTexHeapIndex());
		terrain->SetShader(shader);
		Add<Material>(L"Terrain", terrain);
	}

	{
		shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Score");
		float dU = 1.f / 5.f;
		float dV = 1.f / 2.f;
		for (int i = 0; i < 10; ++i)
		{
			shared_ptr<Material> number = make_shared<Material>();
			number->SetDiffuseSrvHeapIndex(Get<Texture>(L"Number")->GetTexHeapIndex());
			number->SetShader(shader);
			number->SetScoreIndex(Vec2(i % 5, i / 5));

			Matrix matTrans = Matrix::CreateScale(0.2f, 0.5f, 1.f);

			number->SetMatTransform(matTrans);
			
			wstring num = to_wstring(i);
			Add<Material>(L"Number" + num, number);
		}
	}

	{
		shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Shadow");
		shared_ptr<Material> material = make_shared<Material>();
		material->SetShader(shader);
		Add<Material>(L"Shadow", material);
	}
}
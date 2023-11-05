#include "pch.h"
#include "Resources.h"

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
	shared_ptr<Mesh> findMesh = Get<Mesh>("Point");
	if (findMesh)
		return findMesh;

	vector<Vertex> vec(1);
	vec[0] = Vertex(Vec3(0, 0, 0), Vec2(0.5f, 0.5f), Vec3(0.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f));

	vector<uint32> idx(1);
	idx[0] = 0;

	shared_ptr<Mesh> mesh = make_shared<Mesh>();
	mesh->Init(vec, idx);
	Add("Point", mesh);

	return mesh;
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
	float Width = 10000.f;
	float depth = 10000.f;

	uint32 vertexCount = m * n;
	uint32 faceCount = (m - 1) * (n - 1) * 2;

	float halfWidth = 0.5f * Width;
	float halfDepth = 0.5f * depth;

	float dx = Width / (n - 1);
	float dz = depth / (m - 1);
	
	float du = 1.f / (n - 1);
	float dv = 1.f / (m - 1);

	vector<Vertex> vec(vertexCount);
	for (uint32 i = 0; i < m; ++i) {
		float z = -halfDepth + i * dz;
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

sptr<Texture> Resources::CreateTexture(const string& name, DXGI_FORMAT format, uint32 Width, uint32 Height, const D3D12_HEAP_PROPERTIES& heapProperty, D3D12_HEAP_FLAGS heapFlags, RENDER_GROUP_TYPE groupType, D3D12_RESOURCE_FLAGS resFlags, Vec4 clearColor)
{
	sptr<Texture> texture = make_shared<Texture>();
	texture->Create(format, Width, Height, heapProperty, heapFlags, groupType, resFlags, clearColor);
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
			RASTERIZER_TYPE::CULL_NONE,
			DEPTH_STENCIL_TYPE::LESS_EQUAL
		};

		sptr<Shader> shader = make_shared<Shader>();
		shader->LoadGraphicsShader(info, L"..\\Output\\cso\\Sky_vs.cso", L"..\\Output\\cso\\Sky_ps.cso");
		Add<Shader>("SkyBox", shader);
	}

	// Tex
	{
		ShaderInfo info = {
			SHADER_TYPE::FORWARD,
			RASTERIZER_TYPE::CULL_NONE,
			DEPTH_STENCIL_TYPE::NO_DEPTH_TEST_NO_WRITE
		};

		sptr<Shader> shader = make_shared<Shader>();
		shader->LoadGraphicsShader(info, L"..\\Output\\cso\\Tex_vs.cso", L"..\\Output\\cso\\Tex_ps.cso");
		Add<Shader>("Tex", shader);
	}

	// Forward
	{
		ShaderInfo info = {
			SHADER_TYPE::FORWARD,
		};

		sptr<Shader> shader = make_shared<Shader>();
		shader->LoadGraphicsShader(info, L"..\\Output\\cso\\Forward_vs.cso", L"..\\Output\\cso\\Forward_ps.cso");
		Add<Shader>("Forward", shader);
	}

	// Grid
	{
		ShaderInfo info = {
			SHADER_TYPE::FORWARD,
			RASTERIZER_TYPE::CULL_NONE,
		};

		sptr<Shader> shader = make_shared<Shader>();
		shader->LoadGraphicsShader(info, L"..\\Output\\cso\\Forward_vs.cso", L"..\\Output\\cso\\Forward_ps.cso");
		Add<Shader>("Grid", shader);
	}

	// Deferred
	{
		ShaderInfo info = {
			SHADER_TYPE::DEFERRED,
		};

		sptr<Shader> shader = make_shared<Shader>();
		shader->LoadGraphicsShader(info, L"..\\Output\\cso\\Deferred_vs.cso", L"..\\Output\\cso\\Deferred_ps.cso");
		Add<Shader>("Deferred", shader);
	}

	// DirLight
	{
		ShaderInfo info = {
			SHADER_TYPE::LIGHTING,
			RASTERIZER_TYPE::CULL_NONE,
			DEPTH_STENCIL_TYPE::NO_DEPTH_TEST_NO_WRITE,
			BLEND_TYPE::ONE_TO_ONE_BLEND,
		};

		sptr<Shader> shader = make_shared<Shader>();
		shader->LoadGraphicsShader(info, L"..\\Output\\cso\\DirLighting_vs.cso", L"..\\Output\\cso\\DirLighting_ps.cso");
		Add<Shader>("DirLight", shader);
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

		shared_ptr<Shader> shader = make_shared<Shader>();
		shader->LoadGraphicsShader(info, L"..\\Output\\cso\\PointLighting_vs.cso", L"..\\Output\\cso\\PointLighting_ps.cso");
		Add<Shader>("PointLight", shader);
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

		shared_ptr<Shader> shader = make_shared<Shader>();
		shader->LoadGraphicsShader(info, L"..\\Output\\cso\\SpotLighting_vs.cso", L"..\\Output\\cso\\SpotLighting_ps.cso");
		Add<Shader>("SpotLight", shader);
	}

	// Particle
	{
		ShaderInfo info =
		{
			SHADER_TYPE::FORWARD,
			RASTERIZER_TYPE::CULL_NONE,
			DEPTH_STENCIL_TYPE::LESS_NO_WRITE,
			BLEND_TYPE::ALPHA_BLEND,
			D3D_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_POINTLIST,
		};

		shared_ptr<Shader> shader = make_shared<Shader>();
		shader->LoadGraphicsShader(info, L"..\\Output\\cso\\Particle_vs.cso", L"..\\Output\\cso\\Particle_ps.cso", L"..\\Output\\cso\\Particle_gs.cso");
		Add<Shader>("Particle", shader);
	}

	// ComputeParticle
	{
		shared_ptr<Shader> shader = make_shared<Shader>();
		shader->LoadComputeShader(L"..\\Output\\cso\\Particle_cs.cso");
		Add<Shader>("ComputeParticle", shader);
	}

	//// Particle2
	//{
	//	ShaderInfo info =
	//	{
	//		SHADER_TYPE::FORWARD,
	//		RASTERIZER_TYPE::CULL_NONE,
	//		DEPTH_STENCIL_TYPE::LESS_NO_WRITE,
	//		BLEND_TYPE::ALPHA_BLEND,
	//	};

	//	shared_ptr<Shader> shader = make_shared<Shader>();
	//	shader->LoadGraphicsShader(info, L"..\\Output\\cso\\Particle_vs.cso", L"..\\Output\\cso\\Particle_ps.cso");
	//	Add<Shader>("Particle2", shader);
	//}


	// Final
	{
		ShaderInfo info =
		{
			SHADER_TYPE::LIGHTING,
			RASTERIZER_TYPE::CULL_BACK,
			DEPTH_STENCIL_TYPE::NO_DEPTH_TEST_NO_WRITE,
		};

		shared_ptr<Shader> shader = make_shared<Shader>();
		shader->LoadGraphicsShader(info, L"..\\Output\\cso\\Final_vs.cso", L"..\\Output\\cso\\Final_ps.cso");
		Add<Shader>("Final", shader);
	}

	// Compute Shader
	{
		shared_ptr<Shader> shader = make_shared<Shader>();
		shader->LoadComputeShader(L"..\\Output\\cso\\Compute_cs.cso");
		Add<Shader>("Compute", shader);
	}
}

void Resources::CreateDefaultTexture()
{
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

		"Particle",

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

		L"..\\Resources\\Texture\\particle.dds",

		L"..\\Resources\\Texture\\Sky.dds",
	};

	for (int i = 0; i < TEXTUREFILE_COUNT; ++i) {
		auto texMap = GET_SINGLE(Resources)->Load<Texture>(texNames[i], texFileNames[i]);

		if (i == (TEXTUREFILE_COUNT - 1))
			texMap->CreateSRVFromDescHeap(TEXTURE_TYPE::TEXTURECUBE);
		else
			texMap->CreateSRVFromDescHeap(TEXTURE_TYPE::TEXTURE2D);
	}

	sptr<Texture> texture = GET_SINGLE(Resources)->CreateTexture("UAVTexture",
		DXGI_FORMAT_R8G8B8A8_UNORM, 1024, 1024,
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE, RENDER_GROUP_TYPE::COMPUTE,
		D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
}

void Resources::CreateDefaultMaterial()
{
	{
		shared_ptr<Shader> shader = Get<Shader>("Tex");
		auto Pos = make_shared<Material>();
		Pos->SetMatCBIndex(0);
		Pos->SetDiffuseSrvHeapIndex(Get<Texture>("PositionTarget")->GetTexHeapIndex());
		Pos->SetShader(shader);
		Add<Material>("PositionTarget", move(Pos));
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

	{
		shared_ptr<Shader> shader = Get<Shader>("DirLight");
		auto dirLight = make_shared<Material>();
		dirLight->SetMatCBIndex(11);
		dirLight->SetShader(shader);
		Add<Material>("DirLight", move(dirLight));
	}

	{
		shared_ptr<Shader> shader = Get<Shader>("PointLight");
		auto pointLight = make_shared<Material>();
		pointLight->SetMatCBIndex(12);
		pointLight->SetShader(shader);
		Add<Material>("PointLight", pointLight);
	}

	{
		shared_ptr<Shader> shader = Get<Shader>("SpotLight");
		auto spotLight = make_shared<Material>();
		spotLight->SetMatCBIndex(13);
		spotLight->SetShader(shader);
		Add<Material>("SpotLight", spotLight);
	}

	{
		shared_ptr<Shader> shader = Get<Shader>("Tex");
		auto diffuseLight = make_shared<Material>();
		diffuseLight->SetMatCBIndex(14);
		diffuseLight->SetDiffuseSrvHeapIndex(Get<Texture>("DiffuseLightTarget")->GetTexHeapIndex());
		diffuseLight->SetShader(shader);
		Add<Material>("DiffuseLightTarget", move(diffuseLight));
	}

	{
		shared_ptr<Shader> shader = Get<Shader>("Tex");
		auto specLight = make_shared<Material>();
		specLight->SetMatCBIndex(15);
		specLight->SetDiffuseSrvHeapIndex(Get<Texture>("SpecularLightTarget")->GetTexHeapIndex());
		specLight->SetShader(shader);
		Add<Material>("SpecularLightTarget", move(specLight));
	}

	{
		shared_ptr<Shader> shader = Get<Shader>("Final");
		auto finalMaterial = make_shared<Material>();
		finalMaterial->SetMatCBIndex(16);
		finalMaterial->SetShader(shader);
		Add<Material>("Final", move(finalMaterial));
	}

	{
		shared_ptr<Shader> shader = Get<Shader>("Compute");
		auto material = make_shared<Material>();
		material->SetMatCBIndex(17);
		material->SetShader(shader);
		Add<Material>("Compute", move(material));
	}

	{
		shared_ptr<Shader> tex = Get<Shader>("Tex");
		auto material = make_shared<Material>();
		material->SetMatCBIndex(18);
		material->SetShader(tex);
		material->SetDiffuseSrvHeapIndex(Get<Texture>("UAVTexture")->GetTexHeapIndex());
		Add<Material>("UAVMaterial", move(material));
	}

	//{
	//	shared_ptr<Shader> shader = Get<Shader>("Particle");
	//	auto particle = make_shared<Material>();
	//	particle->SetMatCBIndex(19);
	//	particle->SetDiffuseSrvHeapIndex(Get<Texture>("particle")->GetTexHeapIndex());
	//	particle->SetShader(shader);
	//	Add<Material>("particle", move(particle));
	//}

	{
		shared_ptr<Shader> shader = Get<Shader>("Particle");
		auto particle = make_shared<Material>();
		particle->SetMatCBIndex(19);
		particle->SetDiffuseSrvHeapIndex(Get<Texture>("Particle")->GetTexHeapIndex());
		particle->SetShader(shader);
		Add<Material>("Particle", move(particle));
	}

	{
		shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>("ComputeParticle");
		shared_ptr<Material> material = make_shared<Material>();
		material->SetShader(shader);

		Add<Material>("ComputeParticle", material);
	}
}
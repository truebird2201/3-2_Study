//------------------------------------------------------- ----------------------
// File: Object.h
//-----------------------------------------------------------------------------

#pragma once

#include "Mesh.h"
#include "Camera.h"

#define DIR_FORWARD					0x01
#define DIR_BACKWARD				0x02
#define DIR_LEFT					0x04
#define DIR_RIGHT					0x08
#define DIR_UP						0x10
#define DIR_DOWN					0x20
#define DIR_JUMP					0x21
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define MATERIAL_ALBEDO_MAP			0x01
#define MATERIAL_SPECULAR_MAP		0x02
#define MATERIAL_NORMAL_MAP			0x04
#define MATERIAL_METALLIC_MAP		0x08
#define MATERIAL_EMISSION_MAP		0x10
#define MATERIAL_DETAIL_ALBEDO_MAP	0x20
#define MATERIAL_DETAIL_NORMAL_MAP	0x40
///////////////////////////////////////////////
#define RESOURCE_TEXTURE2D			0x01
#define RESOURCE_TEXTURE2D_ARRAY	0x02	//[]
#define RESOURCE_TEXTURE2DARRAY		0x03
#define RESOURCE_TEXTURE_CUBE		0x04
#define RESOURCE_BUFFER				0x05


class CShader;


struct CB_GAMEOBJECT_INFO
{
	XMFLOAT4X4						m_xmf4x4World;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
class CTexture
{
public:
	CTexture(int nTextureResources, UINT nResourceType, int nSamplers, int nRootParameters);
	virtual ~CTexture();

private:
	int								m_nReferences = 0;

	UINT							m_nTextureType;

	int								m_nTextures = 0;
	ID3D12Resource** m_ppd3dTextures = NULL;
	ID3D12Resource** m_ppd3dTextureUploadBuffers;

	UINT* m_pnResourceTypes = NULL;

	_TCHAR(*m_ppstrTextureNames)[64] = NULL;

	DXGI_FORMAT* m_pdxgiBufferFormats = NULL;
	int* m_pnBufferElements = NULL;

	int								m_nRootParameters = 0;
	int* m_pnRootParameterIndices = NULL;
	D3D12_GPU_DESCRIPTOR_HANDLE* m_pd3dSrvGpuDescriptorHandles = NULL;

	int								m_nSamplers = 0;
	D3D12_GPU_DESCRIPTOR_HANDLE* m_pd3dSamplerGpuDescriptorHandles = NULL;

public:
	void AddRef() { m_nReferences++; }
	void Release() { if (--m_nReferences <= 0) delete this; }

	void SetSampler(int nIndex, D3D12_GPU_DESCRIPTOR_HANDLE d3dSamplerGpuDescriptorHandle);

	void UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList, int nParameterIndex, int nTextureIndex);
	void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);
	void ReleaseShaderVariables();

	void LoadTextureFromDDSFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, wchar_t* pszFileName, UINT nResourceType, UINT nIndex);

	void SetRootParameterIndex(int nIndex, UINT nRootParameterIndex);
	void SetGpuDescriptorHandle(int nIndex, D3D12_GPU_DESCRIPTOR_HANDLE d3dSrvGpuDescriptorHandle);

	int GetRootParameters() { return(m_nRootParameters); }
	int GetTextures() { return(m_nTextures); }
	_TCHAR* GetTextureName(int nIndex) { return(m_ppstrTextureNames[nIndex]); }
	ID3D12Resource* GetResource(int nIndex) { return(m_ppd3dTextures[nIndex]); }
	D3D12_GPU_DESCRIPTOR_HANDLE GetGpuDescriptorHandle(int nIndex) { return(m_pd3dSrvGpuDescriptorHandles[nIndex]); }
	int GetRootParameter(int nIndex) { return(m_pnRootParameterIndices[nIndex]); }

	UINT GetTextureType() { return(m_nTextureType); }
	UINT GetTextureType(int nIndex) { return(m_pnResourceTypes[nIndex]); }
	DXGI_FORMAT GetBufferFormat(int nIndex) { return(m_pdxgiBufferFormats[nIndex]); }
	int GetBufferElements(int nIndex) { return(m_pnBufferElements[nIndex]); }

	D3D12_SHADER_RESOURCE_VIEW_DESC GetShaderResourceViewDesc(int nIndex);

	void ReleaseUploadBuffers();
};
struct MATERIALLOADINFO
{
	XMFLOAT4						m_xmf4AlbedoColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	XMFLOAT4						m_xmf4EmissiveColor = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4						m_xmf4SpecularColor = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

	float							m_fGlossiness = 0.0f;
	float							m_fSmoothness = 0.0f;
	float							m_fSpecularHighlight = 0.0f;
	float							m_fMetallic = 0.0f;
	float							m_fGlossyReflection = 0.0f;

	UINT							m_nType = 0x00;

	//char							m_pstrAlbedoMapName[64] = { '\0' };
	//char							m_pstrSpecularMapName[64] = { '\0' };
	//char							m_pstrMetallicMapName[64] = { '\0' };
	//char							m_pstrNormalMapName[64] = { '\0' };
	//char							m_pstrEmissionMapName[64] = { '\0' };
	//char							m_pstrDetailAlbedoMapName[64] = { '\0' };
	//char							m_pstrDetailNormalMapName[64] = { '\0' };
};

struct MATERIALSLOADINFO
{
	int								m_nMaterials = 0;
	MATERIALLOADINFO* m_pMaterials = NULL;
};

class CMaterialColors
{
public:
	CMaterialColors() { }
	CMaterialColors(MATERIALLOADINFO* pMaterialInfo);
	virtual ~CMaterialColors() { }

private:
	int								m_nReferences = 0;

public:
	void AddRef() { m_nReferences++; }
	void Release() { if (--m_nReferences <= 0) delete this; }

	XMFLOAT4						m_xmf4Ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	XMFLOAT4						m_xmf4Diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4						m_xmf4Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f); //(r,g,b,a=power)
	XMFLOAT4						m_xmf4Emissive = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
};

class CMaterial
{
public:
	CMaterial();
	virtual ~CMaterial();

private:
	int								m_nReferences = 0;

public:
	void AddRef() { m_nReferences++; }
	void Release() { if (--m_nReferences <= 0) delete this; }

	CShader* m_pShader = NULL;

	CMaterialColors* m_pMaterialColors = NULL;

	void SetMaterialColors(CMaterialColors* pMaterialColors);
	void SetShader(CShader* pShader);
	void SetIlluminatedShader() { SetShader(m_pIlluminatedShader); }
	void SetTerrainShader() { SetShader(m_pTerrainShader); }

	void UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList);

protected:
	static CShader* m_pIlluminatedShader;
	static CShader* m_pTerrainShader;

public:
	static void CMaterial::PrepareShaders(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature);
};

class CGameObject
{
private:
	int								m_nReferences = 0;

public:
	void AddRef();
	void Release();

public:
	CGameObject();
	CGameObject(int nMeshes);
	virtual ~CGameObject();

public:
	char							m_pstrFrameName[64];
	bool							check = false;
	float							speed = 0;


	CMesh** m_ppMeshes = NULL;
	int m_nMeshes = 0;
	CMesh* m_pAABBMesh = NULL;

	int								m_nMaterials = 0;
	CMaterial**						m_ppMaterials = NULL;

	XMFLOAT4X4						m_xmf4x4Transform;
	XMFLOAT4X4						m_xmf4x4World;

	XMFLOAT3						m_AABBCenter = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3						m_AABBExtents = XMFLOAT3(0.0f, 0.0f, 0.0f);
	BoundingBox						m_AABB;



	CGameObject* m_pParent = NULL;
	CGameObject* m_pChild = NULL;
	CGameObject* m_pSibling = NULL;

	CCamera* m_camera = NULL;

	LPVOID						m_pPlayerUpdatedContext;

	void SetPlayerUpdatedContext(LPVOID pContext) { m_pPlayerUpdatedContext = pContext; }
	virtual void OnPlayerUpdateCallback(float fTimeElapsed);
	void SetMesh(CMesh* pMesh, int nIndex=0);
	void SetAABBMesh(CMesh* pAABBMesh);
	void SetShader(CShader* pShader);
	void SetShader(int nMaterial, CShader* pShader);
	void SetMaterial(int nMaterial, CMaterial* pMaterial);
	void UpdateAABB();

	void SetChild(CGameObject* pChild, bool bReferenceUpdate = false);

	virtual void BuildMaterials(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList) { }

	virtual void OnInitialize() { }
	virtual void Animate(float fTimeElapsed, XMFLOAT4X4* pxmf4x4Parent = NULL);

	virtual void OnPrepareRender() { }
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera = NULL);

	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void ReleaseShaderVariables();

	virtual void UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList, XMFLOAT4X4* pxmf4x4World);
	virtual void UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList, CMaterial* pMaterial);

	virtual void ReleaseUploadBuffers();

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetLook();
	XMFLOAT3 GetUp();
	XMFLOAT3 GetRight();

	void SetLook(XMFLOAT3 a) { m_xmf4x4World._31 = a.x; m_xmf4x4World._32 = a.y; m_xmf4x4World._33 = a.z; }
	void SetUp(XMFLOAT3 a) { m_xmf4x4World._21 = a.x; m_xmf4x4World._22 = a.y; m_xmf4x4World._23 = a.z; }
	void SetRight(XMFLOAT3 a) { m_xmf4x4World._11 = a.x; m_xmf4x4World._12 = a.y; m_xmf4x4World._13 = a.z; }

	
	void SetPosition(float x, float y, float z);
	void SetPosition(XMFLOAT3 xmf3Position);
	void SetScale(float x, float y, float z);

	void MoveStrafe(float fDistance = 1.0f);
	void MoveUp(float fDistance = 1.0f);
	void MoveForward(float fDistance = 1.0f);

	void Rotate(float fPitch = 10.0f, float fYaw = 10.0f, float fRoll = 10.0f);
	void Rotate(XMFLOAT3* pxmf3Axis, float fAngle);
	void Rotate(XMFLOAT4* pxmf4Quaternion);

	CGameObject* GetParent() { return(m_pParent); }
	void UpdateTransform(XMFLOAT4X4* pxmf4x4Parent = NULL);
	CGameObject* FindFrame(char* pstrFrameName);

	UINT GetMeshType() { return((m_ppMeshes) ? m_ppMeshes[0]->GetType() : 0); }

public:
	static MATERIALSLOADINFO* LoadMaterialsInfoFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, FILE* pInFile);
	static CMeshLoadInfo* LoadMeshInfoFromFile(FILE* pInFile);

	static CGameObject* LoadFrameHierarchyFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, FILE* pInFile);
	static CGameObject* LoadGeometryFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, char* pstrFileName);

	static void PrintFrameInfo(CGameObject* pGameObject, CGameObject* pParent);
	
	
};

////////////////////////////////////////////////////////////////////


class CHeightMapTerrain : public CGameObject
{
public:
	CHeightMapTerrain(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList
		* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, LPCTSTR pFileName, int
		nWidth, int nLength, int nBlockWidth, int nBlockLength, XMFLOAT3 xmf3Scale, XMFLOAT4
		xmf4Color);
	virtual ~CHeightMapTerrain();
private:
	CHeightMapImage* m_pHeightMapImage;
	int m_nWidth;
	int m_nLength;
	XMFLOAT3 m_xmf3Scale = { 1.0f,1.0f,1.0f };
public:
	float GetHeight(float x, float z) { return(m_pHeightMapImage->GetHeight(x / m_xmf3Scale.x, z / m_xmf3Scale.z) * m_xmf3Scale.y); }

	XMFLOAT3 GetNormal(float x, float z) { return(m_pHeightMapImage->GetHeightMapNormal(int(x / m_xmf3Scale.x), int(z / m_xmf3Scale.z))); }

	int GetHeightMapWidth() { return(m_pHeightMapImage->GetHeightMapWidth()); }
	int GetHeightMapLength() { return(m_pHeightMapImage->GetHeightMapLength()); }

	XMFLOAT3 GetScale() { return(m_xmf3Scale); }

	float GetWidth() { return(m_nWidth * m_xmf3Scale.x); }
	float GetLength() { return(m_nLength * m_xmf3Scale.z); }
};


/// //////////////////////////////////////////////////////////////////////
class CRotatingObject : public CGameObject
{
public:
	CRotatingObject();
	virtual ~CRotatingObject();

private:
	XMFLOAT3					m_xmf3RotationAxis;
	float						m_fRotationSpeed;

public:
	void SetRotationSpeed(float fRotationSpeed) { m_fRotationSpeed = fRotationSpeed; }
	void SetRotationAxis(XMFLOAT3 xmf3RotationAxis) { m_xmf3RotationAxis = xmf3RotationAxis; }

	virtual void Animate(float fTimeElapsed, XMFLOAT4X4* pxmf4x4Parent = NULL);
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera = NULL);
};



class CRevolvingObject : public CGameObject
{
public:
	CRevolvingObject();
	virtual ~CRevolvingObject();

private:
	XMFLOAT3					m_xmf3RevolutionAxis;
	float						m_fRevolutionSpeed;

public:
	void SetRevolutionSpeed(float fRevolutionSpeed) { m_fRevolutionSpeed = fRevolutionSpeed; }
	void SetRevolutionAxis(XMFLOAT3 xmf3RevolutionAxis) { m_xmf3RevolutionAxis = xmf3RevolutionAxis; }

	virtual void Animate(float fTimeElapsed, XMFLOAT4X4* pxmf4x4Parent = NULL);
};

class CHellicopterObject : public CGameObject
{
public:
	CHellicopterObject();
	virtual ~CHellicopterObject();


protected:
	CGameObject* m_pMainRotorFrame = NULL;
	CGameObject* m_pTailRotorFrame = NULL;

public:
	XMFLOAT3 target[30] = { { 9993.642578,1781.7,1898.392334 },
		{10664.567383,1731.708008,1896.088867},
{10387.879883,1731.708008,3039.410889},
{10053.763672,1731.708008,4122.112305},
{9489.864258,1731.708008,5031.618652},
{8749.383789,1731.708008,5616.764648},
{7879.607910,1731.708008,5825.460449},
{7108.064453,1731.708008,5577.484863},
{6557.710449,1731.708008,4960.970215},
{6223.214844,1731.708008,4124.794434},
{5843.142578,1731.708008,3178.603760},
{5274.423340,1731.708008,2459.263672},
{4490.493652,1731.708008,2115.061523},
{3635.420166,1731.708008,2302.247070},
{3002.269531,1731.708008,2877.288086},
{2616.467285,1731.708008,3693.192139},
{2463.145264,1731.708008,4516.299316},
{2441.480713,1731.708008,5432.000977},
{2528.486084,1731.708008,6316.228027},
{2770.440674,1731.708008,7203.001465},
{3096.812500,1731.708008,7874.625488},
{3587.687012,1731.708008,8510.458984},
{4200.087891,1731.708008,9057.870117},
{4882.723145,1731.708008,9488.553711},
{5737.229980,1731.708008,9858.669922},
{6534.335449,1731.708008,10050.710938},
{7465.119141,1731.708008,10140.161133},
{8425.211914,1731.708008,10155.407227},
{9229.575195,1731.708008,10144.954102},
{10218.685547,1731.708008,10232.862305}, };
	int move = 0;
	int state = 1;
	XMFLOAT3 TargetPosition;
	virtual void OnInitialize();
	void FollowPlayer(float fTimeElapsed);
	virtual void Animate(float fTimeElapsed, XMFLOAT4X4* pxmf4x4Parent = NULL);
	bool fly = false;
	bool attack = false;
	bool goal = false;
};

class CBulletObject : public CGameObject
{
public:
	CBulletObject();
	virtual ~CBulletObject();
	XMFLOAT3 m_xmf3Position;
	XMFLOAT3 m_xmDirect;
	XMFLOAT3 m_xmf3Velocity;
	void Move(const XMFLOAT3& xmf3Shift, bool bUpdateVelocity);
	void Move(ULONG nDirection, float fDistance, bool bVelocity = false);
protected:

public:
	bool							ready = true;
	virtual void OnInitialize();
	virtual void Animate(float fTimeElapsed, XMFLOAT4X4* pxmf4x4Parent = NULL);
};

class CDangerObject : public CGameObject
{
public:
	CDangerObject();
	virtual ~CDangerObject();
	XMFLOAT3 m_xmf3Position;
	XMFLOAT3 m_xmDirect;
	XMFLOAT3 m_xmf3Velocity;
	int cnt = 0;
	void Move(const XMFLOAT3& xmf3Shift, bool bUpdateVelocity);
	void Move(ULONG nDirection, float fDistance, bool bVelocity = false);
protected:

public:
	bool							ready = true;
	virtual void OnInitialize();
	virtual void Animate(float fTimeElapsed, XMFLOAT4X4* pxmf4x4Parent = NULL);
};

class CApacheObject : public CHellicopterObject
{
public:
	CApacheObject();
	virtual ~CApacheObject();

public:
	virtual void OnInitialize();
	virtual void Animate(float fTimeElapsed, XMFLOAT4X4* pxmf4x4Parent = NULL);
};

class CGunshipObject : public CHellicopterObject
{
public:
	CGunshipObject();
	virtual ~CGunshipObject();

public:
	virtual void OnInitialize();
};

class CSuperCobraObject : public CHellicopterObject
{
public:
	CSuperCobraObject();
	virtual ~CSuperCobraObject();

public:
	virtual void OnInitialize();
};

class CMi24Object : public CHellicopterObject
{
public:
	CMi24Object();
	virtual ~CMi24Object();

public:
	virtual void OnInitialize();
};

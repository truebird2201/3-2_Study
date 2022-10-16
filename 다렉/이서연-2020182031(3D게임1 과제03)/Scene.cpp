//-----------------------------------------------------------------------------
// File: CScene.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "Scene.h"
#include "random"
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")


random_device rd;
uniform_real_distribution <float> uid(6.0, 30.0);
default_random_engine dre{ rd() };

CScene::CScene()
{
}

CScene::~CScene()
{
}

void CScene::BuildDefaultLightsAndMaterials()
{
	m_nLights = 27;
	m_pLights = new LIGHT[m_nLights];
	lightc = new XMFLOAT4[m_nLights*3];
	XMFLOAT3 lp[27] = { {11978.439453,3304.153320 - 1000,10053.290039},
		{0,0,0},
{10395.049805,3304.153320-1000,11859.727539},
{0,0,0},
{6849.378418,3814.953125 - 1000,8973.542969},
{4696.297852,3105.585938 - 1000,11284.440430},
{1605.381470,4000.000000 - 1000,11363.588867},
{3790.971191,4000.000000 - 1000,8746.353516},
{1529.015991,3446.918701 - 1000,6721.133789},
{3016.977539,3248.465088 - 1000,5680.728027},
{1638.476685,3215.683838 - 1000,2446.232178},
{2077.042969,3212.639404 - 1000,1073.671997},
{3884.145264,3211.184814 - 1000,2656.025879},
{5550.649414,3211.184814 - 1000,1317.083008},
{6441.461914,3211.184814 - 1000,1031.025269},
{5895.856445,3211.184814 - 1000,3251.383789},
{6631.851562,3211.184814 - 1000,4353.807129},
{5194.274414,3211.184814 - 1000,5436.239746},
{5203.338867,3211.184814 - 1000,6567.179199},
{6991.941406,3211.184814 - 1000,7305.508789},
{8247.627930,3211.184814 - 1000,5212.005371},
{11529.509766,3211.184814 - 1000,5544.770996},
{11571.407227,3004.473633 - 1000,4636.126953},
{8880.255859,2713.269775 - 1000,3404.395996},
{8513.835938,2683.495361 - 1000,2420.965576},
{9103.330078,2677.357910 - 1000,1205.649414},
{10476.409180,2673.120605 - 1000,1135.551758}, };
	::ZeroMemory(m_pLights, sizeof(LIGHT) * m_nLights);

	m_xmf4GlobalAmbient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);

	for (int i = 0; i < m_nLights - 1; ++i) {
		lightc[i * 3] = XMFLOAT4(uid(dre), uid(dre), uid(dre), 1.0f);
		lightc[i * 3+1] = XMFLOAT4(uid(dre), uid(dre), uid(dre), 1.0f);
		lightc[i * 3+2] = XMFLOAT4(uid(dre), uid(dre), uid(dre), 1.0f);
		m_pLights[i].m_bEnable = true;
		m_pLights[i].m_nType = POINT_LIGHT;
		m_pLights[i].m_fRange = 100000.0f;
		m_pLights[i].m_xmf4Ambient = lightc[i * 3];
		m_pLights[i].m_xmf4Diffuse = lightc[i * 3 + 1];
		m_pLights[i].m_xmf4Specular = lightc[i * 3 + 2];
		m_pLights[i].m_xmf3Position = lp[i];
		m_pLights[i].m_xmf3Direction = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_pLights[i].m_xmf3Attenuation = XMFLOAT3(1.0f, 0.001f, 0.0001f);
	}

	m_pLights[1].m_bEnable = true;
	m_pLights[1].m_nType = SPOT_LIGHT;
	m_pLights[1].m_fRange = 500.0f;
	m_pLights[1].m_xmf4Ambient = XMFLOAT4(uid(dre), uid(dre), uid(dre), 1.0f);
	m_pLights[1].m_xmf4Diffuse = XMFLOAT4(uid(dre), uid(dre), uid(dre), 1.0f);
	m_pLights[1].m_xmf4Specular = XMFLOAT4(uid(dre), uid(dre), uid(dre), 0.0f);
	m_pLights[1].m_xmf3Position = XMFLOAT3(-50.0f, 20.0f, -5.0f);
	m_pLights[1].m_xmf3Direction = XMFLOAT3(0.0f, 0.0f, 1.0f);
	m_pLights[1].m_xmf3Attenuation = XMFLOAT3(1.0f, 0.01f, 0.0001f);
	m_pLights[1].m_fFalloff = 8.0f;
	m_pLights[1].m_fPhi = (float)cos(XMConvertToRadians(40.0f));
	m_pLights[1].m_fTheta = (float)cos(XMConvertToRadians(20.0f));

	m_pLights[3].m_bEnable = true;
	m_pLights[3].m_nType = DIRECTIONAL_LIGHT;
	m_pLights[3].m_xmf4Ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	m_pLights[3].m_xmf4Diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	m_pLights[3].m_xmf4Specular = XMFLOAT4(0.4f, 0.4f, 0.4f, 0.0f);
	m_pLights[3].m_xmf3Direction = XMFLOAT3(0.0f, -1.0f, 0.0f);
}

void CScene::BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
{
	
}

void CScene::ReleaseObjects()
{
	if (m_pd3dGraphicsRootSignature) m_pd3dGraphicsRootSignature->Release();

	if (m_ppGameObjects)
	{
		for (int i = 0; i < m_nGameObjects; i++) if (m_ppGameObjects[i]) m_ppGameObjects[i]->Release();
		delete[] m_ppGameObjects;
	}
	if (m_pTerrain)delete m_pTerrain;

	ReleaseShaderVariables();

	if (m_pLights) delete[] m_pLights;
}

ID3D12RootSignature *CScene::CreateGraphicsRootSignature(ID3D12Device *pd3dDevice)
{
	ID3D12RootSignature *pd3dGraphicsRootSignature = NULL;

	D3D12_ROOT_PARAMETER pd3dRootParameters[3];

	pd3dRootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[0].Descriptor.ShaderRegister = 1; //Camera
	pd3dRootParameters[0].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	pd3dRootParameters[1].Constants.Num32BitValues = 32;
	pd3dRootParameters[1].Constants.ShaderRegister = 2; //GameObject
	pd3dRootParameters[1].Constants.RegisterSpace = 0;
	pd3dRootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[2].Descriptor.ShaderRegister = 4; //Lights
	pd3dRootParameters[2].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	D3D12_ROOT_SIGNATURE_FLAGS d3dRootSignatureFlags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT | D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;
	D3D12_ROOT_SIGNATURE_DESC d3dRootSignatureDesc;
	::ZeroMemory(&d3dRootSignatureDesc, sizeof(D3D12_ROOT_SIGNATURE_DESC));
	d3dRootSignatureDesc.NumParameters = _countof(pd3dRootParameters);
	d3dRootSignatureDesc.pParameters = pd3dRootParameters;
	d3dRootSignatureDesc.NumStaticSamplers = 0;
	d3dRootSignatureDesc.pStaticSamplers = NULL;
	d3dRootSignatureDesc.Flags = d3dRootSignatureFlags;

	ID3DBlob *pd3dSignatureBlob = NULL;
	ID3DBlob *pd3dErrorBlob = NULL;
	D3D12SerializeRootSignature(&d3dRootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &pd3dSignatureBlob, &pd3dErrorBlob);
	pd3dDevice->CreateRootSignature(0, pd3dSignatureBlob->GetBufferPointer(), pd3dSignatureBlob->GetBufferSize(), __uuidof(ID3D12RootSignature), (void **)&pd3dGraphicsRootSignature);
	if (pd3dSignatureBlob) pd3dSignatureBlob->Release();
	if (pd3dErrorBlob) pd3dErrorBlob->Release();

	return(pd3dGraphicsRootSignature);
}

void CScene::CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
{
	UINT ncbElementBytes = ((sizeof(LIGHTS) + 255) & ~255); //256ÀÇ ¹è¼ö
	m_pd3dcbLights = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbElementBytes, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);

	m_pd3dcbLights->Map(0, NULL, (void **)&m_pcbMappedLights);
}

void CScene::UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList)
{
	::memcpy(m_pcbMappedLights->m_pLights, m_pLights, sizeof(LIGHT) * m_nLights);
	::memcpy(&m_pcbMappedLights->m_xmf4GlobalAmbient, &m_xmf4GlobalAmbient, sizeof(XMFLOAT4));
	::memcpy(&m_pcbMappedLights->m_nLights, &m_nLights, sizeof(int));
}

void CScene::ReleaseShaderVariables()
{
	if (m_pd3dcbLights)
	{
		m_pd3dcbLights->Unmap(0, NULL);
		m_pd3dcbLights->Release();
	}
}

void CScene::ReleaseUploadBuffers()
{
	for (int i = 0; i < m_nGameObjects; i++) m_ppGameObjects[i]->ReleaseUploadBuffers();
	for (int i = 0; i < m_nBullets; i++) m_ppBullets[i]->ReleaseUploadBuffers();
	m_ppDangers->ReleaseUploadBuffers();
	m_pTerrain->ReleaseUploadBuffers();
}

bool CScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return(false);
}

bool CScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case 'R': printf("{%f,%f,%f},\n", m_pPlayer->GetPosition().x, m_pPlayer->GetPosition().y, m_pPlayer->GetPosition().z);
			break;
		case 'T': 
			(topview) ? topview = false : topview = true;
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
	return(false);
}

bool CScene::ProcessInput(UCHAR *pKeysBuffer)
{
	return(false);
}

void CScene::AnimateObjects(float fTimeElapsed)
{

	m_fElapsedTime = fTimeElapsed;

	BoundingCheck();

	for (int i = 0; i < m_nGameObjects; i++) {
		if (m_ppGameObjects[i]->fly == true) {
			m_ppGameObjects[i]->MoveUp(-0.7f);
			m_ppGameObjects[i]->Rotate(0.0f, -1.0f, -0.5f);
			if (m_ppGameObjects[i]->GetPosition().y < 500) {

				m_ppGameObjects[0] = new CApacheObject();
				m_ppGameObjects[0]->SetChild(pApacheModel, true);
				m_ppGameObjects[0]->OnInitialize();
				m_ppGameObjects[0]->SetPosition({ 9993.642578,1781.7,1898.392334 });
				m_ppGameObjects[0]->SetScale(7.5f, 7.5f, 7.5f);
				m_ppGameObjects[0]->Rotate(0.0f, 90.0f, 0.0f);
				m_ppGameObjects[0]->m_AABBCenter = m_ppGameObjects[0]->GetPosition();
				m_ppGameObjects[0]->m_AABBExtents = { 10.2f,5.2f,10.2f };
				m_ppGameObjects[0]->m_AABB.Center = m_ppGameObjects[0]->GetPosition();
				m_ppGameObjects[0]->m_AABB.Extents = { 50.2 * 2.5f,25.2 * 2.5f,50.2 * 2.5f };
			}
		}
		m_ppGameObjects[i]->TargetPosition = m_pPlayer->GetPosition();
		m_ppGameObjects[i]->FollowPlayer(fTimeElapsed);
		if (m_ppGameObjects[i]->attack && m_ppDangers->ready == true && m_ppGameObjects[i]->fly==false) {
			m_ppDangers->ready = false;
			m_ppDangers->SetPosition(m_ppGameObjects[i]->GetPosition());
			m_ppDangers->m_xmDirect = Vector3::Normalize(Vector3::Subtract(m_pPlayer->GetPosition(),m_ppDangers->GetPosition()));

		}
	}

	m_ppDangers->Animate(fTimeElapsed, NULL);

	if (m_pLights)
	{
		m_pLights[1].m_xmf3Position = m_pPlayer->GetPosition();
		m_pLights[1].m_xmf3Direction = m_pPlayer->GetLookVector();
		for (int i = 0; i < m_nGameObjects; i++) {
			if (m_ppGameObjects[i]->attack == true) {
				for (int j = 0; j < m_nLights; j++) {
					if (j != 3) {
						m_pLights[j].m_xmf4Ambient = XMFLOAT4(10.3f, 0.0f, 0.0f, 1.0f);
						m_pLights[j].m_xmf4Diffuse = XMFLOAT4(10.3f, 0.0f, 0.0f, 1.0f);
						m_pLights[j].m_xmf4Specular = XMFLOAT4(10.3f, 0.0f, 0.0f, 1.0f);
					}
				}
			}
			else {
				for (int j = 0; j < m_nLights; j++) {
					if (j != 3) {
						m_pLights[j].m_xmf4Ambient = lightc[j * 3];
						m_pLights[j].m_xmf4Diffuse = lightc[j * 3 + 1];
						m_pLights[j].m_xmf4Specular = lightc[j * 3 + 2];
					}
				}
			}
		}
	}
	

}

void CScene::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera)
{
	pd3dCommandList->SetGraphicsRootSignature(m_pd3dGraphicsRootSignature);

	pCamera->SetViewportsAndScissorRects(pd3dCommandList);
	pCamera->UpdateShaderVariables(pd3dCommandList);

	UpdateShaderVariables(pd3dCommandList);
	if (topview) {
		pCamera->SetOffset(XMFLOAT3(0.0f, 15000.0f, -800.0f));
	}
	else {
		pCamera->SetOffset(XMFLOAT3(0.0f, 200.0f, -400.0f));
	}
	D3D12_GPU_VIRTUAL_ADDRESS d3dcbLightsGpuVirtualAddress = m_pd3dcbLights->GetGPUVirtualAddress();
	pd3dCommandList->SetGraphicsRootConstantBufferView(2, d3dcbLightsGpuVirtualAddress); //Lights
	if (m_pTerrain)m_pTerrain->Render(pd3dCommandList, pCamera);
	for (int i = 0; i < m_nGameObjects; i++)
	{
		if (m_ppGameObjects[i])
		{
			m_ppGameObjects[i]->Animate(m_fElapsedTime, NULL);
			m_ppGameObjects[i]->UpdateTransform(NULL);
			m_ppGameObjects[i]->Render(pd3dCommandList, pCamera);
		}
	}
	for (int i = 0; i < m_nBullets; i++)
	{
		if (m_ppBullets[i]->ready==false)
		{
			m_ppBullets[i]->Animate(m_fElapsedTime, NULL);
			m_ppBullets[i]->UpdateTransform(NULL);
			m_ppBullets[i]->Render(pd3dCommandList, pCamera);
		}
	}
	m_ppDangers->Animate(m_fElapsedTime, NULL);
	m_ppDangers->UpdateTransform(NULL);
	m_ppDangers->Render(pd3dCommandList, pCamera);
}

void CScene::BoundingCheck() {
	for (int i = 0; i < m_nGameObjects; ++i)
	{
		m_ppGameObjects[i]->UpdateAABB();
	}
	for (int j = 0; j < m_nBullets; ++j) {
		m_ppBullets[j]->UpdateAABB();
	}

	for (int i = 0; i < m_nGameObjects; ++i)
	{
		for (int j = 0; j < m_nBullets; ++j) {
			if (m_ppGameObjects[i]->m_AABB.Intersects(m_ppBullets[j]->m_AABB)&& m_ppBullets[j]->ready == false)
			{
				m_ppGameObjects[i]->fly = true;
				m_ppBullets[j]->ready = true;
				m_pPlayer->point++;
			}
		}
	}
	if (m_pPlayer->m_AABB.Intersects(m_ppDangers->m_AABB) && m_ppDangers->ready == false)end=true;
}
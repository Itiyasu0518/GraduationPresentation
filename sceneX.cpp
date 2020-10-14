//=============================================================================
//
// sceneX���� [sceneX.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "sceneX.h"
#include "manager.h"
#include "renderer.h"
#include "light.h"
#include "object.h"
#include "camera.h"

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
LPD3DXEFFECT CSceneX::m_pToonShader = NULL;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CSceneX::CSceneX(CScene::PRIORITY obj = CScene::PRIORITY_MODEL) : CScene(obj)
{
	SetObjType(CScene::PRIORITY_MODEL);

	m_pBuffMat = NULL;
	m_pMesh = NULL;
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_size = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CSceneX::~CSceneX()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CSceneX::Init(void)
{
	return S_OK;
}

//=============================================================================
// �J������
//=============================================================================
void CSceneX::Uninit(void)
{

}

//=============================================================================
// �X�V����
//=============================================================================
void CSceneX::Update(void)
{
#ifdef _DEBUG
	Debug();
#endif
}

//=============================================================================
// �`�揈��
//=============================================================================
void CSceneX::Draw(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	D3DXVECTOR3 pos = GetPosition();

	D3DXMATRIX		mtxRot, mtxTrans;				//�v�Z�p�}�g���b�N�X
	D3DXMATERIAL	*pMat;							//���݂̃}�e���A���ۑ��p
	D3DMATERIAL9	matDef;							//�}�e���A���f�[�^�ւ̃|�C���^

	//�f�o�C�X���擾����
	pDevice = pRenderer->GetDevice();

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);					//���ׂẴf�[�^���^

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A�����ɑ΂���|�C���^���擾
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	D3DXMATRIX mtxView, mtxProj;

	pDevice->GetTransform(D3DTS_VIEW, &mtxView);
	pDevice->GetTransform(D3DTS_PROJECTION, &mtxProj);

	// �V�F�[�_�[����
	if (m_pToonShader != NULL)
	{
		m_pToonShader->SetTechnique("ToonShading");
		D3DXMATRIX mAll = m_mtxWorld * mtxView * mtxProj;
		m_pToonShader->SetMatrix("WVP", &mAll);

		m_pToonShader->SetMatrix("mProj", &mtxProj);
		m_pToonShader->SetMatrix("mView", &mtxView);
		m_pToonShader->SetMatrix("mWorld", &m_mtxWorld);

		m_pToonShader->SetTexture("ShadeTexture", CManager::GetResource("Shade.bmp"));
		m_pToonShader->SetTexture("LineTexture", CManager::GetResource("Outline.bmp"));

		CCamera *pCamera = CManager::GetCamera();
		CLight *pLight = CManager::GetLight();

		D3DXVECTOR4 lightPos = pLight->GetPos();

		m_pToonShader->SetVector("LightPos", (D3DXVECTOR4*)&lightPos);
		m_pToonShader->SetVector("EyePos", (D3DXVECTOR4*)&pCamera->GetPosV());

		m_pToonShader->Begin(NULL, 0);
	}

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		if (m_pToonShader != NULL)
		{
			m_pToonShader->BeginPass(0);
			m_pToonShader->SetFloatArray("Diffuse", (FLOAT*)&pMat[nCntMat].MatD3D.Diffuse, 4);

			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				// �e�N�X�`���̐ݒ�
				m_pToonShader->SetTexture("DecalTexture", CManager::GetResource(pMat[nCntMat].pTextureFilename));
			}
			else
			{
				// �e�N�X�`���̐ݒ�
				m_pToonShader->SetTexture("DecalTexture", CManager::GetResource("data/tex/default.jpg"));
			}
		}

		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		if (pMat[nCntMat].pTextureFilename != NULL)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, CManager::GetResource(pMat[nCntMat].pTextureFilename));
		}

		// �`��
		m_pMesh->DrawSubset(nCntMat);

		if (m_pToonShader != NULL)
		{
			m_pToonShader->EndPass();
		}
	}

	if (m_pToonShader != NULL)
	{
		m_pToonShader->End();
	}

	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
// �N���G�C�g�֐�
//=============================================================================
CSceneX *CSceneX::Create(void)
{
	CSceneX *pSceneX;
	pSceneX = new CSceneX(CScene::PRIORITY_MODEL);

	if (pSceneX != NULL)
	{// �V�[�������݂��Ă����Ƃ�
		pSceneX->Init();				// ����������
	}
	return pSceneX;
}

//=============================================================================
// ���f�����̐ݒ�
//=============================================================================
void CSceneX::BindModel(LPD3DXMESH pMesh, DWORD nNumMat, LPD3DXBUFFER pBuffMat)
{
	m_pMesh = pMesh;
	m_nNumMat = nNumMat;
	m_pBuffMat = pBuffMat;
}

//=============================================================================
// �J���[�̐ݒ�
//=============================================================================
void CSceneX::SetColor(D3DXCOLOR col)
{
	m_col = col;
}

//=============================================================================
// �T�C�Y�̐ݒ�
//=============================================================================
void CSceneX::SetSize(D3DXVECTOR3 size)
{
	m_size = size;
}

//=============================================================================
// ��]�l�̐ݒ�
//=============================================================================
void CSceneX::SetRotation(D3DXVECTOR3 rot)
{
	m_rot = rot;
}

//=============================================================================
// �V�F�[�_�[�̏���������
//=============================================================================
void CSceneX::InitShader(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	CRenderer *pRenderer = CManager::GetRenderer();

	// �f�o�C�X���擾����
	pDevice = pRenderer->GetDevice();

	//�V�F�[�_�[��ǂݍ���
	if (FAILED(D3DXCreateEffectFromFile(pDevice, "ToonShader.fx", NULL, NULL,
		0, NULL, &m_pToonShader, NULL)))
	{
		MessageBox(NULL, "�V�F�[�_�[�t�@�C���ǂݍ��ݎ��s", "", MB_OK);
	}
}

//=============================================================================
// �V�F�[�_�[�̊J������
//=============================================================================
void CSceneX::UninitShader(void)
{
	// �V�F�[�_�[�̊J��
	if (m_pToonShader != NULL)
	{
		m_pToonShader->Release();
		m_pToonShader = NULL;
	}
}

//=============================================================================
// �C���X�y�N�^�[��\��
//=============================================================================
void CSceneX::ShowInspector(void)
{
	D3DXVECTOR3 pos = GetPosition();
	if (ImGui::CollapsingHeader("Transform"))
	{
		ImGui::DragFloat3("Position", (float*)&pos, 0.1f);
		ImGui::DragFloat3("Rotation", (float*)&m_rot, 0.01f);
		ImGui::Text("");

		SetPosition(pos);
	}

	CScene::ShowInspector();
}

#ifdef _DEBUG
//=============================================================================
// �f�o�b�O����
//=============================================================================
void CSceneX::Debug(void)
{

}
#endif
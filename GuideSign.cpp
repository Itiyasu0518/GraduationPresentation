//=============================================================================
//
// �ē���󏈗� [GuideSign.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "GuideSign.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define GUIDESIGN "data/model/yajirusi.x"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CGuideSign::CGuideSign(CScene::PRIORITY obj = CScene::PRIORITY_UI) : CSceneX(obj)
{
	SetObjType(CScene::PRIORITY_UI);
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CGuideSign::~CGuideSign()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CGuideSign::Init(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();	// �f�o�C�X�̎擾

	// ����������
	CSceneX::Init();

	SetShader(SHADERTYPE_TOON);

	LPD3DXMESH		pMesh;							// ���b�V�����ւ̃|�C���^
	DWORD			nNumMat;							// �}�e���A�����̐�
	LPD3DXBUFFER	pBuffMat;						// �}�e���A�����ւ̃|�C���^

	CManager::GetModelResource(GUIDESIGN, pBuffMat, nNumMat, pMesh);
	BindModel(pMesh, nNumMat, pBuffMat);

	D3DXVECTOR3 pos, size, rot;
	pos = D3DXVECTOR3(0, 50.0f, 50.0f);
	size = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);

	SetPosition(pos);			// �ʒu�̐ݒ�
	SetSize(size);			// �傫���ݒ�
	SetRotation(rot);			// ��]�ݒ�
	return S_OK;
}

//=============================================================================
// �J������
//=============================================================================
void CGuideSign::Uninit(void)
{
	// �J������
	CSceneX::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CGuideSign::Update(void)
{
	CCamera *pCamera = CManager::GetCamera();
	D3DXVECTOR3 pos = D3DXVECTOR3(0, 0, 0);
	D3DXVECTOR3 rot = D3DXVECTOR3(0, 0, 0);

	// �J�����̏�񂪂���Ƃ�
	if (pCamera != NULL)
	{
		pos = pCamera->GetPosition();	// �ʒu�擾
		rot = pCamera->GetRotation();	// ��]�擾
	}

#ifdef _DEBUG
	Debug();				// �f�o�b�O����
#endif
}

//=============================================================================
// �`�揈��
//=============================================================================
void CGuideSign::Draw(void)
{
	CSceneX::Draw();
}

//=============================================================================
// �N���G�C�g�֐�
//=============================================================================
CGuideSign *CGuideSign::Create(void)
{
	CGuideSign *pGuideSign = NULL;
	pGuideSign = new CGuideSign(CScene::PRIORITY_UI);

	if (pGuideSign != NULL)
	{
		pGuideSign->Init();
	}
	return pGuideSign;
}

//=============================================================================
// ���[�h����
//=============================================================================
HRESULT CGuideSign::Load(void)
{
	CManager::LoadModel(GUIDESIGN);
	return S_OK;
}

#ifdef _DEBUG
//=============================================================================
// �f�o�b�O����
//=============================================================================
void CGuideSign::Debug(void)
{

}
#endif
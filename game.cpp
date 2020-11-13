//=============================================================================
//
// �Q�[������ [game.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "game.h"
#include "fade.h"
#include "renderer.h"
#include "scene.h"
#include "manager.h"
#include "inputKeyboard.h"
#include "meshField.h"
#include "player.h"
#include "scene3D.h"
#include "meshSphere.h"
#include "meshCube.h"
#include "meshCapsule.h"
#include "enemy.h"
#include "sky.h"
#include "tree.h"
#include "object.h"
#include "circle.h"
#include "stage.h"
#include "meshOrbit.h"
#include "gauge2D.h"
#include "effect.h"
#include "tree.h"
#include "number.h"
#include "enemyUi.h"
#include "messageWindow.h"
#include "time.h"
#include "result.h"
#include "ui.h"
#include "network.h"
#include "wall.h"
#include "speed.h"
#include "GuideSign.h"

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
CMeshField	*CGame::m_pMeshField = NULL;		// ���b�V���t�B�[���h�̃|�C���^
CPlayer		*CGame::m_pPlayer = NULL;			// �v���C���[�̃|�C���^
CMeshSphere *CGame::m_pMeshSphere = NULL;		// ���b�V���X�t�B�A�̃|�C���^
CEnemy		*CGame::m_pEnemy = NULL;			// �G�̃|�C���^
CSky		*CGame::m_pSky = NULL;				// ��̃|�C���^
CHouse		*CGame::m_pHouse = NULL;			// �Ƃ̃|�C���^
CSpeed		*CGame::m_pSpeed = NULL;			// �����̃|�C���^

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CGame::CGame()
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CGame::~CGame()
{

}

//=============================================================================
//Init����
//=============================================================================
HRESULT CGame::Init(void)
{
	// �G�t�F�N�g�̐���
	CEffect::Create();

	// �v���C���[�̐���
	m_pPlayer = CPlayer::Create();

	// ��̍쐬
	m_pSky = CSky::Create();

	CObject::LoadScript();

	// �e��A�Z�b�g�̐������ݒu
	//CMeshField::LoadRand("data/stage/rand.csv", false);			// �����̓Ǎ�
	//CObject::LoadModel("data/stage/object.csv");					// ���f�����̓Ǎ�
	//CEnemy::LoadEnemy("data/stage/enemy.csv");					// �G���̓Ǎ�

	CEnemy *pEnemy = CEnemy::Create();			// �N���G�C�g����

	if (pEnemy != NULL)
	{// �G�����݂��Ă����Ƃ�
		pEnemy->SetPosition(D3DXVECTOR3(0.0f, 0.0f, -100.0f));			// �|�W�V���������߂�
		pEnemy->SetTarget(TARGETTYPE_PLAYER);				// �U���Ώۂ̐ݒ�
	}

	// ���b�Z�[�W�E�B���h�E�̍쐬
	CMessageWindow::Create(CMessageWindow::MESSAGETYPE_START);

	// ���Ԃ̃N���G�C�g����
	CTime::Create();

	// �����̐���
	CSpeed::Create();

	// ���f�����̓ǂݍ���
	CObject::LoadModelTest("data/text/model.txt");

	// �Ǐ��̓ǂݍ���
	CMeshWall::LoadWall("data/text/wall.txt", false);

	// �ē����̐���
	CGuideSign::Create();

	// �l�b�g���[�N�ł̃Q�[��������������
	CManager::GetNetwork()->InitGame();

	return S_OK;
}

//=============================================================================
// Update�֐�
//=============================================================================
void CGame::Update(void)
{

}

//=============================================================================
// Draw�֐�
//=============================================================================
void CGame::Draw(void)
{

}

//=============================================================================
// Uninit�֐�
//=============================================================================
void CGame::Uninit(void)
{
	CObject::Unload();

	// �|���S���̊J��
	CScene::ReleaseAll();
}

//=============================================================================
// �A�Z�b�g�̓ǂݍ���
//=============================================================================
void CGame::LoadAsset(void)
{
	// =============== �A�Z�b�g�̓ǂݍ��� ===============//
	CPlayer::Load();
	CSky::Load();
	CMeshField::Load();
	CMeshOrbit::Load();
	CEnemy::Load();
	CObject::Load();
	CMeshSphere::Load();
	CGauge2D::Load();
	CEffect::Load();
	CNumber::Load();
	CEnemyUi::Load();
	CMessageWindow::Load();
	CGuideSign::Load();
}
//=============================================================================
//
// puzzle���� [puzzle.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "puzzle.h"
#include "fade.h"
#include "renderer.h"
#include "scene.h"
#include "manager.h"
#include "inputKeyboard.h"
#include "inputController.h"
#include "box.h"
#include "network.h"

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CPuzzle::CPuzzle()
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CPuzzle::~CPuzzle()
{

}

//=============================================================================
//Init����
//=============================================================================
HRESULT CPuzzle::Init(void)
{
	LoadAsset();
	pBox = CBox::Create();

	// �e��A�Z�b�g�̐������ݒu
	//CMeshField::LoadRand("data/stage/rand.csv", false);				// �����̓Ǎ�
	//CObject::LoadModel("data/stage/object.csv");						// ���f�����̓Ǎ�
	//CEnemy::LoadEnemy("data/stage/enemy.csv");						// �G���̓Ǎ�


	return S_OK;
}

//=============================================================================
// Update�֐�
//=============================================================================
void CPuzzle::Update(void)
{


#ifdef _DEBUG
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();
	CInputController *pInputController = CManager::GetInputController();

	if (CFade::GetFade() == CFade::FADE_NONE)
	{//�t�F�[�h�����������Ă��Ȃ��Ƃ�
		if (pInputKeyboard != NULL)
		{// �L�[�{�[�h�����݂��Ă����Ƃ�
			if (pInputKeyboard->GetTriggerKeyboard(DIK_RETURN))
			{// �w��̃L�[�������ꂽ�Ƃ�
				if (CManager::GetNetwork() != NULL)
				{
					if (CManager::GetNetwork()->Connect() == S_OK)
					{
						CFade::SetFade(CManager::MODE_GAME);					//�t�F�[�h������
					}
				}
			}
		}
		if (pInputController->GetJoypadUse(0))
		{// �R���g���[���[����������Ă���Ƃ�
		 //�Q�[���̑J��
			if (pInputController->GetControllerTrigger(0, JOYPADKEY_A) ||			// �Q�[���p�b�h��A�{�_���������ꂽ�Ƃ�
				pInputController->GetControllerTrigger(0, JOYPADKEY_START))			// �Q�[���p�b�h��START�{�^���������ꂽ�Ƃ�
			{
				CFade::SetFade(CManager::MODE_GAME);					//�t�F�[�h������
			}
		}
	}
#endif
}

//=============================================================================
// Draw�֐�
//=============================================================================
void CPuzzle::Draw(void)
{

}

//=============================================================================
// Uninit�֐�
//=============================================================================
void CPuzzle::Uninit(void)
{
	// �|���S���̊J��
	CScene::ReleaseAll();
}

//=============================================================================
// �A�Z�b�g�̓ǂݍ���
//=============================================================================
void CPuzzle::LoadAsset(void)
{
	CBox::Load();
}
//==================================================================================================================
//
// box[box.cpp]
// Author:Ryouma Inoue
//
//==================================================================================================================
#include "manager.h"
#include "renderer.h"
#include "Box.h"
#include "inputKeyboard.h"
#include "player.h"
#include "game.h"
#include "scene2D.h"
#include "takaseiLibrary.h"

//==================================================================================================================
// �}�N����`
//==================================================================================================================
#define WhileX 50.0f																// �C�`�}�X�̒�����
#define WhileY 50.0f																// �C�`�}�X�̒�������
#define WhileZ 50.0f																// �C�`�}�X�̒����c

//==================================================================================================================
// �ÓI�����o�ϐ��̏�����
//==================================================================================================================
LPDIRECT3DTEXTURE9 CBox::m_pTexture = NULL;			// �e�N�X�`���ϐ�

//==================================================================================================================
// �O���[�o���ϐ�
//==================================================================================================================

//==================================================================================================================
// �R���X�g���N�^
//==================================================================================================================
CBox::CBox(PRIORITY type = CScene::PRIORITY_FLOOR) :CScene(type)
{

}

//==================================================================================================================
// �f�X�g���N�^
//==================================================================================================================
CBox::~CBox()
{

}

//==================================================================================================================
// ����������
//==================================================================================================================
HRESULT CBox::Init(void)
{
	// ������
	m_nCntMove_X = 0;
	m_nCntMove_Y = 0;
	m_bPlacement = false;
	m_bCreate = false;
	m_bMove = false;

	for (int nCntDepth = 0; nCntDepth < Box_Depth; nCntDepth++)
	{
		for (int nCntWidth = 0; nCntWidth < Box_Width; nCntWidth++)
		{
			m_bPuzzle[nCntDepth][nCntWidth] = false;

			m_pBlock[nCntDepth][nCntWidth] = CScene2D::Create(PRIORITY_UI);

			if (m_pBlock[nCntDepth][nCntWidth] != NULL)
			{
				m_pBlock[nCntDepth][nCntWidth]->SetPosition(D3DXVECTOR3(100.0f + nCntWidth * 55.0f,
					100.0f + nCntDepth * 55.0f,
					0.0f));
				m_pBlock[nCntDepth][nCntWidth]->SetSize(D3DXVECTOR3(50.0f, 50.0f, 0.0f));
				m_pBlock[nCntDepth][nCntWidth]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				m_pBlock[nCntDepth][nCntWidth]->SetTransform();
			}
		}
	}

	//m_bPuzzle[nCntMove_Y][nCntMove_X] = true;
	//m_bPuzzle[nCntMove_Y][nCntMove_X + 1] = true;
	//m_bPuzzle[nCntMove_Y + 1][nCntMove_X] = true;
	//m_bPuzzle[nCntMove_Y + 1][nCntMove_X + 1] = true;

	return S_OK;
}

//==================================================================================================================
// �I������
//==================================================================================================================
void CBox::Uninit(void)
{

}

//==================================================================================================================
// �X�V����
//==================================================================================================================
void CBox::Update(void)
{
	// �L�[�{�[�h�擾
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();

	// �c���J�E���g
	for (int nDepth = 0; nDepth < Box_Depth; nDepth++)
	{
		// �����J�E���g
		for (int nWide = 0; nWide < Box_Width; nWide++)
		{
			// �����z�u
			if (nDepth == m_nCntMove_Y && nWide == m_nCntMove_X)
			{
				m_bPuzzle[nDepth][nWide] = true;
			}
			else if (nDepth == m_nCntMove_Y && nWide == m_nCntMove_X + 1)
			{
				m_bPuzzle[nDepth][nWide] = true;
			}
			else if (nDepth == m_nCntMove_Y + 1 && nWide == m_nCntMove_X)
			{
				m_bPuzzle[nDepth][nWide] = true;
			}
			else if (nDepth == m_nCntMove_Y + 1 && nWide == m_nCntMove_X + 1)
			{
				m_bPuzzle[nDepth][nWide] = true;
			}
			else
			{
				m_bPuzzle[nDepth][nWide] = false;
			}

			// ��Ԋm�F
			if (m_bPuzzle[nDepth][nWide] == true)
			{
				// �e�N�X�`���ύX
				m_pBlock[nDepth][nWide]->BindTexture(CManager::GetResource("data/tex/grass.jpg"));

				// �z�u���Ă��邩�ǂ���
				if (m_bPlacement == false)
				{
					// �F�̕ύX
					m_pBlock[nDepth][nWide]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));
				}
				else
				{
					// �F�̕ύX
					m_pBlock[nDepth][nWide]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
					m_bCreate = true;
				}
			}
			else
			{
				// �e�N�X�`���ύX
				m_pBlock[nDepth][nWide]->BindTexture(CManager::GetResource("data/tex/SignBoard3.png"));
				// �F�̕ύX
				m_pBlock[nDepth][nWide]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}


		}
	}

	if (m_bMove == false)
	{
		// -----------------------------------------
		// �ړ�����
		// -----------------------------------------
		// ���E����
		if (pKeyboard->GetTriggerKeyboard(MOVE_LEFT))
		{
			m_nCntMove_X--;
		}
		else if (pKeyboard->GetTriggerKeyboard(MOVE_RIGHT))
		{
			m_nCntMove_X++;
		}
		// �㉺����
		else if (pKeyboard->GetTriggerKeyboard(MOVE_ACCEL))
		{
			m_nCntMove_Y--;
		}
		else if (pKeyboard->GetTriggerKeyboard(MOVE_BRAKE))
		{
			m_nCntMove_Y++;
		}
	}

	// �z�u����
	if (pKeyboard->GetTriggerKeyboard(MOVE_JUMP))
	{
		m_bPlacement = true;
		m_bMove = true;
	}

	// �ړ�����
	if (m_nCntMove_X <= 0)
	{
		m_nCntMove_X = 0;
	}
	if (m_nCntMove_Y <= 0)
	{
		m_nCntMove_Y = 0;
	}
	if (m_nCntMove_X >= Box_Width - 2)
	{
		m_nCntMove_X = Box_Width - 2;
	}
	if (m_nCntMove_Y >= Box_Depth - 2)
	{
		m_nCntMove_Y = Box_Depth - 2;
	}
}

//==================================================================================================================
// �`�揈��
//==================================================================================================================
void CBox::Draw(void)
{

}

//==================================================================================================================
// �|���S������
//==================================================================================================================
CBox *CBox::Create(void)
{
	// �V�[�����I�Ɋm��
	CBox *pBox = new CBox(CScene::PRIORITY_FLOOR);

	if (pBox != NULL)
	{
		// �V�[��������
		pBox->Init();
	}

	// �l��Ԃ�
	return pBox;
}

//==================================================================================================================
// �e�N�X�`����񃍁[�h
//==================================================================================================================
HRESULT CBox::Load(void)
{
	return S_OK;
}

//==================================================================================================================
// �e�N�X�`�����j��
//==================================================================================================================
void CBox::Unload(void)
{
	// �e�N�X�`���̊J��

}
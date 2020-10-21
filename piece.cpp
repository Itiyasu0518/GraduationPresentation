//==================================================================================================================
//
// �s�[�X[piece.cpp]
// Author:Ryouma Inoue
//
//==================================================================================================================
#include "manager.h"
#include "renderer.h"
#include "piece.h"
#include "inputKeyboard.h"
#include "player.h"
#include "game.h"
#include "scene2D.h"
#include "takaseiLibrary.h"
#include "debug.h"

//==================================================================================================================
// �}�N����`
//==================================================================================================================
#define WhileX 50.0f																// �C�`�}�X�̒�����
#define WhileY 50.0f																// �C�`�}�X�̒�������
#define WhileZ 50.0f																// �C�`�}�X�̒����c
#define Range_X 13																	// ���͈̔�
#define Range_Y 6																	// �c�͈̔�

//==================================================================================================================
// �ÓI�����o�ϐ��̏�����
//==================================================================================================================
LPDIRECT3DTEXTURE9 CPiece::m_pTexture = NULL;			// �e�N�X�`���ϐ�

//==================================================================================================================
// �O���[�o���ϐ�
//==================================================================================================================

//==================================================================================================================
// �R���X�g���N�^
//==================================================================================================================
CPiece::CPiece(PRIORITY type = CScene::PRIORITY_FLOOR) :CScene(type)
{

}

//==================================================================================================================
// �f�X�g���N�^
//==================================================================================================================
CPiece::~CPiece()
{

}

//==================================================================================================================
// ����������
//==================================================================================================================
HRESULT CPiece::Init(void)
{
	// ������
	m_nCntMove_X = 0;
	m_nCntMove_Y = 0;
	m_bPlacement = false;
	m_bCreate = false;
	m_bMove = false;
	m_Status = PieceStatus_None;

	for (int nCntDepth = 0; nCntDepth < Piece_Depth; nCntDepth++)
	{
		for (int nCntWidth = 0; nCntWidth < Piece_Width; nCntWidth++)
		{
			m_bPuzzle[nCntDepth][nCntWidth] = false;

			m_pBlock[nCntDepth][nCntWidth] = CScene2D::Create(PRIORITY_UI);

			if (m_pBlock[nCntDepth][nCntWidth] != NULL)
			{
				m_pBlock[nCntDepth][nCntWidth]->SetPosition(D3DXVECTOR3(100.0f + nCntWidth * 55.0f,
					100.0f + nCntDepth * 55.0f,
					0.0f));
				m_pBlock[nCntDepth][nCntWidth]->SetSize(D3DXVECTOR3(50.0f, 50.0f, 0.0f));
				m_pBlock[nCntDepth][nCntWidth]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
				m_pBlock[nCntDepth][nCntWidth]->SetTransform();
				
			}
		}
	}
	return S_OK;
}

//==================================================================================================================
// �I������
//==================================================================================================================
void CPiece::Uninit(void)
{

}

//==================================================================================================================
// �X�V����
//==================================================================================================================
void CPiece::Update(void)
{
	// �L�[�{�[�h�擾
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();

	// �c���J�E���g
	for (int nDepth = 0; nDepth < Piece_Depth; nDepth++)
	{
		// �����J�E���g
		for (int nWide = 0; nWide < Piece_Width; nWide++)
		{
			// �|�W�V�����擾
			m_pos = m_pBlock[nDepth][nWide]->GetPosition();

			// �����z�u
			if (nDepth == m_nCntMove_Y && nWide == m_nCntMove_X)
			{
				m_bPuzzle[nDepth][nWide] = true;
				m_Status = PieceStatus_Plaacement;
			}
			else if (nDepth == m_nCntMove_Y && nWide == m_nCntMove_X + 1)
			{
				m_bPuzzle[nDepth][nWide] = true;
				m_Status = PieceStatus_Plaacement;
			}
			else if (nDepth == m_nCntMove_Y + 1 && nWide == m_nCntMove_X)
			{
				m_bPuzzle[nDepth][nWide] = true;
				m_Status = PieceStatus_Plaacement;
			}
			else if (nDepth == m_nCntMove_Y + 1 && nWide == m_nCntMove_X + 1)
			{
				m_bPuzzle[nDepth][nWide] = true;
				m_Status = PieceStatus_Plaacement;
			}
			else
			{
				m_bPuzzle[nDepth][nWide] = false;
				m_Status = PieceStatus_Not;
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
				}
			}
			else
			{
				// �F�ݒ�
				m_pBlock[nDepth][nWide]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
			}
		}
	}

	// �z�u���Ă��Ȃ�������
	if (m_bPlacement == false)
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
	if (m_bMove == true)
	{
		// ����
		if (pKeyboard->GetTriggerKeyboard(DIK_C))
		{
			m_bCreate = true;
			m_bMove = false;
		}
	}

	// �g�O�ɍs���Ȃ��悤�ɂ���
	if (m_nCntMove_X <= 0)
	{
		m_nCntMove_X = 0;
	}
	else if (m_nCntMove_X >= Range_X)
	{
		m_nCntMove_X = Range_X;
	}

	if (m_nCntMove_Y <= 0)
	{
		m_nCntMove_Y = 0;
	}
	else if (m_nCntMove_Y >= Range_Y)
	{
		m_nCntMove_Y = Range_Y;
	}
}

//==================================================================================================================
// �`�揈��
//==================================================================================================================
void CPiece::Draw(void)
{

}

//==================================================================================================================
// �|���S������
//==================================================================================================================
CPiece *CPiece::Create(void)
{
	// �V�[�����I�Ɋm��
	CPiece *pPiece = new CPiece(CScene::PRIORITY_FLOOR);

	if (pPiece != NULL)
	{
		// �V�[��������
		pPiece->Init();
	}

	// �l��Ԃ�
	return pPiece;
}

//==================================================================================================================
// �e�N�X�`����񃍁[�h
//==================================================================================================================
HRESULT CPiece::Load(void)
{
	return S_OK;
}

//==================================================================================================================
// �e�N�X�`�����j��
//==================================================================================================================
void CPiece::Unload(void)
{
	// �e�N�X�`���̊J��

}
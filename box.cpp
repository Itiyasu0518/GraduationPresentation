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
#include "piece.h"

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
bool CBox::m_bPuzzle[Box_Depth][Box_Width] = {};

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
	m_nCntChange = 0;
	m_nPieceNum = 0;
	m_nSelect = 0;
	m_bPlacement = false;
	m_bRelease = false;
	m_bCreate = false;
	m_bMove = false;

	// �u���b�N�̏�����
	for (int nCntDepth = 0; nCntDepth < Box_Depth; nCntDepth++)
	{
		for (int nCntWidth = 0; nCntWidth < Box_Width; nCntWidth++)
		{
			// �p�Y��������
			m_bPuzzle[nCntDepth][nCntWidth] = false;
			// �i�[�p
			m_bPuzzleStorage[nCntDepth][nCntWidth] = false;
			// ����
			m_pBlock[nCntDepth][nCntWidth] = CScene2D::Create(PRIORITY_UI);

			if (m_pBlock[nCntDepth][nCntWidth] != NULL)
			{
				// �ʒu
				m_pBlock[nCntDepth][nCntWidth]->SetPosition(D3DXVECTOR3(100.0f + nCntWidth * 55.0f,
					100.0f + nCntDepth * 55.0f,
					0.0f));
				// �T�C�Y
				m_pBlock[nCntDepth][nCntWidth]->SetSize(D3DXVECTOR3(50.0f, 50.0f, 0.0f));
				// �F
				m_pBlock[nCntDepth][nCntWidth]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				// ���f
				m_pBlock[nCntDepth][nCntWidth]->SetTransform();
			}
		}
	}

	// �ŏ��̃s�[�X����
	m_pPiece[m_nPieceNum] = CPiece::Create();
	m_pPiece[m_nPieceNum]->SetPieceType(CPiece::PieceType_Square);


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

	if (m_pPiece[m_nPieceNum] != NULL)
	{
		// �z�u���擾
		m_bPiece = m_pPiece[m_nPieceNum]->GetMove();
	}

	if (m_bPiece == true)
	{
		// ����
		if (pKeyboard->GetTriggerKeyboard(DIK_C))
		{
			// �s�[�X�����Z
			m_nPieceNum++;
			// �s�[�X����
			m_pPiece[m_nPieceNum] = CPiece::Create();
			// �s�[�X�^�C�v�ݒ�
			m_pPiece[m_nPieceNum]->SetPieceType(CPiece::PieceType_Square);
			// �z�u���
			m_pPiece[m_nPieceNum]->SetMove(false);
		}

		// ����
		if (pKeyboard->GetTriggerKeyboard(DIK_V))
		{
			// �s�[�X�����Z
			m_nPieceNum++;
			// �s�[�X����
			m_pPiece[m_nPieceNum] = CPiece::Create();
			// �s�[�X�^�C�v�ݒ�
			m_pPiece[m_nPieceNum]->SetPieceType(CPiece::PieceType_Rectangle);
			// �z�u���
			m_pPiece[m_nPieceNum]->SetMove(false);
		}
		// ����
		if (pKeyboard->GetTriggerKeyboard(DIK_B))
		{
			// �s�[�X�����Z
			m_nPieceNum++;
			// �s�[�X����
			m_pPiece[m_nPieceNum] = CPiece::Create();
			// �s�[�X�^�C�v�ݒ�
			m_pPiece[m_nPieceNum]->SetPieceType(CPiece::PieceType_T_Type);
			// �z�u���
			m_pPiece[m_nPieceNum]->SetMove(false);
		}
		// ����
		if (pKeyboard->GetTriggerKeyboard(DIK_X))
		{
			// �s�[�X�����Z
			m_nPieceNum++;
			// �s�[�X����
			m_pPiece[m_nPieceNum] = CPiece::Create();
			// �s�[�X�^�C�v�ݒ�
			m_pPiece[m_nPieceNum]->SetPieceType(CPiece::PieceType_Delete);
			// �z�u���
			m_pPiece[m_nPieceNum]->SetMove(false);
		}

		
		// �㉺����
		if (pKeyboard->GetTriggerKeyboard(MOVE_ACCEL))
		{
			// �Z���N�g�J�E���g���Z
			m_nSelect++;
		}
		else if (pKeyboard->GetTriggerKeyboard(MOVE_BRAKE))
		{
			// �Z���N�g�J�E���g���Z
			m_nSelect--;
		}

		// �Z���N�g�J�E���g����
		if (m_nSelect >= m_nPieceNum)
		{
			m_nSelect = m_nPieceNum;
		}
		else if (m_nSelect <= 0)
		{
			m_nSelect = 0;
		}

		for (int nCntDepth = 0; nCntDepth < Box_Depth; nCntDepth++)
		{
			for (int nCntWidth = 0; nCntWidth < Box_Width; nCntWidth++)
			{
				// �I������Ă���Ƃ��̐F
				m_pPiece[m_nSelect]->SetCol(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));

			
					if (m_nSelect != m_nPieceNum)
					{
						m_pPiece[m_nSelect + 1]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
					}
					if (m_nSelect != 0)
					{
						m_pPiece[m_nSelect - 1]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
					}
				

				// Q�������ꂽ��
				if (pKeyboard->GetTriggerKeyboard(DIK_Q))
				{
					if (m_pPiece[m_nSelect] != NULL)
					{
						// �I�΂�Ă�s�[�X�̏��i�[
						m_bPuzzleStorage[nCntDepth][nCntWidth] = m_pPiece[m_nSelect]->GetPuzzle(nCntDepth, nCntWidth);
						// ��Ԕ�r
						if (m_bPuzzle[nCntDepth][nCntWidth] == true && m_bPuzzleStorage[nCntDepth][nCntWidth] == true)
						{
							// ��ԏ�����
							m_bPuzzle[nCntDepth][nCntWidth] = false;
						}
						// �s�[�X��ԕύX
						m_pPiece[m_nSelect]->SetRelease(true);
						// ��ԕύX
						m_bRelease = true;
					}
				}
			}
		}
		if (m_bRelease == true)
		{
			m_nSelect = 0;
			m_bRelease = false;
		}
		// �s�[�X����
		
		//m_pPiece[m_nPieceNum]->SetPlaacement(false);
	}
	else
	{
		// Z�������ꂽ��
		if (pKeyboard->GetTriggerKeyboard(DIK_Z))
		{
			// �`�F���W
			if (m_nCntChange == 0)
			{
				// �^�C�v�ύX
				m_pPiece[m_nPieceNum]->SetPieceType(CPiece::PieceType_Square);
				// �J�E���g���Z
				m_nCntChange++;
			}
			else if (m_nCntChange == 1)
			{
				// �^�C�v�ύX
				m_pPiece[m_nPieceNum]->SetPieceType(CPiece::PieceType_Rectangle);
				// �J�E���g���Z
				m_nCntChange++;
			}
			else if (m_nCntChange == 2)
			{
				// �^�C�v�ύX
				m_pPiece[m_nPieceNum]->SetPieceType(CPiece::PieceType_T_Type);
				// �J�E���g���Z
				m_nCntChange++;
			}
			else if (m_nCntChange == 3)
			{
				// �^�C�v�ύX
				m_pPiece[m_nPieceNum]->SetPieceType(CPiece::PieceType_Delete);
				// �J�E���g������
				m_nCntChange = 0;
			}
		}
	}
	for (int nCntDepth = 0; nCntDepth < Box_Depth; nCntDepth++)
	{
		for (int nCntWidth = 0; nCntWidth < Box_Width; nCntWidth++)
		{
			if (m_pPiece[m_nPieceNum] != NULL)
			{
				// �z�u���擾
				m_bPlacement = m_pPiece[m_nPieceNum]->GetPlaacement();
				// �ݒu����Ă���
				if (m_bPlacement == true)
				{
					// ���i�[
					m_bPuzzleStorage[nCntDepth][nCntWidth] = m_pPiece[m_nPieceNum]->GetPuzzle(nCntDepth, nCntWidth);
				}
			}
			if (m_bPuzzle[nCntDepth][nCntWidth] == false && m_bPuzzleStorage[nCntDepth][nCntWidth] == true)
			{
				// �z�u
				m_bPuzzle[nCntDepth][nCntWidth] = true;
			}
		}
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
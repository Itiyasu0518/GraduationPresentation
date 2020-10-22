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
	m_nCntMove_X = 0;
	m_nCntMove_Y = 0;
	m_nPieceNum = 0;
	m_bPlacement = false;
	m_bCreate = false;
	m_bMove = false;

	for (int nCntDepth = 0; nCntDepth < Box_Depth; nCntDepth++)
	{
		for (int nCntWidth = 0; nCntWidth < Box_Width; nCntWidth++)
		{
			m_bPuzzle[nCntDepth][nCntWidth] = false;
			m_bPuzzleStorage[nCntDepth][nCntWidth] = false;
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
			m_nPieceNum++;
			m_pPiece[m_nPieceNum] = CPiece::Create();
			m_pPiece[m_nPieceNum]->SetPieceType(CPiece::PieceType_Square);
			// �z�u���
			m_pPiece[m_nPieceNum]->SetMove(false);
		}

		// ����
		if (pKeyboard->GetTriggerKeyboard(DIK_V))
		{
			m_nPieceNum++;
			m_pPiece[m_nPieceNum] = CPiece::Create();
			m_pPiece[m_nPieceNum]->SetPieceType(CPiece::PieceType_Rectangle);
			// �z�u���
			m_pPiece[m_nPieceNum]->SetMove(false);
		}
		// ����
		if (pKeyboard->GetTriggerKeyboard(DIK_B))
		{
			m_nPieceNum++;
			m_pPiece[m_nPieceNum] = CPiece::Create();
			m_pPiece[m_nPieceNum]->SetPieceType(CPiece::PieceType_T_Type);
			// �z�u���
			m_pPiece[m_nPieceNum]->SetMove(false);
		}
		// �s�[�X����
		
		//m_pPiece[m_nPieceNum]->SetPlaacement(false);
	}

	for (int nCntDepth = 0; nCntDepth < Box_Depth; nCntDepth++)
	{
		for (int nCntWidth = 0; nCntWidth < Box_Width; nCntWidth++)
		{
			if (m_pPiece[m_nPieceNum] != NULL)
			{
				m_bPlacement = m_pPiece[m_nPieceNum]->GetPlaacement();
				if (m_bPlacement == true)
				{
					m_bPuzzleStorage[nCntDepth][nCntWidth] = m_pPiece[m_nPieceNum]->GetPuzzle(nCntDepth, nCntWidth);
				}
			}
			if (m_bPuzzle[nCntDepth][nCntWidth] == false && m_bPuzzleStorage[nCntDepth][nCntWidth] == true)
			{
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
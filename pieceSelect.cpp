//=============================================================================
//
// �s�[�X�Z���N�g���� [pieceSelect.cpp]
// Author : Ryouma Inoue
//
//=============================================================================
#include "pieceSelect.h"
#include "manager.h"
#include "renderer.h"
#include "inputKeyboard.h"
#include "takaseiLibrary.h"
#include "box.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define	PIECE_01 100		// ��01�̃��f�����A�h���X
#define PIECE_FILE "data/text/piece.txt"

//==============================================================================
// �R���X�g���N�^
//==============================================================================
CPieceSelect::CPieceSelect(PRIORITY type = CScene::PRIORITY_UI) :CScene(type)
{
	SetObjType(PRIORITY_BG);	//�I�u�W�F�N�g�^�C�v�̐ݒ�

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CPieceSelect::~CPieceSelect()
{

}

//==============================================================================
// ����������
//==============================================================================
HRESULT CPieceSelect::Init(void)
{
	// ������
	m_nSelectCnt = 0;
	m_bPiece = false;

	for (int nCnt = 0; nCnt < MAX_CORE; nCnt++)
	{
		m_pPieceSelect[nCnt] = CScene2D::Create(PRIORITY_UI);
		m_bSelect[nCnt] = false;
	}

	LoadPiece();
	//SetPiece(PIECETYPE_CORE_00, D3DXVECTOR3(500.0f, 500.0f, 0.0f), D3DXVECTOR3(100.0f, 100.0f, 0.0f), "data/tex/core.png");

	return S_OK;
}

//=============================================================================
// �J������
//=============================================================================
void CPieceSelect::Uninit(void)
{

}

//=============================================================================
// �X�V����
//=============================================================================
void CPieceSelect::Update(void)
{
	// �L�[�{�[�h�擾
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();

	m_bPiece = CBox::GetPiece();

	if (m_bPiece == false)
	{
		// Z�������ꂽ��
		if (pKeyboard->GetTriggerKeyboard(DIK_Z))
		{
			switch (m_nSelectCnt)
			{
			case 0:
				m_bSelect[PIECETYPE_CORE_16] = false;
				m_bSelect[PIECETYPE_CORE_01] = true;
				m_nSelectCnt++;
				break;
			case 1:
				m_bSelect[PIECETYPE_CORE_01] = false;
				m_bSelect[PIECETYPE_CORE_04] = true;
				m_nSelectCnt++;
				break;
			case 2:
				m_bSelect[PIECETYPE_CORE_04] = false;
				m_bSelect[PIECETYPE_CORE_12] = true;
				m_nSelectCnt++;
				break;
			case 3:
				m_bSelect[PIECETYPE_CORE_12] = false;
				m_bSelect[PIECETYPE_CORE_00] = true;
				m_nSelectCnt++;
				break;
			case 4:
				m_bSelect[PIECETYPE_CORE_00] = false;
				m_bSelect[PIECETYPE_CORE_05] = true;
				m_nSelectCnt++;
				break;
			case 5:
				m_bSelect[PIECETYPE_CORE_05] = false;
				m_bSelect[PIECETYPE_CORE_08] = true;
				m_nSelectCnt++;
				break;
			case 6:
				m_bSelect[PIECETYPE_CORE_08] = false;
				m_bSelect[PIECETYPE_CORE_14] = true;
				m_nSelectCnt++;
				break;
			case 7:
				m_bSelect[PIECETYPE_CORE_14] = false;
				m_bSelect[PIECETYPE_CORE_15] = true;
				m_nSelectCnt++;
				break;
			case 8:
				m_bSelect[PIECETYPE_CORE_15] = false;
				m_bSelect[PIECETYPE_CORE_16] = true;
				m_nSelectCnt = 0;
				break;
			}
		}
	}

	for (int nCnt = 0; nCnt < MAX_CORE; nCnt++)
	{
		if (m_bSelect[nCnt] == true)
		{
			m_pPieceSelect[nCnt]->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		}
		else
		{
			m_pPieceSelect[nCnt]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		}
	}

}

//=============================================================================
// �`�揈��
//=============================================================================
void CPieceSelect::Draw(void)
{

}

//==============================================================================
// ��������
//==============================================================================
CPieceSelect *CPieceSelect::Create(void)
{
	CPieceSelect *pPieceSelect;		// �w�i�̃|�C���^

	pPieceSelect = new CPieceSelect(CScene::PRIORITY_BG);		// �w�i�̐���
	pPieceSelect->Init();							// �w�i�̏�����
	return pPieceSelect;
}

//==============================================================================
// �A�Z�b�g�̐�������
//==============================================================================
HRESULT CPieceSelect::Load(void)
{
	//�e�N�X�`���̓ǂݍ���
	CManager::Load("data/tex/back.png");
	return S_OK;
}

//==============================================================================
// �s�[�X�̐ݒ�
//==============================================================================
void CPieceSelect::SetPiece(PIECETYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 size, std::string Add)
{
	m_pPieceSelect[type]->BindTexture(Add);									// �e�N�X�`���̃|�C���^��n��
	m_pPieceSelect[type]->SetCenter(CScene2D::TEXTUREVTX_CENTER);			// ���S�̐ݒ�
	m_pPieceSelect[type]->SetSize(D3DXVECTOR3(size.x, size.y, 0.0f));		// �傫���̐ݒ�
	m_pPieceSelect[type]->SetPosition(D3DXVECTOR3(pos.x, pos.y, 0.0f));		// �ʒu�̐ݒ�
	m_pPieceSelect[type]->SetTransform();
}

//==============================================================================
// �s�[�X�̃��[�h
//==============================================================================
void CPieceSelect::LoadPiece(void)
{
	FILE *pFile = NULL;																// �t�@�C��
	char cReadText[128];															// ����
	char cHeadText[128];															// ��r
	char cDie[128];
	D3DXVECTOR3 pos;
	D3DXVECTOR3 size;
	char aAdd[64];
	int type;

	pFile = fopen(PIECE_FILE, "r");				// �t�@�C�����J���܂��͍��

	if (pFile != NULL)						//�t�@�C�����ǂݍ��߂��ꍇ
	{
		//�X�N���v�g������܂Ń��[�v
		while (strcmp(cHeadText, "SCRIPT") != 0)
		{
			fgets(cReadText, sizeof(cReadText), pFile);
			sscanf(cReadText, "%s", &cHeadText);
		}

		//�X�N���v�g��������
		if (strcmp(cHeadText, "SCRIPT") == 0)
		{
			//�G���h�X�N���v�g������܂�
			while (strcmp(cHeadText, "END_SCRIPT") != 0)
			{
				fgets(cReadText, sizeof(cReadText), pFile);
				sscanf(cReadText, "%s", &cHeadText);

				//���s
				if (strcmp(cReadText, "\n") != 0)
				{
					if (strcmp(cHeadText, "PIECESET") == 0)
					{//�L�����N�^�[�̏������̂Ƃ�

							//�G���h�L�����N�^�[�Z�b�g������܂Ń��[�v
						while (strcmp(cHeadText, "END_PIECESET") != 0)
						{
							fgets(cReadText, sizeof(cReadText), pFile);
							sscanf(cReadText, "%s", &cHeadText);

							
							if (strcmp(cHeadText, "POS") == 0)
							{//�p�[�c�����̂Ƃ�
								sscanf(cReadText, "%s %s %f %f %f", &cDie, &cDie,
									&pos.x,
									&pos.y,
									&pos.z);
							}
							else if (strcmp(cHeadText, "TYPE") == 0)
							{//�p�[�c�����̂Ƃ�
								sscanf(cReadText, "%s %s %d", &cDie, &cDie,
									&type);
							}
							else if (strcmp(cHeadText, "SIZE") == 0)
							{//�p�[�c�����̂Ƃ�
								sscanf(cReadText, "%s %s %f %f %f", &cDie, &cDie,
									&size.x,
									&size.y,
									&size.z);

							}
							else if (strcmp(cHeadText, "TEX") == 0)
							{//�p�[�c�����̂Ƃ�
								memset(&aAdd, 0, sizeof(aAdd));

								sscanf(cReadText, "%s %s %s", &cDie, &cDie,
									&aAdd);
							}
						}
						SetPiece((PIECETYPE)type, pos, size, aAdd);
					}
				}

			}
		}
		fclose(pFile);				// �t�@�C�������
	}
	else
	{
		MessageBox(NULL, "���̃A�N�Z�X���s�I", "WARNING", MB_ICONWARNING);	// ���b�Z�[�W�{�b�N�X�̐���
	}
}
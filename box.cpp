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
CBox *CBox::m_pBox = NULL;							// ���b�V���t�B�[���h�̏��
CPlayer *CBox::m_pPlayer = NULL;					// �v���C���[�̏��

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
	CRenderer *pRenderer = CManager::GetRenderer();						// �����_���[���擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();					// �f�o�C�X�̎擾

																		// �ʒu�E��]�̏����ݒ�
	m_pos = D3DXVECTOR3(100.0f, 100.0f, 0.0f);								// �ʒu
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);								// ��]
	m_nNumVertex = ((Box_Width + 1) * Box_Depth * 2) + (Box_Depth - 1) * 2;			// �����_��
	m_nNumIndex = (Box_Depth + 1) * (Box_Width + 1);					// ���C���f�b�N�X��
	m_nNumPolygon = m_nNumVertex - 2;									// �O�p�`�̐�
	m_aVecA[Box_Depth * Box_Width] = {};								// �@���x�N�g����ʂ̐����ꎞ�I�Ɋi�[
	m_aVecB[Box_Depth * Box_Width] = {};								// �@���x�N�g����ʂ̐����ꎞ�I�Ɋi�[
	m_vectorA = D3DXVECTOR3(0, 0, 0);									// ��ڂ̊O�ϗp�ϐ�
	m_vectorB = D3DXVECTOR3(0, 0, 0);									// ��ڂ̊O�ϗp�ϐ�
	m_vectorC = D3DXVECTOR3(0, 0, 0);									// �O�ڂ̊O�ϗp�ϐ�
	nNumber = 0;														// �z��̔ԍ�
	StartBox = Box_Width + 1;												// �n�܂锠
	EndBox = 0;															// ������锠
	fDivide = 0;														// sin�̒��g������ϐ�

																		// ���_���̍쐬
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * m_nNumIndex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	//�C���f�b�N�X�o�b�t�@�𐶐�
	pDevice->CreateIndexBuffer(
		sizeof(WORD) * m_nNumVertex,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^�擾
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVtx, 0);

	// �c���J�E���g
	for (int nDepth = 0; nDepth < Box_Depth + 1; nDepth++)
	{
		// �����J�E���g
		for (int nWide = 0; nWide < Box_Width + 1; nWide++)
		{
			///////////////////////////////////////////////�i�~�i�~�`�`///////////////////////////////////////////////
			// ���_���W�̐ݒ�
			//m_pVtx[0].pos = D3DXVECTOR3((-WhileX * Width) / 2 + WhileX * nWide, cosf(D3DX_PI / 6 * nDepth + fDivide) * WhileY, (WhileZ / 2 * Depth) - WhileZ * nDepth);

			//// ���_���W�̐ݒ�
			//m_pVtx[0].pos.x = (-WhileX * Box_Width) / 2 + WhileX * nWide;
			//m_pVtx[0].pos.y = (WhileZ / 2 * Box_Depth) - WhileZ * nDepth;
			//m_pVtx[0].pos.z = 0.0f;
			// ���W�̐ݒ�
			m_pVtx[0].pos.x = m_pos.x + WhileX * nWide;
			m_pVtx[0].pos.y = m_pos.y + WhileZ * nDepth;
			m_pVtx[0].pos.z = 0.0f;

			// 1.0�ŌŒ�A�������W
			m_pVtx[0].rhw = 1.0f;


			// ���_�J���[
			m_pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// �e�N�X�`���`�ʂ̈ʒu
			m_pVtx[0].tex = D3DXVECTOR2(1.0f * nWide, 1.0f * nDepth);

			m_pVtx++;
		}
	}

	// ���_�f�[�^���A�����b�N
	m_pVtxBuff->Unlock();

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^�擾
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVtx, 0);

	//////////////////////////////�@���x�N�g��//////////////////////////////
	// �c���J�E���g
	for (int nDepth = 0; nDepth < Box_Depth + 1; nDepth++)
	{
		// �����J�E���g
		for (int nWide = 0; nWide < Box_Width + 1; nWide++)
		{
			// �C���f�b�N�X�ԍ�
			int nIdx = nDepth * (Box_Width + 1) + nWide;

			// �[�͌v�Z�����Ȃ�
			if (nWide != Box_Width && nDepth != Box_Depth)
			{
				m_vectorA = m_pVtx[StartBox].pos - m_pVtx[EndBox].pos;										// ��ڂ̃x�N�g��
				m_vectorB = m_pVtx[StartBox + 1].pos - m_pVtx[EndBox].pos;									// ��ڂ̃x�N�g��
				m_vectorC = m_pVtx[EndBox + 1].pos - m_pVtx[EndBox].pos;									// �O�ڂ̃x�N�g��
				D3DXVec3Cross(&m_aVecA[nNumber], &m_vectorA, &m_vectorB);									// ��̃x�N�g���̒����x�N�g��
				D3DXVec3Normalize(&m_aVecA[nNumber], &m_aVecA[nNumber]);									// ���K������
				D3DXVec3Cross(&m_aVecB[nNumber], &m_vectorB, &m_vectorC);									// ��̃x�N�g���̒����x�N�g��
				D3DXVec3Normalize(&m_aVecB[nNumber], &m_aVecB[nNumber]);									// ���K������

																											// �z��̔ԍ�1�v���X����
				StartBox++;
				EndBox++;
				nNumber++;
			}

		}
		// �z��̔ԍ�1�v���X����
		StartBox++;
		EndBox++;
	}

	// ���_�f�[�^���A�����b�N
	m_pVtxBuff->Unlock();

	WORD *pIdx;				// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^

							// �C���f�b�N�X�o�b�t�@�����b�N���C���f�b�N�X�f�[�^�ւ̃|�C���^���擾
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	// �c���J�E���g
	for (int nCntY = 0; nCntY < Box_Depth; nCntY++)
	{
		// ��s�ڂ���Ȃ��Ƃ�
		if (nCntY != 0)
		{
			pIdx[0] = (Box_Width + 1) + nCntY * (Box_Width + 1);
			pIdx++;
		}
		// �����J�E���g
		for (int nCntX = 0; nCntX < Box_Width + 1; nCntX++)
		{
			pIdx[0] = (Box_Width + 1) + nCntX + nCntY * (Box_Width + 1);
			pIdx[1] = nCntX + nCntY * (Box_Width + 1);
			pIdx += 2;
		}

		// �k�ރ|���S��
		if (nCntY + 1 < Box_Depth)
		{
			pIdx[0] = Box_Width + nCntY * (Box_Width + 1);
			pIdx++;
		}
	}

	// �C���f�b�N�X�o�b�t�@���A�����b�N����
	m_pIdxBuff->Unlock();

	return S_OK;
}

//==================================================================================================================
// �I������
//==================================================================================================================
void CBox::Uninit(void)
{
	// ���_�o�b�t�@�̊J��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// �C���f�b�N�X�o�b�t�@�̊J��
	if (m_pIdxBuff != NULL)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}
}

//==================================================================================================================
// �X�V����
//==================================================================================================================
void CBox::Update(void)
{
	VERTEX_2D *m_pVtx;

	// �L�[�{�[�h�擾
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

	// �ď�����
	nNumber = 0;										// �z��̔ԍ�
	StartBox = Box_Width + 1;							// �n�܂锠
	EndBox = 0;											// ������锠

	// ������
	fDivide -= 0.05f;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^�擾
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVtx, 0);

	// �c���J�E���g
	for (int nDepth = 0; nDepth < Box_Depth + 1; nDepth++)
	{
		// �����J�E���g
		for (int nWide = 0; nWide < Box_Width + 1; nWide++)
		{
			///////////////////////////////////////////////�i�~�i�~�`�`///////////////////////////////////////////////
			// ���_���W�̐ݒ�
			//m_pVtx[0].pos = D3DXVECTOR3((-WhileX * Box_Width) / 2 + WhileX * nWide,
			//	(sinf(D3DX_PI / 6 * nDepth + fDivide) + 1) / 2 * WhileY,
			//	(WhileZ / 2 * Box_Depth) - WhileZ * nDepth);

			/////////////////////////////////////����///////////////////////////////////////////////
		/*	 ���W�̐ݒ�
			m_pVtx[0].pos.x = WhileX * nWide;
			m_pVtx[0].pos.y = WhileZ * nDepth;
			m_pVtx[0].pos.z = 0.0f;
*/
			// ���_�J���[
			m_pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// �e�N�X�`���`�ʂ̈ʒu
			m_pVtx[0].tex = D3DXVECTOR2(1.0f * nWide, 1.0f * nDepth);

			m_pVtx++;
		}
	}

	// ���_�f�[�^���A�����b�N
	m_pVtxBuff->Unlock();

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^�擾
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVtx, 0);

	//////////////////////////////�@���x�N�g��//////////////////////////////
	// �c���J�E���g
	for (int nDepth = 0; nDepth < Box_Depth + 1; nDepth++)
	{
		// �����J�E���g
		for (int nWide = 0; nWide < Box_Width + 1; nWide++)
		{
			// �C���f�b�N�X�ԍ�
			int nIdx = nDepth * (Box_Width + 1) + nWide;

			// �[�͌v�Z�����Ȃ�
			if (nWide != Box_Width && nDepth != Box_Depth)
			{
				m_vectorA = m_pVtx[StartBox].pos - m_pVtx[EndBox].pos;										// ��ڂ̃x�N�g��
				m_vectorB = m_pVtx[StartBox + 1].pos - m_pVtx[EndBox].pos;									// ��ڂ̃x�N�g��
				m_vectorC = m_pVtx[EndBox + 1].pos - m_pVtx[EndBox].pos;									// �O�ڂ̃x�N�g��
				D3DXVec3Cross(&m_aVecA[nNumber], &m_vectorA, &m_vectorB);									// ��̃x�N�g���̒����x�N�g��
				D3DXVec3Normalize(&m_aVecA[nNumber], &m_aVecA[nNumber]);									// ���K������
				D3DXVec3Cross(&m_aVecB[nNumber], &m_vectorB, &m_vectorC);									// ��̃x�N�g���̒����x�N�g��
				D3DXVec3Normalize(&m_aVecB[nNumber], &m_aVecB[nNumber]);									// ���K������

																											// �z��̔ԍ�1�v���X����
				StartBox++;
				EndBox++;
				nNumber++;
			}
		}
		// �z��̔ԍ���1�v���X����
		StartBox++;
		EndBox++;
	}

	// ���_�f�[�^���A�����b�N
	m_pVtxBuff->Unlock();
}

//==================================================================================================================
// �`�揈��
//==================================================================================================================
void CBox::Draw(void)
{
	// �����_���[���擾
	CRenderer *pRenderer = CManager::GetRenderer();			// �����_���[�̏��擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();		// �f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;							// �v�Z�p�}�g���b�N�X

															// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(m_pIdxBuff);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	// �|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_nNumVertex, 0, m_nNumPolygon);
}

//==================================================================================================================
// �ʒu�ݒ�
//==================================================================================================================
void CBox::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//==================================================================================================================
// �|���S������
//==================================================================================================================
CBox *CBox::Create(void)
{
	// �V�[�����I�Ɋm��
	m_pBox = new CBox(CScene::PRIORITY_FLOOR);

	if (m_pBox != NULL)
	{
		// �V�[��������
		m_pBox->Init();
	}

	// �l��Ԃ�
	return m_pBox;
}

//==================================================================================================================
// �e�N�X�`����񃍁[�h
//==================================================================================================================
HRESULT CBox::Load(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();						// �����_���[�̏��擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();					// �f�o�C�X���擾����

																		// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,									// �f�o�C�X�ւ̃|�C���^
		"data/TEXTURE/water.jpg",										// �t�@�C���̖��O
		&m_pTexture);													// �ǂݍ��ރ������[

	return S_OK;
}

//==================================================================================================================
// �e�N�X�`�����j��
//==================================================================================================================
void CBox::Unload(void)
{
	// �e�N�X�`���̊J��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//==================================================================================================================
// ���b�V���t�B�[���h���擾
//==================================================================================================================
CBox * CBox::GetBox(void)
{
	return m_pBox;
}

//==================================================================================================================
// �ʒu�擾
//==================================================================================================================
D3DXVECTOR3 CBox::GetPos(void)
{
	return m_pos;
}

//==================================================================================================================
// �ړ��ʎ擾
//==================================================================================================================
D3DXVECTOR3 CBox::GetMove(void)
{
	return m_move;
}

//==================================================================================================================
// �����Z�o�p�֐�
//==================================================================================================================
float CBox::GetHeight(D3DXVECTOR3 pos)
{
	float Height = 0.0f;
	bool bRange = false;

	// �ď�����
	nNumber = 0;										// �z��̔ԍ�
	StartBox = Box_Width + 1;								// �n�܂锠
	EndBox = 0;											// ������锠

														// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^�擾
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVtx, 0);

	//�c���J�E���g
	for (int nDepth = 0; nDepth < Box_Depth + 1; nDepth++)
	{
		// �����J�E���g
		for (int nWide = 0; nWide < Box_Width + 1; nWide++)
		{
			// �͈͊O�ɂ���Ƃ�
			if (!bRange)
			{
				// �[�͌v�Z�����Ȃ�
				if (nWide != Box_Width && nDepth != Box_Depth)
				{
					// �S�Ẵx�N�g�����o��
					D3DXVECTOR3 vecA = m_pVtx[StartBox].pos - m_pVtx[EndBox].pos;
					D3DXVECTOR3 vecB = m_pVtx[StartBox + 1].pos - m_pVtx[EndBox].pos;
					D3DXVECTOR3 vecC = m_pVtx[StartBox + 1].pos - m_pVtx[StartBox].pos;
					D3DXVECTOR3 vecD = m_pVtx[EndBox + 1].pos - m_pVtx[EndBox].pos;
					D3DXVECTOR3 vecE = m_pVtx[StartBox + 1].pos - m_pVtx[EndBox + 1].pos;

					// ������pos���璸�_�܂ł��x�N�g���ɂ���
					D3DXVECTOR3 vec0 = pos - m_pVtx[EndBox].pos;
					D3DXVECTOR3 vec1 = pos - m_pVtx[EndBox + 1].pos;
					D3DXVECTOR3 vec2 = pos - m_pVtx[StartBox].pos;
					D3DXVECTOR3 vec3 = pos - m_pVtx[StartBox + 1].pos;

					// �����x�N�g���p�ϐ�
					D3DXVECTOR3 norB = D3DXVECTOR3(0, 0, 0);
					D3DXVECTOR3 norA = D3DXVECTOR3(0, 0, 0);
					D3DXVECTOR3 norC = D3DXVECTOR3(0, 0, 0);
					D3DXVECTOR3 norD = D3DXVECTOR3(0, 0, 0);
					D3DXVECTOR3 norE = D3DXVECTOR3(0, 0, 0);
					D3DXVECTOR3 norF = D3DXVECTOR3(0, 0, 0);

					//////�����x�N�g���Z�o//////
					// ��ڂ̎O�p�`
					D3DXVec3Cross(&norA, &vecA, &vec0);										// �����x�N�g���Z�o
					D3DXVec3Cross(&norB, &vec0, &vecB);										// �����x�N�g���Z�o
					D3DXVec3Cross(&norC, &vecC, &vec2);										// �����x�N�g���Z�o

																							// ��ڂ̎O�p�`
					D3DXVec3Cross(&norD, &vecB, &vec0);										// �����x�N�g���Z�o
					D3DXVec3Cross(&norE, &vec0, &vecD);										// �����x�N�g���Z�o
					D3DXVec3Cross(&norF, &vec1, &vecE);										// �����x�N�g���Z�o

																							// �@�����ʂ̕\���ゾ�����Ƃ�
					if (m_aVecA[nNumber] >= 0)
					{
						// �O�p�`�̒��ɂ���Ƃ�
						if (norA.y <= 0 && norB.y <= 0 && norC.y <= 0)
						{
							// ���b�V���t�B�[���h�̒��ɂ���
							bRange = true;

							// ������pos�����_�Ɠ����ł���Ƃ�
							if (pos == m_pVtx[StartBox].pos)
							{
								// �ʂȒ��_���g���č������Z�o
								// �w�肳�ꂽ�ʒu�ł̍������Z�o
								Height = m_pVtx[StartBox + 1].pos.y + (-m_aVecA[nNumber].x * vec3.x - m_aVecA[nNumber].z * vec3.z) / m_aVecA[nNumber].y;
							}
							else
							{
								// �w�肳�ꂽ�ʒu�ł̍������Z�o
								Height = m_pVtx[StartBox].pos.y + (-m_aVecA[nNumber].x * vec2.x - m_aVecA[nNumber].z * vec2.z) / m_aVecA[nNumber].y;
							}
						}
					}

					// �@�����ʂ̕\���ゾ�����Ƃ�
					if (m_aVecB[nNumber] >= 0)
					{
						// �O�p�`�̒��ɂ���Ƃ�
						if (norD.y <= 0 && norE.y <= 0 && norF.y <= 0)
						{
							// ���b�V���t�B�[���h�̒��ɂ���
							bRange = true;

							// ������pos�����_�Ɠ����ł���Ƃ�
							if (pos == m_pVtx[EndBox + 1].pos)
							{
								// �ʂȒ��_���g���č������Z�o
								// �w�肳�ꂽ�ʒu�ł̍������Z�o
								Height = m_pVtx[EndBox].pos.y + (-m_aVecB[nNumber].x * (vec0.x) - m_aVecB[nNumber].z * (vec0.z)) / m_aVecB[nNumber].y;
							}
							else
							{
								// �w�肳�ꂽ�ʒu�ł̍������Z�o
								Height = m_pVtx[EndBox + 1].pos.y + (-m_aVecB[nNumber].x * (vec1.x) - m_aVecB[nNumber].z * (vec1.z)) / m_aVecB[nNumber].y;
							}
						}
					}

					// �z��̔ԍ�1�v���X����
					StartBox++;
					EndBox++;
					nNumber++;
				}
			}
		}
		// ���I�������z��̔ԍ���1�v���X����
		StartBox++;
		EndBox++;
	}

	// ���_�f�[�^���A�����b�N
	m_pVtxBuff->Unlock();

	return Height;
}

//==================================================================================================================
// �ړ��ʐݒ�
//==================================================================================================================
void CBox::SetMove(D3DXVECTOR3 move)
{
	m_move = move;
}


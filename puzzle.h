//=============================================================================
//
// �p�Y������ [puzzle.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _PUZZLE_H_
#define _PUZZLE_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"
#include "box.h"

//=============================================================================
// �O���錾
//=============================================================================
class CBox;

//=============================================================================
// �N���X��`
//=============================================================================
class CPuzzle
{
public:
	CPuzzle();					// �R���X�g���N�^
	~CPuzzle();					// �f�X�g���N�^
	HRESULT Init(void);			// ����������
	void Uninit(void);			// �J������
	void Update(void);			// �X�V����
	void Draw(void);			// �`�揈��

	static void	LoadAsset(void);												// �A�Z�b�g�̓ǂݍ��ݏ���
	static float GetSpeed(int nPieceNum) { return m_fSpeed[nPieceNum]; }		// �X�s�[�h�̎擾
	static int GetPieceNum(void) { return m_nPieceNum; }						// �s�[�X��

private:
	CBox *m_pBox;								// �{�b�N�X�̃|�C���^

	static float m_fSpeed[Piece_Num];			// �X�s�[�h
	static int m_nPieceNum;						// �s�[�X��

};
#endif
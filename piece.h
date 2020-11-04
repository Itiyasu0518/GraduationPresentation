//==================================================================================================================
//
// �s�[�X[piece.h]
// Author:Ryouma Inoue
//
//==================================================================================================================
#ifndef _PIECE_H_
#define _PIECE_H_

//==================================================================================================================
// �C���N���[�h�t�@�C��
//==================================================================================================================
#include "main.h"
#include "scene.h"

//==================================================================================================================
// �}�N����`
//==================================================================================================================
#define Piece_Depth 8																// �c�̃|���S����
#define Piece_Width 15																// ���̃u���b�N��

//==================================================================================================================
// �O���錾
//==================================================================================================================
class CPlayer;
class CScene2D;

//==================================================================================================================
//
// �N���X�̒�`
//
//==================================================================================================================
class CPiece : public CScene
{
public:
	enum PieceType
	{
		PieceType_None = 0,
		PieceType_Square,
		PieceType_Rectangle,
		PieceType_T_Type,
		PieceType_L_Type,
		PieceType_Square_1,
		PieceType_Rectangle_1,
		PieceType_Rectangle_2,
		PieceType_Speed,
		PieceType_Speed_1,
		PieceType_Diagonal,
		PieceType_Max,
	};

	CPiece(PRIORITY type);																			// �R���X�g���N�^
	~CPiece();																						// �f�X�g���N�^
	HRESULT Init(void);																				// ����������
	void Uninit(void);																				// �I������
	void Update(void);																				// �X�V����
	void Draw(void);																				// �`�揈��

	void OnTriggerEnter(CCollider *col) {};
	void OnCollisionEnter(CCollider *col) {};

	bool GetCreate(void) { return m_bCreate; }														// �z�u���擾
	bool GetMove(void) { return m_bMove; }															// �ړ����擾
	bool GetPlaacement(void) { return m_bPlacement; }												// �z�u���擾
	bool GetPuzzle(int nDepth, int nWidth) { return m_bPuzzle[nDepth][nWidth]; }					// �p�Y�����擾

	void SetPieceType(PieceType type);																// �^�C�v�ݒ�
	void SetPiece(void);																			// �s�[�X�ݒ�
	void SetMove(bool bMove) { m_bMove = bMove; }													// �ړ�
	void SetRelease(bool bRelease) { m_bRelease = bRelease; }										// �������ǂ���
	void SetCol(D3DXCOLOR col) { m_col = col; }														// �J���[�ݒ�
	static CPiece *Create(void);																	// ��������
	static HRESULT Load(void);																		// �e�N�X�`����񃍁[�h
	static void Unload(void);																		// �e�N�X�`�����A�����[�h

protected:

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;															// �e�N�X�`���ւ̃|�C���^
	bool m_bPuzzle[Piece_Depth][Piece_Width];														// �g�p���Ă��邩�ǂ���
	CScene2D *m_pBlock[Piece_Depth][Piece_Width];													// �V�[��2D�̃|�C���^

	D3DXVECTOR3 m_pos;																				// �ʒu
	D3DXCOLOR m_col;																				// �J���[

	int m_nCntMove_X;																				// �ړ��J�E���gX
	int m_nCntMove_Y;																				// �ړ��J�E���gY
	PieceType m_PieceType;																			// �s�[�X�̏��
	bool m_bPlacement;																				// �z�u���Ă��邩�ǂ���
	bool m_bCreate;																					// �������邩�ǂ���
	bool m_bMove;																					// �������ǂ���
	bool m_bBox[Piece_Depth][Piece_Width];															// �{�b�N�X�̏��
	bool m_bPut;																					// �u��
	bool m_bRelease;																				// ����
};
#endif
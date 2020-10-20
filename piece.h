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

	CPiece(PRIORITY type);															// �R���X�g���N�^
	~CPiece();																		// �f�X�g���N�^
	HRESULT Init(void);																// ����������
	void Uninit(void);																// �I������
	void Update(void);																// �X�V����
	void Draw(void);																// �`�揈��

	void OnTriggerEnter(CCollider *col) {};
	void OnCollisionEnter(CCollider *col) {};
	bool GetPlaacement(void) { return m_bCreate; }							// �z�u���擾

	void SetPlaacement(bool bCreate) { m_bCreate = bCreate; }
	static CPiece *Create(void);													// ��������
	static HRESULT Load(void);														// �e�N�X�`����񃍁[�h
	static void Unload(void);														// �e�N�X�`�����A�����[�h

protected:

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;											// �e�N�X�`���ւ̃|�C���^
	bool m_bPuzzle[Piece_Depth][Piece_Width];										// �g�p���Ă��邩�ǂ���
	CScene2D *m_pBlock[Piece_Depth][Piece_Width];									// �V�[��2D�̃|�C���^

	D3DXVECTOR3 m_pos;																// �ʒu

	int m_nCntMove_X;																// �ړ��J�E���gX
	int m_nCntMove_Y;																// �ړ��J�E���gY

	bool m_bPlacement;																// �z�u���Ă��邩�ǂ���
	bool m_bCreate;																	// �������邩�ǂ���
	bool m_bMove;																	// �������ǂ���
};
#endif
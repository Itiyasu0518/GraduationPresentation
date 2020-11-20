//==================================================================================================================
//
// box[box.h]
// Author:Ryouma Inoue
//
//==================================================================================================================
#ifndef _BOX_H_
#define _BOX_H_

//==================================================================================================================
// �C���N���[�h�t�@�C��
//==================================================================================================================
#include "main.h"
#include "scene.h"

//==================================================================================================================
// �}�N����`
//==================================================================================================================
#define Box_Depth 8																	// �c�̃|���S����
#define Box_Width 15																// ���̃u���b�N��
#define Piece_Num 50																// �s�[�X��

//==================================================================================================================
// �O���錾
//==================================================================================================================
class CPlayer;
class CScene2D;
class CPiece;

//==================================================================================================================
//
// �N���X�̒�`
//
//==================================================================================================================
class CBox : public CScene
{
public:

	CBox(PRIORITY type);															// �R���X�g���N�^
	~CBox();																		// �f�X�g���N�^
	HRESULT Init(void);																// ����������
	void Uninit(void);																// �I������
	void Update(void);																// �X�V����
	void Draw(void);																// �`�揈��

	void OnTriggerEnter(CCollider *col) {};
	void OnCollisionEnter(CCollider *col) {};

	static CBox *Create(void);														// ��������
	static HRESULT Load(void);														// �e�N�X�`����񃍁[�h
	static void Unload(void);														// �e�N�X�`�����A�����[�h
	static bool GetPuzzle(int nDepth, int nWidth) { return m_bPuzzle[nDepth][nWidth]; }	// �p�Y�����擾

	bool GetPlaacement(void){ return m_bCreate; }									// �z�u���擾
	static bool GetPiece(void) {return m_bPiece;}
	float GetSpeed(int nPieceNum) { return m_fSpeed[nPieceNum]; }					// �X�s�[�h�擾
	float GetRate(int nPieceNum) { return m_fRate[nPieceNum]; }						// �X�s�[�h�㏸���擾
	float GetTurning(int nPieceNum) { return m_fTurning[nPieceNum]; }				// ���񑬓x
	float GetDecay(int nPieceNum) { return m_fDecay[nPieceNum]; }					// ������
	int GetPower(int nPieceNum) { return m_nPower[nPieceNum]; }						// �p���[
	int GetPieceNum(void) { return m_nPieceNum; }									// �s�[�X��

protected:

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;											// �e�N�X�`���ւ̃|�C���^
	CScene2D *m_pBlock[Box_Depth][Box_Width];										// �V�[��2D�̃|�C���^
	CPiece *m_pPiece[Piece_Num];													// �s�[�X

	int m_nCntChange;																// �ς���J�E���g
	int m_nPieceNum;																// �s�[�X��
	int m_nSelect;																	// �I���J�E���g

	float m_fSpeed[Piece_Num];														// �X�s�[�h
	float m_fRate[Piece_Num];														// �X�s�[�h�㏸��
	float m_fTurning[Piece_Num];													// ���񑬓x
	float m_fDecay[Piece_Num];														// ������
	int m_nPower[Piece_Num];														// �p���[


	static bool m_bPuzzle[Box_Depth][Box_Width];									// �g�p���Ă��邩�ǂ���
	bool m_bPuzzleStorage[Box_Depth][Box_Width];									// �ۊ�
	bool m_bPlacement;																// �z�u���Ă��邩�ǂ���
	bool m_bCreate;																	// �������邩�ǂ���
	bool m_bMove;																	// �������ǂ���
	static bool m_bPiece;
	bool m_bRelease;
};
#endif
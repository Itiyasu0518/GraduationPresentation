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

//==================================================================================================================
// �O���錾
//==================================================================================================================
class CPlayer;

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
	void SetPos(D3DXVECTOR3 pos);													// �ʒu�ݒ菈��
	void SetMove(D3DXVECTOR3 move);													// �ړ��ʐݒ菈��

	D3DXVECTOR3 GetPos(void);														// �ʒu�擾����
	D3DXVECTOR3 GetMove(void);														// �ړ��ʎ擾����
	float GetHeight(D3DXVECTOR3 pos);												// �����Z�o����

	void OnTriggerEnter(CCollider *col) {};
	void OnCollisionEnter(CCollider *col) {};

	static CBox *Create(void);														// ��������
	static HRESULT Load(void);														// �e�N�X�`����񃍁[�h
	static void Unload(void);														// �e�N�X�`�����A�����[�h
	static CBox *GetBox(void);														// ���b�V���t�B�[���h���擾

protected:

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;											// �e�N�X�`���ւ̃|�C���^
	static CBox *m_pBox;															// ���b�V���t�B�[���h�̃|�C���^
	static CPlayer *m_pPlayer;														// �v���C���[�̏��|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;												// �o�b�t�@
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;												// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	VERTEX_2D *m_pVtx;																// ���_�i�[
	D3DXMATRIX m_mtxWorld;															// ���[���h�}�g���b�N�X
	D3DXVECTOR3 m_pos;																// �ʒu
	D3DXVECTOR3 m_rot;																// ��]
	D3DXVECTOR3 m_move;																// �ړ�
	int m_nNumVertex;																// �����_��
	int m_nNumIndex;																// ���C���f�b�N�X��
	int m_nNumPolygon;																// ���|���S����
	int m_nNumIndexPolygon;															// �C���f�b�N�X�̂Ƃ��̃|���S����
	D3DXVECTOR3 m_aVecA[Box_Depth * Box_Width];										// �@���x�N�g����ʂ̐����ꎞ�I�Ɋi�[
	D3DXVECTOR3 m_aVecB[Box_Depth * Box_Width];										// �@���x�N�g����ʂ̐����ꎞ�I�Ɋi�[
	D3DXVECTOR3 m_vectorA;															// A�x�N�g��
	D3DXVECTOR3 m_vectorB;															// B�x�N�g��
	D3DXVECTOR3 m_vectorC;															// C�x�N�g��
	int nNumber;																	// �O�p�`�̔z��̔ԍ�
	int StartBox;																	// �n�܂锠
	int EndBox;																		// ������锠
	float fDivide;																	// sin�̒��g������ϐ�

};
#endif
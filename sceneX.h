//=============================================================================
//
// sceneX���� [sceneX.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _SCENEX_H_
#define _SCENEX_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"
#include "scene.h"

//=============================================================================
// �N���X��`
//=============================================================================
class CSceneX : public CScene
{
public:
	CSceneX(PRIORITY obj);				// �R���X�g���N�^
	~CSceneX();							// �f�X�g���N�^
	HRESULT Init(void);					// ����������
	void Uninit(void);					// �J������
	void Update(void);					// �X�V����
	void Draw(void);					// �`�揈��

	static CSceneX *Create(void);		// �N���G�C�g����
	void BindModel(LPD3DXMESH pMesh, DWORD nNumMat, LPD3DXBUFFER pBuffMat);	// ���f���̐ݒ�

	void SetColor(D3DXCOLOR col);													// �F��ݒ肷��
	void SetSize(D3DXVECTOR3 size);													// �傫����ݒ肷��
	void SetRotation(D3DXVECTOR3 rot);												// ��]�l��ݒ�

	static void InitShader(void);													// �V�F�[�_�[�̏���������
	static void UninitShader(void);													// �V�F�[�_�[�̊J������
	static LPD3DXEFFECT GetShader(void) { return m_pToonShader; }					// �V�F�[�_�[�̎擾

	D3DXCOLOR GetColor(void) { return m_col; }										// �F��Ԃ�
	D3DXVECTOR3 GetSize(void) { return m_size; }									// �傫����Ԃ�
	D3DXVECTOR3 GetRotation(void) { return m_rot; }									// ��]�l���擾
	D3DXMATRIX GetMtxWorld(void) { return m_mtxWorld; }								// ���[���h�}�g���b�N�X���̎擾

	void OnTriggerEnter(CCollider *col) {};
	void OnCollisionEnter(CCollider *col) {};
	void ShowInspector(void);

private:
#ifdef _DEBUG
	void Debug(void);
#endif
	static LPD3DXEFFECT		m_pToonShader;											// �g�D�[���V�F�[�_�[

	LPDIRECT3DTEXTURE9	*m_pTexture;												// �e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3 m_size;																// �傫��
	D3DXVECTOR3 m_rot;																// ��]��
	D3DXCOLOR	m_col;																// �F
	D3DXMATRIX	m_mtxWorld;															// ���[���h�}�g���b�N�X

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;												// ���_�o�b�t�@�ւ̃|�C���^
	LPD3DXMESH		m_pMesh;														// ���b�V�����ւ̃|�C���^
	DWORD			m_nNumMat;														// �}�e���A�����̐�
	LPD3DXBUFFER	m_pBuffMat;														// �}�e���A�����ւ̃|�C���^

	int m_nLife;																	//���C�t
	int m_nLifeMax;
	float m_fAngle;																	//�p�x
	float m_fLength;																//����
};
#endif
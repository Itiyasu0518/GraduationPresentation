//=============================================================================
//
// �s�[�X�Z���N�g���� [pieceSelect.h]
// Author : Ryouma Inoue
//
//=============================================================================
#ifndef _BG_H_
#define _BG_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"
#include "scene2D.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define	MAX_CORE 17		// �R�A�̍ő吔

//=============================================================================
// �O���錾
//=============================================================================
class CScene2D;

//=============================================================================
// �N���X��`
//=============================================================================
class CPieceSelect : public CScene
{
public:
	//�R�A�^�C�v
	typedef enum
	{
		PIECETYPE_NONE = -1,		// �Ȃ�
		PIECETYPE_CORE_00,			// �R�A
		PIECETYPE_CORE_01,			// �R�A
		PIECETYPE_CORE_02,			// �R�A
		PIECETYPE_CORE_03,			// �R�A
		PIECETYPE_CORE_04,			// �R�A
		PIECETYPE_CORE_05,			// �R�A
		PIECETYPE_CORE_06,			// �R�A
		PIECETYPE_CORE_07,			// �R�A
		PIECETYPE_CORE_08,			// �R�A
		PIECETYPE_CORE_09,			// �R�A
		PIECETYPE_CORE_10,			// �R�A
		PIECETYPE_CORE_11,			// �R�A
		PIECETYPE_CORE_12,			// �R�A
		PIECETYPE_CORE_13,			// �R�A
		PIECETYPE_CORE_14,			// �R�A
		PIECETYPE_CORE_15,			// �R�A
		PIECETYPE_CORE_16,			// �R�A
		PIECETYPE_CORE_17,			// �R�A
		PIECETYPE_MAX				// �ő吔
	} PIECETYPE;					// �^�C�v

	CPieceSelect(PRIORITY obj);		// �R���X�g���N�^
	~CPieceSelect();				// �f�X�g���N�^
	HRESULT Init(void);				// ����������
	void Uninit(void);				// �J������
	void Update(void);				// �X�V����
	void Draw(void);				// �`�揈��

	void OnTriggerEnter(CCollider *col) {};
	void OnCollisionEnter(CCollider *col) {};

	static CPieceSelect *Create(void);	// �N���G�C�g����
	static HRESULT Load(void);			// ���[�h����

	void ShowInspector(void) {};
	void SetPiece(PIECETYPE type,D3DXVECTOR3 pos,D3DXVECTOR3 size, std::string Add);
	void LoadPiece(void);

private:
	CScene2D *m_pPieceSelect[MAX_CORE];

	PIECETYPE m_type;
};
#endif
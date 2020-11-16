//=============================================================================
//
// ���[�U�[�C���^�[�t�F�[�X���� [ui.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _UI_H_
#define _UI_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"

//=============================================================================
// �O���錾
//=============================================================================
class CScene2D;

//=============================================================================
// �N���X��`
//=============================================================================
class CUi
{
public:
	CUi();			// �R���X�g���N�^
	~CUi();						// �f�X�g���N�^
	HRESULT Init(void);			// ����������
	void Uninit(void);			// �J������
	void Update(void);			// �X�V����
	void Draw(void);			// �`�揈��

	static CUi *Create(void);	// �N���G�C�g����
	static HRESULT Load(void);	// ���[�h����
	bool LoadScript(const std::string &add);		// �X�N���v�g�f�[�^���[�h����

	void ShowInspector(void) {};

	void CreateTexture(std::string Add);
	void DeleteTexture(int nIndex);
	void SceneDebug(void);
	void SaveScript(std::string Add);

private:
	std::vector<CScene2D*> m_Scene;
};
#endif
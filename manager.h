//=============================================================================
//
// �}�l�[�W���[���� [manager.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define D3DXVECTOR3_ZERO D3DXVECTOR3(0.0f, 0.0f, 0.0f);
#define	SHADERADD_LAMBERT ("data/shader/LambertShader.fx")
#define	SHADERADD_TOON ("data/shader/ToonShader.fx")
#define TEXTUREADD_DEFAULT ("data/tex/default.jpg")

//=============================================================================
// �񋓑̒�`
//=============================================================================
typedef enum
{
	LOADTYPE_TEXTURE = 0,
	LOADTYPE_MODEL,
	LOADTYPE_MAX
} LOADTYPE;

//=============================================================================
// �\���̒�`
//=============================================================================
typedef struct
{
	LPD3DXMESH		pMesh;									// ���b�V�����ւ̃|�C���^
	DWORD			nNumMat;									// �}�e���A�����̐�
	LPD3DXBUFFER		pBuffMat;								// �}�e���A�����ւ̃|�C���^
} MODEL_INFO;

//=============================================================================
// �O���錾
//=============================================================================
class CRenderer;
class CInputKeyboard;
class CInputMouse;
class CInputController;
class CGame;
class CTitle;
class CPuzzle;
class CCamera;
class CLight;
class CResult;
class CRanking;
class CSound;
class CNetwork;

//=============================================================================
// �N���X��`
//=============================================================================
class CManager
{
public:
	// ���[�h
	typedef enum
	{
		MODE_NONE = -1,
		MODE_TITLE,							// �^�C�g��
		MODE_DEMO_PLAY,						// �f���v���C
		MODE_CHARACTER_SELECT,				// �L�����N�^�[�I��
		MODE_STAGE_SELECT,					// �X�e�[�W�I��
		MODE_PUZZLE_CUSTOM,					// �p�Y���J�X�^��
		MODE_GAME,							// �Q�[��
		MODE_RESULT,						// ���U���g
		MODE_RANKING,						// �����L���O
		MODE_MAX
	} MODE;

	CManager();														// �R���X�g���N�^
	~CManager();													// �f�X�g���N�^
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);		// ����������
	void Uninit(void);												// �J������
	void Update(void);												// �X�V����
	void Draw(void);												// �`�揈��

	static void SetMode(MODE mode);									// ���[�h�̐ݒ�

	static MODE GetMode(void) { return m_mode; }					// ���[�h���擾
	static CRenderer *GetRenderer(void) { return m_pRenderer; }								// �����_���[�̎擾
	static CInputKeyboard *GetInputKeyboard(void) { return m_pInputKeyboard; }				// �L�[�{�[�h�̎擾
	static CInputMouse *GetInputMouse(void) { return m_pInputMouse; }						// �}�E�X�̎擾
	static CInputController *GetInputController(void) { return m_pInputController; }		// �R���g���[���[�̎擾
	static CSound *GetSound(void) { return m_pSound; }										// �T�E���h�̎擾
	static CCamera *GetCamera(void) { return m_pCamera; }									// �J�����̎擾
	static CLight *GetLight(void) { return m_pLight; }										// ���C�g�̎擾
	static CNetwork *GetNetwork(void) { return m_pNetwork; }								// �l�b�g���[�N�̎擾

	static CGame *GetGame(void) { return m_pGame; }											// �Q�[���̎擾

	static void ConvertStringToFloat(char* text, char* delimiter, float* pResult);			// �����񕪊��ϊ�(float)����
	static std::vector<std::string> split(std::string& input, char delimiter);				// �����񕪊�����
	static float GetDistance(D3DXVECTOR3 FirstTarget, D3DXVECTOR3 SecondTarget);			// 2�_�Ԃ̋������擾
	static int GetRand(int nValue);															// �����_�������̎擾

	static float easeOut(float fValue);														// �C�[�W���O(Out)
	static float easeIn(float fTime, float fStart, float fDifference, float fTotal);		// �C�[�W���O(In)
	static float easeInAndOut(float fTime, float fStart, float fDifference, float fTotal);	// �C�[�W���O(InOut)

	static HRESULT Load(std::string Add);													// �e�N�X�`���̃��[�h
	static LPDIRECT3DTEXTURE9 GetResource(std::string Add);									// �e�N�X�`���̎擾

	static HRESULT LoadModel(std::string &Add);												// X�t�@�C���̃��[�h
	static bool GetModelResource(std::string &Add, LPD3DXBUFFER &pBuffMat, DWORD &nNumMat, LPD3DXMESH &pMesh);	// X�t�@�C���̎擾

	static HRESULT LoadShader(std::string &Add);											// X�t�@�C���̃��[�h
	static LPD3DXEFFECT GetShaderResource(std::string &Add);								// X�t�@�C���̎擾

	static D3DXVECTOR3 GetCursorPosWithCenter(void);

	static std::map<std::string, MODEL_INFO> GetModelMap(void) { return m_ModelMap; }
	static std::vector<std::vector<std::string>> FileContens(char const * cns_cFile, char delimiter);

private:
	static void	LoadSystemFile(void);					// �V�X�e���t�@�C���̃��[�h
	static void TexRelease(void);						// �e�N�X�`���̊J��
	static void ModelRelease(void);						// ���f���f�[�^�̊J��
	static void ShaderRelease(void);					// �V�F�[�_�[�f�[�^�̊J��

	static MODE m_mode;									// ���[�h
	static CRenderer *m_pRenderer;						// �����_���[�̃|�C���^
	static CInputKeyboard *m_pInputKeyboard;			// �L�[�{�[�h�̃|�C���^
	static CInputMouse *m_pInputMouse;					// �}�E�X�̃|�C���^
	static CInputController *m_pInputController;		// �R���g���[���[�̃|�C���^

	static CCamera *m_pCamera;							// �J�����̃|�C���^
	static CLight *m_pLight;							// ���C�g�̃|�C���^
	static CNetwork *m_pNetwork;						// �l�b�g���[�N�̃|�C���^

	static CGame *m_pGame;								// �Q�[���̃|�C���^
	static CTitle *m_pTitle;							// �^�C�g���̃|�C���^
	static CPuzzle *m_pPuzzle;							// �p�Y���̃|�C���^
	static CResult *m_pResult;							// ���U���g�̃|�C���^
	static CRanking *m_pRanking;						// �����L���O�̃|�C���^

	static CSound *m_pSound;							// �T�E���h�̃|�C���^

	static std::map<std::string, LPDIRECT3DTEXTURE9> m_TexMap;			// �e�N�X�`���}�b�v
	static std::map<std::string, MODEL_INFO> m_ModelMap;				// ���f���}�b�v
	static std::map<std::string, LPD3DXEFFECT> m_ShaderMap;				// �V�F�[�_�[�}�b�v
};
#endif
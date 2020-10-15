//=============================================================================
//
// �v���C���[���� [player.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "main.h"
#include "character.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_KEY 30
#define NORMAL_SPEED 1.0f
#define SP_SPEED 2.0f

//=============================================================================
// �O���錾
//=============================================================================
class CMeshCapsule;
class CMeshCube;
class CMeshOrbit;
class CColliderSphere;
class CColliderBox;
class CScene2D;
class CModel;
class CPlayerUi;

//=============================================================================
// �N���X��`
//=============================================================================
class CPlayer : public CCharacter
{
public:
	//���[�V�����^�C�v
	typedef enum
	{
		ANIMATIONTYPE_NONE = -1,				// �Ȃ�
		ANIMATIONTYPE_NEUTRAL = 0,				// �ҋ@
		ANIMATIONTYPE_WALK,						// ����
		ANIMATIONTYPE_RUN,						// ����
		ANIMATIONTYPE_ATTACK_1,					// ��{�U��1
		ANIMATIONTYPE_ATTACK_2,					// ��{�U��2
		ANIMATIONTYPE_ATTACK_3,					// ��{�U��3
		ANIMATIONTYPE_ATTACK_4,					// ��{�U��4
		ANIMATIONTYPE_ATTACK_5,					// ���ߍU��
		ANIMATIONTYPE_ATTACK_6,					// �h���U��1
		ANIMATIONTYPE_RUNATTACK,				// ����U��
		ANIMATIONTYPE_DAMAGE,					// �_���[�W
		ANIMATIONTYPE_SWITCHWEAPON,				// ����̐؂�ւ�
		ANIMATIONTYPE_JUMP_1,					// �W�����v1
		ANIMATIONTYPE_JUMP_2,					// �W�����v2
		ANIMATIONTYPE_JUMP_3,					// �W�����v3
		ANIMATIONTYPE_JUMP_4,					// �W�����v4
		ANIMATIONTYPE_JUMP_5,					// �W�����v5
		ANIMATIONTYPE_MAX						// �A�j���[�V�����̍ő吔
	} ANIMATIONTYPE;							// �A�j���[�V�����^�C�v

	typedef enum
	{
		PLAYERSTATE_NONE = -1,					// �����Ȃ�
		PLAYERSTATE_NORMAL = 0,					// �ʏ�
		PLAYERSTATE_FLOWER,						// �\��
		PLAYERSTATE_MAX
	} PLAYERSTATE;

	CPlayer(PRIORITY obj);						// �v���C���[�̃R���X�g���N�^(�I�u�W�F�N�g�^�C�v)
	~CPlayer();									// �v���C���[�̃f�X�g���N�^
	HRESULT Init(void);							// ����������
	void Uninit(void);							// �J������
	void Update(void);							// �X�V����
	void Draw(void);							// �`�揈��

	static CPlayer *Create(void);				// �v���C���[����
	static HRESULT Load(void);					// �f�ރf�[�^�̎擾
	void SetDeathblow(float nValue);			// �K�E�Z�|�C���g���̐ݒ�

	void OnTriggerEnter(CCollider *col);
	void OnCollisionEnter(CCollider *col);
	void ShowInspector(void) {};

	void BehaviorForMaxFrame(void);				// �ő�t���[�����ɓ��B�����Ƃ��̏���
	void BehaviorForMaxKey(void);				// �ő�L�[���ɓ��B�����Ƃ��̏���

	float GetDeathblow(void) { return m_fDeathblow; }				// �K�E�Z�|�C���g���̎擾
	CPlayerUi *GetPlayerUi(void) { return m_pPlayerUi; }			// �v���C���[UI�̎擾
	D3DXVECTOR3 GetMove(void) { return m_move; }					// �ړ��ʂ̎擾
	PLAYERSTATE GetState(void) { return m_state; }					// �v���C���[�̏�Ԃ��擾
	D3DXMATRIX GetMtxWorld(void) { return m_mtxWorld; }				// ���[���h�}�g���b�N�X�̎擾
	D3DXVECTOR3 GetRotDest(void) { return m_dest; }					// ��]�ŏI���B�n�_

private:
#ifdef _DEBUG
	void Debug(void);																// �f�o�b�O�����֐�
#endif

	void MoveNearEnemy(void);														// �߂��ɂ���G�Ɉړ����鏈��
	void Collision(void);															// �����蔻�菈��
	void Input(void);																// �L�[���͏��֐�

	/*================= �v���C���[�֘A =================*/
	PLAYERSTATE						m_state;										// �v���C���[���
	D3DXVECTOR3						m_dest;											// ���f���̍ŏI���B�_
	D3DXVECTOR3						m_difference;									// ���f���̍ő��]
	D3DXVECTOR3						m_move;											// �ړ���
	D3DXVECTOR3						m_size;											// �傫��
	D3DXVECTOR3						m_rot;											// ��]��
	D3DXCOLOR						m_color;										// �F
	float							m_fSpeed;										// �X�s�[�h
	float							m_fDeathblow;									// �K�E�Z�|�C���g
	int								m_nLife;										// �̗�
	int								m_nActionCount;									// ���̃A�N�V�����܂ł̃J�E���^
	int								m_nParticleCount;								// �p�[�e�B�N�������܂ł̃J�E���^
	bool							m_bJump;										// �W�����v
	bool							m_bEvent;										// �C�x���g�����t���O
	CPlayerUi						*m_pPlayerUi;									// �L�����N�^�[����UI

	/*=============== 3D�����_�����O�֘A ===============*/
	LPDIRECT3DVERTEXBUFFER9			m_pVtxBuff;										// ���_�o�b�t�@�ւ̃|�C���^
	LPD3DXMESH						m_pMesh;										// ���b�V�����ւ̃|�C���^
	DWORD							m_nNumMat;										// �}�e���A�����̐�
	LPD3DXBUFFER					m_pBuffMat;										// �}�e���A�����ւ̃|�C���^
	D3DXMATRIX						m_mtxWorld;										// ���[���h�}�g���b�N�X

	CMeshOrbit						*m_pMeshOrbit;									// �O�Ղ̃|�C���^
	int								m_nCntAttacCombo;								// �R���{����������܂ł̃J�E���^

	/*===============�����蔻��===============*/
	CColliderSphere					*m_pColWeaponSphere;							// ����̓����蔻��̃|�C���^
	CColliderSphere					*m_pColHandSphere;								// ��̓����蔻��̃|�C���^
	CColliderSphere					*m_pColPlayerSphere;							// �v���C���[�̓����蔻��̃|�C���^
	CColliderBox					*m_pBox;										// ��
};
#endif
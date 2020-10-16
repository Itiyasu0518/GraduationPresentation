//=============================================================================
//
// �v���C���[���� [player.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "player.h"
#include "game.h"
#include "manager.h"
#include "renderer.h"
#include "inputKeyboard.h"
#include "inputController.h"
#include "scene3D.h"
#include "meshField.h"
#include "model.h"
#include "meshCapsule.h"
#include "colliderSphere.h"
#include "meshCube.h"
#include "enemy.h"
#include "meshOrbit.h"
#include "camera.h"
#include "colliderBox.h"
#include "stage.h"
#include "sound.h"
#include "scene2D.h"
#include "effect.h"
#include "gauge2D.h"
#include "messageWindow.h"
#include "result.h"
#include "fade.h"
#include "takaseiLibrary.h"
#include "sceneX.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define	SCRIPT_PLAYER "data/animation/girl.txt"		// �Ԃ�����̃��f�����A�h���X
#define COUNTER_COMBO 30							// �h���U����t�J�E���^
#define JUMP_MAX 10									// �W�����v�̉����x
#define ROT_AMOUNT 0.05f							// ��]�̍������炵�Ă�����
#define ROT_LIMIT 0.8f								// ��]����
#define ROT_SPEED 0.2f								// ��]���x

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CPlayer::CPlayer(CScene::PRIORITY obj = CScene::PRIORITY_PLAYER) : CCharacter(obj)
{
	// �D��x�̐ݒ�
	SetObjType(CScene::PRIORITY_PLAYER);				// �I�u�W�F�N�g�^�C�v

	m_nLife = 100;										// �̗͂̏�����
	m_fSpeed = NORMAL_SPEED;							// �X�s�[�h�̏�����
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// ��]�̏�����
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// �ړ��ʂ̏�����
	m_dest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// �ړ���̏�����
	m_difference = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ���̏�����
	m_pColPlayerSphere = NULL;							// �v���C���[�����蔻��|�C���^�̏�����
	m_pColWeaponSphere = NULL;							// �n�����蔻��|�C���^�̏�����
	m_pColHandSphere = NULL;							// ��̓����蔻��̏�����
	m_pBox = NULL;
	m_bJump = false;									// �W�����v�t���O�̏�����
	m_nCntAttacCombo = COUNTER_COMBO;					// �U���h���J�E���^�̏�����
	m_state = PLAYERSTATE_NORMAL;						// �v���C���[�X�e�[�^�X�̏�����
	m_nActionCount = 0;									// �A�N�V�����J�E���^�̏�����
	m_nParticleCount = 0;								// �p�[�e�B�N���J�E���^�̏�����
	m_fDeathblow = 0.0f;								// �K�E�Z�|�C���g�̏�����
	m_bEvent = false;									// �C�x���g�����t���O�̏�����

	m_pPlayerUi = NULL;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CPlayer::~CPlayer()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CPlayer::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	CRenderer *pRenderer = CManager::GetRenderer();

	//�f�o�C�X���擾����
	pDevice = pRenderer->GetDevice();

	CCamera *pCamera = CManager::GetCamera();
	D3DXVECTOR3 pos = GetPosition();						//�v���C���[�̈ʒu�擾

	pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �v���C���[�̈ʒu�ݒ�

	if (pCamera != NULL)
	{
		pCamera->SetPosCamera(pos, D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
	}

	// �L�����N�^�[�̏���������
	CCharacter::Init();

	// �A�j���[�V�����̐ݒ�
	AnimationSwitch(ANIMATIONTYPE_NEUTRAL);

	// �v���C���[���f�����̓ǂݍ���
	LoadScript(SCRIPT_PLAYER, ANIMATIONTYPE_MAX);

	// ����̓����蔻��𐶐�
	m_pColWeaponSphere = CColliderSphere::Create(true, 30.0f);

	if (m_pColWeaponSphere != NULL)
	{ // ���̂̃|�C���^��NULL�ł͂Ȃ��Ƃ�
		m_pColWeaponSphere->SetScene(this);											// �Ăяo���� �̐ݒ�
		m_pColWeaponSphere->SetTag("weapon");										// �^�O �̐ݒ�
	}

	// ����̓����蔻��𐶐�
	m_pColHandSphere = CColliderSphere::Create(true, 50.0f);

	if (m_pColHandSphere != NULL)
	{ // ���̂̃|�C���^��NULL�ł͂Ȃ��Ƃ�
		m_pColHandSphere->SetScene(this);											// �Ăяo���� �̐ݒ�
		m_pColHandSphere->SetTag("weapon");											// �^�O �̐ݒ�
	}

	// �v���C���[�̓����蔻��𐶐�
	m_pColPlayerSphere = CColliderSphere::Create(false, 20.0f);

	if (m_pColPlayerSphere != NULL)
	{ //���̂̃|�C���^��NULL�ł͂Ȃ��Ƃ�
		m_pColPlayerSphere->SetScene(this);
		m_pColPlayerSphere->SetTag("player");										// �^�O �̐ݒ�
		m_pColPlayerSphere->SetPosition(pos);										// �ʒu �̐ݒ�
		m_pColPlayerSphere->SetOffset(D3DXVECTOR3(0.0f, 100.0f, 0.0f));
	}

	// �ʒu�̐ݒ�
	SetPosition(pos);

	// �O�Ղ̐���
	m_pMeshOrbit = CMeshOrbit::Create();

	return S_OK;
}

//=============================================================================
// �J������
//=============================================================================
void CPlayer::Uninit(void)
{
	if (m_pColWeaponSphere != NULL)
	{// ����̓����蔻�肪���݂��Ă����Ƃ�
		if (m_pColWeaponSphere != NULL)
		{
			m_pColWeaponSphere->Release();
		}
	}

	if (m_pColPlayerSphere != NULL)
	{// ����̓����蔻�肪���݂��Ă����Ƃ�
		m_pColPlayerSphere->Release();
	}

	CCharacter::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CPlayer::Update(void)
{
	D3DXVECTOR3 pos;
	CSound *pSound = CManager::GetSound();
	float fHeight = 0.0f;
	CModel *pModel = GetModel();

	// �A�j���[�V�������̎擾
	ANIMATIONTYPE animType = (ANIMATIONTYPE)GetAnimType();
	int currentKey = GetCurrentKey();
	int currentFrame = GetCurrentFrame();
	bool bAnimPlayState = GetAnimPlayState();

	if (!m_bEvent)
	{// �C�x���g���������Ă��Ȃ������Ƃ�
		// ���͏���
		Input();
	}

	pos = GetPosition();				// �ʒu�̎擾

	//���̍������擾����
	CScene *pSceneNext = NULL;														// ������
	CScene *pSceneNow = GetScene(PRIORITY_FLOOR);									// �V�[���̐擪�A�h���X���擾

	CScene *pScene = NowFloor(pos);													// ���݂���t�B�[���h���擾

	if (pScene != NULL)
	{// �������Ă��鏰�����݂����Ƃ�
		CMeshField *pFloor = (CMeshField*)pScene;									// �L���X�g
		fHeight = pFloor->GetHeight(pos);

		RANDTYPE Type = pFloor->GetRandType();

		if (animType == ANIMATIONTYPE_WALK || animType == ANIMATIONTYPE_RUN)
		{
			if (currentKey == 5 || currentKey == 0)
			{
				if (currentFrame == 0)
				{
					if (Type == RANDTYPE_GRASS)
					{
						pSound->PlaySoundA((SOUND_LABEL)(CManager::GetRand(3) + (int)SOUND_LABEL_SE_GRASS_1));
					}
					else if (Type == RANDTYPE_SAND)
					{
						pSound->PlaySoundA((SOUND_LABEL)(CManager::GetRand(3) + (int)SOUND_LABEL_SE_SAND_1));
					}
				}
			}
		}
	}

	//�p�x
	if (animType != ANIMATIONTYPE_ATTACK_4)
	{
		if (!m_bJump)
		{
			m_dest.y = atan2f(-m_move.x, -m_move.z);
		}
	}

	// �����蔻��Ǘ�����
	Collision();

	// �ʒu�X�V
	pos += m_move;

	if (!m_bJump)
	{
		// ����
		m_move.x += (0 - m_move.x) * 0.12f;
		m_move.z += (0 - m_move.z) * 0.12f;
	}

	//�d�͏���
	if (m_bJump)
	{// �W�����v���Ă����Ƃ�
		if (animType != ANIMATIONTYPE_JUMP_1)
		{
			if (pos.y > fHeight)
			{// ���݂̍���������荂�������Ƃ�
				m_move.y += -0.7f;
			}
			else
			{// ���݂̍����������Ⴉ������
				pos.y = fHeight;											// ���̍��������߂�
				m_move.y = 0.0f;
				if (m_bJump)
				{
					// �����̃G�t�F�N�g�\��
					CEffect::SandSmokeEffect(pos);

					m_bJump = false;										// �W�����v�����ς���
					SetAnimPlayState(true);									// �A�j���[�V�����̍ĊJ
					AnimationSwitch(ANIMATIONTYPE_JUMP_5);					// �A�j���[�V�����̕ύX
					pSound->PlaySoundA(SOUND_LABEL_SE_LANDING);				// ���n���̍Đ�
				}
			}
		}
	}
	else
	{
		pos.y = fHeight;											// ���̍��������߂�
	}

	// �G�t�F�N�g�̐���
	if (animType == ANIMATIONTYPE_ATTACK_5)
	{
		if (currentKey == 1)
		{
			if (currentFrame > 0)
			{
				CEffect::PetalCluster(D3DXVECTOR3(pModel[4].GetMtxWorld()._41, pModel[4].GetMtxWorld()._42, pModel[4].GetMtxWorld()._43), m_rot);
			}
		}
	}

	//���̂̃|�C���^��NULL�ł͂Ȃ��Ƃ�
	if (m_pColWeaponSphere != NULL)
	{
		D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 40.0f, -80.0f);
		D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXMATRIX	mtxRot, mtxTrans, mtxView, mtxMeshRot, mtxMeshTrans;				//�v�Z�p�}�g���b�N�X
		D3DXMATRIX mtx;			// ����̃}�g���b�N�X

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&mtx);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxMeshRot, rot.y, rot.x, rot.z);
		D3DXMatrixMultiply(&mtx, &mtx, &mtxMeshRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxMeshTrans, pos.x, pos.y, pos.z);
		D3DXMatrixMultiply(&mtx, &mtx, &mtxMeshTrans);

		D3DXMatrixMultiply(&mtx, &mtx, &pModel[14].GetMtxWorld());

		m_pColWeaponSphere->SetPosition(D3DXVECTOR3(mtx._41, mtx._42, mtx._43));
	}

	if (m_pColHandSphere != NULL)
	{// ��̓����蔻�肪���݂��Ă����Ƃ�
		D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, -50.0f, 0.0f);
		D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXMATRIX	mtxRot, mtxTrans, mtxView, mtxMeshRot, mtxMeshTrans;				//�v�Z�p�}�g���b�N�X
		D3DXMATRIX mtx;			// ����̃}�g���b�N�X

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&mtx);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxMeshRot, rot.y, rot.x, rot.z);
		D3DXMatrixMultiply(&mtx, &mtx, &mtxMeshRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxMeshTrans, pos.x, pos.y, pos.z);
		D3DXMatrixMultiply(&mtx, &mtx, &mtxMeshTrans);

		D3DXMatrixMultiply(&mtx, &mtx, &pModel[4].GetMtxWorld());

		m_pColHandSphere->SetPosition(D3DXVECTOR3(mtx._41, mtx._42, mtx._43));
	}

	//���̂̃|�C���^��NULL�ł͂Ȃ��Ƃ�
	if (m_pColPlayerSphere != NULL)
	{
		m_pColPlayerSphere->SetPosition(pos);		// ���݈ʒu �̍X�V
	}

	SetPosition(pos);

	if (m_pBox != NULL)
	{
		m_pBox->SetPosition(pos);
	}

	//�L�����N�^�[�̐U�����
	m_difference.y = m_rot.y - m_dest.y;

	//�f�B�t�@�����X�̒l���w�肵���l
	if (m_difference.y > D3DX_PI)
	{
		m_difference.y -= D3DX_PI * 2;
	}
	else if (m_difference.y < -D3DX_PI)
	{
		m_difference.y += D3DX_PI * 2;
	}

	// 1�t���[��������̉�]�ʂ���
	m_rot.y -= m_difference.y * 0.1f;

	if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2;
	}
	else if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2;
	}

	// ����o���G�t�F�N�g����
	if (animType == ANIMATIONTYPE_SWITCHWEAPON)
	{// �O�̃A�j���[�V����������̐؂�ւ��������Ƃ�
		if (!bAnimPlayState)
		{// ���݃A�j���[�V���������Ă��Ȃ������Ƃ�
			m_nActionCount++;				// �J�E���^��+1����

			if (m_nActionCount > 50)
			{// �J�E���^��50�ȏゾ�����Ƃ�
				pModel[14].SetActive(true);		// ����̍X�V��������
				SetAnimPlayState(true);				// �A�j���[�V�����̍ĊJ
				AnimationSwitch(ANIMATIONTYPE_NEUTRAL);	// �ҋ@���[�V�����ɐݒ�
				m_nActionCount = 0;					// �J�E���^�����Z�b�g

				int nNumVertices;					//���_��
				DWORD sizeFVF;						//���_�t�H�[�}�b�g
				BYTE	*pVertexBuffer;				//���_�o�b�t�@�ւ̃|�C���^
				LPD3DXMESH pMesh = pModel[14].GetMesh();		// ���b�V�����̎擾

				//���_�����擾
				nNumVertices = pMesh->GetNumVertices();

				//���_�t�H�[�}�b�g�̃T�C�Y���擾
				sizeFVF = D3DXGetFVFVertexSize(pMesh->GetFVF());

				//���_�o�b�t�@�����b�N
				pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVertexBuffer);

				// �p�[�e�B�N������
				for (int nCount = 0; nCount < nNumVertices; nCount++)
				{
					D3DXVECTOR3	vtx = *(D3DXVECTOR3*)pVertexBuffer;

					if (nCount % 50 == 0)
					{// 50���1�񐶐�����
						D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						D3DXMATRIX	mtxRot, mtxTrans, mtxView, mtxMeshRot, mtxMeshTrans;				//�v�Z�p�}�g���b�N�X
						D3DXMATRIX mtx;			// ����̃}�g���b�N�X
						D3DXVECTOR3 pos = GetPosition();

						// ���[���h�}�g���b�N�X�̏�����
						D3DXMatrixIdentity(&mtx);

						// ��]�𔽉f
						D3DXMatrixRotationYawPitchRoll(&mtxMeshRot, rot.y, rot.x, rot.z);
						D3DXMatrixMultiply(&mtx, &mtx, &mtxMeshRot);

						// �ʒu�𔽉f
						D3DXMatrixTranslation(&mtxMeshTrans, vtx.x, vtx.y, vtx.z);
						D3DXMatrixMultiply(&mtx, &mtx, &mtxMeshTrans);

						// �}�g���b�N�X���̍���
						D3DXMatrixMultiply(&mtx, &mtx, &pModel[14].GetMtxWorld());

						// �G�t�F�N�g�̐���
						CEffect::FallingPetals(false, D3DXVECTOR3(mtx._41, mtx._42, mtx._43), 50);
					}

					pVertexBuffer += sizeFVF;			//�T�C�Y���̃|�C���^��i�߂�
				}

				//���_�o�b�t�@���A�����b�N
				pMesh->UnlockVertexBuffer();
			}
		}
	}

	// �L�����N�^�[�̍X�V����
	CCharacter::Update();

#ifdef _DEBUG
	Debug();
#endif
}

//=============================================================================
// �`�揈��
//=============================================================================
void CPlayer::Draw(void)
{
	CCharacter::Draw();
}

//=============================================================================
// �N���G�C�g����
//=============================================================================
CPlayer *CPlayer::Create(void)
{
	CPlayer *pPlayer;
	pPlayer = new CPlayer(CScene::PRIORITY_PLAYER);
	pPlayer->Init();
	return pPlayer;
}

//=============================================================================
// ���[�h����
//=============================================================================
HRESULT CPlayer::Load(void)
{
	return S_OK;
}

//=============================================================================
// �K�E�Z�|�C���g���̐ݒ�
//=============================================================================
void CPlayer::SetDeathblow(float nValue)
{
	m_fDeathblow = nValue;
}

//=============================================================================
// �����蔻��(trigger)
//=============================================================================
void CPlayer::OnTriggerEnter(CCollider *col)
{
	std::string sTag = col->GetTag();
	CModel *pModel = GetModel();

	if (col->GetScene()->GetObjType() == PRIORITY_ENEMY)
	{
		if (sTag == "weapon")
		{
			CSound *pSound = CManager::GetSound();				// �T�E���h�̎擾
			pSound->PlaySoundA(SOUND_LABEL_SE_PUNCH);			// �_���[�W���̍Đ�
			m_nLife -= 5;										// �̗͂����
			AnimationSwitch(ANIMATIONTYPE_DAMAGE);				// �A�j���[�V������ύX

			D3DXVECTOR3 vec;

			vec = GetPosition() - col->GetPosition();			//���������߂�(���������߂邽��)
			D3DXVec3Normalize(&vec, &vec);						//���K������

			m_move.x = vec.x * 25;
			m_move.z = vec.z * 25;

			if (m_nLife < 0)
			{
				CResult::SetIdxKill(CEnemy::GetEnemyKill());			// Kill�������U���g�ɓn��
				CFade::SetFade(CManager::MODE_RESULT);					// ���U���g�ɑJ��
			}
		}

		if (sTag == "enemy")
		{
			ANIMATIONTYPE animType = (ANIMATIONTYPE)GetAnimType();

			if (animType != ANIMATIONTYPE_ATTACK_5)
			{
				D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 40.0f, -80.0f);
				D3DXMATRIX	mtxRot, mtxTrans, mtxView, mtxMeshRot, mtxMeshTrans;				//�v�Z�p�}�g���b�N�X
				D3DXMATRIX mtx;			// ����̃}�g���b�N�X

				// ���[���h�}�g���b�N�X�̏�����
				D3DXMatrixIdentity(&mtx);

				// �ʒu�𔽉f
				D3DXMatrixTranslation(&mtxMeshTrans, pos.x, pos.y, pos.z);
				D3DXMatrixMultiply(&mtx, &mtx, &mtxMeshTrans);

				D3DXMatrixMultiply(&mtx, &mtx, &pModel[14].GetMtxWorld());

				for (int nCount = 0; nCount < 20; nCount++)
				{
					CEffect::FallingPetals(true, D3DXVECTOR3(mtx._41, mtx._42, mtx._43), 150);
				}
			}
		}
	}
	if (sTag == "house")
	{

	}
}

//=============================================================================
// �����蔻��(collider)
//=============================================================================
void CPlayer::OnCollisionEnter(CCollider *col)
{
	std::string sTag = col->GetTag();
}

//========================================================================================
// �A�j���[�V�����t���[���̍ő吔�ɓ��B�����Ƃ��̏���
//========================================================================================
void CPlayer::BehaviorForMaxFrame(void)
{

}

//========================================================================================
// �A�j���[�V�����L�[�̍ő吔�ɓ��B�����Ƃ��̏���
//========================================================================================
void CPlayer::BehaviorForMaxKey(void)
{
	CModel *pModel = GetModel();
	ANIMATIONTYPE animType = (ANIMATIONTYPE)GetAnimType();
	D3DXVECTOR3 rot = pModel[0].GetRotation();		// ��]�ʎ擾

	switch (animType)
	{
	case ANIMATIONTYPE_ATTACK_3:
		rot.y = 1.5f;										// Y���̉�]�ʕύX
		pModel[0].SetRotation(rot);						// ��]�ʂ̐ݒ�
		break;
	case ANIMATIONTYPE_ATTACK_4:
		rot.x = 0.0f;										// X���̉�]�ʕύX
		pModel[0].SetRotation(rot);						// ��]�ʂ̐ݒ�
		break;
	case ANIMATIONTYPE_RUNATTACK:
		rot.x = 0.72f;										// X���̉�]�ʕύX
		pModel[0].SetRotation(rot);						// ��]�ʂ̐ݒ�
		break;
	case ANIMATIONTYPE_JUMP_1:
		AnimationSwitch(ANIMATIONTYPE_JUMP_2);
		break;
	case ANIMATIONTYPE_JUMP_2:
		AnimationSwitch(ANIMATIONTYPE_JUMP_3);
		break;
	case ANIMATIONTYPE_JUMP_3:
		AnimationSwitch(ANIMATIONTYPE_JUMP_4);
		break;
	case ANIMATIONTYPE_JUMP_4:
		SetAnimPlayState(false);
		break;
	case ANIMATIONTYPE_SWITCHWEAPON:
		int nNumVertices;			//���_��
		DWORD sizeFVF;				//���_�t�H�[�}�b�g
		BYTE	*pVertexBuffer;		//���_�o�b�t�@�ւ̃|�C���^
		LPD3DXMESH pMesh = pModel[14].GetMesh();

		//���_�����擾
		nNumVertices = pMesh->GetNumVertices();

		//���_�t�H�[�}�b�g�̃T�C�Y���擾
		sizeFVF = D3DXGetFVFVertexSize(pMesh->GetFVF());

		//���_�o�b�t�@�����b�N
		pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVertexBuffer);

		// �p�[�e�B�N������
		for (int nCount = 0; nCount < nNumVertices; nCount++)
		{
			D3DXVECTOR3	vtx = *(D3DXVECTOR3*)pVertexBuffer;

			if (nCount % 50 == 0)
			{// 50���1�񐶐�����
				D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				D3DXMATRIX	mtxRot, mtxTrans, mtxView, mtxMeshRot, mtxMeshTrans;				//�v�Z�p�}�g���b�N�X
				D3DXMATRIX mtx;			// ����̃}�g���b�N�X
				D3DXVECTOR3 pos = GetPosition();

				// ���[���h�}�g���b�N�X�̏�����
				D3DXMatrixIdentity(&mtx);

				// ��]�𔽉f
				D3DXMatrixRotationYawPitchRoll(&mtxMeshRot, rot.y, rot.x, rot.z);
				D3DXMatrixMultiply(&mtx, &mtx, &mtxMeshRot);

				// �ʒu�𔽉f
				D3DXMatrixTranslation(&mtxMeshTrans, vtx.x, vtx.y, vtx.z);
				D3DXMatrixMultiply(&mtx, &mtx, &mtxMeshTrans);

				D3DXMatrixMultiply(&mtx, &mtx, &pModel[14].GetMtxWorld());

				switch (m_state)
				{
				case PLAYERSTATE_NORMAL:
					CEffect::FallingPetals(false, D3DXVECTOR3(mtx._41, mtx._42, mtx._43), 50);
					break;
				case PLAYERSTATE_FLOWER:
					CEffect::PetalsGather(D3DXVECTOR3(mtx._41, mtx._42, mtx._43));
					break;
				}
			}

			pVertexBuffer += sizeFVF;			//�T�C�Y���̃|�C���^��i�߂�
		}

		//���_�o�b�t�@���A�����b�N
		pMesh->UnlockVertexBuffer();

		CCamera *pCamera = CManager::GetCamera();

		switch (m_state)
		{
		case PLAYERSTATE_NORMAL:
			pCamera->SetDistance(350.0f);			// ���_�ƒ����_�̋�����ς���
			pModel[14].SetActive(false);			// ����̕`����~
			m_state = PLAYERSTATE_FLOWER;			// �X�e�[�^�X��\���ɕύX
			m_fSpeed = SP_SPEED;					// �X�s�[�h��ς���
			m_nActionCount = 0;						// �J�E���^�̏�����
			break;
		case PLAYERSTATE_FLOWER:
			m_state = PLAYERSTATE_NORMAL;			// �X�e�[�^�X��ʏ�ɖ߂�
			m_fSpeed = NORMAL_SPEED;				// �X�s�[�h��ʏ�ɖ߂�
			SetAnimPlayState(false);				// �A�j���[�V�����̒�~����
			m_bEvent = false;						// �C�x���g�t���O������
			return;
			break;
		}
		break;
	}

	switch (m_state)
	{
	case PLAYERSTATE_NORMAL:
		//�U���̔h���J�E���^�̃��Z�b�g
		if (animType == ANIMATIONTYPE_ATTACK_1 || animType == ANIMATIONTYPE_ATTACK_2 ||
			animType == ANIMATIONTYPE_ATTACK_3)
		{// ���[�V�������U�����[�V�����������Ƃ�
			m_nCntAttacCombo = 0;
		}
		break;
	case PLAYERSTATE_FLOWER:
		//�U���̔h���J�E���^�̃��Z�b�g
		if (animType == ANIMATIONTYPE_ATTACK_1)
		{// ���[�V�������U�����[�V�����������Ƃ�
			m_nCntAttacCombo = 0;
		}
		break;
	}

	if (animType != ANIMATIONTYPE_JUMP_2 && animType != ANIMATIONTYPE_JUMP_3 && animType != ANIMATIONTYPE_JUMP_4)
	{
		ANIMATION *pAnimation = GetAnimData();
		//���[�v���邩�ǂ���
		if (pAnimation[animType].nLoop)
		{
			//�L�[�̃��Z�b�g
			ResetKeyAndFrame();
		}
		else
		{
			//�j���[�g�������[�V����
			AnimationSwitch(ANIMATIONTYPE_NEUTRAL);
		}
	}
}

//=============================================================================
// �߂��ɂ���G�̋߂��܂ňړ����鏈��
//=============================================================================
void CPlayer::MoveNearEnemy(void)
{
	bool bTarget = false;				// �͈͓��ɓG�����݂��Ă����Ƃ�
	float fMinDistance = 750000.0f;		// ���݂̍ŒZ����
	float fDistance = 0.0f;				// ���݂̋���
	D3DXVECTOR3 enemyPos;				// �ŒZ�����ɂ���G�̈ʒu
	CScene *pSceneNext = NULL;														// ������
	CScene *pSceneNow = CScene::GetScene(CScene::PRIORITY_ENEMY);					// �擪�A�h���X�̎擾

																					// �����Ȃ��Ȃ�܂ŌJ��Ԃ�
	while (pSceneNow != NULL)
	{
		pSceneNext = CScene::GetSceneNext(pSceneNow, CScene::PRIORITY_ENEMY);		//����A�b�v�f�[�g�Ώۂ��T����
		fDistance = CManager::GetDistance(GetPosition(), pSceneNow->GetPosition());		// ���������߂�

		if (fDistance < fMinDistance)
		{
			if (!bTarget)
			{// �^�[�Q�b�g�̑��݂��m�F����Ă��Ȃ������Ƃ�
				bTarget = true;				// �ʒu�̈ړ���������
			}

			enemyPos = pSceneNow->GetPosition();			// �G�̈ʒu���L�^����
		}

		pSceneNow = pSceneNext;								//����A�b�v�f�[�g�Ώۂ��i�[
	}

	if (bTarget)
	{// �ړ��̋����o�Ă����Ƃ�
		SetPosition(enemyPos);			// �ʒu�̈ړ�
	}
}

//=============================================================================
// �����蔻�菈��
//=============================================================================
void CPlayer::Collision(void)
{
	ANIMATIONTYPE animType = (ANIMATIONTYPE)GetAnimType();

	switch (animType)
	{
	case ANIMATIONTYPE_NEUTRAL:					// �ҋ@���[�V�����̂Ƃ�
		if (m_pColWeaponSphere != NULL)
		{// ����̓����蔻�肪���݂��Ă����Ƃ�
			if (m_pColWeaponSphere->GetUse())
			{// �����蔻��̑Ώۂ������Ƃ�
				m_pColWeaponSphere->SetUse(false);		// �Ώۂ���O��
			}
			if (m_pColHandSphere->GetUse())
			{
				m_pColHandSphere->SetUse(false);
			}
		}
		if (m_pMeshOrbit != NULL)
		{// �O�Ղ����݂��Ă����Ƃ�
			if (m_pMeshOrbit->GetOrbit())
			{
				m_pMeshOrbit->SetOrbit(false);
			}
		}
		break;
	case ANIMATIONTYPE_WALK:						// �������[�V�����̂Ƃ�
		if (m_pColWeaponSphere != NULL)
		{// ����̓����蔻�肪���݂��Ă����Ƃ�
			if (m_pColWeaponSphere->GetUse())
			{// �����蔻��̑Ώۂ������Ƃ�
				m_pColWeaponSphere->SetUse(false);		// �Ώۂ���O��
			}
			if (m_pColHandSphere->GetUse())
			{
				m_pColHandSphere->SetUse(false);
			}
		}
		if (m_pMeshOrbit != NULL)
		{// �O�Ղ����݂��Ă����Ƃ�
			if (m_pMeshOrbit->GetOrbit())
			{
				m_pMeshOrbit->SetOrbit(false);
			}
		}
		break;
	case ANIMATIONTYPE_ATTACK_1:				// �U�����[�V�����̂Ƃ�
		if (m_pColWeaponSphere != NULL)
		{// ����̓����蔻�肪���݂��Ă����Ƃ�
			if (!m_pColWeaponSphere->GetUse())
			{// �����蔻��̑ΏۊO�������Ƃ�
				m_pColWeaponSphere->SetUse(true);		// �Ώۂɂ���
			}
		}
		if (m_pMeshOrbit != NULL)
		{// �O�Ղ����݂��Ă����Ƃ�
			if (!m_pMeshOrbit->GetOrbit())
			{
				m_pMeshOrbit->SetOrbit(true);
			}
		}
		break;
	case ANIMATIONTYPE_ATTACK_2:				// �U�����[�V�����̂Ƃ�
		if (m_pColWeaponSphere != NULL)
		{// ����̓����蔻�肪���݂��Ă����Ƃ�
			if (!m_pColWeaponSphere->GetUse())
			{// �����蔻��̑ΏۊO�������Ƃ�
				m_pColWeaponSphere->SetUse(true);		// �Ώۂɂ���
			}
		}
		if (m_pMeshOrbit != NULL)
		{// �O�Ղ����݂��Ă����Ƃ�
			if (!m_pMeshOrbit->GetOrbit())
			{// �O�Ղ̍X�V����~���Ă����Ƃ�
				m_pMeshOrbit->SetOrbit(true);
			}
		}
		break;
	case ANIMATIONTYPE_ATTACK_3:				// �U�����[�V�����̂Ƃ�
		if (m_pColWeaponSphere != NULL)
		{// ����̓����蔻�肪���݂��Ă����Ƃ�
			if (!m_pColWeaponSphere->GetUse())
			{// �����蔻��̑ΏۊO�������Ƃ�
				m_pColWeaponSphere->SetUse(true);		// �Ώۂɂ���
			}
		}
		if (m_pMeshOrbit != NULL)
		{// �O�Ղ����݂��Ă����Ƃ�
			if (!m_pMeshOrbit->GetOrbit())
			{// �O�Ղ̍X�V����~���Ă����Ƃ�
				m_pMeshOrbit->SetOrbit(true);
			}
		}
	case ANIMATIONTYPE_ATTACK_4:				// �U�����[�V�����̂Ƃ�
		if (m_pColWeaponSphere != NULL)
		{// ����̓����蔻�肪���݂��Ă����Ƃ�
			if (!m_pColWeaponSphere->GetUse())
			{// �����蔻��̑ΏۊO�������Ƃ�
				m_pColWeaponSphere->SetUse(true);		// �Ώۂɂ���
			}
		}
		if (m_pMeshOrbit != NULL)
		{// �O�Ղ����݂��Ă����Ƃ�
			if (!m_pMeshOrbit->GetOrbit())
			{// �O�Ղ̍X�V����~���Ă����Ƃ�
				m_pMeshOrbit->SetOrbit(true);
			}
		}
		break;
	case ANIMATIONTYPE_ATTACK_6:				// �U�����[�V�����̂Ƃ�
		if (m_pColWeaponSphere != NULL)
		{// ����̓����蔻�肪���݂��Ă����Ƃ�
			if (!m_pColWeaponSphere->GetUse())
			{// �����蔻��̑ΏۊO�������Ƃ�
				m_pColWeaponSphere->SetUse(true);		// �Ώۂɂ���
			}
		}
		if (m_pMeshOrbit != NULL)
		{// �O�Ղ����݂��Ă����Ƃ�
			if (!m_pMeshOrbit->GetOrbit())
			{// �O�Ղ̍X�V����~���Ă����Ƃ�
				m_pMeshOrbit->SetOrbit(true);
			}
		}
		break;
	}
}

//=============================================================================
// �L�[���͏�񏈗�
//=============================================================================
void CPlayer::Input(void)
{
	ANIMATIONTYPE animType = (ANIMATIONTYPE)GetAnimType();

	// �L�[�{�[�h�̎擾
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();

	// �Q�[���p�b�h�̎擾
	CInputController *pGamepad = CManager::GetInputController();

	// �J�����̎擾
	CCamera *pCamera = CManager::GetCamera();

	// �T�E���h�̎擾
	CSound *pSound = CManager::GetSound();

	D3DXVECTOR3 rot = pCamera->GetRotation();
	D3DXVECTOR3 nor;
	D3DXVECTOR3 Diff;	// �v�Z�p�i�[�ϐ�
	float nValueH = 0;									//�R���g���[���[
	float nValueV = 0;									//�R���g���[���[

	if (m_nCntAttacCombo <= COUNTER_COMBO)
	{// �U���h����t�J�E���^��15�J�E���g�����������Ƃ�
		m_nCntAttacCombo++;				// �J�E���^��1�v���X
	}

	// ====================== �R���g���[���[ ====================== //

	// �U��
	{
		if (pGamepad != NULL)
		{// �Q�[���p�b�h�����݂����Ƃ�
			if (pGamepad->GetJoypadUse(0))
			{// �g�p�\�������Ƃ�
				pGamepad->GetJoypadStickLeft(0, &nValueH, &nValueV);

				//�ړ�
				m_move += D3DXVECTOR3(sinf(D3DX_PI * 1.0f + rot.y) * (m_fSpeed * nValueV), 0, cosf(D3DX_PI * 1.0f + rot.y) * (m_fSpeed * nValueV));
				m_move += D3DXVECTOR3(sinf(D3DX_PI * 0.5f + rot.y) * (m_fSpeed * nValueH), 0, cosf(D3DX_PI * 0.5f + rot.y) * (m_fSpeed * nValueH));

#ifdef _DEBUG
				CDebugProc::Log("�ړ��� : %.2f %.2f %.2f", m_move.x, m_move.y, m_move.z);

				if (pGamepad->GetControllerPress(0, JOYPADKEY_A))
				{
					CDebugProc::Log("�R���g���[���[ : 0��\n");
				}
				if (pGamepad->GetControllerPress(1, JOYPADKEY_A))
				{
					CDebugProc::Log("�R���g���[���[ : 1��\n");
				}

				CDebugProc::Log("�R���g���[���[H : %f\n", nValueH);
				CDebugProc::Log("�R���g���[���[V : %f\n", nValueV);
#endif
			}
		}

		// ====================== �L�[�{�[�h ====================== //

		if (pKeyboard->GetTriggerKeyboard(DIK_1))
		{
			for (int nCount = 0; nCount < 20; nCount++)
			{
				float fAngle = float(CManager::GetRand(314)) - float(CManager::GetRand(314));
				float fAngle_x = float(CManager::GetRand(314)) - float(CManager::GetRand(314));

				D3DXVECTOR3 particlePos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

				particlePos.x = cosf(D3DX_PI + fAngle) * cosf(D3DX_PI + fAngle_x);
				particlePos.y = sinf(D3DX_PI + fAngle_x);
				particlePos.z = sinf(D3DX_PI + fAngle) * cosf(D3DX_PI + fAngle_x);

				fAngle = float(CManager::GetRand(314)) / 100.0f - float(CManager::GetRand(314)) / 100.0f;
				D3DXVECTOR3 rot;
				rot = D3DXVECTOR3(sinf(fAngle) * 10, cosf(fAngle) * 10, 0.0f);

				CEffect::SetEffect(EFFECTTYPE_ROSE,										// �p�[�e�B�N���̃^�C�v
					GetPosition(),											// �����ʒu
					D3DXVECTOR3(8.0f, 8.0f, 0.0f),							// �T�C�Y
					particlePos * 5.0f,										// �����x�N�g��
					D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					EASINGTYPE_NONE,
					rot,													// �e�N�X�`���̉�]
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),						// �J���[
					200,													// �p�[�e�B�N���̐����J�E���g��
					true,													// �d��
					0,														// ��R
					true,													// �r���{�[�h
					0,														// �\������ӏ�(��)
					0,														// �\������ӏ�(�c)
					D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					0,
					0,
					0);
			}
		}

		if (pKeyboard->GetTriggerKeyboard(DIK_H))
		{// �{�C�̗͊J��
			if (m_state != PLAYERSTATE_FLOWER)
			{// ���݂̃X�e�[�^�X���\����Ԃł͂Ȃ��Ƃ�
				if (m_fDeathblow > 25.0f)
				{
					AnimationSwitch(ANIMATIONTYPE_SWITCHWEAPON);			// �A�j���[�V�����̕ύX
					m_nActionCount = 0;			// �J�E���^�̏�����

					D3DXVECTOR3 cameraRot = m_rot;				// ��]�ʂ��i�[
					cameraRot.y += D3DX_PI;						// ��]�ʂ�3.14�����Z
					pCamera->SetDistance(300.0f);				// ���_�����_�̋�����ݒ�
					pCamera->SetPosCamera(GetPosition(), cameraRot);		// �J�����̏ꏊ��ݒ�
					m_bEvent = true;
				}
			}
		}

		if (pKeyboard->GetTriggerKeyboard(DIK_LSHIFT))
		{
			D3DXVECTOR3 pos = GetPosition();
			pos.y += 100.0f;
			AnimationSwitch(ANIMATIONTYPE_RUNATTACK);
			CEffect::Star(pos);
			CEffect::Halo(pos, D3DXVECTOR3(30.0f, 30.0f, 0.0f));
		}

		if (pKeyboard->GetTriggerKeyboard(DIK_SPACE))
		{// �X�y�[�X�L�[�������ꂽ�Ƃ�
			if (!m_bJump)
			{
				AnimationSwitch(ANIMATIONTYPE_JUMP_1);		// �W�����v���[�V�����ɐ؂�ւ�
				m_bJump = true;			// �W�����v���Ă���
			}
		}

		//���E����
		if (pKeyboard->GetPressKeyboard(MOVE_LEFT))
		{
			m_move.x += sinf(D3DX_PI * 0.5f + rot.y) * m_fSpeed;
			m_move.z += cosf(D3DX_PI * 0.5f + rot.y) * m_fSpeed;

			D3DXVec3Normalize(&nor, &D3DXVECTOR3(m_move.z, m_move.y, -m_move.x));
			m_dest.y = m_rot.y - ROT_SPEED;
		}
		else if (pKeyboard->GetPressKeyboard(MOVE_RIGHT))
		{
			m_move.x += sinf(-D3DX_PI * 0.5f + rot.y) * m_fSpeed;
			m_move.z += cosf(-D3DX_PI * 0.5f + rot.y) * m_fSpeed;

			D3DXVec3Normalize(&nor, &D3DXVECTOR3(m_move.z, m_move.y, -m_move.x));
			m_dest.y = m_rot.y + ROT_SPEED;
		}

		//�㉺����
		else if (pKeyboard->GetPressKeyboard(MOVE_ACCEL))
		{
			m_move.x += sinf(D3DX_PI * 1.0f + rot.y) * m_fSpeed;
			m_move.z += cosf(D3DX_PI * 1.0f + rot.y) * m_fSpeed;

			D3DXVec3Normalize(&nor, &D3DXVECTOR3(m_move.z, m_move.y, -m_move.x));
		}
		else if (pKeyboard->GetPressKeyboard(MOVE_BRAKE))
		{
			m_move.x += sinf(D3DX_PI * 0.0f + rot.y) * m_fSpeed;
			m_move.z += cosf(D3DX_PI * 0.0f + rot.y) * m_fSpeed;

			D3DXVec3Normalize(&nor, &D3DXVECTOR3(m_move.z, m_move.y, -m_move.x));
		}

		// �v���C���[�̉�]�ƖڕW�n�_�̍����i�[
		Diff.y = m_rot.y - m_dest.y;

		// ��]�̕␳
		CTakaseiLibrary::RotRevision(&Diff);

		// �v���C���[�����X�ɉ�]�����Ă���
		m_rot.y -= Diff.y * ROT_AMOUNT;

		// ��]�̐ݒ�
		SetRotation(m_rot);
	}

#ifdef _DEBUG
	if (pKeyboard->GetTriggerKeyboard(DIK_K))
	{// K�������ꂽ�Ƃ�
		m_fDeathblow = 50.0f;				// �K�E�Z�|�C���g���ő�l�܂ŏグ��
	}
	{// K�������ꂽ�Ƃ�
		m_fDeathblow = 50.0f;				// �K�E�Z�|�C���g���ő�l�܂ŏグ��
	}
	if (pKeyboard->GetTriggerKeyboard(DIK_F8))
	{// F8�������ꂽ�Ƃ�
		switch (m_state)
		{
		case PLAYERSTATE_NORMAL:
			m_state = PLAYERSTATE_FLOWER;
			m_fSpeed = SP_SPEED;
			break;
		case PLAYERSTATE_FLOWER:
			m_state = PLAYERSTATE_NORMAL;
			m_fSpeed = NORMAL_SPEED;
			break;
			//=============================================================================
			// �A�j���[�V�����̐؂�ւ�
			//=============================================================================

		}
	}
	if (pKeyboard->GetTriggerKeyboard(DIK_L))
	{
		AnimationSwitch(ANIMATIONTYPE_SWITCHWEAPON);
	}
#endif
}

#ifdef _DEBUG
//=============================================================================
// �f�o�b�O����
//=============================================================================
void CPlayer::Debug(void)
{
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 posOld = GetPosOld();
	ImGui::Begin("System");													// �uSystem�v�E�B���h�E�ɒǉ����܂��B�Ȃ���΍쐬���܂��B

	if (ImGui::CollapsingHeader("player"))
	{
		ImGui::Text("pos = %.2f, %.2f, %.2f", pos.x, pos.y, pos.z);								// �v���C���[�̌��݈ʒu��\��
		ImGui::Text("posOld = %.2f, %.2f, %.2f", posOld.x, posOld.y, posOld.z);								// �v���C���[�̌��݈ʒu��\��
		ImGui::Text("move = %.2f, %.2f, %.2f", m_move.x, m_move.y, m_move.z);								// �v���C���[�̌��݈ʒu��\��
		ImGui::Text("HP = %d", m_nLife);				// �v���C���[�̗̑͂�\��

		switch (m_state)
		{
		case PLAYERSTATE_NORMAL:
			ImGui::Text("normal");
			break;
		case PLAYERSTATE_FLOWER:
			ImGui::Text("flower");
			break;
		}

		if (ImGui::Button("BOSS"))
		{
			pos = D3DXVECTOR3(5972.14f, 100.0f, 8000.62f);
			SetPosition(pos);
		}
	}

	//�f�o�b�O�������I��
	ImGui::End();
}
#endif
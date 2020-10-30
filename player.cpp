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
#include "speed.h"
#include "wall.h"
#include "object.h"

//=============================================================================
// �}�N����`
//=============================================================================
//#define	SCRIPT_PLAYER "data/animation/girl.txt"		// �Ԃ�����̃��f�����A�h���X
#define	SCRIPT_CAR01 "data/animation/car01.txt"		// ��01�̃��f�����A�h���X
#define COUNTER_COMBO 30							// �h���U����t�J�E���^
#define JUMP_MAX 10									// �W�����v�̉����x
#define ROT_AMOUNT 0.1f								// ��]�̍������炵�Ă�����
#define ROT_SPEED 0.2f								// ��]���x
#define ROT_SPEED_DRIFT 0.5f						// �h���t�g����]���x
#define MODEL_FRONT 2								// ���f���O��
#define MODEL_TIRE 2								// �^�C�����f���̐�
#define SPEED_DOWN 0.06f							// �X�s�[�h����

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CPlayer::CPlayer(CScene::PRIORITY obj = CScene::PRIORITY_PLAYER) : CCharacter(obj)
{
	// �D��x�̐ݒ�
	SetObjType(CScene::PRIORITY_PLAYER);				// �I�u�W�F�N�g�^�C�v

	m_nLife = 100;										// �̗͂̏�����
	m_fSpeed = NORMAL_SPEED;							// �X�s�[�h�̏�����
	m_rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);			// ��]�̏�����
	m_fSpeed = 0;;										// �X�s�[�h�̏�����
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// �ړ��ʂ̏�����
	m_dest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// �ړ���̏�����
	m_difference = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ���̏�����
	m_cameraRot = D3DXVECTOR3(0, D3DX_PI, 0);			// �J�����̉�]��񏉊���
	m_pColPlayerSphere = NULL;							// �v���C���[�����蔻��|�C���^�̏�����
	m_bHit = false;										// �����蔻��t���O�̏�������
	m_bJump = false;									// �W�����v�t���O�̏�����
	m_nActionCount = 0;									// �A�N�V�����J�E���^�̏�����
	m_nParticleCount = 0;								// �p�[�e�B�N���J�E���^�̏�����
	m_fDeathblow = 0.0f;								// �K�E�Z�|�C���g�̏�����
	m_bEvent = false;									// �C�x���g�����t���O�̏�����
	m_bDrift = false;									// �h���t�g�t���O����
	m_bMove = false;									// ���ݓ����Ă��邩�̃t���O

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
	D3DXVECTOR3 pos = GetPosition();				// �v���C���[�̈ʒu�擾

	pos = D3DXVECTOR3(0.0f, 50.0f, 0.0f);			// �v���C���[�̈ʒu�ݒ�

	if (pCamera != NULL)
	{
		pCamera->SetPosCamera(pos, D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
	}

	// �L�����N�^�[�̏���������
	CCharacter::Init();

	// �A�j���[�V�����̐ݒ�
	AnimationSwitch(ANIMATIONTYPE_NONE);

	// �v���C���[���f�����̓ǂݍ���
	LoadScript(SCRIPT_CAR01, ANIMATIONTYPE_MAX);

	// �v���C���[�̓����蔻��𐶐�
	m_pColPlayerSphere = CColliderSphere::Create(false, 20.0f);

	if (m_pColPlayerSphere != NULL)
	{ //���̂̃|�C���^��NULL�ł͂Ȃ��Ƃ�
		m_pColPlayerSphere->SetScene(this);
		m_pColPlayerSphere->SetTag("player");										// �^�O �̐ݒ�
		m_pColPlayerSphere->SetPosition(pos);										// �ʒu �̐ݒ�
		m_pColPlayerSphere->SetOffset(D3DXVECTOR3(0.0f, 20.0f, 0.0f));
	}

	// �ʒu�̐ݒ�
	SetPosition(pos);

	return S_OK;
}

//=============================================================================
// �J������
//=============================================================================
void CPlayer::Uninit(void)
{
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
		if (!m_bHit)
		{// �������Ă��Ȃ������Ƃ�
			// ���͏���
			Input();
		}
	}

	pos = GetPosition();				// �ʒu�̎擾

	D3DXVECTOR3 pointLeft, pointRight;
	float fLeftLength, fRightLength;

	CCollider::RayBlockCollision(pos, &pModel[0].GetMtxWorld(), 12.28f, 30.0f);
	pointRight = CCollider::RayRightWallCollision(fRightLength, pos, m_rot, m_move, &pModel[0].GetMtxWorld());
	pointLeft = CCollider::RayLeftWallCollision(fLeftLength, pos, m_rot, m_move, &pModel[0].GetMtxWorld());

	//if (m_pColPlayerSphere != NULL)
	//{
	//	D3DXVECTOR3 center = (pointLeft + pointRight) * 0.5f;
	//	m_pColPlayerSphere->SetPosition(center);

	//	float fLength, playerLength;
	//	fLength = (pointLeft.x - pointRight.x) * (pointLeft.x - pointRight.x) + (pointLeft.z - pointRight.z) * (pointLeft.z - pointRight.z);

	//	playerLength = (center.x - pos.x) * (center.x - pos.x) + (center.z - pos.z) * (center.z - pos.z);

	//	if (fLength * 0.25f < playerLength)
	//	{
	//		D3DXVECTOR3 save = pos - D3DXVECTOR3(center.x, pos.y, center.z);
	//		D3DXVECTOR3 vec;
	//		D3DXVec3Normalize(&vec, &save);			//���K������

	//		float y = pos.y;

	//		// �H�����񂾕������߂�
	//		pos = vec * ((fRightLength + fLeftLength) * 0.5f);
	//		pos.y = y;

	//		// �H�����񂾕������߂�
	//		SetPosition(pos);
	//	}

	//	m_pColPlayerSphere->SetRadius((fRightLength + fLeftLength) * 0.5f);
	//	CDebugProc::Log("���� : %.2f", playerLength);
	//}

	//���̍������擾����
	CScene *pSceneNext = NULL;														// ������
	CScene *pSceneNow = GetScene(PRIORITY_FLOOR);									// �V�[���̐擪�A�h���X���擾

	CScene *pScene = NowFloor(pos);													// ���݂���t�B�[���h���擾

	if (pScene != NULL)
	{// �������Ă��鏰�����݂����Ƃ�
		CMeshField *pFloor = (CMeshField*)pScene;									// �L���X�g
		fHeight = pFloor->GetHeight(pos);

		RANDTYPE Type = pFloor->GetRandType();

		if (animType == ANIMATIONTYPE_RUN)
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

	// �ʒu�X�V
	pos += m_move;

	// �W�����v���Ă��Ȃ��Ƃ�
	if (!m_bJump)
	{
		// ����
		m_move.x += (0 - m_move.x) * SPEED_DOWN;
		m_move.z += (0 - m_move.z) * SPEED_DOWN;
	}

	////�d�͏���
	//if (m_bJump)
	//{// �W�����v���Ă����Ƃ�
	//	if (animType != ANIMATIONTYPE_JUMP_1)
	//	{
	//		if (pos.y > fHeight)
	//		{// ���݂̍���������荂�������Ƃ�
	//			m_move.y += -0.7f;
	//		}
	//		else
	//		{// ���݂̍����������Ⴉ������
	//			pos.y = fHeight;											// ���̍��������߂�
	//			m_move.y = 0.0f;
	//			if (m_bJump)
	//			{
	//				// �����̃G�t�F�N�g�\��
	//				CEffect::SandSmokeEffect(pos);
	//				m_bJump = false;										// �W�����v�����ς���
	//				SetAnimPlayState(true);									// �A�j���[�V�����̍ĊJ
	//				AnimationSwitch(ANIMATIONTYPE_JUMP_5);					// �A�j���[�V�����̕ύX
	//				pSound->PlaySoundA(SOUND_LABEL_SE_LANDING);				// ���n���̍Đ�
	//			}
	//		}
	//	}
	//}
	//else
	//{
	//	pos.y = fHeight;											// ���̍��������߂�
	//}

	// �ʒu�ݒ�
	SetPosition(pos);

	// �����蔻��Ǘ�����
	Collision();

	//	D3DXVECTOR3 move = CManager::Slip(playerPos + m_move, vNormal);// ����x�N�g�����v�Z


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
	}
	if (sTag == "house")
	{

	}
	if (sTag == "goal")
	{
		if (CFade::GetFade() == CFade::FADE_NONE)
		{//�t�F�[�h�����������Ă��Ȃ��Ƃ�
			CFade::SetFade(CManager::MODE_PUZZLE_CUSTOM);					//�t�F�[�h������
		}
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
	//// �ǂ̓����蔻��
	//m_bHit = CollisionWall();
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

	// ���f���̎擾
	CModel *pModel = GetModel();

	D3DXVECTOR3 rot = pCamera->GetRotation();
	D3DXVECTOR3 Diff;	// �v�Z�p�i�[�ϐ�
	float nValueH = 0;									//�R���g���[���[
	float nValueV = 0;									//�R���g���[���[

	// ====================== �R���g���[���[ ====================== //

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

	// ���f��������Ƃ�
	if (pModel != NULL)
	{
		D3DXVECTOR3 modelFrontDiff = D3DXVECTOR3(0, 0, 0);			// ���f���O�֌v�Z�p�}�g���b�N�X
		D3DXVECTOR3 aVec = D3DXVECTOR3(0, 0, 0);					// �v���C���[�����x�x�N�g��
		D3DXVECTOR3 cameraVec = D3DXVECTOR3(0, 0, 0);				// �J�����̕����x�N�g��
		D3DXVECTOR3 moveVec = D3DXVECTOR3(0, 0, 0);					// �v���C���[�^���x�N�g��
		D3DXVECTOR3 fModelRot = pModel[MODEL_FRONT].GetRotation();	// ���f���O�։�]���
		float acceleration = 0.0f;									// �����x
		float fRadius = 100.0f;										// ��]���a
		float fDigit = CSpeed::GetDigit();							// �����擾
		m_fSpeed = 0;

		//if (pKeyboard->GetTriggerKeyboard(DIK_SPACE))
		//{// �X�y�[�X�L�[�������ꂽ�Ƃ�
		//	if (!m_bJump)
		//	{
		//		AnimationSwitch(ANIMATIONTYPE_JUMP_1);		// �W�����v���[�V�����ɐ؂�ւ�
		//		m_bJump = true;			// �W�����v���Ă���
		//	}
		//}

		//�㉺����
		if (pKeyboard->GetPressKeyboard(MOVE_ACCEL))
		{
			// �O�փ��f���̍ŏI�ړI���W
			m_dest.y = 0.0f;

			// ���x�ݒ�
			m_fSpeed = -NORMAL_SPEED;

			// �����Ă���
			m_bMove = true;
		}
		else if (pKeyboard->GetPressKeyboard(MOVE_BRAKE))
		{
			// �O�փ��f���̍ŏI�ړI���W
			m_dest.y = 0.0f;

			// ���x�ݒ�
			m_fSpeed = NORMAL_SPEED;

			// �����Ă���
			m_bMove = true;
		}

		//���E����
		if (pKeyboard->GetPressKeyboard(MOVE_LEFT))
		{
			// �O�փ��f���̍ŏI�ړI���W
			m_dest.y = -ROT_SPEED;
		}
		else if (pKeyboard->GetPressKeyboard(MOVE_RIGHT))
		{
			// �O�փ��f���̍ŏI�ړI���W
			m_dest.y = ROT_SPEED;
		}

		// �h���t�g���Ă��Ȃ��Ƃ�
		if (!m_bDrift)
		{
			// �h���t�g�{�^�����������Ƃ�
			if (pKeyboard->GetPressKeyboard(MOVE_DRIFT))
			{
				//���E����
				if (pKeyboard->GetPressKeyboard(MOVE_LEFT))
				{
					// �h���t�g���Ă����Ԃɂ���
					m_bDrift = true;
				}
				else if (pKeyboard->GetPressKeyboard(MOVE_RIGHT))
				{
					// �h���t�g���Ă����Ԃɂ���
					m_bDrift = true;
				}
			}
		}
		else
		{// �h���t�g���Ă���Ƃ�
			//���E����
			if (pKeyboard->GetPressKeyboard(MOVE_LEFT))
			{
				// ��]���a�ύX
				fRadius = 1000.0f;
			}
			else if (pKeyboard->GetPressKeyboard(MOVE_RIGHT))
			{
				// ��]���a�ύX
				fRadius = 1000.0f;
			}

			// �����x�v�Z
			acceleration = CTakaseiLibrary::OutputAcceleration(fDigit / 2, fRadius);


			// �����x�x�N�g���ݒ�
			aVec.x += sinf(m_rot.y - D3DX_PI / 2) * acceleration;
			aVec.z += cosf(m_rot.y - D3DX_PI / 2) * acceleration;

#ifdef _DEBUG
			CDebugProc::Log("�����x�F%f\n", acceleration);
			CDebugProc::Log("�����x�x�N�g���F%f, %f, %f\n", aVec.x, aVec.y, aVec.z);
#endif

			// �h���t�g�{�^���𗣂����Ƃ�
			if (!pKeyboard->GetPressKeyboard(MOVE_DRIFT))
			{
				// �h���t�g���Ă��Ȃ���Ԃɂ���
				m_bDrift = false;
			}
		}

		{// �O�֐ݒ�
			// ���f���̉�]�ƖڕW�n�_�̍����i�[
			modelFrontDiff.y = fModelRot.y - m_dest.y;

			// ��]�̕␳
			CTakaseiLibrary::RotRevision(&modelFrontDiff);

			// ���f�������X�ɉ�]�����Ă���
			fModelRot.y -= modelFrontDiff.y * ROT_AMOUNT;

			// ��]�̕␳
			CTakaseiLibrary::RotRevision(&fModelRot);

			// ���f���̉�]�̐ݒ�
			pModel[MODEL_FRONT].SetRotation(fModelRot);
		}

		{// �J�����ݒ�
			// �^�C���̉�]�̔��������Ɗi�[
			cameraVec.y = m_cameraRot.y - (m_rot.y + (m_dest.y / 2));

			// ��]�̕␳
			CTakaseiLibrary::RotRevision(&cameraVec);

			// �J���������X�ɉ�]�����Ă���
			m_cameraRot.y -= cameraVec.y * ROT_AMOUNT;

			// ��]�̕␳
			CTakaseiLibrary::RotRevision(&m_cameraRot);

			// �J������]�ݒ�
			SetCameraRot(m_cameraRot);
		}

		// �ړ����Ă����Ƃ�
		if (m_bMove)
		{
			// �^���x�N�g���v�Z
			moveVec.x += sinf(m_rot.y + m_dest.y) * m_fSpeed;
			moveVec.z += cosf(m_rot.y + m_dest.y) * m_fSpeed;

			//// �^���x�N�g���v�Z
			//m_move.x += sinf(m_rot.y + m_dest.y) * m_fSpeed;
			//m_move.z += cosf(m_rot.y + m_dest.y) * m_fSpeed;

			// ��]�̕␳
			//CTakaseiLibrary::RotRevision(&m_dest);

			// �v���C���[�����X�ɉ�]�����Ă���
			m_rot.y += m_dest.y * ROT_AMOUNT;

#ifdef _DEBUG
			CDebugProc::Log("�^���x�N�g���F%f, %f, %f\n", moveVec.x, moveVec.y, moveVec.z);
#endif
		}

		// �v���C���[�������Ă��Ȃ��Ƃ�
		if (fabs(m_move.x) <= 2 && fabs(m_move.z) <= 2)
		{
			// �ړ��s��
			m_bMove = false;
		}

		// �ړ��ʐݒ�
		m_move.x += (moveVec.x + aVec.x) / 2;
		m_move.z += (moveVec.z + aVec.z) / 2;

		// ��]�̕␳
		CTakaseiLibrary::RotRevision(&m_rot);

		// ��]�̐ݒ�
		SetRotation(m_rot);

		//CDebugProc::Log("�v���C���[��]���F%f\n", m_rot.y);
		//CDebugProc::Log("�X�s�[�h�F%f\n", m_fSpeed);
		//CDebugProc::Log("�ړ��ʁF%f, %f, %f\n", m_move.x, m_move.y, m_move.z);
	}

#ifdef _DEBUG
	if (pKeyboard->GetTriggerKeyboard(DIK_K))
	{// K�������ꂽ�Ƃ�
		m_fDeathblow = 50.0f;				// �K�E�Z�|�C���g���ő�l�܂ŏグ��
	}
	{// K�������ꂽ�Ƃ�
		m_fDeathblow = 50.0f;				// �K�E�Z�|�C���g���ő�l�܂ŏグ��
	}

#endif
}

//=============================================================================
// �ǂ̓����蔻��
//=============================================================================
bool CPlayer::CollisionWall(void)
{
	CScene *pSceneNext = NULL;														// ������
	CScene *pSceneNow = CScene::GetScene(CScene::PRIORITY_WALL);					// �擪�A�h���X�̎擾
	D3DXVECTOR3 normal = D3DXVECTOR3_ZERO;

	// �����Ȃ��Ȃ�܂ŌJ��Ԃ�
	while (pSceneNow != NULL)
	{
		pSceneNext = CScene::GetSceneNext(pSceneNow, CScene::PRIORITY_WALL);		//����A�b�v�f�[�g�Ώۂ��T����
		CMeshWall *pMeshWall = (CMeshWall*)pSceneNow;

		if (pMeshWall->GetWallHit(this, normal))
		{
			D3DXVECTOR3 playerPos = GetPosition();
			//playerPos.y += 5.0f;

			OutputDebugString("��������");
			// //�������ԂȂ̂ŁA���点��
			D3DXVECTOR3 move;
			//move = CManager::Slip(playerPos + m_move, normal);// ����x�N�g�����v�Z
			//D3DXVec3Normalize(&move, &move);
			//move *= m_fSpeed;
			//SetPosition(GetPosition() + move);

			//CManager::calcWallScratchVector(&move, m_move, normal);
			CManager::calcReflectVector(&move, m_move, normal);
			m_move = move * 20;
			return true;
		}

		pSceneNow = pSceneNext;								//����A�b�v�f�[�g�Ώۂ��i�[
	}

	return false;
}

//=============================================================================
// ���C�ɂ��ǂ̓����蔻��
//=============================================================================
bool CPlayer::CollisionWallWithRay(void)
{
	FLOAT fDistance = 0;
	D3DXVECTOR3 vNormal;

	D3DXVECTOR3 rot = CManager::GetCamera()->GetRotation();
	D3DXVECTOR3 endPoint;
	endPoint.x = sinf(D3DX_PI * 1.0f + rot.y);
	endPoint.y = 0.0f;
	endPoint.z = cosf(D3DX_PI * 1.0f + rot.y);

	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 playerPos = pos;
	CModel *pModel = GetModel();
	playerPos.y += 15.0f;

	CDebugProc::Log("�����x : %.2f %.2f %.2f\n", m_move.x, m_move.y, m_move.z);
	CDebugProc::Log("�n�_ : %.2f %.2f %.2f\n", pos.x, pos.y, pos.z);
	CDebugProc::Log("�I�_ : %.2f %.2f %.2f\n", playerPos.x + m_move.x, playerPos.y + m_move.y, playerPos.z + m_move.z);

	if (CObject::Collide(playerPos, m_move, &fDistance, &vNormal, pModel[0].GetMtxWorld()) && fDistance <= 30.0f)
	{
		// �������ԂȂ̂ŁA���点��
		D3DXVECTOR3 move = CManager::Slip(m_move, vNormal);// ����x�N�g�����v�Z
		//D3DXVec3Normalize(&move, &move);
		//move *= m_fSpeed;
		m_move = move;

		// ����x�N�g����̕ǂƂ̃��C���� �Q�d�ɔ���
		if (CObject::Collide(playerPos, playerPos + endPoint, &fDistance, &vNormal, pModel[0].GetMtxWorld()) && fDistance <= 20.0f)
		{
			m_move = D3DXVECTOR3_ZERO;//�~�߂�
		}

		return true;
	}

	CDebugProc::Log("���� : %.2f", fDistance);

	return false;
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
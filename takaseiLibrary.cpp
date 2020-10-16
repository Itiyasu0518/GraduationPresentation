//=============================================================================
//
//	���C�u�������� [ kananlibrary.cpp ]
// Author : KANAN NAGANAWA
//
//=============================================================================
#include "takaseiLibrary.h"
#include "manager.h"
#include "renderer.h"
#include "scene2D.h"
#include "scene3D.h"
#include "sceneX.h"
#include "inputKeyboard.h"
#include "inputController.h"
#include "light.h"
#include "game.h"

//=============================================================================
// �}�g���b�N�X�v�Z
//=============================================================================
void CTakaseiLibrary::CalcMatrix(D3DXMATRIX * pMtx, const D3DXVECTOR3 & pos, const D3DXVECTOR3 & rot)
{
	D3DXMATRIX	mtxRot, mtxTrans;			//�v�Z�p

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(pMtx);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(pMtx, pMtx, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(pMtx, pMtx, &mtxTrans);
}

//=============================================================================
// ��]�̕␳
//=============================================================================
void CTakaseiLibrary::RotRevision(D3DXVECTOR3 * rot)
{
	// ����D3DX_PI���傫���Ƃ�
	if (rot->y > D3DX_PI)
	{
		// ����߂�
		rot->y -= D3DX_PI * 2;
	}
	else if (rot->y < -D3DX_PI)
	{// ����-D3DX_PI��菬�����Ƃ�
		// ������₷
		rot->y += D3DX_PI * 2;
	}

	// ��Βl��0.0f��菬�����Ƃ�
	if (fabsf(rot->y) < 0.0f)
	{
		// �����Ȃ���
		rot->y = 0.0f;
	}
}

//=============================================================================
// �������v�Z
//=============================================================================
float CTakaseiLibrary::OutputSqrt(D3DXVECTOR3 difpos)
{
	float fSqrt = sqrt(difpos.x * difpos.x + difpos.y * difpos.y + difpos.z * difpos.z);
	return fSqrt;
}

//=============================================================================
// �x�N�g���v�Z
//=============================================================================
D3DXVECTOR3 CTakaseiLibrary::OutputVector(const D3DXVECTOR3 & start, const D3DXVECTOR3 & end)
{
	// �x�N�g����ۑ�����ϐ�
	D3DXVECTOR3 vector = end - start;
	// �x�N�g���𐳋K��
	D3DXVec3Normalize(&vector, &vector);
	// �x�N�g����Ԃ�
	return vector;
}

//=============================================================================
// �r���{�[�h������
//=============================================================================
void CTakaseiLibrary::Billboard(D3DXMATRIX *mtxWorld, const D3DXMATRIX mtxView)
{
	mtxWorld->_11 = mtxView._11;
	mtxWorld->_12 = mtxView._21;
	mtxWorld->_13 = mtxView._31;
	mtxWorld->_21 = mtxView._12;
	mtxWorld->_22 = mtxView._22;
	mtxWorld->_23 = mtxView._32;
	mtxWorld->_31 = mtxView._13;
	mtxWorld->_32 = mtxView._23;
	mtxWorld->_33 = mtxView._33;
}

//=============================================================================
// �����
//=============================================================================
HRESULT CTakaseiLibrary::Up(CInputKeyboard * Key, CInputController * pGamepad)
{
	// �L�[�{�[�h����
	if (Key && Key->GetTriggerKeyboard(DIK_W))
		return S_OK;

	// �Q�[���p�b�h�ڑ��m�F
	if (pGamepad->GetJoypadUse(0))
	{
		// ���X�e�B�b�N�擾
		float fValueX, fValueZ;
		pGamepad->GetJoypadStickLeft(0, &fValueX, &fValueZ);

		// �X�e�B�b�N����
		if (fValueX <= 1.0f && fValueZ > 0.0f)
			return S_OK;

		// �L�[����
		if (pGamepad->GetControllerTrigger(0, JOYPADKEY_UP))
			return S_OK;
	}

	// ���͖���
	return E_FAIL;
}

//=============================================================================
// ������
//=============================================================================
HRESULT CTakaseiLibrary::Down(CInputKeyboard * Key, CInputController * pGamepad)
{
	// �L�[�{�[�h����
	if (Key && Key->GetTriggerKeyboard(DIK_S))
		return S_OK;

	// �Q�[���p�b�h�ڑ��m�F
	if (pGamepad->GetJoypadUse(0))
	{
		// ���X�e�B�b�N�擾
		float fValueX, fValueZ;
		pGamepad->GetJoypadStickLeft(0, &fValueX, &fValueZ);

		// �X�e�B�b�N����
		if (fValueX >= -1.0f && fValueZ < 0.0f)
			return S_OK;

		// �L�[����
		if (pGamepad->GetControllerTrigger(0, JOYPADKEY_DOWN))
			return S_OK;
	}

	// ���͖���
	return E_FAIL;
}

//=============================================================================
// ������
//=============================================================================
HRESULT CTakaseiLibrary::Left(CInputKeyboard * Key, CInputController * pGamepad)
{
	// �L�[�{�[�h����
	if (Key && Key->GetTriggerKeyboard(DIK_A))
		return TRUE;

	// �Q�[���p�b�h�ڑ��m�F
	if (pGamepad->GetJoypadUse(0))
	{
		// ���X�e�B�b�N�擾
		float fValueX, fValueZ;
		pGamepad->GetJoypadStickLeft(0, &fValueX, &fValueZ);

		// �X�e�B�b�N����
		if (fValueX < 0 && fValueZ >= -1.0f)
			return TRUE;

		// �L�[����
		if (pGamepad->GetControllerTrigger(0, JOYPADKEY_LEFT))
			return TRUE;
	}

	// ���͖���
	return FALSE;
}

//=============================================================================
// �E����
//=============================================================================
HRESULT CTakaseiLibrary::Right(CInputKeyboard * Key, CInputController * pGamepad)
{
	// �L�[�{�[�h����
	if (Key && Key->GetTriggerKeyboard(DIK_D))
		return TRUE;

	// �Q�[���p�b�h�ڑ��m�F
	if (pGamepad->GetJoypadUse(0))
	{
		// ���X�e�B�b�N�擾
		float fValueX, fValueZ;
		pGamepad->GetJoypadStickLeft(0, &fValueX, &fValueZ);

		// �X�e�B�b�N����
		if (fValueX < 0 && fValueZ <= 1.0f)
			return TRUE;

		// �L�[����
		if (pGamepad->GetControllerTrigger(0, JOYPADKEY_RIGHT))
			return TRUE;
	}

	// ���͖���
	return FALSE;
}

//=============================================================================
// �������
//=============================================================================
HRESULT CTakaseiLibrary::Decide(CInputKeyboard * Key, CInputController * pGamepad)
{
	// �L�[�{�[�h����
	if (Key && Key->GetTriggerKeyboard(DIK_RETURN))
		return S_OK;

	// �Q�[���p�b�h�ڑ��m�F
	if (pGamepad->GetJoypadUse(0))
	{
		// �X�e�B�b�N����
		if (pGamepad->GetControllerTrigger(0, JOYPADKEY_A))
			return S_OK;
	}

	// ���͖���
	return E_FAIL;
}

//=============================================================================
// �߂����
//=============================================================================
HRESULT CTakaseiLibrary::Return(CInputKeyboard * Key, CInputController * pGamepad)
{
	// �L�[�{�[�h����
	if (Key && Key->GetTriggerKeyboard(DIK_BACKSPACE))
		return S_OK;

	// �Q�[���p�b�h�ڑ��m�F
	if (pGamepad->GetJoypadUse(0))
	{
		// �X�e�B�b�N����
		if (pGamepad->GetControllerTrigger(0, JOYPADKEY_B))
			return S_OK;
	}

	// ���͖���
	return E_FAIL;
}

//=============================================================================
// �|�[�Y����
//=============================================================================
HRESULT CTakaseiLibrary::Pause(CInputKeyboard * Key, CInputController * pGamepad)
{
	// �L�[�{�[�h����
	if (Key && Key->GetTriggerKeyboard(DIK_P))
		return TRUE;

	// �Q�[���p�b�h�ڑ��m�F
	if (pGamepad->GetJoypadUse(0))
	{
		// �X�e�B�b�N����
		if (pGamepad->GetControllerTrigger(0, JOYPADKEY_START))
			return TRUE;
	}

	// ���͖���
	return FALSE;
}

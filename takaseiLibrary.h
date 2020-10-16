//=============================================================================
//
// ���C�u�������� [ takaseiLbrary.h ]
// Author : Seiya Takahashi
//
//=============================================================================
#ifndef _TAKASEILIBRARY_H_
#define _TAKASEILIBRARY_H_

#define _CRT_SECURE_NO_WARNINGS // �x������

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"

//=============================================================================
// �}�N����`
//=============================================================================
// �x�N�g��
#define ZeroVector3		(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
#define ZeroVector4		(D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f))

// �J���[
#define ZeroColor		(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f))		// ���F
#define AlphaColor		(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f))		// ����
#define NormalColor		(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f))		// �ʏ�
#define BlackColor		(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f))		// ��

// ����
#define ONE_SECOND_FPS		(60)								// ��b���̃t���[����

//=============================================================================
// �O���錾
//=============================================================================
class CInputKeyboard;
class CInputController;

//=============================================================================
// �\���̒�`
//=============================================================================
typedef enum
{	// �ړ��p�̃L�[
	MOVE_ACCEL = DIK_W,		// �A�N�Z������
	MOVE_BRAKE = DIK_S,		// �u���[�L����
	MOVE_LEFT  = DIK_A,		// ������
	MOVE_RIGHT = DIK_D,		// �E����
	MOVE_JUMP  = DIK_SPACE,	// �W�����v����
} PLAYER_MOVE_KEY;

//=============================================================================
// �N���X��`
//=============================================================================
class CTakaseiLibrary
{
public:
	CTakaseiLibrary() {};
	~CTakaseiLibrary() {};

	// �����F�}�g���b�N�X�v�Z
	// [pMtx] �}�g���b�N�X���
	// [pos]  �ʒu
	// [rot]  ��]
	static void CalcMatrix(D3DXMATRIX *pMtx, const D3DXVECTOR3 & pos, const D3DXVECTOR3 & rot);

	// �����F��]�̕␳
	// [rot] ��]
	static void RotRevision(D3DXVECTOR3 *rot);

	// �����F�r���{�[�h��
	// [mtxWorld] �}�g���b�N�X���
	// [mtxView]  �}�g���b�N�X�r���[
	static void Billboard(D3DXMATRIX *mtxWorld, const D3DXMATRIX mtxView);

	// �����F�������v�Z
	// [difpos] �ʒu
	static float OutputSqrt(D3DXVECTOR3 difpos);

	// �����F��̓_����x�N�g���Z�o
	// [start] �n�_
	// [ned]   �I�_
	static D3DXVECTOR3 OutputVector(const D3DXVECTOR3 & start, const D3DXVECTOR3 & end);

	static HRESULT Up(CInputKeyboard	*Key, CInputController *pGamepad);	// �����
	static HRESULT Down(CInputKeyboard	*Key, CInputController *pGamepad);	// ������
	static HRESULT Left(CInputKeyboard	*Key, CInputController *pGamepad);	// ������
	static HRESULT Right(CInputKeyboard *Key, CInputController *pGamepad);	// �E����
	static HRESULT Decide(CInputKeyboard *Key, CInputController *pGamepad);	// �������
	static HRESULT Return(CInputKeyboard *Key, CInputController *pGamepad);	// �߂����
	static HRESULT Pause(CInputKeyboard *Key, CInputController *pGamepad);	// �|�[�Y����

private:

};
#endif
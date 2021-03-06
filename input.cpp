//=============================================================================
//
// 入力処理 [input.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "input.h"

//=============================================================================
// プロトタイプ宣言
//=============================================================================

//=============================================================================
// 静的メンバ変数
//=============================================================================
LPDIRECTINPUT8 CInput::m_pInput = NULL;

//=============================================================================
// コンストラクタ
//=============================================================================
CInput::CInput()
{
	m_pDevice = NULL;		// デバイスの初期化
}

//=============================================================================
// デストラクタ
//=============================================================================
CInput::~CInput()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	// DirectInputオブジェクトの作成
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
// 開放処理
//=============================================================================
void CInput::Uninit(void)
{

}

//=============================================================================
// 更新処理
//=============================================================================
void CInput::Update(void)
{

}
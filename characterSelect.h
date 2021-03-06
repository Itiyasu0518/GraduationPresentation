#pragma once
//=============================================================================
//
// キャラ選択処理 [characterSelect.h]
// Author : Noriyuki Hanzawa
//
//=============================================================================
#ifndef _CHARACTERSELECT_H_
#define _CHARACTERSELECT_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"
#include "scene2D.h"

#define MAX_SELECT_UI (8)

//=============================================================================
// クラス定義
//=============================================================================
class CCharacterSelect
{
public:
	CCharacterSelect();									// コンストラクタ
	~CCharacterSelect();								// デストラクタ
	HRESULT Init(void);									// 初期化処理
	void Uninit(void);									// 開放処理
	void Update(void);									// 更新処理
	void Draw(void);									// 描画処理
	void SetCarType(int type)	 { m_nCarType = type; };// セット
	int GetCarType(void)		 {return m_nCarType; };	// 車の種類取得
	static void LoadAsset(void);						// アセットの読み込み処理

private:
	CScene2D *pBack[MAX_SELECT_UI];					// UI
	int m_nCarType;								// 車の種類
};
#endif
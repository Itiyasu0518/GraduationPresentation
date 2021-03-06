//=============================================================================
//
// 背景処理 [bg.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "bg.h"
#include "manager.h"
#include "renderer.h"

//==============================================================================
// コンストラクタ
//==============================================================================
CBg::CBg(CScene::PRIORITY obj = CScene::PRIORITY_BG) : CScene2D(obj)
{
	SetObjType(PRIORITY_BG);	//オブジェクトタイプの設定
}

//=============================================================================
// デストラクタ
//=============================================================================
CBg::~CBg()
{

}

//==============================================================================
// 初期化処理
//==============================================================================
HRESULT CBg::Init(void)
{
	// 初期化
	CScene2D::Init();

	BindTexture("data/tex/back.png");			// 背景テクスチャのポインタ を渡す

	SetCenter(CScene2D::TEXTUREVTX_CENTER);		// 中心の設定

	SetSize(D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f));	// 大きさの設定
	SetPosition(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));	// 位置の設定
	SetTransform();
	return S_OK;
}

//=============================================================================
// 開放処理
//=============================================================================
void CBg::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CBg::Update(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void CBg::Draw(void)
{
	// 描画処理
	CScene2D::Draw();
}

//==============================================================================
// 生成処理
//==============================================================================
CBg *CBg::Create(void)
{
	CBg *pBg;		// 背景のポインタ

	pBg = new CBg(CScene::PRIORITY_BG);		// 背景の生成
	pBg->Init();							// 背景の初期化
	return pBg;
}

//==============================================================================
// アセットの生成処理
//==============================================================================
HRESULT CBg::Load(void)
{
	//テクスチャの読み込み
	CManager::Load("data/tex/back.png");
	return S_OK;
}
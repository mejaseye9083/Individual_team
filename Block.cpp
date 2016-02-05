/*===========================================================================================
概　要:ブロックとなる基本の設定
作成日:2015.10.26
更新日:2015.12.03
制作者:藤原順基
=============================================================================================*/

//---------------インクルード-----------------------
#include "Block.h"
#include "Camera.h"

//----------------------------------
//機能：コンストラクタ
//引数：なし
//戻値：なし
//----------------------------------
Block::Block()
{
	Number = 0;

	blPoint = BLOCK_CHIP;
}

//----------------------------------
//機能：デストラクタ
//引数：なし
//戻値：なし
//----------------------------------
Block::~Block()
{
}

//----------------------------------
//機能：画像・音楽データの読み込み
//引数：なし
//戻値：成功or失敗
//----------------------------------
HRESULT Block::Load(char* filename)
{

	Wall.Load(filename);
	return S_OK;
}

//----------------------------------
//機能：更新処理
//引数：なし
//戻値：成功
//----------------------------------
HRESULT Block::Update()
{
	return S_OK;
}

//----------------------------------
//機能：当たり判定
//引数：pTarget		敵の情報
//戻値：成功
//----------------------------------
HRESULT Block::Hit(UnitBase* pTarget)
{

	return S_OK;
}

//----------------------------------
//機能：描画処理
//引数：なし
//戻値：成功or失敗
//----------------------------------
HRESULT Block::Render()
{
	return S_OK;
}

//----------------------------------
//機能：外部から呼ぶ描画処理
//引数：なし
//戻値：成功or失敗
//----------------------------------
HRESULT Block::Drow(D3DXVECTOR3 pos, int table)
{
	if (table == 1||table == 2)
	{
		HitZone.left = 0 + pos.x;
		HitZone.top = 0 + pos.y;

		HitZone.right = 32 + pos.x;
		HitZone.bottom = 32 + pos.y;
	}
	else if (table || 0)
	{
		HitZone = { 0, 0, 0, 0 };
	}

	//ステージを構築するブロックの配置
	SpriteData WallData;
	/*
	WallData.size.x = size.x;
	WallData.size.y = size.y;
	*/

	//現状：表示しない部分は消す
	
	WallData.size.x = blPoint;
	WallData.size.y = blPoint;
	
	WallData.cut.x = BLOCK_CHIP*table;

	//キャラの動きに合わせて画面が動くので、ステージの位置も変える
	WallData.pos.x = pos.x + g_stageScrollPosition.x;
	WallData.pos.y = pos.y + g_stageScrollPosition.y;

	Wall.Draw(&WallData);
	return S_OK;
}

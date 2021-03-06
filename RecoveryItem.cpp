/*===========================================================================================
概　要:回復アイテムや残機アップアイテム等
作成日:2016.01.26
更新日:2016.01.26
制作者:藤原順基
===========================================================================================*/

//---------------インクルード-----------------------
#include "RecoveryItem.h"

//----------------------------------
//機能：コンストラクタ
//引数：なし
//戻値：なし
//----------------------------------
RecoveryItem::RecoveryItem()
{
}

//----------------------------------
//機能：デストラクタ
//引数：なし
//戻値：なし
//----------------------------------
RecoveryItem::~RecoveryItem()
{
}

//----------------------------------
//機能：画像・音楽データの読み込み
//引数：なし
//戻値：成功or失敗
//----------------------------------
HRESULT RecoveryItem::Load(char *filename)
{
	if (FAILED(Recover_Aid.Load(filename)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//----------------------------------
//機能：更新処理
//引数：なし
//戻値：成功
//----------------------------------
HRESULT RecoveryItem::Update()
{
	return S_OK;
}

//----------------------------------
//機能：当たり判定
//引数：pTarget		敵の情報
//戻値：成功
//----------------------------------
HRESULT RecoveryItem::Hit(UnitBase* pTarget)
{
	return S_OK;
}

//----------------------------------
//機能：描画処理
//引数：なし
//戻値：成功or失敗
//----------------------------------
HRESULT RecoveryItem::Render()
{
	return S_OK;
}

//----------------------------------
//機能：外部から呼ぶ描画処理
//引数：なし
//戻値：成功or失敗
//----------------------------------
HRESULT RecoveryItem::Drow(D3DXVECTOR3 pos)
{

	SpriteData Recover;

	//キャラの動きに合わせて画面が動くので、ステージの位置も変える
	Recover.pos.x = pos.x + g_stageScrollPosition.x;
	Recover.pos.y = pos.y + g_stageScrollPosition.y;

	Recover_Aid.Draw(&Recover);

	position = pos;

	return S_OK;
}

//testcom2
/*===========================================================================================
概　要:剣で攻撃するクラス
作成日:2016.02.24
更新日:2016.02.24
制作者:藤原順基
=============================================================================================*/

//---------------インクルード-----------------------
#include "Bread.h"
#include "Camera.h"

//----------------------------------
//機能：コンストラクタ
//引数：なし
//戻値：なし
//----------------------------------
Bread::Bread()
{
	//未発射段階
	isbread = FALSE;

	breadlife = NULL;

	//スプライト、2Dエフェクト、オーディオクラスの宣言
	spt = new Sprite;
	fvx = new Effect2D;
}

//----------------------------------
//機能：デストラクタ
//引数：なし
//戻値：なし
//----------------------------------
Bread::~Bread()
{

	//ポインタのデリート処理、最優先で書く
	//newを書いたらすぐ書くくらいに意識する
	SAFE_DELETE(spt);
}

//----------------------------------
//機能：画像・音楽データの読み込み
//引数：なし
//戻値：成功or失敗
//----------------------------------
HRESULT Bread::Load()
{
	//各種画像、エフェクト、音楽読み込み
	if (FAILED(spt->Load("pictures\\bread.png")))
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
HRESULT Bread::Update(D3DXVECTOR3 playerPos)
{

	if (isbread == TRUE)
	{	//自分の位置 = 渡されたプレイヤーの位置
		switch (dirflg)
		{
		case MS_RIGHT:
			position.x = playerPos.x + 25;
			break;

		case MS_LEFT:
			position.x = playerPos.x - 25;
			break;
		}
		position.y = playerPos.y - 8;
		breadlife++;
	}

	//弾の生存時間:4秒弱
	if (breadlife == 0xF)
	{
		Reset();
	}

	//不具合でそうだけどとりあえずok
	return S_OK;
}

//----------------------------------
//機能：当たり判定
//引数：pTarget		敵の位置情報
//戻値：TRUEorFALSE
//----------------------------------
BOOL Bread::Hit(UnitBase *pTarget)
{
	if (typeid(*pTarget) == typeid(Enemy))
	{
		Enemy *pEnemy = (Enemy*)pTarget;
		if (isbread == TRUE)
		{
			//敵の位置をGetPosを通して受け取る
			enemyPosition = pEnemy->GetPos();
			//位置の計算を行う
			int distance =
				(int)(position.x - (enemyPosition.x + g_stageScrollPosition.x)) *
				(int)(position.x - (enemyPosition.x + g_stageScrollPosition.x)) +
				(int)(position.y - (enemyPosition.y + g_stageScrollPosition.y)) *
				(int)(position.y - (enemyPosition.y + g_stageScrollPosition.y));			//(0-0)*(0-0)+(0-0)*(0-0) =((0-0)*(0-0)) + ((0-0)*(0-0)) 

			if (distance <= (28 * 48))
			{
				//弾が当たったら弾を消す
				Reset();
				pEnemy->Kill();
				return TRUE;
			}
		}
	}

	return FALSE;
}

//----------------------------------
//機能：描画処理
//引数：なし
//戻値：成功
//----------------------------------
HRESULT Bread::Render()
{
	SpriteData data;
	//弾の描画を行う
	if (isbread == TRUE)
	{
		data.pos = position;

		data.size = D3DXVECTOR2(28, 48);

		switch (dirflg)
		{
		case MS_RIGHT:
			data.cut.x = 0*28;
			break;

		case MS_LEFT:
			data.cut.x = 1 * 28;
		}

		spt->Draw(&data);
	}
	return S_OK;
}

//----------------------------------
//機能：剣で切る動作
//引数：なし
//戻値：TRUEorFALSE
//----------------------------------
BOOL Bread::bread(D3DXVECTOR3 playerPos, int dir)
{
	//未発射のとき
	if (isbread == FALSE)
	{
		isbread = TRUE;
		dirflg = dir;

		//自分の位置 = 渡されたプレイヤーの位置
		switch (dir)
		{
		case MS_RIGHT:
			position.x = playerPos.x + 25;
			break;

		case MS_LEFT:
			position.x = playerPos.x - 25;
			break;

		default:
			isbread = FALSE;
			break;
		}

		position.y = playerPos.y-8;
		return TRUE;
	}


	return FALSE;
}

/*-----------------------------問題点------------------------
現状ポジションを受け取っている位置からバレットが飛ぶようになっている
つまりカメラが移動してるときに中央にキャラを捉えている際に追従して上に跳ぶ
このままだとバレットがキャラと同調した動きなのですごく不自然
------------------------------------------------------------*/

//----------------------------------
//機能：発射した弾のフラグを初期値に戻す関数
//引数：なし
//戻値：TRUEorFALSE
//----------------------------------
void Bread::Reset()
{
	isbread = FALSE;
	breadlife = NULL;
}
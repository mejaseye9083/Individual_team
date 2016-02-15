/*===========================================================================================
概　要:弾の発射したときの軌道
作成日:2015.06.26
更新日:2016.02.15
制作者:藤原順基
=============================================================================================*/

//---------------インクルード-----------------------
#include "Bullet.h"
#include "Camera.h"

//----------------------------------
//機能：コンストラクタ
//引数：なし
//戻値：なし
//----------------------------------
Bullet::Bullet()
{
	//未発射段階
	isShot = FALSE;

	bulletlife = NULL;

	//スプライト、2Dエフェクト、オーディオクラスの宣言
	spt = new Sprite;
	fvx = new Effect2D;
}

//----------------------------------
//機能：デストラクタ
//引数：なし
//戻値：なし
//----------------------------------
Bullet::~Bullet()
{

	//ポインタのデリート処理、最優先で書く
	//newを書いたらすぐ書くくらいに意識する
	SAFE_DELETE(spt);
	SAFE_DELETE(fvx);
}

//----------------------------------
//機能：画像・音楽データの読み込み
//引数：なし
//戻値：成功or失敗
//----------------------------------
HRESULT Bullet::Load()
{
	//各種画像、エフェクト、音楽読み込み
	if (FAILED(spt->Load("pictures\\RockB.png")))
	{
		return E_FAIL;
	}

	if (FAILED(fvx->Load("SamplePict\\bakuhatu.png", D3DXVECTOR2(128, 128), 0.2f)))
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
HRESULT Bullet::Update()
{
	//右向きの時は右に飛ばしっぱなしにしたい
	if (dirflg == MS_RIGHT)
	{
		//ここでこの弾が向きの影響を受けないようにしたい
		unControl = TRUE;
		if (unControl == TRUE)
		{
			//弾の飛ぶ速度
			position.x += 8;
			//弾の消失位置
			//if (position.x > 640)

			//{
			//	//弾を未発射に戻す
			//	isShot = FALSE;
			//	//フラグを取り消して再度入力を待つ
			//	unControl = FALSE;
			//}
			bulletlife++;

		}
	}

	//左も同様に飛ばしっぱなしにする！
	if (dirflg == MS_LEFT)
	{
		//ここでこの弾が向きの影響を受けないようにしたい
		unControl = TRUE;
		if (unControl == TRUE)
		{
			//弾の飛ぶ速度
			position.x -= 8;
			//弾の消失位置
			//if (position.x < -40)

			//{
			//	//弾を未発射に戻す
			//	isShot = FALSE;
			//	//フラグを取り消して再度入力を待つ
			//	unControl = FALSE;
			//}

			bulletlife++;

		}
	}
	if (bulletlife == 0xF0)
	{
 	 	isShot = FALSE;
		unControl = FALSE;
		bulletlife = NULL;

	}
	//不具合でそうだけどとりあえずok
	return S_OK;
}

//----------------------------------
//機能：当たり判定
//引数：pEnemy		敵の位置情報
//戻値：TRUEorFALSE
//----------------------------------
BOOL Bullet::Hit(Enemy* pEnemy)
{
	if (isShot == TRUE)
	{
		//敵の位置をGetPosを通して受け取る
		D3DXVECTOR3 enemyPosition = pEnemy->GetPos();
		//位置の計算を行う
		int distance =
			(int)(position.x - (enemyPosition.x + g_stageScrollPosition.x)) *
			(int)(position.x - (enemyPosition.x + g_stageScrollPosition.x)) +
			(int)(position.y - (enemyPosition.y + g_stageScrollPosition.y)) *
			(int)(position.y - (enemyPosition.y + g_stageScrollPosition.y));			//(0-0)*(0-0)+(0-0)*(0-0) =((0-0)*(0-0)) + ((0-0)*(0-0)) 

		if (distance <= (24 * 24))
		{
			//弾が当たったら弾を消す
			isShot = FALSE;
			bulletlife = NULL;
			unControl = FALSE;

			fvx->Draw();
			pEnemy->Kill();
			return TRUE;
		}
	}
	return FALSE;
}

//----------------------------------
//機能：描画処理
//引数：なし
//戻値：成功
//----------------------------------
HRESULT Bullet::Render()
{
	SpriteData data;
	//弾の描画を行う
	if (isShot == TRUE)
	{
		data.pos = position;
 		spt->Draw(&data);
	}

	fvx->Draw();
	return S_OK;
}

//----------------------------------
//機能：弾が飛ぶ処理
//引数：なし
//戻値：TRUEorFALSE
//----------------------------------
BOOL Bullet::Shot(D3DXVECTOR3 playerPos, int dir)
{
	//未発射のとき
	if (isShot == FALSE)
	{
		isShot = TRUE;
		dirflg = dir;

		//自分の位置 = 渡されたプレイヤーの位置
		switch (dir)
		{
		case MS_RIGHT:
			position.x = playerPos.x +10;
			break;

		case MS_LEFT:
			position.x = playerPos.x -10;
			break;

		default:
			break;
		}
		position.y = playerPos.y+16;

		return TRUE;
	}
	return FALSE;
}

/*-----------------------------問題点------------------------
現状ポジションを受け取っている位置からバレットが飛ぶようになっている
つまりカメラが移動してるときに中央にキャラを捉えている際に追従して上に跳ぶ
このままだとバレットがキャラと同調した動きなのですごく不自然
------------------------------------------------------------*/
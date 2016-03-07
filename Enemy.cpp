/*===========================================================================================
概　要:敵の移動・攻撃パターン
作成日:2015.11.05
更新日:2016.03.01
制作者:藤原順基
=============================================================================================*/

//---------------インクルード-----------------------
#include "Enemy.h"
#include "Camera.h"
#include "Player.h"

//------------------定数宣言------------------------
#define GRAVITY			1		//重力
#define	INERTIA			0.06
#define	JUMP			-20
#define	JUMP_COUNT		1


Stage *Estg;

//----------------------------------
//機能：コンストラクタ
//引数：なし
//戻値：なし
//----------------------------------
Enemy::Enemy()
{
	position.x = 6*BLOCK_CHIP;
	position.y = 20*BLOCK_CHIP;

	jump = 0;
	jcount = 0;

	anime = 0;
	moveE = 7;

	direction = MS_RIGHT;
	spt = new Sprite;
	fvx = new Effect2D;
	asiba = GROUND_HEIGHT;

	//初期化した値をそのまま書いても問題はない
	FixedPosition.x = position.x;
	FixedPosition.y = position.y;
}

//----------------------------------
//機能：デストラクタ
//引数：なし
//戻値：なし
//----------------------------------
Enemy::~Enemy()
{
	//安全に消去する
	SAFE_DELETE(spt);
	SAFE_DELETE(Estg);
	SAFE_DELETE(fvx);
}

//----------------------------------
//機能：画像・音楽データの読み込み
//引数：なし
//戻値：成功or失敗
//----------------------------------
HRESULT Enemy::Load()
{
	//プレイヤー画像の読み込み
	if (FAILED(spt->Load("pictures\\Actor.png")))
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
HRESULT Enemy::Update()
{
	//プレイヤーがストップしてる間は敵もストップ！
	if (g_Stopflg == FALSE)
	{
		Move(Estg);
		if (state == LOOK_ENEMY)
		{
			Shot();
		}
	}
	return S_OK;
}

//----------------------------------
//機能：当たり判定
//引数：pTarget		敵の情報
//戻値：成功
//----------------------------------
HRESULT Enemy::Hit(UnitBase* pTarget)
{
	//-----------PlaySceneで呼び出したステージをstgに入れる---------
	if (typeid(*pTarget) == typeid(Stage))
	{
		Estg = (Stage*)pTarget;
	}

	if (typeid(*pTarget) == typeid(Player))
	{
		D3DXVECTOR3 playerPosition = pTarget->GetPos();
		RECT playerSize = pTarget->GetZone();

		//プレイヤーとエネミー間の距離を測る
		if (((HitZone.left - 50 <= playerSize.right) && (HitZone.right + 50 >= playerSize.left)) 
			&& ((HitZone.top - 60 <= playerSize.bottom ) && (HitZone.bottom + 30 >= playerSize.top)))
		{
			state = LOOK_ENEMY;
		}
		else 
		{
			state = DEFAULT;
		}
	}
	return S_OK;
}

//----------------------------------
//機能：描画処理
//引数：なし
//戻値：成功or失敗
//----------------------------------
HRESULT Enemy::Render()
{
	//-----------------------プレイヤーの描画-----------------------------
	SpriteData enemy;
	enemy.cut.x = ((anime / 10) % 4) * 32;
	enemy.cut.y = direction * 32;

	enemy.size = D3DXVECTOR2(32, 32);


	enemy.pos = position + g_stageScrollPosition;

	spt->Draw(&enemy);
	//--------------------------------------------------------------------
	return S_OK;
}

//----------------------------------
//機能：移動処理
//引数：なし
//戻値：成功or失敗
//----------------------------------
HRESULT Enemy::Move(Stage* stage)
{
	//-----------当たり判定を追従させる------------
	HitZone.left = 2 + position.x;
	HitZone.top = 0 + position.y;

	HitZone.right = 30 + position.x;
	HitZone.bottom = 32 + position.y;
	//---------------------------------------------

	//----------------------エネミーの移動------------------------------
	switch (state)
	{

		//---------------------通常移動----------------------
	case DEFAULT:
		
		//移動した先が自分の初期地点+40よりも移動したら反転させる
		if (position.x >= FixedPosition.x + 100)
		{
			//向きを逆転させる
			moveE = -7;
			direction = MS_LEFT;
		}
		//移動した先が自分の初期地点-40よりも移動又は壁にぶつかったら反転させる
		if (position.x <= FixedPosition.x - 100)
		{
			//向きを逆転させる
			moveE = 7;
			direction = MS_RIGHT;
		}

		//移動は常にさせるのでif文の外に書いておく
		position.x += moveE;

		break;

	case LOOK_ENEMY:
		//--------------------------ジャンプする------------------------------
		if ((jcount < JUMP_COUNT && isGround) && !jumpBlock)
		{
			jump = JUMP;
			jcount++;
			isGround = FALSE;
			ladderflg = FALSE;
		}

		//--------------------------------------------------------------------

		break;
	}
	//--------------------------------------------------------------------


	//isGroundがFALSEだったとき
	if (!isGround)
	{
		if (jump >= 10)
		{
			jump = 10;
		}
		position.y += jump;
		BMPIkun = (int)position.y;
	}

	//プレイヤーの左下か右下の位置にジャンプの値を足した位置のテーブルの値が1だった時の処理
	if (stage->GetChip((int)(HitZone.right) / BLOCK_CHIP, (int)(HitZone.bottom + jump) / BLOCK_CHIP) == 1
		|| stage->GetChip((int)(HitZone.left) / BLOCK_CHIP, (int)(HitZone.bottom + jump) / BLOCK_CHIP) == 1
		&& !isGround)
	{
		//ポジション から引く値は (1フレーム前の自分のポジションを32で割った余り) + 1
		position.y -= (BMPIkun % BLOCK_CHIP) + 1;
		isGround = TRUE;
		jump = 0;
		jcount = 0;
	}
	//isGroundがTRUEだった場合はジャンプをせずに足場がなくなったはず(1は足場or壁ブロック)
	//ジャンプせずに落下判定に入る場合の処理が下記の処理となる(自由落下)
	else if (stage->GetChip((int)(HitZone.right) / BLOCK_CHIP, (int)(HitZone.bottom + 2) / BLOCK_CHIP) == 0
		&& stage->GetChip((int)(HitZone.left) / BLOCK_CHIP, (int)(HitZone.bottom + 2) / BLOCK_CHIP) == 0
		&& !ladderflg)
	{
		isGround = FALSE;
		jump += GRAVITY;
	}

	//天井（と思われる場所）にぶつかった時
	if (stage->GetChip((int)(HitZone.right) / BLOCK_CHIP, (int)(HitZone.top + jump - 3) / BLOCK_CHIP) == 1
		|| stage->GetChip((int)(HitZone.left) / BLOCK_CHIP, (int)(HitZone.top + jump - 3) / BLOCK_CHIP) == 1)
	{
		jump = (jump * -1);
		jump += GRAVITY;
	}

	//-----------------------------------------

	
	return S_OK;
}

//----------------------------------
//機能：攻撃処理
//引数：なし
//戻値：成功or失敗
//----------------------------------
HRESULT Enemy::Shot()
{

	return S_OK;
}

//----------------------------------
//機能：攻撃を受けて倒されたら
//引数：なし
//戻値：なし
//----------------------------------
void Enemy::Kill()
{
	g_gameScene = SC_CLEAR;
}

/*===========================================================================================
概　要:自機の設定
作成日:2015.10.15
更新日:2016.02.02
制作者:藤原順基
=============================================================================================*/

//---------------インクルード-----------------------
#include "Player.h"
#include "Camera.h"
#include "Enemy.h"
#include "Block.h"

/*
 * 現在エラーが発生中、原因は下記のステージのポインタっぽい
 *
 *
 */

Stage *stg;

//------------------定数宣言------------------------
#define GRAVITY			1		//重力
#define	INERTIA			0.06
#define	JUMP			-12
#define	JUMP_COUNT		1

//----------------------------------
//機能：コンストラクタ
//引数：なし
//戻値：なし
//----------------------------------
Player::Player()
{
	/*position.x = 100;
	position.y = 300;*/

	/*jump = 0;
	jcount = 0;

	anime = 0;
	direction = MS_RIGHT;*/
	ply = new Sprite;
	life = new Sprite;

	//stg = new Stage;		//ここでnewしちゃいけない！（後述先：プレイヤーのHit関数)

	audio = new Audio;

	//asiba = 400;
	Reset();
}

//----------------------------------
//機能：デストラクタ
//引数：なし
//戻値：なし
//----------------------------------
Player::~Player()
{
	//安全に消去する
	SAFE_DELETE(ply);
	SAFE_DELETE(life);
	SAFE_DELETE(stg);
	SAFE_DELETE(audio);
}

//----------------------------------
//機能：画像・音楽データの読み込み
//引数：なし
//戻値：成功or失敗
//----------------------------------
HRESULT Player::Load()
{
	//プレイヤー画像の読み込み
	if (FAILED(ply->Load("SamplePict\\Actor.png")))
	{
		return E_FAIL;
	}

	//ライフメモリ画像の読み込み
	if (FAILED(life->Load("pictures\\LIFE.bmp")))
	{
		return E_FAIL;
	}

	
	//設定してる数まで弾を出せるようにする
	for (int i = 0; i < BULLET_SET; i++)
	{
		if (FAILED(bullet[i].Load()))
		{
			return E_FAIL;
		}
	}

	//SE関連の読み込み
	if (FAILED(audio->Load("songs\\SE\\Rock_BGM.xwb", "songs\\SE\\Rock_BGM.xsb")))
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
HRESULT Player::Update()
{
	//移動時に二次元配列のテーブルを利用する
	Move(stg);
	Shot();

	//自爆コマンド
	if (g_pInput->IsKeyTap(DIK_Y))
	{
		hp -= 2;
		memory_break = (PLAYER_HP - hp) * PLAYER_DM;
	}


	//Ｐキーを押すとゲームオーバーに移行する(確認用)
	if (hp <= 0)
	{
		Reset();
		g_gameScene = SC_GAMEOVER;
	}

	//弾の更新
	BulletUpdate();
	return S_OK;
}

//----------------------------------
//機能：当たり判定
//引数：pTarget		敵の情報
//戻値：成功
//----------------------------------
HRESULT Player::Hit(UnitBase* pTarget)
{
	//--------------------エネミーとの衝突判定----------------------
	//当たり判定を作る、X軸・Y軸・幅・高さが必要になる
	if (typeid(*pTarget) == typeid(Enemy))
	{
		D3DXVECTOR3 enemyPosition = pTarget->GetPos();
		RECT enemySize = pTarget->GetZone();
	
		//当たり判定を出すための計算
		int distance =
			(int)(position.x - enemyPosition.x) *
			(int)(position.x - enemyPosition.x) +
			(int)(position.y - enemyPosition.y) *
			(int)(position.y - enemyPosition.y);

		//衝突判定
		if (distance <= (PLAYER_SIZE * PLAYER_SIZE))
		{

		}

		for (int i = 0; i < BULLET_SET; i++)
		{
			//キャスト演算子を利用しUnitBase*からEnemy*に無理やり変えてしまう
			BOOL b = bullet[i].Hit((Enemy*)pTarget);

		}

	}
	//--------------------------------------------------------------


	//--------------------アイテムとの衝突判定----------------------
	//当たり判定を作る、X軸・Y軸・幅・高さが必要になる
	if (typeid(*pTarget) == typeid(item))
	{
		D3DXVECTOR3 itemPosition = pTarget->GetPos();
		RECT itemSize = pTarget->GetZone();

		//当たり判定を出すための計算
		int distance =
			(int)(position.x - itemPosition.x) *
			(int)(position.x - itemPosition.x) +
			(int)(position.y - itemPosition.y) *
			(int)(position.y - itemPosition.y);

		//衝突判定
		if (distance <= (PLAYER_SIZE * PLAYER_SIZE))
		{
			hp += 8;
			if (hp >= 20)
			{
				hp = 20;
			}
		}

	}
	//--------------------------------------------------------------


	//-----------PlaySceneで呼び出したステージをstgに入れる---------
	if (typeid(*pTarget) == typeid(Stage))
	{
		/*
		 * pTargetはUnitBaseのサブクラスならポインタを通じて確認が出来る
		 * つまり、PlaySceneでStageを呼び出しているので、それをstgに入れればよい！
		 * そうすれば2回呼ぶことでの不正な値の習得はなくなる。
		 *
		 * ～～～～～～～～～～～オマケ～～～～～～～～～～
		 * シングルトンという法則があり、知ることでnewをより効率的に使えるようになるらしい！
		 */
		stg = (Stage*)pTarget;
	}
	//--------------------------------------------------------------

	return S_OK;
}

//----------------------------------
//機能：描画処理
//引数：なし
//戻値：成功or失敗
//----------------------------------
HRESULT Player::Render()
{
	//-----------------------プレイヤーの描画-----------------------------
	SpriteData rock;
	rock.cut.x = ((anime / 10) % 4) * 32;
	rock.cut.y = direction * 32;

	rock.size = D3DXVECTOR2(32, 32);
	
	rock.pos = position + g_stageScrollPosition;

	ply->Draw(&rock);
	//--------------------------------------------------------------------

	//---------------------ライフメモリの描画-----------------------------
	SpriteData Life;
	Life.pos.x = 10;
	Life.pos.y = 20 + memory_break;
	Life.size.y = 160 - memory_break;
	if (Life.size.y <= 0)
	{
		Life.size.y = 0;
	}

	life->Draw(&Life);
	//--------------------------------------------------------------------

	//------------------弾もちゃんと描画処理を書く！----------------------
	//弾が最大数に達したか達してないか
	for (int i = 0; i < BULLET_SET; i++)
	{
		//達してなかったら発射可能
		if (FAILED(bullet[i].Render()))
		{
			return E_FAIL;
		}
	}
	//--------------------------------------------------------------------
	return S_OK;
}

//----------------------------------
//機能：自機の移動
//引数：なし
//戻値：成功
//----------------------------------
HRESULT Player::Move(Stage* stage)
{
	//-----------当たり判定を追従させる------------
	HitZone.left = 0 + position.x;
	HitZone.top = 0 + position.y;

	HitZone.right = 32 + position.x;
	HitZone.bottom = 32 + position.y;
	//---------------------------------------------

	int moveS = 4;
	//----------------------プレイヤーの移動------------------------------
	switch (state)
	{

		//---------------------通常移動----------------------
	case DEFAULT:
		/*
		 *引数で受け取ったStageの中にステージの二次元配列があるので、それを受け取る
		 */

		//右移動の処理
		if (g_pInput->IsKeyPush(DIK_RIGHT) && !ladderflg)
		{

			if (stage->GetChip((int)(HitZone.right + moveS) / BLOCK_CHIP, (int)(HitZone.bottom) / BLOCK_CHIP) == 1
				|| stage->GetChip((int)(HitZone.right + moveS) / BLOCK_CHIP, (int)(HitZone.top) / BLOCK_CHIP) == 1)
			{
				moveS = 0;	//0にする
			}
			
			position.x += moveS;
			direction = MS_RIGHT;
			anime++;
		}

		//左移動の処理(旧判定)
		if (g_pInput->IsKeyPush(DIK_LEFT) && !ladderflg)
		{

			if (stage->GetChip((int)(HitZone.left - moveS) / BLOCK_CHIP, (int)(HitZone.bottom) / BLOCK_CHIP) == 1 
				|| stage->GetChip((int)(HitZone.left - moveS) / BLOCK_CHIP, (int)(HitZone.top) / BLOCK_CHIP) == 1)
			{
				moveS = 0;	//移動速度を0にする
			}

			position.x -= moveS;
			direction = MS_LEFT;
			anime++;
		}

		//------------------要検証---------------------
		/*if (g_pInput->IsKeyPush(DIK_DOWN))
		{
			if (stage->GetChip((int)(position.x + PLAYER_SIZE) / BLOCK_CHIP, (int)(position.y + JUMP + PLAYER_SIZE) / BLOCK_CHIP) == 2
				|| stage->GetChip((int)position.x / BLOCK_CHIP, (int)(position.y - JUMP) / BLOCK_CHIP) == 2)
			{
				if (stage->GetChip((int)((HitZone.right - HitZone.left) + position.x) / BLOCK_CHIP, (int)(HitZone.bottom) / BLOCK_CHIP) == 1)
				{
					position.y += 0;
					direction = MS_UP;
					isGround = TRUE;
					jcount = 0;
				}
				else
				{
					position.y += moveS;
					direction = MS_UP;
					isGround = TRUE;
					jcount = 0;
				}
			}
		}*/

		
		//梯子での移動(暫定版)
		if (stage->GetChip((int)(HitZone.right - 12) / BLOCK_CHIP, (int)((HitZone.bottom - HitZone.top) + position.y) / BLOCK_CHIP) == 2
			|| stage->GetChip((int)(HitZone.left  + 12) / BLOCK_CHIP, (int)((HitZone.bottom - HitZone.top) + position.y) / BLOCK_CHIP) == 2)
		{
			if (g_pInput->IsKeyPush(DIK_DOWN))
			{

				ladderflg = TRUE;
				direction = MS_UP;
				isGround = TRUE;
				jcount = 0;

				if (stage->GetChip((int)((HitZone.right - HitZone.left) + position.x) / BLOCK_CHIP, (int)(HitZone.bottom + moveS) / BLOCK_CHIP) == 1)
				{
					moveS = 0;

				}
				
				if (stage->GetChip((int)((HitZone.right - HitZone.left) + position.x) / BLOCK_CHIP, (int)(HitZone.top) / BLOCK_CHIP) == 0)
				{
					
				}

				position.y += moveS;
				
			}

			if (g_pInput->IsKeyPush(DIK_UP))
			{
				direction = MS_UP;
				isGround = TRUE;
				ladderflg = TRUE;
				jcount = 0;

				if (stage->GetChip((int)((HitZone.right - HitZone.left) + position.x) / BLOCK_CHIP, (int)(HitZone.top + moveS) / BLOCK_CHIP) == 1)
				{
					moveS = 0;

				}

				position.y -= moveS;
			}
		}
		else
		{
			isGround = FALSE;
			ladderflg = FALSE;
		}

		/*
		 * 現在梯子移動は別々に判定させているが、多少長くなっても一つのif文にまとめる方が良さそう
		 * 後で修正するので要点のみ記載
		 *
		 */


		//プレイヤーが真ん中に来るよう、横スクロール位置を算出
		g_stageScrollPosition.x = -position.x + (WINDOW_WIDTH / 2 - 32);


		//-------横スクロールしすぎの場合の処理---------
		if (g_stageScrollPosition.x > 0)
			g_stageScrollPosition.x = 0;

		if (g_stageScrollPosition.x < -((STAGE_WIDTH * BLOCK_CHIP/* * 2*/) - WINDOW_WIDTH))
			g_stageScrollPosition.x = -((STAGE_WIDTH * BLOCK_CHIP/* * 2*/) - WINDOW_WIDTH);
		//----------------------------------------------


		//画面下端まで行ったとき
		if (position.y + g_stageScrollPosition.y > WINDOW_HEIGHT - BLOCK_CHIP)
		{
			state = DOWN_SCROLL;
		}

		//画面上端まで行ったとき
		if (position.y + g_stageScrollPosition.y < 0)
		{
			state = UP_SCROLL;
		}

		break;
		//----------------------------------------------------


		//------------------下へスクロール中------------------
	case DOWN_SCROLL:
		g_stageScrollPosition.y -= 20;	//画面をスクロール
		position.y += 3;				//プレイヤーも移動

		//キリの良いところまでいったら通常状態に戻す
		if ((int)g_stageScrollPosition.y % WINDOW_HEIGHT == 0)
		{
			state = DEFAULT;
		}
		break;
		//----------------------------------------------------


		//------------------上へスクロール中------------------
	case UP_SCROLL:
		g_stageScrollPosition.y += 20;	//画面をスクロール
		position.y -= 3;				//プレイヤーも移動

		//キリの良いところまでいったら通常状態に戻す
		if ((int)g_stageScrollPosition.y % WINDOW_HEIGHT == 0)
		{
			state = DEFAULT;
		}
		break;
		//----------------------------------------------------

		//------------------左へスクロール中------------------
	case LEFT_SCROLL:
		g_stageScrollPosition.x -= 20;	//画面をスクロール
		position.x += 3;				//プレイヤーも移動

		//キリの良いところまでいったら通常状態に戻す
		if ((int)g_stageScrollPosition.x % WINDOW_HEIGHT == 0)
		{
			state = DEFAULT;
		}
		break;
		//----------------------------------------------------

		//------------------右へスクロール中------------------
	case RIGHT_SCROLL:
		g_stageScrollPosition.x += 20;	//画面をスクロール
		position.x -= 3;				//プレイヤーも移動

		//キリの良いところまでいったら通常状態に戻す
		if ((int)g_stageScrollPosition.x % WINDOW_HEIGHT == 0)
		{
			state = DEFAULT;
		}
		break;
		//----------------------------------------------------

	}
	//--------------------------------------------------------------------

	

	//--------------------------ジャンプする------------------------------
	if (g_pInput->IsKeyTap(DIK_SPACE) && (jcount < JUMP_COUNT)&&isGround)
	{
		jump = JUMP;
		jcount++;
		isGround = FALSE;
		ladderflg = FALSE;
	}

	//番兵クン！バグの原因だけど現状の頼みの綱
	int BMPIkun;

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
	else if (stage->GetChip((int)(HitZone.right) / BLOCK_CHIP, (int)(HitZone.bottom + 1) / BLOCK_CHIP) == 0 
		&& stage->GetChip((int)(HitZone.left) / BLOCK_CHIP, (int)(HitZone.bottom + 1) / BLOCK_CHIP) == 0 && !ladderflg)
	{
		isGround = FALSE;
		jump += GRAVITY;
	}

	//天井（と思われる場所）にぶつかった時
	if (stage->GetChip((int)position.x / BLOCK_CHIP, (int)(position.y - JUMP) / BLOCK_CHIP) == 1)
	{
	}

	//--------------------------------------------------------------------

	return S_OK;
}

//----------------------------------
//機能：弾の発射と軌道
//引数：なし
//戻値：成功
//----------------------------------
HRESULT Player::Shot()
{
	//弾を発射する操作
	if (g_pInput->IsKeyTap(DIK_Z) || g_pInput->IsPadButtonRelease(XINPUT_GAMEPAD_A))
	{
		if (isShotKeyFlg == FALSE)
		{
			for (int i = 0; i < BULLET_SET; i++)
			{
				BOOL b;
				b = bullet[i].Shot(position + g_stageScrollPosition,direction);
				//発射できた！
				if (b == TRUE)
				{
					audio->Play("shoot");
					break;
				}
			}
		}
		//キーを押している
		isShotKeyFlg = TRUE;
	}
	else
	{
		//キーを離している
		isShotKeyFlg = FALSE;
	}

	return S_OK;
}

//----------------------------------
//機能：弾の更新処理
//引数：なし
//戻値：なし
//----------------------------------
void Player::BulletUpdate()
{
	for (int i = 0; i < BULLET_SET; i++)
	{
		bullet[i].Update();
	}
}

void Player::Reset()
{
	position.x = 100;
	position.y = 300;

	jump = 0;
	jcount = 0;

	anime = 0;
	direction = MS_RIGHT;

	//asiba = 999;

	hp = PLAYER_HP;
}
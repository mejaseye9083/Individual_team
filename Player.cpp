/*===========================================================================================
概　要:自機の設定
作成日:2015.10.15
更新日:2016.02.15
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
	ply = new Sprite;
	life = new Sprite;
	special = new Sprite;
	audio = new Audio;

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
	SAFE_DELETE(special);
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
	if (FAILED(ply->Load("pictures\\Actor.png")))
	{
		return E_FAIL;
	}

	//ライフメモリ画像の読み込み
	if (FAILED(life->Load("pictures\\LIFE.bmp")))
	{
		return E_FAIL;
	}

	//ライフメモリ画像の読み込み
	if (FAILED(special->Load("pictures\\Special_ber.bmp")))
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

	//プレイヤー画像の読み込み
	if (FAILED(bread.Load()))
	{
		return E_FAIL;
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
	//ストップボタン（ポーズ）
	if (g_pInput->IsKeyTap(DIK_J))
	{
		g_Stopflg = !g_Stopflg;
	}

	if (g_Stopflg == FALSE)
	{
		//移動時に二次元配列のテーブルを利用する
		Move(stg);
		Shot();

		//ヒットポイントが0以下になったらゲームオーバー
		if (hp <= 0)
		{
			Reset();
			audio->Play("dead");
			g_gameScene = SC_GAMEOVER;
		}

		//0の段階で特殊武器は打てないが、画像がおかしくなるので止める
		if (sp <= 0)
		{
			sp = 0;
		}

		//---------無敵時間の管理--------
		if (bonny == DAMAGE)
		{
			Invincible();
		}
		
		if (bonny == PEASE)
		{
			invincibleTime = 0;
		}
		//-------------------------------
		//弾の更新
		BulletUpdate();


		//---------------------デバッグ用コマンド------------------
		//自爆コマンド
		if (g_pInput->IsKeyTap(DIK_Y))
		{
			hp -= 2;
			memory_break_hp = (PLAYER_HP - hp) * PLAYER_DM;
		}

		//即死コマンド
		if (g_pInput->IsKeyTap(DIK_P))
		{
			hp -= 999;
			memory_break_hp = (PLAYER_HP - hp) * PLAYER_DM;
		}
		//---------------------------------------------------------

	}
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
			if (bonny == PEASE)
			{
				//接触ダメージ
				hp -= 3;
				memory_break_hp = (PLAYER_HP - hp) * PLAYER_DM;
				audio->Play("Player_Damage");
				bonny = DAMAGE;
			}
		}

		//弾のヒット処理
		for (int i = 0; i < BULLET_SET; i++)
		{
			//キャスト演算子を利用しUnitBase*からEnemy*に無理やり変えてしまう
			BOOL b = bullet[i].Hit((Enemy*)pTarget);

		}

		//ブレードのヒット処理
		bread.Hit((Enemy*)pTarget);
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

			g_gameScene = SC_CLEAR;
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

	//----ダメージを受けたときの点滅処理----
	switch (bonny)
	{
	case PEASE:
		break;

	case DAMAGE:
		if (invincibleTime % 2 == 0)
		{
			rock.size.x = 0;
		}
		else
		{
			rock.size.x = 32;
		}
		break;
	}
	//--------------------------------------
	
	rock.pos = position + g_stageScrollPosition;

	ply->Draw(&rock);
	//--------------------------------------------------------------------

	//---------------------ライフメモリの描画-----------------------------
	SpriteData Life;
	Life.pos.x = 10;
	Life.pos.y = 20 + memory_break_hp;
	Life.size.y = 160 - memory_break_hp;
	if (Life.size.y <= 0)
	{
		Life.size.y = 0;
	}

	life->Draw(&Life);
	//--------------------------------------------------------------------

	//-------------------特殊武器のメモリの描画---------------------------
	SpriteData Special;
	Special.pos.x = 25;
	Special.pos.y = 20 + memory_break_sp;
	Special.size.y = 160 - memory_break_sp;
	if (Special.size.y <= 0)
	{
		Special.size.y = 0;
	}

	special->Draw(&Special);
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

	//ブレードのレンダー
	bread.Render();
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
	HitZone.left = 2 + position.x;
	HitZone.top = 2 + position.y;

	HitZone.right = 30 + position.x;
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
		//梯子での移動(なくすかも)
		if (stage->GetChip((int)(HitZone.right) / BLOCK_CHIP, (int)((HitZone.bottom - HitZone.top) + position.y) / BLOCK_CHIP) == 2
			|| stage->GetChip((int)(HitZone.left) / BLOCK_CHIP, (int)((HitZone.bottom - HitZone.top) + position.y) / BLOCK_CHIP) == 2)
		{
			//梯子下移動
			if (g_pInput->IsKeyPush(DIK_DOWN))
			{
				//フラグ等々の乱立で見難いので後で要修正
				direction = MS_UP;
				isGround = TRUE;
				ladderflg = TRUE;
				jcount = 0;
				jumpBlock = FALSE;

				//進行方向が壁(1)だった場合は移動出来ないようにする
				if (stage->GetChip((int)((HitZone.right - HitZone.left) + position.x) / BLOCK_CHIP, (int)(HitZone.bottom + moveS) / BLOCK_CHIP) == 1)
				{
					moveS = 0;

				}

				position.y += moveS;
				
			}
		//-----------------------------------------------------------------------------------------------
			//梯子上移動
			if (g_pInput->IsKeyPush(DIK_UP))
			{
				//フラグ等々の乱立で見難いので後で要修正
				direction = MS_UP;
				isGround = TRUE;
				ladderflg = TRUE;
				jcount = 0;

				//進行方向が壁(1)だった場合は移動出来ないようにする
				if (stage->GetChip((int)((HitZone.right - HitZone.left) + position.x) / BLOCK_CHIP, (int)(HitZone.top - moveS) / BLOCK_CHIP) == 1)
				{
					moveS = 0;
					jumpBlock = TRUE;

				}
				//上記の判定はrectのトップ(頭の位置)なので、ジャンプできないようにしてある。
				//1ピクセルでも離れたらジャンプ可能にフラグを切り替える
				else
				{
					jumpBlock = FALSE;
				}
				
				//足の位置-5の高さからムーブの値を引いた値が0(何もない空間)に入る場合
				if (stage->GetChip((int)HitZone.right/ BLOCK_CHIP, (int)(HitZone.bottom -5 -moveS) / BLOCK_CHIP) == 0
					&& stage->GetChip((int)HitZone.left/ BLOCK_CHIP, (int)(HitZone.bottom -5 -moveS) / BLOCK_CHIP) == 0)
				{
					moveS = 0;
					jumpBlock = TRUE;

				}

				position.y += -moveS;
			}
		}
		//それ以外は梯子を掴んでいないと見なし、梯子フラグをFALSEに変える
		else
		{
			ladderflg = FALSE;
		}
		

		//プレイヤーが真ん中に来るよう、横スクロール位置を算出
		g_stageScrollPosition.x = -position.x + (WINDOW_WIDTH / 2 - BLOCK_CHIP);

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
	if ((g_pInput->IsKeyTap(DIK_SPACE) && (jcount < JUMP_COUNT)&&isGround) && !jumpBlock)
	{
		jump = JUMP;
		jcount++;
		isGround = FALSE;
		ladderflg = FALSE;
	}


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
	else if ((stage->GetChip((int)(HitZone.right) / BLOCK_CHIP, (int)(HitZone.bottom + 2) / BLOCK_CHIP) == 0 
		&& stage->GetChip((int)(HitZone.left) / BLOCK_CHIP, (int)(HitZone.bottom + 2) / BLOCK_CHIP) == 0) 
		||(stage->GetChip((int)(HitZone.right) / BLOCK_CHIP, (int)(HitZone.bottom + 2) / BLOCK_CHIP) == 2
		|| stage->GetChip((int)(HitZone.left) / BLOCK_CHIP, (int)(HitZone.bottom + 2) / BLOCK_CHIP) == 2)
		&& !ladderflg)
	{
		isGround = FALSE;
		jump += GRAVITY;
	}

	//天井（と思われる場所）にぶつかった時
	if (stage->GetChip((int)(HitZone.right) / BLOCK_CHIP, (int)(HitZone.top + jump -3) / BLOCK_CHIP) == 1
		|| stage->GetChip((int)(HitZone.left) / BLOCK_CHIP, (int)(HitZone.top + jump -3) / BLOCK_CHIP) == 1)
	{
		jump *= -1;
		jump += GRAVITY;
	}

	if (stage->GetChip((int)(HitZone.right) / BLOCK_CHIP, (int)(HitZone.top - 3) / BLOCK_CHIP) == 1
		|| stage->GetChip((int)(HitZone.left) / BLOCK_CHIP, (int)(HitZone.top - 3) / BLOCK_CHIP) == 1)
	{
		jumpBlock = TRUE;
	}
	else
	{
		jumpBlock = FALSE;
	}
	//--------------------------------------------------------------------

	/*
	 * ジャンプ中のテーブル判定で梯子(2)の部分の判定を行っていない
	 * そのためかjumpに与えた数値(JUMP = -12)にGRAVITYの値がプラスされない現象が起きている
	 * あくまで現状は仕様としてるが、後々判定で減らない仕様にしておきたい(明記したい)
	 * 
	 */

	return S_OK;
}

//----------------------------------
//機能：弾の発射と軌道
//引数：なし
//戻値：成功
//----------------------------------
HRESULT Player::Shot()
{
	//梯子につかまっているときは攻撃をさせない
	if (ladderflg != TRUE)
	{
		//ブレード攻撃
		if (g_pInput->IsKeyTap(DIK_X) && sp > 0)
		{

			BOOL c;
			c = bread.bread(position + g_stageScrollPosition, direction);
			//発射できた！
			if (c == TRUE)
			{
				sp -= 3;
				memory_break_sp = (PLAYER_SP - sp) * PLAYER_DM;
				audio->Play("shoot");
			}

			//キーを押している
			isSpecialFlg = TRUE;
		}
		else
		{
			//キーを離している
			isSpecialFlg = FALSE;
		}

		//弾を発射する操作
		if (g_pInput->IsKeyTap(DIK_Z) || g_pInput->IsPadButtonRelease(XINPUT_GAMEPAD_A))
		{
			if (isShotKeyFlg == FALSE)
			{
				for (int i = 0; i < BULLET_SET; i++)
				{
					BOOL b;
					b = bullet[i].Shot(position + g_stageScrollPosition, direction);
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
	//弾の個数分ループする
	for (int i = 0; i < BULLET_SET; i++)
	{
		bullet[i].Update();
	}

	//上下にスクロールする位置に行ったら弾を全て消す(リセット)
	if (state == DOWN_SCROLL || state == UP_SCROLL)
	{
		for (int i = 0; i < BULLET_SET; i++)
		{
			bullet[i].Reset();
		}
	}

	//追従させるためにポジションを渡しておく
	bread.Update(position+g_stageScrollPosition);
}

//----------------------------------
//機能：初期設定＆再初期化処理
//引数：なし
//戻値：なし
//----------------------------------
void Player::Reset()
{
	position.x = 100;
	position.y = 300;

	jump = 0;
	jcount = 0;

	anime = 0;

	invincibleTime = 0;
	direction = MS_RIGHT;

	//ダメージを受けたか否か
	bonny = PEASE;

	//asiba = 999;
	

	hp = PLAYER_HP;
	sp = PLAYER_SP;
}

//----------------------------------
//機能：ゲームオーバー画面に移動する処理
//引数：なし
//戻値：なし
//----------------------------------
void Player::Invincible()
{
	invincibleTime++;
	if (invincibleTime == 0x80)
	{
		bonny = PEASE;
	}
}
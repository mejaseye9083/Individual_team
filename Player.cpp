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

Stage *stg;

//------------------定数宣言------------------------
#define GRAVITY			1		//重力
#define	INERTIA			0.06
#define	JUMP			-12
#define	JUMP_COUNT		1

//移動操作
#define	MOVE_RIGHT	(g_pInput->IsKeyPush(DIK_RIGHT) || g_pInput->IsPadButtonPush(XINPUT_GAMEPAD_DPAD_RIGHT))
#define MOVE_LEFT	(g_pInput->IsKeyPush(DIK_LEFT) || g_pInput->IsPadButtonPush(XINPUT_GAMEPAD_DPAD_LEFT))
#define	MOVE_UP		(g_pInput->IsKeyPush(DIK_DOWN) || g_pInput->IsPadButtonPush(XINPUT_GAMEPAD_DPAD_DOWN))
#define MOVE_DOWN	(g_pInput->IsKeyPush(DIK_UP) || g_pInput->IsPadButtonPush(XINPUT_GAMEPAD_DPAD_UP))
#define	PUSH_START	g_pInput->IsKeyTap(DIK_J)||g_pInput->IsPadButtonTap(XINPUT_GAMEPAD_START)
#define	PUSH_BACK	g_pInput->IsKeyTap(DIK_U)||g_pInput->IsPadButtonTap(XINPUT_GAMEPAD_BACK)

//攻撃操作
#define	ATTACK_A	(g_pInput->IsKeyTap(DIK_Z) || g_pInput->IsPadButtonTap(XINPUT_GAMEPAD_A))
#define ATTACK_B	(g_pInput->IsKeyTap(DIK_X) || g_pInput->IsPadButtonTap(XINPUT_GAMEPAD_B))


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
	if (PUSH_START)
	{
		g_Stopflg = !g_Stopflg;
	}

	//ポーズ画面になっていないorメニュー画面が開かれていないとき
	if (g_Stopflg == FALSE)
	{
		//移動時に二次元配列のテーブルを利用する
		Move(stg);
		Shot();
		Slash();

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
		AttackUpdate();

		//デバッグ用操作
		DebugCommand();

	}
	return S_OK;
}

//----------------------------------
//機能：当たり判定
//引数：pTarget		//オブジェクトの情報
//戻値：成功
//----------------------------------
HRESULT Player::Hit(UnitBase* pTarget)
{

	HitEnemy((Enemy*)pTarget);			//敵との当たり判定
	HitStage((Stage*)pTarget);			//ステージとの当たり判定
	HitItem((RecoveryItem*)pTarget);	//アイテムとの当たり判定

	return S_OK;
}

//----------------------------------
//機能：当たり判定
//引数：pEnemyにキャストしたpTargetの情報
//戻値：成功
//----------------------------------
HRESULT Player::HitEnemy(Enemy* pEnemy)
{
	//--------------------エネミーとの衝突判定----------------------
	//当たり判定を作る、X軸・Y軸・幅・高さが必要になる
	if (typeid(*pEnemy) == typeid(Enemy))
	{
		D3DXVECTOR3 enemyPosition = pEnemy->GetPos();
		RECT enemySize = pEnemy->GetZone();

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
			BOOL b = bullet[i].Hit(pEnemy);

		}

		//ブレードのヒット処理
		bread.Hit(pEnemy);
	}
	//--------------------------------------------------------------


	return S_OK;
}

//----------------------------------
//機能：当たり判定
//引数：pStageにキャストしたpTargetの情報
//戻値：成功
//----------------------------------
HRESULT Player::HitStage(Stage* pStage)
{
	//-----------PlaySceneで呼び出したステージをstgに入れる---------
	if (typeid(*pStage) == typeid(Stage))
	{
		/*
		* pTargetはUnitBaseのサブクラスならポインタを通じて確認が出来る
		* つまり、PlaySceneでStageを呼び出しているので、それをstgに入れればよい！
		* そうすれば2回呼ぶことでの不正な値の習得はなくなる。
		*
		* ～～～～～～～～～～～オマケ～～～～～～～～～～
		* シングルトンという法則があり、知ることでnewをより効率的に使えるようになるらしい！
		*/
		stg = (Stage*)pStage;
	}
	//--------------------------------------------------------------
	return S_OK;
}

//----------------------------------
//機能：当たり判定
//引数：pItemにキャストしたpTargetの情報
//戻値：成功
//----------------------------------
HRESULT Player::HitItem(RecoveryItem* pItem)
{
	//--------------------アイテムとの衝突判定----------------------
	//当たり判定を作る、X軸・Y軸・幅・高さが必要になる
	if (typeid(*pItem) == typeid(RecoveryItem))
	{
		D3DXVECTOR3 itemPosition = pItem->GetPos();
		RECT itemSize = pItem->GetZone();

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
//引数：Stageの情報
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

	moveS = 4;	//常に4をキープさせる

	//----------------------プレイヤーの移動------------------------------
	switch (state)
	{

		//---------------------通常移動----------------------
	case DEFAULT:

		//右移動の処理
		if (MOVE_RIGHT && !ladderflg)
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
		if (MOVE_LEFT && !ladderflg)
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

		LadderMove(stage);	//梯子移動
		Jump(stage);		//ジャンプ処理


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

	return S_OK;
}

//----------------------------------
//機能：自機のジャンプ処理
//引数：Stageの情報
//戻値：成功
//----------------------------------
HRESULT Player::Jump(Stage* stage)
{
	//--------------------------ジャンプする------------------------------
	if (((g_pInput->IsKeyTap(DIK_SPACE) || g_pInput->IsPadButtonTap(XINPUT_GAMEPAD_X)) && (jcount < JUMP_COUNT) && isGround) && !jumpBlock)
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
		guardman = (int)position.y;	//着地地点を番兵君に渡す
	}

	//プレイヤーの左下か右下の位置にジャンプの値を足した位置のテーブルの値が1だった時の処理
	if (stage->GetChip((int)(HitZone.right) / BLOCK_CHIP, (int)(HitZone.bottom + jump) / BLOCK_CHIP) == 1
		|| stage->GetChip((int)(HitZone.left) / BLOCK_CHIP, (int)(HitZone.bottom + jump) / BLOCK_CHIP) == 1
		&& !isGround)
	{
		//ポジション から引く値は (1フレーム前の自分のポジションを32で割った余り) + 1
		position.y -= (guardman % BLOCK_CHIP) + 1;
		isGround = TRUE;
		jump = 0;
		jcount = 0;
	}
	//isGroundがTRUEだった場合はジャンプをせずに足場がなくなったはず(1は足場or壁ブロック)
	//ジャンプせずに落下判定に入る場合の処理が下記の処理となる(自由落下)
	else if ((stage->GetChip((int)(HitZone.right) / BLOCK_CHIP, (int)(HitZone.bottom + 2) / BLOCK_CHIP) == 0
		&& stage->GetChip((int)(HitZone.left) / BLOCK_CHIP, (int)(HitZone.bottom + 2) / BLOCK_CHIP) == 0)		//右足と左足の真下が何もなかったら落下
		|| (stage->GetChip((int)(HitZone.right) / BLOCK_CHIP, (int)(HitZone.bottom + 2) / BLOCK_CHIP) == 2
		|| stage->GetChip((int)(HitZone.left) / BLOCK_CHIP, (int)(HitZone.bottom + 2) / BLOCK_CHIP) == 2)		//落下時に右か左が梯子に触れても落下続行
		&& !ladderflg)																							//FALSEなら落下続行
	{
		isGround = FALSE;
		jump += GRAVITY;
	}

	//天井（と思われる場所）にぶつかった時
	if (stage->GetChip((int)(HitZone.right) / BLOCK_CHIP, (int)(HitZone.top + jump - 3) / BLOCK_CHIP) == 1
		|| stage->GetChip((int)(HitZone.left) / BLOCK_CHIP, (int)(HitZone.top + jump - 3) / BLOCK_CHIP) == 1)
	{
		jump *= -1;
		jump += GRAVITY;
	}

	//自分の頭が天井に接地してる際にジャンプ出来ない様にする
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

	return S_OK;
}

//----------------------------------
//機能：自機の梯子移動
//引数：Stageの情報
//戻値：成功
//----------------------------------
HRESULT Player::LadderMove(Stage* stage)
{
	//梯子での移動
	if (stage->GetChip((int)(HitZone.right) / BLOCK_CHIP, (int)((HitZone.bottom - HitZone.top) + position.y) / BLOCK_CHIP) == 2
		|| stage->GetChip((int)(HitZone.left) / BLOCK_CHIP, (int)((HitZone.bottom - HitZone.top) + position.y) / BLOCK_CHIP) == 2)
	{
		//梯子下移動
		if (MOVE_UP)
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
		if (MOVE_DOWN)
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
			if (stage->GetChip((int)HitZone.right / BLOCK_CHIP, (int)(HitZone.bottom - 5 - moveS) / BLOCK_CHIP) == 0
				&& stage->GetChip((int)HitZone.left / BLOCK_CHIP, (int)(HitZone.bottom - 5 - moveS) / BLOCK_CHIP) == 0)
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
		//弾を発射する操作
		if (ATTACK_A)
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
//機能：弾の発射と軌道
//引数：なし
//戻値：成功
//----------------------------------
HRESULT	Player::Slash()
{
	//梯子につかまっているときは攻撃をさせない
	if (ladderflg != TRUE)
	{
		//ブレード攻撃
		if (ATTACK_B && sp > 0)
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
	}
	return S_OK;
}

//----------------------------------
//機能：弾の更新処理
//引数：なし
//戻値：なし
//----------------------------------
void Player::AttackUpdate()
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

	//剣は追従させるためにポジションを渡しておく
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

//----------------------------------
//機能：デバッグ用コマンド集
//引数：なし
//戻値：なし
//----------------------------------
void Player::DebugCommand()
{

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

	//SPゲージ回復
	if (g_pInput->IsKeyTap(DIK_I) || g_pInput->IsPadButtonPush(XINPUT_GAMEPAD_Y))
	{
		sp = PLAYER_SP;
	}

	//HPゲージ回復
	if (g_pInput->IsKeyTap(DIK_U) || g_pInput->IsPadButtonPush(XINPUT_GAMEPAD_Y))
	{
		hp = PLAYER_HP;
	}
	//---------------------------------------------------------
}

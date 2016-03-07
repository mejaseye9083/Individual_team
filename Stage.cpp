/*===========================================================================================
概　要:ステージの設定
作成日:2015.10.21
更新日:2016.01.13
制作者:藤原順基
=============================================================================================*/

//---------------インクルード-----------------------
#include "Stage.h"
#include "Camera.h"


Block block;
item  recover;

//------------------定数宣言------------------------
#define GRAVITY			1		//重力
#define	INERTIA			0.15
#define	JUMP			-15
#define	JUMP_COUNT		1

//----------------------------------
//機能：コンストラクタ
//引数：なし
//戻値：なし
//----------------------------------
Stage::Stage()
{
	global.mapW = STAGE_WIDTH;
	global.mapH = STAGE_HEIGHT;

	audio = new Audio;

	Create();
	
}

//----------------------------------
//機能：デストラクタ
//引数：なし
//戻値：なし
//----------------------------------
Stage::~Stage()
{
	SAFE_DELETE(audio);
}

//----------------------------------
//機能：画像・音楽データの読み込み
//引数：なし
//戻値：成功or失敗
//----------------------------------
HRESULT Stage::Load()
{
	//SE関連の読み込み(ステージはStageBGM類のみ)
	if (FAILED(audio->Load("songs\\SE\\Rock_BGM.xwb", "songs\\SE\\Rock_BGM.xsb")))
	{
		return E_FAIL;
	}

	block.Load("pictures\\modelA.bmp");
	recover.Load("pictures\\RecoverAid.png");

	return S_OK;
}

//----------------------------------
//機能：更新処理
//引数：なし
//戻値：成功
//----------------------------------
HRESULT Stage::Update()
{
	//BGM:Technoはループ処理してある
	//BOOL処理で最初はTRUEにしておき、中でFALSEに変えて二度と読み込まれないようにする
	if (Def == TRUE&&g_gameScene == SC_PLAY)
	{
		audio->Play("StageALL");
		Def = FALSE;
	}

	return S_OK;
}

//----------------------------------
//機能：当たり判定
//引数：pTarget		敵の情報
//戻値：成功
//----------------------------------
HRESULT Stage::Hit(UnitBase* pTarget)
{

	return S_OK;
}

//----------------------------------
//機能：描画処理
//引数：なし
//戻値：成功or失敗
//----------------------------------
HRESULT Stage::Render()
{

	//足場を表示させる位置：二次元配列等検討中
	/*D3DXVECTOR3 Bpos(100, 350, 0);
	block.Drow(Bpos);*/
	for (int x = 0; x < global.mapW; x++)
	{
		for (int y = 0; y < global.mapH; y++)
		{

			int px = x * BLOCK_CHIP;
			int py = y * BLOCK_CHIP;
			D3DXVECTOR3 Bpos(px, py, 0);
			block.Drow(Bpos, table[x][y]);
		}
	}

	recover.Drow(D3DXVECTOR3((25*BLOCK_CHIP), (33*BLOCK_CHIP), 0));
	return S_OK;
}

//ランダムマップ作成
void Stage::Create()
{
	//とりあえず全部通れるようにする
	ZeroMemory(table, sizeof(table));

	//0:背景,1:壁,2~6:まだ指定してない
	char szFile[MAX_PATH] = "Stage\\stage2.csv";


	HANDLE hFile;
	hFile = CreateFile(
		szFile,					//ファイル名
		GENERIC_READ,			//アクセスモード(書き込み用)
		0,						//共有(なし)
		NULL,					//セキュリティ属性(継承しない)
		OPEN_EXISTING,			//作成方法(同名ファイルは上書き)
		FILE_ATTRIBUTE_NORMAL,	//属性とフラグ(設定なし)
		NULL					//拡張属性なし(なし)
		);

	//開けなかったら中断
	if (hFile == NULL)	return;

	//ファイルのサイズを取得
	DWORD fileSize = GetFileSize(hFile, NULL);

	//ファイルのサイズ分メモリを確保
	char* data;
	data = new char[fileSize];

	//全部読み込む
	DWORD dwBytes = 0;
	ReadFile(
		hFile,					//ファイルハンドル
		data,					//データを入れる変数
		(DWORD)strlen(data),	//読み込むサイズ
		&dwBytes,				//読み込んだサイズ
		NULL);					//オーバーラップド構造体

	//読み込んだデータからマップの幅と高さを取得
	int index = 0;
	//global.mapW = 20;
	//global.mapH = GetToComma(data, &index);

	//順番に各マスの値を取得
	for (int y = 0; y < STAGE_HEIGHT; y++)
	{
		for (int x = 0; x < STAGE_WIDTH; x++)
		{

			table[x][y] = GetToComma(data, &index);
		}
	}

	/*y=高さ、よってxをネストでループさせてyをプラスすると改行と同じ扱いになる*/

	//newで作ったdataを解放する
	delete data;
}

//-------------------------------------------------
//機能：コンマ区切りのデータを整数で取得
//引数：data[]		コンマ区切りの文字列データ
//引数：*index		検索を開始する位置
//戻値：次のコンマまでの文字列を整数に変換した値
//-------------------------------------------------
int Stage::GetToComma(char data[], int* index)
{
	char chr[600];
	int i = 0;

	//[,]までの一文字ずつchrに入れる
	while (data[*index] != ',')
	{
		chr[i] = data[*index];
		i++;
		(*index)++;
	}

	//最後に「\0」をつける
	chr[i] = '\0';
	i++;
	(*index)++;

	//整数の変換
	int value = atoi(chr);
	return value;
}
/*===========================================================================================
概　略：音を鳴らすプログラム
作成日：2015.08.25
更新日：2015.08.25
制作者：藤原順基
=============================================================================================*/
#pragma once

//---------------インクルード-----------------------
#include "Global.h"
#include <xact3.h>

//------------オーディオクラスの定義----------------
class Audio
{
	IXACT3Engine*		xactEngine;	//XACTエンジン、これがないと音を鳴らせない
	IXACT3WaveBank*		waveBank;	//外部で作成したサウンドバンクの保存位置
	IXACT3SoundBank*	soundBank;	//外部で作成したウェーブバンクの保存位置

	void*        mapWaveBank;	//.xwbファイルを入れる場所
	void*		 soundBankData;	//.xsbファイルを入れる場所

	HRESULT InitEngine();
	HRESULT LoadWaveBank(char* name);
	HRESULT LoadSoundBank(char* name);
	//この三つはpublicのloadでまとめて呼ばれるようにしてあるので
	//弄られないようにプライベートにしておくと安全に取り扱いできる
public:
	Audio();
	~Audio();

	HRESULT Load(char* wname, char* sname);	//ウェーブバンク・サウンドバンクを読み込む
	HRESULT Play(char* cueName);			//音を鳴らす
	HRESULT Stop(char* cueName);			//音を止める
};
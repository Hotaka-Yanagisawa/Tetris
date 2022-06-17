#pragma comment(lib,"winmm.lib")
#include <Windows.h>
#include "DxLib.h"
#include "SceneMgr.h"
#include "input.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "ResultScene.h"
#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
#include "main.h"

bool g_game = true;
bool g_screenMode = true;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ChangeWindowMode(g_screenMode);
	if (DxLib_Init() == -1)    // ＤＸライブラリ初期化処理
	{
		return -1;    // エラーが起きたら直ちに終了
	}
	SetDrawScreen(DX_SCREEN_BACK);
	
	std::srand((unsigned int)time(NULL));

	SceneMgr * pSceneMgr = SceneMgr::GetInstance();

	pSceneMgr->Init();

	timeBeginPeriod(1);
	//---FPS制御の初期化
	int fpsCount = 0;
	DWORD countStartTime = timeGetTime();
	DWORD preExecTime = countStartTime;

	while (g_game)
	{
		DWORD nowTime = timeGetTime();
		if (nowTime - countStartTime >= 1000)
		{
			countStartTime = nowTime;
			std::string st = std::to_string(fpsCount);
			SetMainWindowText("テトリス");
			//SetMainWindowText(st.c_str());
			fpsCount = 0;
		}
		if (nowTime - preExecTime >= 1000 / 60)
		{
			InputUpdate();
			if (isKeyTrigger(KEY_INPUT_O) || isButtonTrigger(XINPUT_BUTTON_RIGHT_THUMB))
			{
				g_screenMode ^= true;
				ChangeWindowMode(g_screenMode);
				pSceneMgr->ReStart();
			}
			//ゲームループ抜ける
			if (CheckHitKey(KEY_INPUT_ESCAPE) || isButtonPress(XINPUT_BUTTON_BACK))
			{
				g_game ^= true;
			}
			pSceneMgr->Update();
			ClearDrawScreen();
			pSceneMgr->Draw();
			ScreenFlip();

			fpsCount++;
			preExecTime = nowTime;
		}
	}
	
	pSceneMgr->Uninit();

	DxLib_End();        // ＤＸライブラリ使用の終了処理

	return 0;			// ソフトの終了
}


void SetGame(bool flag)
{
	g_game = flag;
}

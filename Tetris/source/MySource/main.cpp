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
	if (DxLib_Init() == -1)    // �c�w���C�u��������������
	{
		return -1;    // �G���[���N�����璼���ɏI��
	}
	SetDrawScreen(DX_SCREEN_BACK);
	
	std::srand((unsigned int)time(NULL));

	SceneMgr * pSceneMgr = SceneMgr::GetInstance();

	pSceneMgr->Init();

	timeBeginPeriod(1);
	//---FPS����̏�����
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
			SetMainWindowText("�e�g���X");
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
			//�Q�[�����[�v������
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

	DxLib_End();        // �c�w���C�u�����g�p�̏I������

	return 0;			// �\�t�g�̏I��
}


void SetGame(bool flag)
{
	g_game = flag;
}

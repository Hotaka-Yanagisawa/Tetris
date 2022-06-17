//入力を行う

#include "input.h"
#include "DxLib.h"

char g_KeyBuf[256] = { 0 };
char g_KeyOld[256] = { 0 };
XINPUT_STATE input[2];
XINPUT_STATE oldInput[2];
bool g_isType = true;

void InputUpdate()
{
	memcpy(g_KeyOld, g_KeyBuf, sizeof(g_KeyBuf));
	GetHitKeyStateAll(g_KeyBuf);
	for (int j = 0; j < 2; j++)
	{
		oldInput[j] = input[j];
	}
		GetJoypadXInputState(DX_INPUT_PAD1, &input[0]);
		GetJoypadXInputState(DX_INPUT_PAD2, &input[1]);
}

void InputSearch()
{
	for (int i = 0; i < 256; ++i)
	{
		if (g_KeyBuf[i] == 1)
		{
			g_isType = true;
			return;
		}
	}
	for (int j = 0; j < 2; j++)
	{
		for (int i = 0; i < sizeof(input[j].Buttons) / sizeof(input[j].Buttons[0]); ++i)
		{
			if (input[j].Buttons[i] == 1)
			{
				g_isType = false;
				return;
			}
		}
	}
}


bool isKeyTrigger(int key)
{
	if (g_KeyOld[key] == 0 && g_KeyBuf[key] == 1)
	{
		return true;
	}
	return false;
}

bool isKeyRelease(int key)
{
	if (g_KeyOld[key] == 1 && g_KeyBuf[key] == 0)
	{
		return true;
	}
	return false;
}

// ボタン１６個( 添字には XINPUT_BUTTON_DPAD_UP 等を使用する、0:押されていない  1:押されている
bool isButtonPress(int button,int num)
{
	return input[num].Buttons[button];
}

bool isButtonTrigger(int button, int num)
{
	if (oldInput[num].Buttons[button] == 0 && input[num].Buttons[button] == 1)
	{
		return true;
	}
	return false;
}

bool isButtonRelease(int button, int num)
{
	if (oldInput[num].Buttons[button] == 1 && input[num].Buttons[button] == 0)
	{
		return true;
	}
	return false;
}

bool GetType()
{
	return g_isType;
}

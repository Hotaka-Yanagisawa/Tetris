#pragma once

bool isKeyTrigger(int);

bool isKeyRelease(int);

void InputUpdate();

void InputSearch();

bool isButtonPress(int key,int num = 0);
bool isButtonTrigger(int key,int num = 0);
bool isButtonRelease(int key,int num = 0);

bool GetType();
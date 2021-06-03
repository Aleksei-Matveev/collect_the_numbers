#include "stdafx.h"

//���� ����
//##################################################################################
void StopGame(BOOL clearButton = FALSE) {
	KillTimer(hDialogGame, TIMER);
	EnableWindow(hBtnStop, FALSE);
	EnableMenuItem(hMenuGame, ID_TIME_GAME, MF_ENABLED);
	EnableMenuItem(hMenuGame, ID_BTN_START, MF_ENABLED);
	EnableMenuItem(hMenuGame, ID_BTN_MENU_NEW, MF_GRAYED);
	EnableWindow(hBtnNewGame, TRUE);
	SendMessage(hProgressBar, PBM_SETPOS, 0, 0);

	for (auto it = hButtonPad.begin(); it != hButtonPad.end(); it++) {
		EnableWindow(it->second, FALSE);
		if (clearButton)
			SendMessage(it->second, WM_SETTEXT, 0, (LPARAM)_TEXT(""));
	}
}
//##################################################################################

//������������� ����� ����
//##################################################################################
void InitGame() {
	BYTE numberRand;//��������� �����
	
	for (auto it = hButtonPad.begin(); it != hButtonPad.end(); it++)//���������� ������
		EnableWindow(it->second, TRUE);

	for (int i = 0; i < MAX; i++) {			//���������� �����
		numberRand = (BYTE)1 + rand() % 99;
		for (int y = 0; y <= i; y++) {

			if (numberRand == random[y]) {
				numberRand = (BYTE)1 + rand() % 100;
				y = 0;
			}
		}
		random[i] = sortrandom[i] = numberRand;//��������� ������� ������
	}

	for (auto it = hButtonPad.begin(); it != hButtonPad.end(); it++) {
		_stprintf_s(buffer, _TEXT("%d"), random[it->first - 1001]);
		SendMessage(it->second, WM_SETTEXT, 0, (LPARAM)buffer);   //������ ����� �� �������
	}

	std::sort(sortrandom, sortrandom + MAX);//��������� ������ :=)
}
//##################################################################################
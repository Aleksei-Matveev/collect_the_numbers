#include "stdafx.h"

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int) {
	HACCEL hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR2));
	hDialogGame = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DIALOG_GAME), NULL, DlgGameProc);

	srand((UINT)time(NULL));
	
	INITCOMMONCONTROLSEX cc;
	cc.dwICC = ICC_WIN95_CLASSES;
	cc.dwSize = sizeof(cc);
	InitCommonControlsEx(&cc);

	ShowWindow(hDialogGame, 1);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		if (!TranslateAccelerator(hDialogGame, hAccel, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return msg.wParam;
}

BOOL CALLBACK DlgGameProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	//Строка состояния
	//##################################################################################
	static HWND hStatusBar;
	int partsStatusBar[3] = { 0, 205,-1 };
	//##################################################################################

	//Дескрипторы 
	//##################################################################################
	static HWND hListBox;//Список	
	static HWND hEditSpin;//Текстовое поле для счетчика	
	static HWND hTableRecords;//Таблицца рекордов
	
	//##################################################################################
	
	static int index = 0;//Индекс для доступа к элементам массива
	
	switch (uMsg)
	{
	case WM_INITDIALOG:	
		loadDisk();
//Меню
//##################################################################################
		hMenuGame = LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MENU1));
		SetMenu(hWnd, hMenuGame);
//##################################################################################

//Прогресс бар
//##################################################################################
		hProgressBar = GetDlgItem(hWnd, IDC_PROGRESS1);
		SendMessage(hProgressBar, PBM_SETBKCOLOR, 0, RGB(255, 255, 255));
		SendMessage(hProgressBar, PBM_SETBARCOLOR, 0, RGB(50, 250, 100));
		SendMessage(hProgressBar, PBM_SETRANGE, 0, MAKELPARAM(0, 60));
//##################################################################################

//Статус бар
//##################################################################################
		hStatusBar = CreateStatusWindow(WS_CHILD | WS_VISIBLE | CCS_BOTTOM, _TEXT(""), hWnd, 1);
		SendMessage(hStatusBar, SB_SETPARTS, 3, (LPARAM)partsStatusBar);
//##################################################################################

//Запрос дескрипторов элементов управления
//##################################################################################
		hBtnNewGame = GetDlgItem(hWnd, ID_BTN_START);
		hBtnStop = GetDlgItem(hWnd, ID_BTN_STOP);
		EnableWindow(hBtnStop, FALSE);

		hSpin = GetDlgItem(hWnd, IDC_SPIN1);
		SendMessage(hSpin, UDM_SETRANGE32, timeGameMin, timeGameMax);

		hListBox = GetDlgItem(hWnd, IDC_LISTBOX);

		hEditSpin = GetDlgItem(hWnd, IDC_EDIT1);
		SendMessage(hEditSpin, WM_SETTEXT, 0, (LPARAM)_TEXT("60"));		
		
		EnableMenuItem(hMenuGame, ID_BTN_MENU_NEW, MF_GRAYED);
//##################################################################################

//Запрос ID кнопок игровой панели
//##################################################################################
		for (int i = 0; i < MAX; i++) {
			hBtn[i] = GetDlgItem(hWnd, IDC_BUTTON1 + i);
			hButtonPad.insert(std::pair<int, HWND>(IDC_BUTTON1 + i, hBtn[i]));
		}
		//Дизактивация кнопок по умолчанию
		for (auto it = hButtonPad.begin(); it != hButtonPad.end(); it++)
			EnableWindow(it->second, FALSE);
//##################################################################################	

		return TRUE;

	case WM_COMMAND:
	{
		auto it = hButtonPad.find(LOWORD(wParam));//Находим ID в массиве

//Обработка ввода
		if (it != hButtonPad.end()) {
			if (random[it->first - 1001] == sortrandom[index]) {
				//Если ввод верный
				//##################################################################################	
				index++;
				EnableWindow(it->second, FALSE);
				_stprintf_s(buffer, _TEXT("%d"), random[it->first - 1001]);
				SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)buffer);
				//Суммируем заработанные очки				
				totalPoints += (double)points + ((double)points / (timePlay < 1 ? 1 : timePlay));

				_stprintf_s(buffer, _TEXT("Счет иры : %d"), totalPoints);
				SendMessage(hStatusBar, SB_SETTEXT, (WPARAM)1, (LPARAM)buffer);
				//##################################################################################

				//Если цифр больше не осталось ИГРОК ПОБЕДИЛ
				//##################################################################################
				if (index == MAX) {
					StopGame(FALSE);

					//Сбор статистики за игру
					//##################################################################################

					if (timePlay > 9)//Очень сомневаюсь что кто-то выиграет меньше чем за 10 секунд  :=)
						_stprintf_s(buffer, _TEXT("Время игры 0:%d"), timePlay);
					else
						_stprintf_s(buffer, _TEXT("Время игры 0:0%d"), timePlay);
					SendMessage(hStatusBar, SB_SETTEXT, (WPARAM)2, (LPARAM)buffer);

					MessageBox(hWnd, _TEXT("         ПОБЕДА!\n  ПОЗДРАВЛЯЮ!"), _TEXT("Конец игры"), MB_OK);
					index = 0;
					totalPoints+= ((double)points/timePlay)*maxPoints;
					DialogBox(hInstance, MAKEINTRESOURCE(IDD_NAME), hWnd, DlgNameProc);
					return TRUE;
				}
			}
			else {
				//Иначе  ПОТЕРЯ ПОТЕРЬ игрок ПРОИГРАЛ
				//##################################################################################
				StopGame(FALSE);
				MessageBox(hWnd, _TEXT("           НЕУДАЧА!\nПопробуйте еще раз"), _TEXT("Конец игры"), MB_OK);
				index = 0;
				return TRUE;
			}
		}
	}
//##################################################################################

//Обработка сообщений
//##################################################################################
		switch (LOWORD(wParam)) {

//СТАРТ ИГРЫ
//##################################################################################
		case ID_BTN_START:
			totalPoints = 0;
			EnableWindow(hBtnStop, TRUE);
			EnableMenuItem(hMenuGame, ID_BTN_MENU_NEW, MF_ENABLED);
			timeGame = SendMessage(hSpin, UDM_GETPOS32, 0, 0);
			timeLeft = timeGame;
			points = maxPoints - timeGame;
			SendMessage(hProgressBar, PBM_SETRANGE, 0, MAKELPARAM(0, timeGame));
			SendMessage(hListBox, LB_RESETCONTENT, 0, 0);
			InitGame();
			EnableMenuItem(hMenuGame, ID_BTN_START, MF_GRAYED);
			EnableMenuItem(hMenuGame, ID_TIME_GAME, MF_GRAYED);
			EnableWindow(hBtnNewGame, FALSE);
			SetTimer(hWnd, TIMER, 1000, NULL);
			SendMessage(hStatusBar, SB_SETTEXT, (WPARAM)1, (LPARAM)_TEXT(""));
			SendMessage(hStatusBar, SB_SETTEXT, (WPARAM)2, (LPARAM)_TEXT(""));
			_stprintf_s(buffer, _TEXT("Счет иры :"));
			SendMessage(hStatusBar, SB_SETTEXT, (WPARAM)1, (LPARAM)buffer);
			return TRUE;

//СТОП ИГРЫ
//##################################################################################
		case ID_BTN_STOP:
			StopGame(TRUE);
			SendMessage(hStatusBar, SB_SETTEXT, (WPARAM)1, (LPARAM)_TEXT(""));
			SendMessage(hStatusBar, SB_SETTEXT, (WPARAM)2, (LPARAM)_TEXT(""));
			EnableMenuItem(hMenuGame, ID_BTN_MENU_NEW, MF_GRAYED);
			return TRUE;
//##################################################################################

//СООБЩЕНИЯ МЕНЮ
//##################################################################################
		case ID_BTN_MENU_NEW:
			if (MessageBox(hWnd, _TEXT("Начать Заново"), _TEXT("РЕСТАРТ"), MB_YESNO) == IDYES) {
				StopGame(TRUE);
				SendMessage(hWnd, WM_COMMAND, (WPARAM)ID_BTN_START, 0);
			}
			return TRUE;

		case ID_BTN_MENU_EXIT:
			SendMessage(hWnd, WM_CLOSE, 0, 0);
			return TRUE;

		case ID_TABLE_RECORD:
			DialogBox(hInstance, MAKEINTRESOURCE(IDD_TABLERECORDS), hWnd, DlgTableRecordsProc);
			return TRUE;

		case ID_TIME_GAME:
			DialogBox(hInstance, MAKEINTRESOURCE(IDD_TIMEGAME), hWnd, DlgTimeGameProc);
			return TRUE;

		case ID_AUTOR:
			DialogBox(hInstance, MAKEINTRESOURCE(IDD_AUTOR), hWnd, DlgAutorProc);
			return TRUE;
		}
		return TRUE;
//##################################################################################

//ТАЙМЕР
//##################################################################################
	case WM_TIMER:
		timePlay = timeGame - timeLeft;
		timeLeft--;
		if(timeLeft>9)
		_stprintf_s(buffer, _TEXT("Осталось 0:%d секунд"), timeLeft);
		else
			if(timeLeft == 0)
				_stprintf_s(buffer, _TEXT("Время игры истекло"));
			else
			_stprintf_s(buffer, _TEXT("Осталось 0:0%d секунд"), timeLeft);

		SendMessage(hStatusBar, SB_SETTEXT, (WPARAM)2, (LPARAM)buffer);
		SendMessage(hProgressBar, PBM_DELTAPOS, 1, 0);
		if (SendMessage(hProgressBar, PBM_GETPOS, 0, 0) == timeGame) {
			KillTimer(hWnd, TIMER);
			MessageBox(hWnd, _TEXT("      Время игры истекло"), _TEXT("Конец игры"), MB_OK);
			StopGame(FALSE);
		}
		return TRUE;
//##################################################################################

	case WM_CLOSE:
		if (MessageBox(hWnd, _TEXT("\tУже уходите ?"), _TEXT("Выход"), MB_YESNO)==IDYES) {
			saveDisk();
			for (auto it = Table.begin(); it != Table.end(); it++)
				delete[] it->second;
			delete[] fileBuffer;
			
			KillTimer(hWnd, TIMER);
			DestroyWindow(hWnd);
		}
		return TRUE;

	case WM_DESTROY:
		
		PostQuitMessage(0);
		return TRUE;
	}
	return FALSE;
}

//Процедура запроса 
//##################################################################################
BOOL CALLBACK DlgNameProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	static HWND hEditTable;
	static HWND hTotalPoint;//Статик суммы набранных очков

	switch (uMsg){
	case WM_INITDIALOG:
		hTotalPoint = GetDlgItem(hWnd, IDC_TOTALPOINT);
		hEditTable = GetDlgItem(hWnd, IDC_EDITTABLE);
		_stprintf_s(buffer, _TEXT("Набранные очки : %d"), totalPoints);
		SendMessage(hTotalPoint, WM_SETTEXT, 0, (LPARAM)buffer);		
	return TRUE;
	
	case WM_COMMAND:
		
		switch (LOWORD(wParam))
		{
		case IDC_SAVE:
			TCHAR* textBuffer=new TCHAR[19];
			SendMessage(hEditTable, WM_GETTEXT, 19, (LPARAM)textBuffer);
			if (_tcslen(textBuffer) < 1)
				_tcscpy(textBuffer, _TEXT("NONAME"));
			
			Table.insert(std::pair<int, TCHAR*>(totalPoints, textBuffer));
		
			SendMessage(hWnd, WM_CLOSE, 0, 0);
			return TRUE;
		}
		return TRUE;

	case WM_CLOSE:		
		EndDialog(hWnd, 0);
		return TRUE;
	}
	return FALSE;
}
//##################################################################################

//процедура обработки настройки времени
//##################################################################################
BOOL CALLBACK DlgTimeGameProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	static HWND hSliderTimeGame;
	static HWND hStaticTextTimeGame;
	
	switch (uMsg)
	{
	case WM_INITDIALOG:
		static int posSlider;
		hSliderTimeGame = GetDlgItem(hWnd, IDC_SLIDER_TIME_GAME);
		hStaticTextTimeGame = GetDlgItem(hWnd, IDC_STATIC_TIME_GAME);

		posSlider = SendMessage(hSpin, UDM_GETPOS32, 0, 0);

		SendMessage(hSliderTimeGame, TBM_SETRANGE, TRUE, MAKELPARAM(10, 60));
		SendMessage(hSliderTimeGame, TBM_SETLINESIZE, 0, 1);
		SendMessage(hSliderTimeGame, TBM_SETTICFREQ, 1, 0);
		SendMessage(hSliderTimeGame, TBM_SETPOS, true, posSlider);
		
		_stprintf_s(buffer, _TEXT("Время игры : %d секунд"), posSlider);
		SendMessage(hStaticTextTimeGame, WM_SETTEXT, sizeof(buffer), LPARAM(buffer));
		return TRUE;

	case WM_HSCROLL:
		posSlider = SendMessage(hSliderTimeGame, TBM_GETPOS, 0, 0);
		_stprintf_s(buffer, _TEXT("Время игры : %d секунд"), posSlider);
		SendMessage(hStaticTextTimeGame, WM_SETTEXT, sizeof(buffer), LPARAM(buffer));
		return TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == ID_EXIT_TIME_GAME)
			SendMessage(hWnd, WM_CLOSE, 0, 0);
		return TRUE;

	case WM_CLOSE:
		SendMessage(hSpin, UDM_SETPOS32, 0, LPARAM(posSlider));
		EndDialog(hWnd, 0);
		return TRUE;

		return TRUE;
	}

	return FALSE;
}

//Процедура таблицы рекордов
//##################################################################################

BOOL CALLBACK DlgTableRecordsProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	static HWND hStaticTablePlace;
	static HWND hStaticTableName;
	static HWND hStaticTablePoints;
	
	TCHAR* bufferTable = new TCHAR[1024];
	auto it = Table.rbegin();
	
	switch (uMsg)
	{
	case WM_INITDIALOG:
		hStaticTablePlace = GetDlgItem(hWnd, IDC_TABLE_RECORDS_PLACE);
		hStaticTableName = GetDlgItem(hWnd, IDC_TABLE_RECORDS_NAME);
		hStaticTablePoints = GetDlgItem(hWnd, IDC_TABLE_RECORDS_POINTS);

		memset(bufferTable, 0, sizeof(bufferTable));
		for (int i = 0; i < 10; i++) {
			_stprintf_s(buffer, _TEXT("%d\n\n"), i + 1);
			_tcscat(bufferTable, buffer);
			SendMessage(hStaticTablePlace, WM_SETTEXT, 10, LPARAM(bufferTable));

		}
		memset(bufferTable, 0, sizeof(bufferTable));
		for (int i = 0; i < 10; i++) {
			_tcscat(bufferTable, it->second);
			_tcscat(bufferTable, TEXT("\n\n"));
			SendMessage(hStaticTableName, WM_SETTEXT, 20, LPARAM(bufferTable));
			it++;
		}
		memset(bufferTable, 0, sizeof(bufferTable));
		it = Table.rbegin();
		for (int i = 0; i < 10; i++) {
			_stprintf_s(buffer, _TEXT("%d\n\n"), it->first);
			_tcscat(bufferTable, buffer);
			SendMessage(hStaticTablePoints, WM_SETTEXT, 20, LPARAM(bufferTable));
			it++;
		}	
		delete[] bufferTable;
		return TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == ID_EXIT_TABLE)
			SendMessage(hWnd, WM_CLOSE, 0, 0);
		return TRUE;

	case WM_CLOSE:
		EndDialog(hWnd, 0);
		return TRUE;
	}
	return FALSE;
}
//Автор
//##################################################################################
BOOL CALLBACK DlgAutorProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg)
	{
	case WM_COMMAND:
		if (LOWORD(wParam) == ID_CLOSE)
			SendMessage(hWnd, WM_CLOSE, 0, 0);
			return TRUE;

	case WM_CLOSE:
		EndDialog(hWnd, 0);
		return TRUE;
	}
	return FALSE;
}
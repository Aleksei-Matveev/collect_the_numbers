#pragma once

#pragma comment(lib,"comctl32")

#include <Windows.h>
#include "resource.h"
#include <tchar.h>
#include <fstream>
#include <CommCtrl.h>
#include <map>
#include <ctime>
#include <algorithm>
#include <codecvt>

#define MAX			16
#define TIMER		1

BOOL CALLBACK DlgGameProc(HWND, UINT, WPARAM, LPARAM);

BOOL CALLBACK DlgNameProc(HWND, UINT, WPARAM, LPARAM);

BOOL CALLBACK DlgTableRecordsProc(HWND, UINT, WPARAM, LPARAM);

BOOL CALLBACK DlgTimeGameProc(HWND, UINT, WPARAM, LPARAM);

BOOL CALLBACK DlgAutorProc(HWND, UINT, WPARAM, LPARAM);

HINSTANCE hInstance = GetModuleHandle(NULL);

HWND hDialogGame; //Главное окно
HMENU hMenuGame;//Меню
HWND hProgressBar;//Индикатор

std::map<int, HWND> hButtonPad;//Массив ID и HWND кнопок игрового поля 
std::multimap<int, TCHAR*>Table;//массив таблицы рекордов

TCHAR buffer[100];//буфер для сообщений
TCHAR** fileBuffer = NULL;
//Дескрипторы кнопок
//##################################################################################
HWND hBtnNewGame;//Кнопка НОВАЯ ИГРА
HWND hBtnStop; //Кнопка СТОП
HWND hBtn[16] = { 0 }; //Дескрипторы кнопок игрового поля
HWND hSpin;//Счетчик
//##################################################################################

//Массивы случайных чисел
//##################################################################################
int random[MAX] = { 0 };//Массив случайных чисел
int sortrandom[MAX] = { 0 };//Отсортированный массив случайных чисел
//##################################################################################

//Для статистики заработанных очков
//##################################################################################
int timeGame = 60;//Общее время игры.
int timePlay;//время потраченное на игру до победы
int maxPoints = 100;// максимальное очко за 1 правильное выбранное число
int totalPoints;//Итоговый счет
int points;//Подсчет очков
int timeLeft;//остаток времени
//##################################################################################

//Данные о продолжительности игры
//##################################################################################
int timeGameMin = 10;// минимальное время
int timeGameMax = 60;//максимальное время 

#include "game.h"
#include "workdisk.h"
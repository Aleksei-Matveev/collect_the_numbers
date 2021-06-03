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

HWND hDialogGame; //������� ����
HMENU hMenuGame;//����
HWND hProgressBar;//���������

std::map<int, HWND> hButtonPad;//������ ID � HWND ������ �������� ���� 
std::multimap<int, TCHAR*>Table;//������ ������� ��������

TCHAR buffer[100];//����� ��� ���������
TCHAR** fileBuffer = NULL;
//����������� ������
//##################################################################################
HWND hBtnNewGame;//������ ����� ����
HWND hBtnStop; //������ ����
HWND hBtn[16] = { 0 }; //����������� ������ �������� ����
HWND hSpin;//�������
//##################################################################################

//������� ��������� �����
//##################################################################################
int random[MAX] = { 0 };//������ ��������� �����
int sortrandom[MAX] = { 0 };//��������������� ������ ��������� �����
//##################################################################################

//��� ���������� ������������ �����
//##################################################################################
int timeGame = 60;//����� ����� ����.
int timePlay;//����� ����������� �� ���� �� ������
int maxPoints = 100;// ������������ ���� �� 1 ���������� ��������� �����
int totalPoints;//�������� ����
int points;//������� �����
int timeLeft;//������� �������
//##################################################################################

//������ � ����������������� ����
//##################################################################################
int timeGameMin = 10;// ����������� �����
int timeGameMax = 60;//������������ ����� 

#include "game.h"
#include "workdisk.h"
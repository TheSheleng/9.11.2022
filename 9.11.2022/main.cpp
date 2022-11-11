#include <windows.h>
#include <time.h>
#include "resource.h"

BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);

#define COUNT_NUM_LIST 20

#define NUM_MIN -10
#define NUM_MAX 10

#define DEFAULT_STR_SIZE 10

struct
{
	static const int COUNT_DEF = 3;

	const WCHAR* Name[COUNT_DEF]{
		TEXT("Сложение"),
		TEXT("Произведение"),
		TEXT("Среднее арифмет."),
	};
} nbyiActin;

HWND hAct, hNum, hCal, hOut;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPWSTR lpszCmdLine, int nCmdShow)
{

	return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG), NULL, (DLGPROC)DlgProc);
}

BOOL CALLBACK DlgProc(HWND hWnd, UINT mess, WPARAM wParam, LPARAM lParam)
{
	WCHAR str_nun[DEFAULT_STR_SIZE];
	switch (mess)
	{
	case WM_INITDIALOG:
	{
		//Деструкторы елем.
		hAct = GetDlgItem(hWnd, IDC_ACTION);
		hNum = GetDlgItem(hWnd, IDC_NUMBERS);
		hCal = GetDlgItem(hWnd, IDC_CALCULATE);
		hOut = GetDlgItem(hWnd, IDC_STATIC_OUT);

		//Возможные действия
		for (int i = 0; i < nbyiActin.COUNT_DEF; ++i) 
			SendMessage(
				hAct, 
				CB_ADDSTRING, 
				0, 
				LPARAM(nbyiActin.Name[i]
			));

		//Заполнения чисел для вибор
		srand(time(NULL));
		for (int i = 0; i < COUNT_NUM_LIST; ++i)
		{
			_itow_s(NUM_MIN + rand() % (NUM_MAX - NUM_MIN + 1), str_nun, DEFAULT_STR_SIZE);
			SendMessage(hNum, LB_ADDSTRING, 0, LPARAM(str_nun));
		}
	}
	return TRUE;

	case  WM_COMMAND:
	{
		if (LOWORD(wParam) == IDC_CALCULATE)
		{
			//Фиксирование действия
			int iAction = SendMessage(hAct, CB_GETCURSEL, 0, 0);
			if (iAction == -1)
			{
				MessageBox(
					hWnd, 
					TEXT("Выберите действие"), 
					TEXT("Предупреждение"), 
					MB_OK | MB_ICONINFORMATION
				);
				break;
			}

			//Количество елем.
			int nCount = SendMessage(hNum, LB_GETSELCOUNT, 0, 0);
			int* OperVal = new int[nCount]; //Оперируемые значения
			int* index = new int[nCount]; //Массив индексов строк
			SendMessage(hNum, LB_GETSELITEMS, nCount, LPARAM(index));

			for (int i = 0; i < nCount; i++)
			{
				//Копирование строк
				WCHAR* pBuffer = new WCHAR[SendMessage(hNum, LB_GETTEXTLEN, index[i], 0) + 1];
				SendMessage(hNum, LB_GETTEXT, index[i], LPARAM(pBuffer));
				OperVal[i] = _wtoi(pBuffer);

				delete[] pBuffer;
			}

			//Выпонение действия
			int Answer = 0;
			switch (iAction)
			{
			case 0: //Сложение
				for (int i = 0; i < nCount; ++i)
					Answer += OperVal[i];
			break;

			case 1: //Произведение
				Answer += 1;
				for (int i = 0; i < nCount; ++i)
					Answer *= OperVal[i];
			break;

			case 2: //Сред. ареф.
				for (int i = 0; i < nCount; ++i)
					Answer += OperVal[i];
				Answer /= nCount;
			break;
			}

			//Вывод
			_itow_s(Answer, str_nun, DEFAULT_STR_SIZE);
			SendMessage(hOut, WM_SETTEXT, 0, (WPARAM)str_nun);

			delete[] OperVal, index;
		}
	}
	return TRUE;

	case WM_CLOSE: 
		EndDialog(hWnd, NULL);
		return TRUE;
	}
	return FALSE;
}
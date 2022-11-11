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
		TEXT("��������"),
		TEXT("������������"),
		TEXT("������� �������."),
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
		//����������� ����.
		hAct = GetDlgItem(hWnd, IDC_ACTION);
		hNum = GetDlgItem(hWnd, IDC_NUMBERS);
		hCal = GetDlgItem(hWnd, IDC_CALCULATE);
		hOut = GetDlgItem(hWnd, IDC_STATIC_OUT);

		//��������� ��������
		for (int i = 0; i < nbyiActin.COUNT_DEF; ++i) 
			SendMessage(
				hAct, 
				CB_ADDSTRING, 
				0, 
				LPARAM(nbyiActin.Name[i]
			));

		//���������� ����� ��� �����
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
			//������������ ��������
			int iAction = SendMessage(hAct, CB_GETCURSEL, 0, 0);
			if (iAction == -1)
			{
				MessageBox(
					hWnd, 
					TEXT("�������� ��������"), 
					TEXT("��������������"), 
					MB_OK | MB_ICONINFORMATION
				);
				break;
			}

			//���������� ����.
			int nCount = SendMessage(hNum, LB_GETSELCOUNT, 0, 0);
			int* OperVal = new int[nCount]; //����������� ��������
			int* index = new int[nCount]; //������ �������� �����
			SendMessage(hNum, LB_GETSELITEMS, nCount, LPARAM(index));

			for (int i = 0; i < nCount; i++)
			{
				//����������� �����
				WCHAR* pBuffer = new WCHAR[SendMessage(hNum, LB_GETTEXTLEN, index[i], 0) + 1];
				SendMessage(hNum, LB_GETTEXT, index[i], LPARAM(pBuffer));
				OperVal[i] = _wtoi(pBuffer);

				delete[] pBuffer;
			}

			//��������� ��������
			int Answer = 0;
			switch (iAction)
			{
			case 0: //��������
				for (int i = 0; i < nCount; ++i)
					Answer += OperVal[i];
			break;

			case 1: //������������
				Answer += 1;
				for (int i = 0; i < nCount; ++i)
					Answer *= OperVal[i];
			break;

			case 2: //����. ����.
				for (int i = 0; i < nCount; ++i)
					Answer += OperVal[i];
				Answer /= nCount;
			break;
			}

			//�����
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
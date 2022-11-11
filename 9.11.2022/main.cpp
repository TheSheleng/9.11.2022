#include <windows.h>
#include "resource.h"

BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);

HWND hList, hCombo, hCheck, hEdit;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPWSTR lpszCmdLine, int nCmdShow)
{
	return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG), NULL, (DLGPROC)DlgProc);
}

BOOL CALLBACK DlgProc(HWND hWnd, UINT mess, WPARAM wParam, LPARAM lParam)
{
	switch (mess)
	{
	case WM_INITDIALOG:
		hCombo = GetDlgItem(hWnd, IDC_COMBO2);

		SendMessage(hCombo, CB_ADDSTRING, 0, LPARAM(TEXT("Зима")));
		SendMessage(hCombo, CB_ADDSTRING, 0, LPARAM(TEXT("Весна")));
		SendMessage(hCombo, CB_ADDSTRING, 0, LPARAM(TEXT("Лето")));
		SendMessage(hCombo, CB_ADDSTRING, 0, LPARAM(TEXT("Осень")));

		return TRUE;

	case WM_CLOSE: 
		EndDialog(hWnd, NULL);
		return TRUE;
	}
	return FALSE;
}
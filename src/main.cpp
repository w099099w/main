#include "../head/stdafx.h"

#define  DIALOGBASEW 480
#define  DIALOGBASEH 410
#define	 DIALOGBASEW1 718
#define  DIALOGBASEH1 430
Manifest* manifest = new Manifest();
HINSTANCE hInst = NULL;
int  cx = GetSystemMetrics(SM_CXSCREEN) / 2;
int  cy = GetSystemMetrics(SM_CYSCREEN) / 2;

INT_PTR CALLBACK childDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG: {
		MoveWindow(hwndDlg, cx - DIALOGBASEW1 / 2, cy - DIALOGBASEH1 / 2, DIALOGBASEW1, DIALOGBASEH1, TRUE);
		//manifest->showStatue(hwndDlg,"开始获取文件...\r\n");
		SetTimer(hwndDlg,100,1000, NULL);
	}break;
	case WM_TIMER: {
		KillTimer(hwndDlg, 100);
		manifest->buildManifest(hwndDlg);
	}break;
	case WM_COMMAND: {
		if (BN_CLICKED == HIWORD(wParam))
		{
			switch (LOWORD(wParam))
			{
			case IDCANCEL: {
				EndDialog(hwndDlg, 0);
			}break;
			case BUTTON_CLOSED2: {
				EndDialog(hwndDlg, 0);
			}break;
			case BUTTON_RETURN2: {
				EndDialog(hwndDlg, IDD_DIALOG1);
			}break;
			}
		}
	}break;
	}
	return 0;
}



INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_INITDIALOG: {
			MoveWindow(hwndDlg, cx- DIALOGBASEW/2, cy- DIALOGBASEH/2, DIALOGBASEW, DIALOGBASEH, TRUE);
		}break;
		case WM_COMMAND:{
			if (BN_CLICKED == HIWORD(wParam))
			{
				switch (LOWORD(wParam))
				{
					case IDCANCEL:{
						EndDialog(hwndDlg, 0);
					}break;
					case IDOK:{
						if (manifest->checkParams(hwndDlg)) {
							EndDialog(hwndDlg, IDD_DIALOG2);
						}	
					}break;
					case BUTTON_FINDDATAPATH:{
						manifest->showChooseFloder(hwndDlg, EDIT_FINDPATH);
					}break;
					case BUTTON_SAVEDATAPATH: {
						manifest->showChooseFloder(hwndDlg, EDIT_SAVEPATH);
					}break;
				}
			}
		}break;
	}
	return 0;
}

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	hInst = hInstance;
	AllocConsole();
	freopen("CON", "r", stdin);
	freopen("CON", "w", stdout);
	freopen("CON", "w", stderr);
	INT_PTR r = DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), HWND_DESKTOP, DialogProc);
	while (r) {
		switch (r) {
			case IDD_DIALOG1:r = DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), HWND_DESKTOP, DialogProc); break;
			case IDD_DIALOG2:r = DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG2), HWND_DESKTOP, childDialogProc); break;
		}
		if (r == 0) {
			break;
		}
	}
	
	FreeConsole();
	return 1;
}
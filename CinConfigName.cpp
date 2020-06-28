// CinConfigName.cpp: 实现文件
//

#include "pch.h"
#include "main.h"
#include "CinConfigName.h"
#include "afxdialogex.h"

string CinConfigName::m_cinstr = ""; // 
// CinConfigName 对话框

IMPLEMENT_DYNAMIC(CinConfigName, CDialogEx)

CinConfigName::CinConfigName(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CINNAME_DIALOG, pParent)
{

}

CinConfigName::~CinConfigName()
{
}

void CinConfigName::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, EDIT_CINNAME, m_edit);
}


BEGIN_MESSAGE_MAP(CinConfigName, CDialogEx)
	ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()


// CinConfigName 消息处理程序


void CinConfigName::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nID)
	{
		case SC_CLOSE: {
			EndDialog(0);//放弃输入
		}break;
	}
	CDialogEx::OnSysCommand(nID, lParam);
}


BOOL CinConfigName::OnCommand(WPARAM wParam, LPARAM lParam)
{
	if (BN_CLICKED == HIWORD(wParam))
	{
		switch (LOWORD(wParam))
		{
			case BUTTON_CONFIRMNAME: {
				char buff[32] = {};
				GetDlgItemTextA(GetSafeHwnd(), EDIT_CINNAME, buff, 32);
				CinConfigName::m_cinstr = buff;
				if (CinConfigName::m_cinstr.length() == 0) {
					MessageBoxA(GetSafeHwnd(), "输入的名称不合法!", "提示!", MB_OK);
				}else {
					for (vector<string>::iterator it = mainView->m_cList.begin(); it != mainView->m_cList.end(); it++) {
						if (*it == CinConfigName::m_cinstr) {
							MessageBoxA(GetSafeHwnd(), "已有该名称的配置!您可以选择保存或换一个名称!", "提示!", MB_OK);
							return 1;
						}
					}
					EndDialog(1);
				}
			}break;
		}
	}

	return CDialogEx::OnCommand(wParam, lParam);
}


BOOL CinConfigName::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_edit.SetLimitText(5);
	// TODO:  在此添加额外的初始化
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

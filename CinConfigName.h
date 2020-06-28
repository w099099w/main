#pragma once
// CinConfigName 对话框
#include "pch.h"
class CinConfigName : public CDialogEx
{
	DECLARE_DYNAMIC(CinConfigName)

public:
	CinConfigName(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CinConfigName();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CINNAME_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_edit;
	static string m_cinstr;
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnInitDialog();
};
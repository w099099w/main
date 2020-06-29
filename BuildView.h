#pragma once
#include "pch.h"
// BuildView 对话框

class BuildView : public CDialogEx
{
	DECLARE_DYNAMIC(BuildView)

public:
	BuildView(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~BuildView();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BUILD_DIALOG };
#endif

protected:
	HICON m_hIcon;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	void setTitle(string title);
public:
	int m_findTime;
	int m_runtime;
	int allcount;
	int curcount;
	bool addString(string s);
	bool setAllLabel(string s);
	bool setPercentLabel(int cur, int all);
	bool setCurLabel(string s);
	void setButtonStaue(int buttonID, bool isclick);
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	CProgressCtrl curProgressCtrl;
	CProgressCtrl allProgressCtrl;
	CStatic allLabel;
	CStatic curLabel;
	CStatic percentLabel;
};

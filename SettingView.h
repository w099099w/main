#pragma once
#include "pch.h"

// SettingView 对话框
class SettingView : public CDialogEx
{
// 构造
public:
	SettingView(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAIN_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
private:
		CString cachePath;
// 实现
protected:
	HICON m_hIcon;
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID,LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP();
public:
	HWND m_hwnd;
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	bool saveConfig(string appName);
	void setButtonStaue(int buttonID, bool isclick);
	void SetList();
	string m_config;
	CButton m_formatFloder;
	CButton m_copy;
	CButton m_useConfig;
	CButton m_createZip;
	afx_msg void OnCbnSelchangeConfiglist();
	bool setConfigshow(map<string, string> info);
	void reset();
	vector<string> m_cList;
	CComboBox m_configList;
};
extern SettingView* mainView;

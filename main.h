
// main.h: PROJECT_NAME 应用程序的主头文件
//

#pragma once
#include "pch.h"
class Manifest;
#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含 'pch.h' 以生成 PCH"
#endif

// SetView:
// 有关此类的实现，请参阅 main.cpp
//

class SetView : public CWinApp
{
public:
	SetView();
	~SetView();
	Manifest* m_manifest;
// 重写
public:
	virtual BOOL InitInstance();
// 实现
	DECLARE_MESSAGE_MAP()
};

extern SetView theApp;

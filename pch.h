// pch.h: 这是预编译标头文件。
// 下方列出的文件仅编译一次，提高了将来生成的生成性能。
// 这还将影响 IntelliSense 性能，包括代码完成和许多代码浏览功能。
// 但是，如果此处列出的文件中的任何一个在生成之间有更新，它们全部都将被重新编译。
// 请勿在此处添加要频繁更新的文件，这将使得性能优势无效。

#if defined _M_IX86  
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")  
#elif defined _M_X64  
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")  
#else  
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")  
#endif

#ifndef PCH_H
#define PCH_H

#define _WIN32_WINNT 0x0502
//内部使用的宏
#define TIMER_INITDIALOG               1
#define TIMER_RUN					   2
#define TIMER_SUB                      3
#define TIMER_FIND                     4
#define TIME_RUNTIME                   3
#define MAX_CONFIGNUM				   12
#define TASK_NONE                      100
#define TASK_FOMATFLODER               101
#define TASK_CHECKFILE                 102
#define TASK_BUILDMANIFEST             103
#define TASK_COPYFILE                  104
#define TASK_CREATEZIP                 105
#define TASK_SUCCESS                   106
//结构体
struct RATIO
{
	int cur, all;
};
// 添加要在此处预编译的标头
using namespace std;
#include "resource.h"
#include <iostream>
#include <vector>
#include <io.h>
#include <fstream>
#include <thread>
#include <afxcontrolbars.h>
#include "afxdialogex.h"
#include "framework.h"
#include "head/tool/ThreadPool.h"
#include "head/tool/cJSON.h"
#include "head/tool/md5.h"
#include "head/tool/zip.h"
#include "head/tool/CJsonObject.hpp"
#include "head/tool/WriteRedini.h"
#include "head/manifest.h"
#include "CinConfigName.h"
#include "AboutDlg.h"
#include "BuildView.h"
#include "SettingView.h"
#include "main.h"
#endif //PCH_H

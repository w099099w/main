// BuildView.cpp: 实现文件
//
#include "BuildView.h"

DWORD dwDlgHandle = NULL;
// BuildView 对话框

void ThreadSetAll()
{
	BuildView* dDlg = (BuildView*)dwDlgHandle;	//取得句柄信息
	while (true) {
		if (dDlg->allLabelString != "") {
			dDlg->setAllLabel(dDlg->allLabelString);
			dDlg->allLabelString = "";
		}
		if (dDlg->curLabelString != "") {
			dDlg->setCurLabel(dDlg->curLabelString);
			dDlg->curLabelString = "";
		}
		if (dDlg->percentLabelValue.cur != -1 && dDlg->percentLabelValue.all != -1) {
			dDlg->setPercentLabel(dDlg->percentLabelValue.cur, dDlg->percentLabelValue.all);
			dDlg->percentLabelValue.cur = dDlg->percentLabelValue.all = -1;
		}
		if (dDlg->curProgressPos != -1) {
			dDlg->curProgressCtrl.SetPos(dDlg->curProgressPos);
			dDlg->curProgressPos = -1;
		}
		if (dDlg->allProgressPos != -1) {
			dDlg->allProgressCtrl.SetPos(dDlg->allProgressPos);
			dDlg->allProgressPos = -1;
		}
		/*
		dDlg->setAllLabel(dDlg->allLabelString);
		dDlg->setCurLabel(dDlg->curLabelString);
		dDlg->setPercentLabel(dDlg->percentLabelValue.cur, dDlg->percentLabelValue.all);
		dDlg->curProgressCtrl.SetPos(dDlg->curProgressPos);
		dDlg->allProgressCtrl.SetPos(dDlg->allProgressPos);
		*/
		if (dDlg->endThread) {
			break;
		}
	}
	ExitThread(0);
}
void Threadtask() {
	BuildView* dDlg = (BuildView*)dwDlgHandle;
	int thisCur = dDlg->curTask;
	while (true)
	{
		if (thisCur != dDlg->curTask) {
			switch (dDlg->curTask) {
			case TASK_NONE: {
				dDlg->curLabelString = "准备中...";
				dDlg->allLabelString = "准备中...";
			}break;
			case TASK_FOMATFLODER: {
				dDlg->fomatFloder();
			}break;
			case TASK_CHECKFILE: {
				dDlg->checkFile();
			}break;
			case TASK_BUILDMANIFEST: {
				dDlg->buildManifest();
			}break;
			case TASK_COPYFILE: {
				dDlg->copyfile();
			}break;
			case TASK_CREATEZIP: {
				dDlg->createZip();
			}break;
			}
		}
		int thisCur = dDlg->curTask;
		if (thisCur == TASK_SUCCESS) {
			dDlg->tasksuccess();
			break;
		}
		else if (dDlg->endThread) {
			break;
		}
	}
	ExitThread(0);
}
IMPLEMENT_DYNAMIC(BuildView, CDialogEx)
BuildView::BuildView(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_BUILD_DIALOG, pParent),
	m_runtime(TIME_RUNTIME),
	m_findTime(0),
	allcount(0),
	curcount(0)
{
	theApp.m_manifest->setMFCParam(&allLabelString, &curLabelString, &percentLabelValue, &allProgressPos, &curProgressPos);
	theApp.m_manifest->reset();
	allProgressPos = curProgressPos = -1;
	percentLabelValue.cur = percentLabelValue.all = -1;
	curTask = TASK_NONE;
	H_ThreadAll = NULL;
	H_Threadtask = NULL;
	endThread = false;
}

BuildView::~BuildView()
{
}

void BuildView::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDP_CURPERCENT, curProgressCtrl);
	DDX_Control(pDX, IDP_ALLPERCENT, allProgressCtrl);
	DDX_Control(pDX, IDS_ALLLABEL, allLabel);
	DDX_Control(pDX, IDS_STATELABEL, curLabel);
	DDX_Control(pDX, IDS_PERCENTLABEL, percentLabel);
}


BEGIN_MESSAGE_MAP(BuildView, CDialogEx)
ON_WM_TIMER()
ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()


BOOL BuildView::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
		case BUTTON_RETURN2: {
			theApp.m_manifest->reset();
			KillTimer(TIMER_RUN);
			EndDialog(IDD_MAIN_DIALOG);
		}break;
		case BUTTON_CLOSED2: {
			KillTimer(TIMER_RUN);
			EndDialog(0);//用返回值关闭上级对话框
		}break;
		case BUTTON_OPENTARGETDIR: {
			string openDir = "start " + theApp.m_manifest->getSavePath();
			system(openDir.c_str());
		}break;
	}
	// TODO: 在此添加专用代码和/或调用基类
	return CDialogEx::OnCommand(wParam, lParam);
}

void BuildView::setTitle(string title)
{
	SetWindowText(StoWs(title).c_str());
}

bool BuildView::setAllLabel(string s) {
	std::wstring stemp = StoWs("当前操作:"+s);
	LPCWSTR result = stemp.c_str();
	GetDlgItem(IDS_ALLLABEL)->SetWindowText(result);
	return true;
}
bool BuildView::setPercentLabel(int cur, int all)
{
	std::wstring stemp = StoWs("总进度:" + to_string(cur) + "/" + to_string(all));
	LPCWSTR result = stemp.c_str();
	GetDlgItem(IDS_PERCENTLABEL)->SetWindowText(result);
	return true;
}
bool BuildView::setCurLabel(string s)
{
	std::wstring stemp = StoWs(s);
	LPCWSTR result = stemp.c_str();
	GetDlgItem(IDS_STATELABEL)->SetWindowText(result);
	return true;
}
void BuildView::setButtonStaue(int buttonID,bool isclick)
{
	GetDlgItem(buttonID)->EnableWindow(isclick);
}

void BuildView::nextTaskState()
{
	switch (curTask)
	{
	case TASK_NONE: {
		if (mainView->m_formatFloder.GetCheck() == 1) {
			curTask = TASK_FOMATFLODER;
		}
		else {
			curTask = TASK_CHECKFILE;
		}
	}break;
	case TASK_FOMATFLODER: {
		curTask = TASK_CHECKFILE;
	}break;
	case TASK_CHECKFILE: {
		curTask = TASK_BUILDMANIFEST;
	}break;
	case TASK_BUILDMANIFEST: {
		if (mainView->m_copy.GetCheck() == 1) {
			curTask = TASK_COPYFILE;
		}
		else if (mainView->m_createZip.GetCheck() == 1) {
			curTask = TASK_CREATEZIP;
		}
		else {
			curTask = TASK_SUCCESS;
		}
	}break;
	case TASK_COPYFILE: {
		if (mainView->m_createZip.GetCheck() == 1) {
			curTask = TASK_CREATEZIP;
		}
		else {
			curTask = TASK_SUCCESS;
		}
	}break;
	case TASK_CREATEZIP: {
		curTask = TASK_SUCCESS;
	}break;
	default:
		break;
	}
}

bool BuildView::fomatFloder()
{
	setTitle("执行用户配置->格式化保存文件夹...");
	curLabelString = "正在删除" + theApp.m_manifest->getSavePath() + "下的所有文件...";
	allLabelString = "正在删除" + theApp.m_manifest->getSavePath() + "下的所有文件...";
	theApp.m_manifest->DeleteDirectory(theApp.m_manifest->getSavePath().c_str());
	curProgressPos = 100;
	curLabelString = "删除完成!";
	curcount += 1;
	percentLabelValue.cur = curcount;
	percentLabelValue.all = allcount;
	allProgressPos = (int)(((float)curcount / (float)allcount) * 100);
	nextTaskState();
	return true;
}

bool BuildView::checkFile()
{
	curProgressCtrl.SetPos(0);
	setTitle("正在检索...");
	allLabelString = "正在检索文件...";
	curLabelString = "准备检索文件...";
	SetTimer(TIMER_FIND, 1000, NULL);
	theApp.m_manifest->findFiles();//嵌入进度显示
	curcount += 1;
	curProgressPos = 100;
	percentLabelValue.cur = curcount;
	percentLabelValue.all = allcount;
	allProgressPos = (int)(((float)curcount / (float)allcount) * 100);
	KillTimer(TIMER_FIND);
	nextTaskState();
	return true;
}

bool BuildView::buildManifest()
{
	setTitle("正在生成version.manifest...");
	curLabelString = "正在生成version.manifest...";
	curProgressPos = 0;

	//生成version.manifest;
	if (theApp.m_manifest->buildVesion()) {
		UpdateData(FALSE);
		curLabelString = "version.manifest生成完成";
	}
	else {
		curLabelString = "发生错误,文件生成失败...";
		setButtonStaue(BUTTON_CLOSED2, TRUE);
		setButtonStaue(BUTTON_RETURN2, TRUE);
		MessageBoxA(GetSafeHwnd(), "version.manifest生成失败!", "错误!", MB_OK);
		nextTaskState();
		return false;
	}
	//生成project.manifest;
	setTitle("正在生成project.manifest...");
	allLabelString = "正在生成project.manifest";
	curLabelString = "正在生成project.manifest...";
	curProgressPos = 0;
	if (theApp.m_manifest->buildProject(curcount, allcount, GetSafeHwnd())) {
		setTitle("project.manifest生成完成");
		curLabelString = "生成成功文件保存至" + theApp.m_manifest->getSavePath() + "  文件名:project.manifest\r\n";
		allLabelString = "文件[2/2]---->已完成所有生成....";
		setTitle("生成完成");
	}
	else {
		setCurLabel("发生错误,文件生成失败...");
		setButtonStaue(BUTTON_CLOSED2, TRUE);
		setButtonStaue(BUTTON_RETURN2, TRUE);
		MessageBoxA(GetSafeHwnd(), "project.manifest生成失败!", "错误!", MB_OK);
		nextTaskState();
		return false;
	}
	curcount += 1;
	curProgressPos = 100;
	percentLabelValue.cur = curcount;
	percentLabelValue.all = allcount;
	allProgressPos = (int)(((float)curcount / (float)allcount) * 100);
	nextTaskState();
	return true;
}

bool BuildView::copyfile()
{
	curProgressCtrl.SetPos(0);
	setTitle("正在创建目录...");
	allLabelString = "正在创建目录...";
	theApp.m_manifest->createFloder(curcount, allcount);
	setTitle("正在复制文件...");
	allLabelString = "正在复制文件...";
	if (!theApp.m_manifest->copyFiles(curcount, allcount, GetSafeHwnd())) {
		MessageBoxA(GetSafeHwnd(), "文件复制失败,请手动复制文件", "错误!", MB_OK);
		nextTaskState();
		return false;
	}
	curcount += 1;
	curProgressPos = 100;
	percentLabelValue.cur = curcount;
	percentLabelValue.all = allcount;
	allProgressPos = (int)(((float)curcount / (float)allcount) * 100);
	nextTaskState();
	return true;
}

bool BuildView::createZip()
{
	curProgressPos = 0;
	setTitle("正在创建压缩文件...");
	allLabelString = "创建压缩文件...";
	string zipName = theApp.m_manifest->getSavePath() + "/version" + theApp.m_manifest->getLineVersion() + "-" + theApp.m_manifest->getversion() + ".zip";
	if (!theApp.m_manifest->createZip(curcount, allcount, zipName, theApp.m_manifest->getSavePath())) {
		MessageBoxA(GetSafeHwnd(), "创建压缩文件失败,请手动压缩", "错误!", MB_OK);
		nextTaskState();
		return false;
	}
	curcount += 1;
	curProgressPos = 100;
	percentLabelValue.cur = curcount;
	percentLabelValue.all = allcount;
	allProgressPos = (int)(((float)curcount / (float)allcount) * 100);
	nextTaskState();
	return false;
}

bool BuildView::tasksuccess()
{
	curProgressPos = 100;
	allProgressPos = 100;
	percentLabelValue.cur = allcount;
	percentLabelValue.all = allcount;
	allProgressPos = (int)(((float)allcount / (float)allcount) * 100);
	setTitle("完成!");
	allLabelString = "已完成所有操作!";
	curLabelString = "已完成所有操作!";
	setButtonStaue(BUTTON_CLOSED2, TRUE);
	setButtonStaue(BUTTON_RETURN2, TRUE);
	return false;
}

BOOL BuildView::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	dwDlgHandle = (DWORD)this;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	setButtonStaue(BUTTON_CLOSED2, FALSE);
	setButtonStaue(BUTTON_RETURN2, FALSE);
	//创建线程
	H_ThreadAll = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadSetAll, NULL, NULL, NULL);
	H_Threadtask = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Threadtask, NULL, NULL, NULL);
	
	SetTimer(1, 300, NULL);
	allProgressCtrl.SetRange(0, 100);
	curProgressCtrl.SetRange(0, 100);
	allProgressCtrl.SetPos(0);
	curProgressCtrl.SetPos(0);
	allcount = 2;
	curcount = 0;
	// TODO:  在此添加额外的初始化
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void BuildView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
	case TIMER_INITDIALOG: {
		KillTimer(TIMER_INITDIALOG);
		allLabelString = "准备开始";
		setTitle("准备");
		curLabelString = "准备...";
		if (mainView->m_formatFloder.GetCheck() == 1) {
			allcount += 1;
		}
		if (mainView->m_copy.GetCheck() == 1) {
			allcount += 1;
		}
		if (mainView->m_createZip.GetCheck() == 1) {
			allcount += 1;
		}
		percentLabelValue.cur = curcount;
		percentLabelValue.all = allcount;
		SetTimer(TIMER_SUB,1000,NULL);
	}break;
	case TIMER_FIND: {
		m_findTime += 1;
		char t[16] = {};
		_itoa(m_findTime, t, 10);
		string st(t);
		curLabelString = "正在检索  已耗时 " + st + " 秒...";
	}break;
	case TIMER_SUB: {
		char time[8] = {};
		_itoa(m_runtime, time, 10);
		setTitle("距离开始生成还有" + (string)time + "秒...");
		if (m_runtime == 0) {
			KillTimer(TIMER_SUB);
			SetTimer(TIMER_RUN, 100, NULL);
		}
		m_runtime -= 1;
	}break;
	case TIMER_RUN: {
		curProgressPos = 0;
		KillTimer(TIMER_RUN);
		nextTaskState();
	}
	default:
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}

void BuildView::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nID)
	{
		case SC_CLOSE: {
			closeHandle();
			EndDialog(0);//用返回值关闭上级对话框
		}break;
		case WM_CLOSE: {
			closeHandle();
		}
	}
	CDialogEx::OnSysCommand(nID, lParam);
}
void BuildView::closeHandle() {
	endThread = true;
	if (H_ThreadAll) {
		TerminateThread(H_ThreadAll, 0);
		CloseHandle(H_ThreadAll);
		H_ThreadAll = NULL;
	}
	if (H_Threadtask) {
		TerminateThread(H_Threadtask, 0);
		CloseHandle(H_Threadtask);
		H_Threadtask = NULL;
	}
}
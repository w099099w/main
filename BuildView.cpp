// BuildView.cpp: 实现文件
//

#include "BuildView.h"

// BuildView 对话框

IMPLEMENT_DYNAMIC(BuildView, CDialogEx)
BuildView::BuildView(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_BUILD_DIALOG, pParent),
	m_runtime(TIME_RUNTIME),
	m_findTime(0),
	allcount(0),
	curcount(0)
{
	theApp.m_manifest->setMFCParam(&allLabel, &curLabel, &percentLabel, &allProgressCtrl, &curProgressCtrl);
	theApp.m_manifest->reset();
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

bool BuildView::addString(string s) {
	return true;
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

BOOL BuildView::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	setButtonStaue(BUTTON_CLOSED2, FALSE);
	setButtonStaue(BUTTON_RETURN2, FALSE);
	SetTimer(1, 300, NULL);
	allProgressCtrl.SetRange(0, 100);
	curProgressCtrl.SetRange(0, 100);
	allProgressCtrl.SetPos(0);
	curProgressCtrl.SetPos(0);
	allcount = 3;
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
		setAllLabel("准备开始");
		setTitle("准备");
		setCurLabel("准备...");
		if (mainView->m_formatFloder.GetCheck() == 1) {
			allcount += 1;
		}
		if (mainView->m_copy.GetCheck() == 1) {
			allcount += 1;
		}
		if (mainView->m_createZip.GetCheck() == 1) {
			allcount += 1;
		}
		setPercentLabel(curcount, allcount);
		SetTimer(TIMER_SUB,1000,NULL);
	}break;
	case TIMER_FIND: {
		m_findTime += 1;
		char t[16] = {};
		_itoa(m_findTime, t, 10);
		string st(t);
		setCurLabel("正在检索(已耗时( " + st + " 秒)...");
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
		curProgressCtrl.SetPos(0);
		KillTimer(TIMER_RUN);
		setTitle("执行用户配置->格式化保存文件夹...");
		//格式化
		if (mainView->m_formatFloder.GetCheck() == 1) {
			setCurLabel("正在删除" + theApp.m_manifest->getSavePath() + "下的所有文件...");
			setAllLabel("正在删除" + theApp.m_manifest->getSavePath() + "下的所有文件...");
			theApp.m_manifest->DeleteDirectory(theApp.m_manifest->getSavePath().c_str());
			curProgressCtrl.SetPos(100);
			setCurLabel("删除完成!" );
			curcount += 1;
			setPercentLabel(curcount, allcount);
			allProgressCtrl.SetPos((int)(((float)curcount / (float)allcount) * 100));
		}
		//文件检索
		curProgressCtrl.SetPos(0);
		setTitle("正在检索...");
		setAllLabel("正在检索文件...");
		setCurLabel("准备检索文件...");
		SetTimer(TIMER_FIND, 1000, NULL);
		theApp.m_manifest->findFiles();//嵌入进度显示
		curcount += 1;
		curProgressCtrl.SetPos(100);
		setPercentLabel(curcount, allcount);
		allProgressCtrl.SetPos((int)(((float)curcount / (float)allcount) * 100));
		KillTimer(TIMER_FIND);


		setTitle("正在生成version.manifest...");
		setAllLabel("正在生成version.manifest...");
		curProgressCtrl.SetPos(0);

		//生成version.manifest;
		if (theApp.m_manifest->buildVesion()) {
			UpdateData(FALSE);
			setCurLabel("version.manifest生成完成");
		}else{
			setCurLabel("发生错误,文件生成失败...");
			setButtonStaue(BUTTON_CLOSED2, TRUE);
			setButtonStaue(BUTTON_RETURN2, TRUE);
			MessageBoxA(GetSafeHwnd(), "version.manifest生成失败!", "错误!", MB_OK);
			return;
		}
		curcount += 1;
		curProgressCtrl.SetPos(100);
		setPercentLabel(curcount, allcount);
		allProgressCtrl.SetPos((int)(((float)curcount / (float)allcount) * 100));

		//生成project.manifest;
		setTitle("正在生成project.manifest...");
		setAllLabel("正在生成project.manifest");
		setCurLabel("正在生成project.manifest...");
		curProgressCtrl.SetPos(0);
		if (theApp.m_manifest->buildProject(curcount,allcount,GetSafeHwnd())) {
			setTitle("project.manifest生成完成");
			setCurLabel("生成成功文件保存至" + theApp.m_manifest->getSavePath() + "  文件名:project.manifest\r\n");
			setCurLabel("文件[2/2]---->已完成所有生成....");
			setTitle("生成完成");
		}
		else {
			setCurLabel("发生错误,文件生成失败...");
			setButtonStaue(BUTTON_CLOSED2, TRUE);
			setButtonStaue(BUTTON_RETURN2, TRUE);
			MessageBoxA(GetSafeHwnd(), "project.manifest生成失败!", "错误!", MB_OK);
			return;
		}
		curcount += 1;
		curProgressCtrl.SetPos(100);
		setPercentLabel(curcount, allcount);
		allProgressCtrl.SetPos((int)(((float)curcount / (float)allcount) * 100));

		//复制文件
		if (mainView->m_copy.GetCheck() == 1) {
			curProgressCtrl.SetPos(0);
			setTitle("正在创建目录...");
			setAllLabel("正在创建目录...");
			theApp.m_manifest->createFloder(curcount, allcount);
			setTitle("正在复制文件...");
			setAllLabel("正在复制文件...");
			if (!theApp.m_manifest->copyFiles(curcount, allcount,GetSafeHwnd())) {
				MessageBoxA(GetSafeHwnd(), "文件复制失败,请手动复制文件", "错误!", MB_OK);
				return;
			}
			curcount += 1;
			curProgressCtrl.SetPos(100);
			setPercentLabel(curcount, allcount);
			allProgressCtrl.SetPos((int)(((float)curcount / (float)allcount) * 100));
		}
		//创建压缩文件
		if (mainView->m_createZip.GetCheck() == 1) {
			curProgressCtrl.SetPos(0);
			setTitle("正在创建压缩文件...");
			setAllLabel("创建压缩文件...");
			string zipName = theApp.m_manifest->getSavePath() + "/version" + theApp.m_manifest->getLineVersion() + "-" + theApp.m_manifest->getversion() + ".zip";
			if (!theApp.m_manifest->createZip(curcount, allcount, zipName, theApp.m_manifest->getSavePath())) {
				MessageBoxA(GetSafeHwnd(), "创建压缩文件失败,请手动压缩", "错误!", MB_OK);
				return;
			}
			curcount += 1;
			curProgressCtrl.SetPos(100);
			setPercentLabel(curcount, allcount);
			allProgressCtrl.SetPos((int)(((float)curcount / (float)allcount) * 100));
		}
		curProgressCtrl.SetPos(100);
		allProgressCtrl.SetPos(100);
		setPercentLabel(curcount, allcount);
		allProgressCtrl.SetPos((int)(((float)curcount / (float)allcount) * 100));
		setTitle("已完成所有操作!");
		setAllLabel("已完成所有操作!");
		setCurLabel("已完成所有操作!");
		setButtonStaue(BUTTON_CLOSED2, TRUE);
		setButtonStaue(BUTTON_RETURN2, TRUE);
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
			EndDialog(0);//用返回值关闭上级对话框
		}break;
	}
	CDialogEx::OnSysCommand(nID, lParam);
}
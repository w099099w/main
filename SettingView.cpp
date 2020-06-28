
// SettingView.cpp: 实现文件

#include "SettingView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// SettingView 对话框

SettingView* mainView;//全局变量

SettingView::SettingView(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MAIN_DIALOG, pParent),
	m_config("")
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	mainView = this;

}

void SettingView::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, SINGLE_FORMATFLODER, m_formatFloder);
	DDX_Control(pDX, SINGLE_COPEFILE, m_copy);
	DDX_Control(pDX, SINGLE_USECONFIG, m_useConfig);
	DDX_Control(pDX, LIST_CONFIGLIST, m_configList);
}

BEGIN_MESSAGE_MAP(SettingView, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CBN_SELCHANGE(LIST_CONFIGLIST, &SettingView::OnCbnSelchangeConfiglist)
END_MESSAGE_MAP()


// SettingView 消息处理程序

BOOL SettingView::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_hwnd = GetSafeHwnd();
	if (!PathIsDirectory(L"./config")) {
		CreateDirectory(L"./config", NULL);
	}
	ifstream in(L"./config/config.ini");
	if (!in.good()) {
		cout << "创建文件\n";
		ofstream out;
		out.open(L"./config/config.ini",ios::binary);
		in.close();
		out.close();
	}//创建配置文件
	SetList();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void SettingView::OnSysCommand(UINT nID, LPARAM lParam)
{
	switch (nID) {
	case IDM_ABOUTBOX: {
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}break;
	default: break;
	}
	CDialogEx::OnSysCommand(nID, lParam);
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void SettingView::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR SettingView::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL SettingView::OnCommand(WPARAM wParam, LPARAM lParam)
{
	if (BN_CLICKED == HIWORD(wParam))
	{
		switch (LOWORD(wParam))
		{
			case BUTTON_BUILD: {
				if (theApp.m_manifest->checkParams(GetSafeHwnd())) {
					BuildView dlgBuildView;
					INT_PTR nResponse =  dlgBuildView.DoModal();
					switch (nResponse) {
						case 0: {
							EndDialog(0);
						}break;
					}
				}	
			}break;
			case BUTTON_FINDDATAPATH: {
				theApp.m_manifest->showChooseFloder(GetSafeHwnd(), EDIT_FINDPATH);
			}break;
			case BUTTON_SAVEDATAPATH: {
				theApp.m_manifest->showChooseFloder(GetSafeHwnd(), EDIT_SAVEPATH);
			}break;
			case SINGLE_USECONFIG: {
				if (m_useConfig.GetCheck() == 1) {
					getAllSection("./config/config.ini", m_cList);
					SetList();
				}
			}break;
			case BUTTON_DELETECONFIG: {
				theApp.m_manifest->deleteConfig(m_config);
				reset();
				getAllSection("./config/config.ini", m_cList);
				SetList();
				setButtonStaue(BUTTON_SAVECONFIG, FALSE);
				setButtonStaue(BUTTON_DELETECONFIG, FALSE);
			}break;
			case BUTTON_NEWCONFIG: {
				if (m_cList.size() == MAX_CONFIGNUM) {
					MessageBoxA(m_hwnd, "当前已超过最大配置数量,请删除后再保存!", "提示", MB_OK);
					return 0;
				}
				if (theApp.m_manifest->checkParams(GetSafeHwnd())) {
					CinConfigName dlgCinName;
					INT_PTR nResponse = dlgCinName.DoModal();
					switch (nResponse) {
					case 0: {
						MessageBoxA(m_hwnd, "您取消了保存!", "提示", MB_OK);
					}break;
					case 1: {
							saveConfig(CinConfigName::m_cinstr);
							getAllSection("./config/config.ini", m_cList);
							SetList();
							LPCTSTR result = StoWs(CinConfigName::m_cinstr).c_str();
							int nIndex = m_configList.FindStringExact(0, result);
							m_configList.SetCurSel(nIndex);
							setButtonStaue(BUTTON_SAVECONFIG, TRUE);
							setButtonStaue(BUTTON_DELETECONFIG, TRUE);
							MessageBoxA(m_hwnd, "新建配置成功!", "提示", MB_OK);
						}
					}
				}
			}break;
			case BUTTON_SAVECONFIG: {
				if (theApp.m_manifest->checkParams(m_hwnd)) {
					saveConfig(m_config);
					MessageBoxA(GetSafeHwnd(), "保存到配置成功!", "提示!", MB_OK);
				}
			}break;
		}
	}
	return CDialogEx::OnCommand(wParam, lParam);
}
bool SettingView::saveConfig(string appName) {
		WriteINIString(theApp.m_manifest->getdataPath(), appName, "datapath", "./config/config.ini");
		WriteINIString(theApp.m_manifest->getSavePath(), appName, "savepath", "./config/config.ini");
		WriteINIString(theApp.m_manifest->getversion(), appName, "version", "./config/config.ini");
		WriteINIString(theApp.m_manifest->getLineVersion(), appName, "inlingversion", "./config/config.ini");
		WriteINIString(theApp.m_manifest->getRemotePath(), appName, "remotepath", "./config/config.ini");
		WriteINIString(m_copy.GetCheck() == 1 ? "1":"0" , appName, "usecopy", "./config/config.ini");
		WriteINIString(m_formatFloder.GetCheck() == 1 ? "1" : "0", appName, "useformat", "./config/config.ini");
		return true;
}
void SettingView::setButtonStaue(int buttonID, bool isclick)
{
	GetDlgItem(buttonID)->EnableWindow(isclick);
}
void SettingView::SetList() {
	cout << "setlist\n";
	if (m_useConfig.GetCheck() == 1) {
		m_configList.SetCurSel(0);
		m_configList.ResetContent();//清除缓存重新读取
		for (vector<string>::iterator it = m_cList.begin(); it != m_cList.end(); it++) {
			cout << *it << "\n";
			m_configList.AddString(StoWs(*it).c_str());
		}
		setButtonStaue(LIST_CONFIGLIST, TRUE);
		setButtonStaue(BUTTON_NEWCONFIG, TRUE);
	}
	else {
		m_configList.SetWindowText(L"");
		setButtonStaue(BUTTON_DELETECONFIG, FALSE);
		setButtonStaue(LIST_CONFIGLIST, FALSE);
		setButtonStaue(BUTTON_NEWCONFIG, FALSE);
		setButtonStaue(BUTTON_SAVECONFIG, FALSE);
	}
}
void SettingView::OnCbnSelchangeConfiglist()
{
	int nIndex = m_configList.GetCurSel();
	CString strCBText;
	m_configList.GetLBText(nIndex, strCBText);
	m_configList.GetWindowText(strCBText);
	string useConfig(CW2A(strCBText.GetString()));
	setButtonStaue(BUTTON_SAVECONFIG, TRUE); 
	setButtonStaue(BUTTON_DELETECONFIG, TRUE);
	m_config = useConfig;
	setConfigshow(theApp.m_manifest->getInfoFromConfig(useConfig));
	// TODO: 在此添加控件通知处理程序代码
}
bool SettingView::setConfigshow(map<string, string> info) {
	std::cout << "触发\n";
	if (info.size() == 0) {
		setButtonStaue(BUTTON_SAVECONFIG, FALSE);
		setButtonStaue(BUTTON_DELETECONFIG, FALSE);
	}
	for (map<string, string>::iterator it = info.begin(); it != info.end(); ++it) {
		string key = it->first;
		string value = it->second;
		if ("ERROR!!" == value && key != "inlingversion") {
			MessageBoxA(m_hwnd, "读取当前配置文件错误!", "警告!", MB_OK);
			return false;
		}
		if (key == "datapath") {
			theApp.m_manifest->setFindPath(value);
			SetDlgItemTextA(m_hwnd, EDIT_FINDPATH, value.data());
		}
		else if (key == "savepath") {
			theApp.m_manifest->setSavePath(value);
			SetDlgItemTextA(m_hwnd, EDIT_SAVEPATH, value.data());
		}
		else if (key == "version") {
			theApp.m_manifest->setversion(value);
			SetDlgItemTextA(m_hwnd, EDIT_VERSION, value.data());
		}
		else if (key == "inlingversion") {
			theApp.m_manifest->setinLineVersion(value);
			SetDlgItemTextA(m_hwnd, EDIT_INLINEVERSION, value.data());
		}
		else if (key == "remotepath") {
			theApp.m_manifest->setRemotePath(value);
			SetDlgItemTextA(m_hwnd, EDIT_REMOTEPATH, value.data());
		}
		else if (key == "useformat") {
			m_formatFloder.SetCheck(atoi(value.data()));
		}
		else if (key == "usecopy") {
			m_copy.SetCheck(atoi(value.data()));
		}
	}
	return true;
}
void SettingView::reset() {
	theApp.m_manifest->setFindPath("");
	theApp.m_manifest->setSavePath("");
	theApp.m_manifest->setversion("");
	theApp.m_manifest->setinLineVersion("");
	theApp.m_manifest->setRemotePath("");
	m_formatFloder.SetCheck(0);
	m_copy.SetCheck(0);
	SetDlgItemTextA(m_hwnd, EDIT_FINDPATH, "");
	SetDlgItemTextA(m_hwnd, EDIT_SAVEPATH, "");
	SetDlgItemTextA(m_hwnd, EDIT_VERSION, "");
	SetDlgItemTextA(m_hwnd, EDIT_INLINEVERSION, "");
	SetDlgItemTextA(m_hwnd, EDIT_REMOTEPATH, "");
}

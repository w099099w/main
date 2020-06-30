#include "../../head/manifest.h"
#include "../../head/tool/zip.h"
Manifest::Manifest()
	:
m_dataPath(""),
m_savePath(""),
m_version(""),
m_inLineVersion(""),
maxsize(0),
m_saveitem{ "datapath","savepath","version","inlingversion","remotepath","useformat","usecopy","createzip"}
{
	m_makeMd5 = new MD5();
	m_buff = new char[MAX_BUFLENGTH];
	m_Json = new CJsonObject();
}
TCHAR* stringToTchar(string in) {
	TCHAR file[MAX_PATH];
	_stprintf_s(file, MAX_PATH, _T("%S"), in.c_str());//%S���ַ�
	return file;
}
Manifest::~Manifest()
{
	if (m_makeMd5) {
		delete m_makeMd5;
		m_makeMd5 = NULL;
	}
	if (m_buff) {
		delete[] m_buff;
	}
	if (m_Json) {
		delete m_Json;
	}
}

void Manifest::reset()
{
	m_fileVector.clear();
	m_creatFloder.clear();
	m_Json->Clear();
}

void Manifest::setMFCParam(void* allLabel, void* curLabel, void* percentLabel, void* allProgress, void* curProgress)
{
	m_allLabel = allLabel;
	m_curLabel = curLabel;
	m_percentLabel = percentLabel;
	m_allProgress = allProgress;
	m_curProgress = curProgress;
}
bool Manifest::setAllLabel(string s) {
	string* a = (string*)m_allLabel;
	*a = s;
	return true;
}
bool Manifest::setPercentLabel(int cur, int all)
{
	RATIO* a = (RATIO*)m_percentLabel;
	(*a).cur = cur;
	(*a).all = all;
	return true;
}
bool Manifest::setCurLabel(string s)
{
	string* a = (string*)m_curLabel;
	*a = s;
	return true;
}
string Manifest::getversion()
{
	return m_version;
}

string Manifest::getLineVersion()
{
	return m_inLineVersion;
}

string Manifest::getRemotePath()
{
	return m_remotePath;
}

string Manifest::getdataPath()
{
	return m_dataPath;
}

string Manifest::getSavePath()
{
	return m_savePath;
}

map<string, string> Manifest::getInfoFromConfig(string configName,string configPath)
{
	map<string, string> temp;
	int size = sizeof(m_saveitem) / sizeof(string);
	for (int i = 0; i < size; ++i) {
		temp.insert(make_pair(m_saveitem[i], ReadINIStr(configName, m_saveitem[i], configPath)));
	}
	return temp;
}
void Manifest::checkFiles(string path, vector<MANIFEST>& files) {
	intptr_t   hFile = 0;//�ļ�������������������
	struct _finddata_t fileinfo;//�ļ���Ϣ
	string p;
	if ((hFile = _findfirst(p.assign(path).append("/*").c_str(), &fileinfo)) != -1)
		//������ҵ���һ���ļ�
	{
		do
		{
			if ((fileinfo.attrib & _A_SUBDIR))//������ļ���
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0) {
					checkFiles(p.assign(path).append("/").append(fileinfo.name), files);
				}
			}
			else//������ļ�
			{
				setCurLabel("�����ļ�" + p.assign(path).append("/").append(fileinfo.name));
				MANIFEST file;
				file.path = p.assign(path).append("/").append(fileinfo.name);
				file.size = fileinfo.size;
				if (maxsize < file.size) {
					maxsize = file.size;//��ȡ����ļ���С
				}
				if (strstr(fileinfo.name, ".zip") || strstr(fileinfo.name, ".zIp")
					|| strstr(fileinfo.name, ".ziP") || strstr(fileinfo.name, ".Zip") ||
					strstr(fileinfo.name, ".ZIp") || strstr(fileinfo.name, ".ZiP") ||
					strstr(fileinfo.name, ".ZIP") || strstr(fileinfo.name, ".zIP")) {
					file.zip = true;
				}
				else {
					file.zip = false;
				}
				files.push_back(file);
			}
		} while (_findnext(hFile, &fileinfo) == 0); //��Ѱ�ҵ������ļ�

		_findclose(hFile);  //�������ң��رվ��
	}
}
void Manifest::getFiles(string path, vector<MANIFEST>& files)
{
	intptr_t   hFile = 0;//�ļ�������������������
	struct _finddata_t fileinfo;//�ļ���Ϣ
	string p;
	if ((hFile = _findfirst(p.assign(path).append("/*").c_str(), &fileinfo)) != -1)
		//������ҵ���һ���ļ�
	{
		do
		{
			if ((fileinfo.attrib & _A_SUBDIR))//������ļ���
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0) {
					int dataPathLen = (int)m_dataPath.length();
					string floderPath = p.assign(path).append("/").append(fileinfo.name);
					string copyPath = m_savePath + floderPath.substr(dataPathLen);
					m_creatFloder.push_back(copyPath);
					getFiles(p.assign(path).append("/").append(fileinfo.name), files);
				}
			}
			else//������ļ�
			{
				setCurLabel("�����ļ�" + p.assign(path).append("/").append(fileinfo.name));
				MANIFEST file;
				file.path = p.assign(path).append("/").append(fileinfo.name);
				file.size = fileinfo.size;
				if (maxsize < file.size) {
					maxsize = file.size;//��ȡ����ļ���С
				}
				if (strstr(fileinfo.name, ".zip") || strstr(fileinfo.name, ".zIp")
					|| strstr(fileinfo.name, ".ziP") || strstr(fileinfo.name, ".Zip") ||
					strstr(fileinfo.name, ".ZIp") || strstr(fileinfo.name, ".ZiP") ||
					strstr(fileinfo.name, ".ZIP") || strstr(fileinfo.name, ".zIP")) {
					file.zip = true;
				}
				else {
					file.zip = false;
				}
				files.push_back(file);
			}
		} while (_findnext(hFile, &fileinfo) == 0); //��Ѱ�ҵ������ļ�

		_findclose(hFile);  //�������ң��رվ��
	}
}

void Manifest::showChooseFloder(HWND hwndDlg, int EDITCODE)
{
	BROWSEINFO  bi;
	bi.hwndOwner = NULL;
	bi.pidlRoot = CSIDL_DESKTOP;//�ļ��еĸ�Ŀ¼���˴�Ϊ����
	bi.pszDisplayName = NULL;
	bi.lpszTitle = NULL;//��ʾλ�ڶԻ������ϲ�����ʾ��Ϣ
	bi.ulFlags = BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;//���½��ļ��а�ť
	bi.lpfn = NULL;
	bi.iImage = 0;
	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);//����ѡ��Ի���
	TCHAR* path = new TCHAR[MAX_PATH];
	if (pidl == NULL)
	{
		MessageBoxA(hwndDlg, "û��ѡ��Ŀ¼", "��ʾ", MB_OK);
	}
	else {
		SHGetPathFromIDList(pidl, path);
		int iLength = WideCharToMultiByte(CP_ACP, 0, path, -1, NULL, 0, NULL, NULL);
		WideCharToMultiByte(CP_ACP, 0, path, -1, m_buff, iLength, NULL, NULL);
		switch (EDITCODE)
		{
		case EDIT_FINDPATH: {
			setFindPath(m_buff);
		}break;
		case EDIT_SAVEPATH: {
			setSavePath(m_buff);
		}break;
		}
		showData(hwndDlg, EDITCODE, m_buff);
	}
}

bool Manifest::showData(HWND hwndDlg, int EDITCODE, string src)
{
	if (isFloder(src)) {
		SetDlgItemTextA(hwndDlg, EDITCODE, src.data());
		return true;
	}
	else {
		MessageBoxA(hwndDlg, "ѡ��Ĳ����ļ���!", "����", MB_OK);
		SetDlgItemTextA(hwndDlg, EDITCODE, "");
		return false;
	}
}

void Manifest::DeleteDirectory(CString strPath)
{
	CFileFind tempFind;
	TCHAR sTempFileFind[MAX_PATH] = { 0 };
	wsprintf(sTempFileFind, _T("%s\\*.*"), strPath);
	BOOL IsFinded = tempFind.FindFile(sTempFileFind);
	while (IsFinded)
	{
		IsFinded = tempFind.FindNextFile();
		if (!tempFind.IsDots())
		{
			TCHAR sFoundFileName[200] = { 0 };
			_tcscpy(sFoundFileName, tempFind.GetFileName().GetBuffer(200));
			if (tempFind.IsDirectory())
			{
				TCHAR sTempDir[200] = { 0 };
				wsprintf(sTempDir, _T("%s\\%s"), strPath, sFoundFileName);
				DeleteDirectory(sTempDir); //ɾ���ļ����µ��ļ�
				RemoveDirectory(sTempDir); //�Ƴ����ļ�
			}
			else
			{
				TCHAR sTempFileName[200] = { 0 };
				wsprintf(sTempFileName, _T("%s\\%s"), strPath, sFoundFileName);
				DeleteFile(sTempFileName);
			}
		}
	}
	tempFind.Close();
}

void Manifest::deleteConfig(string configName,string path)
{
	WritePrivateProfileStringA(configName.c_str(), NULL, NULL, path.c_str());
}

inline bool exists_file(const std::string& name) {
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}

bool Manifest::createZip(int cur,int all,string fileName, string sourceDir)
{
	if (exists_file(fileName)) {
		remove(fileName.c_str());
	}
	setAllLabel("���ڼ���ļ�...");
	vector<MANIFEST> fileList;
	checkFiles(sourceDir, fileList);
	int* a = (int*)m_curProgress;
	int* b = (int*)m_allProgress;
	int dataPathLen = (int)sourceDir.length(),percent = 0, vectorSize = (int)fileList.size();
	float currtnt = 0;
	ZRESULT isOk = ZR_OK;
	HZIP hz = CreateZip(stringToTchar(fileName), 0);
	if (!hz) {
		std::cout << "����ʧ��";
	}
	for (vector<MANIFEST>::iterator it = fileList.begin(); it != fileList.end(); it++) {
		string strPath = "./"+it->path.substr(dataPathLen + 1);
		isOk = ZipAdd(hz, stringToTchar(strPath), stringToTchar(it->path));
		if (isOk != ZR_OK) {
			break;
		}
		currtnt += 1;//�ٷֱ�
		percent = (int)((currtnt / vectorSize) * 100);
		char t[8] = {};
		_itoa(percent, t, 10);
		string st(t);
		string stadd = "����ѹ���ļ� "+ fileName +"==>�����" + st + "%";
		*a = percent;
		*b = (int)(((float)cur / (float)all) * 100) + (int)(((float)1 / (float)all) * 100 * ((float)currtnt / (float)vectorSize));
		setAllLabel(stadd);
		setCurLabel("[����ѹ��] �ļ�:" + strPath);
	}
	CloseZip(hz);
	return isOk == ZR_OK;
}

bool Manifest::findFiles()
{
	getFiles(m_dataPath, m_fileVector);
	return true;
}

int Manifest::CopyFiles(char* SourceFile, char* NewFile)
{
	std::ifstream in;
	std::ofstream out;
	try
	{
		in.open(SourceFile, std::ios::binary);//��Դ�ļ�
		if (in.fail())//��Դ�ļ�ʧ��
		{
			std::cout << "Error 1: Fail to open the source file." << std::endl;
			in.close();
			out.close();
			return 0;
		}
		out.open(NewFile, ios::binary);//����Ŀ���ļ�
		if (out.fail())//�����ļ�ʧ��
		{
			std::cout << "Error 2: Fail to create the new file." << std::endl;
			out.close();
			in.close();
			return 0;
		}
		else//�����ļ�
		{
			out << in.rdbuf();
			out.close();
			in.close();
			return 1;
		}
	}
	catch (std::exception e)
	{
	}
	return 1;
}

int creatFLODER(LPCWSTR path) {
	CreateDirectory(path, NULL);
	return 1;
}

bool Manifest::createFloder(int cur, int all) {
	ThreadPool pool(4);
	int percent = 0,currtnt = 0;
	int vectorSize = m_creatFloder.size();
	int* a = (int*)m_curProgress;
	int* b = (int*)m_allProgress;
	for (vector<string>::iterator it = m_creatFloder.begin(); it != m_creatFloder.end(); it++) {
		setCurLabel("[���ڴ���Ŀ¼] :" + *it);
		percent = (int)((currtnt / vectorSize) * 100);
		*a = percent;
		*b = (int)(((float)cur / (float)all) * 100) + (int)(((float)1 / (float)all) * 100 * ((float)currtnt / (float)vectorSize));
		auto result = pool.enqueue(creatFLODER, stringToLPCWSTR(*it));
		if (!result.get()) {
			return false;
		}
	}
	return true;
}

bool Manifest::copyFiles(int cur, int all,HWND hwnd) {
	int* a = (int*)m_curProgress;
	int* b = (int*)m_allProgress;
	FILE* pfr = 0;
	FILE* pfw = 0;
	int dataPathLen = (int)m_dataPath.length();
	float currtnt = 0;
	int percent = 0;
	int vectorSize = (int)m_fileVector.size();
	char* buf = new char[maxsize];
	for (vector<MANIFEST>::iterator it = m_fileVector.begin(); it != m_fileVector.end(); it++) {
		string strPath = it->path.substr(dataPathLen + 1);
		setCurLabel("[���ڸ���]  �ļ�:" + strPath + " �� " + (m_savePath + strPath).data() + "...");
		if (!CopyFiles(&it->path[0], &(m_savePath + "/" + strPath)[0])) {
			return false;
		}
		currtnt += 1;//�ٷֱ�
		percent = (int)((currtnt / vectorSize) * 100);
		char t[8] = {};
		_itoa(percent, t, 10);
		string st(t);
		string stadd = "���ڸ����ļ�...==>�����" + st + "%";
		*a = percent;
		*b = (int)(((float)cur / (float)all) * 100) + (int)(((float)1 / (float)all) * 100 * ((float)currtnt / (float)vectorSize));
		SetWindowText(hwnd, StoWs(stadd).c_str());
	}
	delete[] buf;
	return true;
}

bool Manifest::checkParams(HWND hwndDlg)
{
	if (!m_dataPath.length() || !isFloder(m_dataPath)) {
		MessageBoxA(hwndDlg, "����Ŀ¼����ȷ!", "����", MB_OK);
		return false;
	}
	if (!m_savePath.length() || !isFloder(m_savePath)) {
		MessageBoxA(hwndDlg, "�ļ�����Ŀ¼����ȷ!", "����", MB_OK);
		return false;
	}
	GetDlgItemTextA(hwndDlg, EDIT_REMOTEPATH, m_buff, MAX_BUFLENGTH);
	m_remotePath = m_buff;
	GetDlgItemTextA(hwndDlg, EDIT_VERSION, m_buff, MAX_BUFLENGTH);
	m_version = m_buff;
	GetDlgItemTextA(hwndDlg, EDIT_INLINEVERSION, m_buff, MAX_BUFLENGTH);
	m_inLineVersion = m_buff;
	if (!m_version.length()) {
		MessageBoxA(hwndDlg, "�ȸ��°汾����Ϊ��!", "����", MB_OK);
		return false;
	}
	if (!m_remotePath.length()) {
		MessageBoxA(hwndDlg, "Զ��Ŀ¼����Ϊ��!", "����", MB_OK);
		return false;
	}
	m_fileVector.clear();//�������
	m_Json->Clear();//�������
	maxsize = 0;//�������
	return true;
}

bool Manifest::buildVesion()
{	
	m_Json->Add("packageUrl", m_remotePath+"/");
	m_Json->Add("remoteManifestUrl", m_remotePath+"/project.manifest");
	m_Json->Add("remoteVersionUrl", m_remotePath + "/version.manifest");
	if (m_inLineVersion.length() == 0) {
		m_Json->Add("version", m_version);
	}
	else {
		CJsonObject tempJson;
		tempJson.Add("hotUpdate", m_version);
		tempJson.Add("android", m_inLineVersion);
		m_Json->Add("version", tempJson.ToString());
	}
	//�ļ���������ɿ�ʼд��
	return writeFiles(m_savePath+"/version.manifest",m_Json->ToString());
}

bool Manifest::buildProject(int cur,int all,HWND hwnd)
{
	int* a = (int*)m_curProgress;
	int* b = (int*)m_allProgress;
	char tem[32] = {};
	if (m_Json->IsEmpty()) {
		return false;//û���ļ�ͷfalse
	}
	m_Json->AddEmptySubObject("assets");
	int dataPathLen = (int)m_dataPath.length();
	int percent = 0;
	float currtnt = 0;
	int vectorSize = (int)m_fileVector.size();
	for (vector<MANIFEST>::iterator it = m_fileVector.begin(); it != m_fileVector.end(); it++) {
		string strPath = it->path.substr(dataPathLen+1);
		_itoa(it->size, tem, 10);
		string md5 = m_makeMd5->MD5file(it->path);
		//��װJSON
		CJsonObject temp;
		temp.Add("size", it->size);
		temp.Add("md5", md5);
		(*m_Json)["assets"].Add(strPath, temp);
		currtnt += 1;//�ٷֱ�
		percent = (int)((currtnt / vectorSize)*100);
		char t[8] = {};
		_itoa(percent, t, 10);
		string st(t);
		string stadd = "��������project.manifest==>�����" + st + "%";
		*a = percent;
		*b = (int)(((float)cur / (float)all) * 100)+(int)(((float)1 / (float)all) * 100 * ((float)currtnt / (float)vectorSize));
		setAllLabel(stadd);
		setCurLabel("[���ڼ���]  �ļ�:" + strPath +"  md5:"+ md5 + +"  ��С:" + tem);
	}
	return writeFiles(m_savePath + "/project.manifest", m_Json->ToString());
}

bool Manifest::writeFiles(string fileName, string info)
{
	FILE* pf = nullptr;
	fopen_s(&pf,fileName.c_str(),"w+");//�򿪵ķ�ʽ
	if (!pf) {
		fclose(pf);
		pf = nullptr;
		return false;
	}
	fwrite(info.c_str(), info.length(),1,pf);
	fclose(pf);
	return true;
}

bool Manifest::isFloder(string src)
{
	WIN32_FIND_DATAA FindFileData;
	FindFirstFileA(src.data(), &FindFileData);
	if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
		return true;
	}
	return false;
}

bool Manifest::setFindPath(string findPath)
{
	m_dataPath = findPath;
	return true;
}

bool Manifest::setSavePath(string savePath)
{
	m_savePath = savePath;
	return true;
}

bool Manifest::setversion(string version)
{
	m_version = version;
	return true;
}

bool Manifest::setinLineVersion(string inLineVersion)
{
	m_inLineVersion = inLineVersion;
	return true;
}

bool Manifest::setRemotePath(string remotePath)
{
	m_remotePath = remotePath;
	return true;
}

string Manifest::getdataPathFromConfig(string configName)
{
	return string();
}

string Manifest::getversionFromConfig(string configName)
{
	return string();
}

string Manifest::getLineVersionFromConfig(string configName)
{
	return string();
}

string Manifest::getRemotePathFromConfig(string configName)
{
	return string();
}

string Manifest::getSavePathFromConfig(string configName)
{
	return string();
}

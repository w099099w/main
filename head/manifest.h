#ifndef _BUILD_VERSION_MANIFEST_H_
#define _BUILD_VERSION_MANIFEST_H_
#define MAX_BUFLENGTH  1024//30M用于显示

#include "../pch.h"
#include <shellapi.h>
#include <tchar.h>
#pragma comment(lib, "shell32.lib")
struct MANIFEST {
	unsigned int size;
	bool zip;
	string path;
};
class MD5;
class Manifest
{
public:
	Manifest();
	~Manifest();
	void reset();
	void setMFCParam(void* allLabel, void* curLabel, void* percentLabel, void* allProgress, void* curProgress);
	bool setAllLabel(string s);
	bool setPercentLabel(int cur, int all);
	bool setCurLabel(string s);
	bool showData(HWND hwndDlg, int EDITCODE,string src);
	bool setFindPath(string findPath);
	bool setSavePath(string savePath);
	bool setversion(string version); 
	bool setinLineVersion(string inLineVersion);
	bool setRemotePath(string remotePath);
	bool findFiles();
	int CopyFiles(char* SourceFile, char* NewFile);
	bool checkParams(HWND hwndDlg);
	bool createFloder(int cur, int all);
	bool copyFiles(int cur, int all, HWND hwnd);
	bool buildVesion();
	bool buildProject(int cur, int all, HWND hwnd);
	bool writeFiles(string fileName,string info);
	void showChooseFloder(HWND hwndDlg, int EDITCODE);
	void DeleteDirectory(CString strPath);
	void deleteConfig(string configName);
	string getversion();
	string getLineVersion();
	string getRemotePath();
	string getdataPath();
	string getSavePath();
	map<string, string> getInfoFromConfig(string configName);
	string getversionFromConfig(string configName);
	string getLineVersionFromConfig(string configName);
	string getRemotePathFromConfig(string configName);
	string getdataPathFromConfig(string configName);
	string getSavePathFromConfig(string configName);
	string m_saveitem[7];
private:
	vector<string> m_creatFloder;
	unsigned int maxsize;
	string m_dataPath;
	string m_savePath;
	string m_version;
	string m_inLineVersion;
	string m_remotePath;
	void* m_allLabel;
	void* m_curLabel;
	void* m_percentLabel;
	void* m_allProgress;
	void* m_curProgress;
	MD5* m_makeMd5;
	CJsonObject* m_Json;
	vector<MANIFEST> m_fileVector;
	void getFiles(string path, vector<MANIFEST>& files);
	char* m_buff;//用于显示信息,信息转换
	bool isFloder(string src);
};
#endif

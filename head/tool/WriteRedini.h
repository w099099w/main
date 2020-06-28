#ifndef  __WRITE_RED_INI_H__
#define  __WRITE_RED_INI_H__
#include "../../pch.h"

inline string WStringToString(const std::wstring& wstr)
{
	std::string str;
	int nLen = (int)wstr.length();
	str.resize(nLen, ' ');
	int nResult = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wstr.c_str(), nLen, (LPSTR)str.c_str(), nLen, NULL, NULL);
	if (nResult == 0)
	{
		return "";
	}
	return str;
}
inline wstring StoWs(const string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	wstring r(buf);
	delete[] buf;
	return r;
}
inline LPCWSTR stringToLPCWSTR(std::string orig)
{
	size_t origsize = orig.length() + 1;
	const size_t newsize = 100;
	size_t convertedChars = 0;
	wchar_t* wcstring = (wchar_t*)malloc(sizeof(wchar_t) * (origsize));
	mbstowcs_s(&convertedChars, wcstring, origsize, orig.c_str(), _TRUNCATE);
	return wcstring;
}
inline void showStatue(CEdit* Edit, string text)
{
	Edit->SetLimitText(UINT_MAX);//设置显示最大字符数
	int len = Edit->GetWindowTextLength();
	Edit->SetSel(len, len);//将插入光标放在最后
	std::wstring stemp = StoWs(text);
	LPCWSTR result = stemp.c_str();
	Edit->ReplaceSel(result);
}
///INI读入float
inline float ReadINIf(string appname,string keyname,string filename)
{
	static char buf[MAX_PATH];
	GetPrivateProfileStringA(appname.c_str(),keyname.c_str(),"",buf,255,filename.c_str());
	return (float)atof(buf);
}
///INI读入unsigned int
inline unsigned int	ReadINIul(string appname,string keyname,string filename)
{
	static char buf[MAX_PATH];
	GetPrivateProfileStringA(appname.c_str(),keyname.c_str(),"",buf,255,filename.c_str());
	return (unsigned int)strtoul(buf,NULL,10);
}
///INI读入int
inline int	ReadINIint(string appname,string keyname,string filename)
{
	static char buf[MAX_PATH];
	GetPrivateProfileStringA(appname.c_str(),keyname.c_str(),"",buf,255,filename.c_str());
	return atoi(buf);
}
///INI读入字符串
inline string ReadINIStr(string appname,string keyname,string filename)
{
	static char buf[MAX_PATH];
	GetPrivateProfileStringA(appname.c_str(),keyname.c_str(),"ERROR!!",buf,255,filename.c_str());
	return string(buf);
}///INI写入int
inline void WriteINIint(int num,string appname,string keyname,string filename)
{
	static char buf[MAX_PATH];
	_itoa_s(num,buf,MAX_PATH,10);
	WritePrivateProfileStringA(appname.c_str(),keyname.c_str(),buf,filename.c_str());
}
inline void WriteINIString(string str, string appname, string keyname, string filename)
{
	WritePrivateProfileStringA(appname.c_str(), keyname.c_str(), str.c_str(), filename.c_str());
}
inline bool getAllSection(string filename , vector<string> &tem)
{
	tem.clear();
	char strRetunrStringBuf[1024];
	char strVaule[256];
	int iReturnSize = 0;
	int iCountTotal = 0;
	int iCountPrivate = 0;
	int iBufSize = 1 * 1024;

	memset(strRetunrStringBuf, 0, sizeof(strRetunrStringBuf));
	iReturnSize = GetPrivateProfileSectionNamesA(strRetunrStringBuf,
		iBufSize,
		filename.c_str());

	while (iCountTotal < iReturnSize)
	{
		memset(strVaule, 0, sizeof(strVaule));
		memcpy(strVaule, &strRetunrStringBuf[iCountTotal], strlen(&strRetunrStringBuf[iCountTotal]));
		iCountPrivate = (int)strlen(&strRetunrStringBuf[iCountTotal]) + 1;
		iCountTotal += iCountPrivate;
		string temp = strVaule;
		tem.push_back(temp);
	}
	return true;
}
#endif

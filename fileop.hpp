#include <direct.h>
#include <io.h>
#include <iostream>

#include <sys/stat.h> 　
#include <sys/types.h>

//!创建文件夹 1成功 
int creatFolder(const char* folderPath)
{
	//不存在则创建
	if (0 != _access(folderPath, 0))
	{
		int result = _mkdir(folderPath);   // 返回 0 表示创建成功，-1 表示失败

		if (result == 0)
			return 1;
		else
			return 0;

	}
	
	std::string path =folderPath;
    int isCreate = mkdir(path.c_str(),S_IRUSR | S_IWUSR | S_IXUSR | S_IRWXG | S_IRWXO);
    if( isCreate==0 )
        printf("create path:%s\n",path.c_str());
    else
        printf("create path failed! error code : %s \n",isCreate,path.c_str());
    

	return 1;
}
//!替换字符串
void Replace(std::string& str, const char* oldvalue, const char* newvalue)
{
	if (str.length() == 0)
	{
		return;
	}
	int len1 = strlen(oldvalue);
	int len2 = strlen(newvalue);
	if (len1 == len2)
	{
		if (memcmp(oldvalue, newvalue, len1) == 0)
		{
			return;
		}
	}


	std::vector<PosionUnit> vect;
	int newlen = 0;
	char* p = NULL;
	int len = str.size();
	int leftlen = str.size();
	char* buf = (char*)str.c_str();
	bool flg = false;
	do
	{

		p = (char*)strstr(buf, oldvalue);
		if (p)
		{
			PosionUnit unitold;
			unitold.p = buf;
			unitold.len = p - buf;
			vect.push_back(unitold);
			PosionUnit unitnew;
			unitnew.newvalue = true;
			vect.push_back(unitnew);
			buf = p + len1;
			leftlen -= (unitold.len + len1);
			flg = true;
			newlen += (unitold.len + len2);
		}
		else
		{
			if (buf != NULL && leftlen && flg)
			{
				PosionUnit unitnew;
				unitnew.p = buf;
				unitnew.len = leftlen;
				vect.push_back(unitnew);
				newlen += leftlen;
			}
			break;
		}

	} while (p);

	if (flg)				//发现数据 需要进行
	{

		len = 0;
		char* buf = new char[newlen + 1];
		buf[newlen] = 0;
		std::vector<PosionUnit>::iterator itt;
		for (itt = vect.begin(); itt != vect.end(); ++itt)
		{
			if (!itt->newvalue)
			{
				memcpy(buf + len, itt->p, itt->len);
				len += itt->len;
			}
			else
			{

				memcpy(buf + len, newvalue, len2);
				len += len2;
			}
		}
		str = buf;
		delete[] buf;
	}

}
//清理指定文件夹下所有文件 包括文件目录本身
//[in] const wstring wstDirectory : 要清理的文件目录
//返回值 : 无
void RemoveAll(std::string wst)
{
	std::string  wstCurrentFindPath;
	wstCurrentFindPath.assign(wst);
	wstCurrentFindPath.append("\\*.*");

	std::string  wstCurrentFile;
	WIN32_FIND_DATA wfd;
	HANDLE h = FindFirstFile(wstCurrentFindPath.c_str(), &wfd);
	if (h == INVALID_HANDLE_VALUE)
	{
		return;
	}
	do
	{
		if (lstrcmp(wfd.cFileName, ".") == 0 ||
			lstrcmp(wfd.cFileName, "..") == 0)
		{
			continue;
		}
		wstCurrentFile.assign(wst);
		wstCurrentFile.append("\\");
		wstCurrentFile.append(wfd.cFileName);
		if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			RemoveAll(wstCurrentFile);
		}
		else
		{
			DeleteFile(wstCurrentFile.c_str());
		}
	} while (FindNextFile(h, &wfd));
	FindClose(h);
	RemoveDirectory(wst.c_str());
}


std::string String_TO_UTF8(std::string str)
{

	int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
	wchar_t* pwBuf = new wchar_t[nwLen + 1];
	ZeroMemory(pwBuf, nwLen * 2 + 2);

	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);

	int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

	char* pBuf = new char[nLen + 1];
	ZeroMemory(pBuf, nLen + 1);

	::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

	std::string retStr(pBuf);

	delete[]pwBuf;
	delete[]pBuf;

	pwBuf = NULL;
	pBuf = NULL;

	return retStr;

}

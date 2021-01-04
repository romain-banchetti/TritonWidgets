#include <windows.h>
#include <string>
#include "twFileInfo.h"
#include "twApplication.h"
#include <time.h>

using namespace std;

twFileInfo::twFileInfo(twString path)
{
	m_path = path;
	if (FindFirstFile(m_path.c_str(), &m_fileInfo) == INVALID_HANDLE_VALUE)
		m_exist = false;
	else
		m_exist = true;
}

bool twFileInfo::isExist()
{
	return m_exist;
}

int twFileInfo::getLastWrite()
{
	SYSTEMTIME systemTime;
	FILETIME fileTime;
	HANDLE handle = CreateFile(m_path.c_str(), 0, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	GetFileTime(handle, NULL, NULL, &fileTime);
	CloseHandle(handle);
	FileTimeToSystemTime(&fileTime, &systemTime);
	struct tm tm;
	tm.tm_year = systemTime.wYear - 1900;
	tm.tm_mon = systemTime.wMonth - 1;
	tm.tm_mday = systemTime.wDay;
	tm.tm_hour = systemTime.wHour;
	tm.tm_min = systemTime.wMinute;
	tm.tm_sec = systemTime.wSecond;
	return mktime(&tm) + 3600;
}

void twFileInfo::setLastWrite(int date)
{
	FILETIME fileTime;
	SYSTEMTIME systemTime;
	struct tm tm;
	time_t time = date - 3600;
	tm = *localtime(&time);
	systemTime.wYear = tm.tm_year + 1900;
	systemTime.wMonth = tm.tm_mon + 1;
	systemTime.wDay = tm.tm_mday;
	systemTime.wHour = tm.tm_hour;
	systemTime.wMinute = tm.tm_min;
	systemTime.wSecond = tm.tm_sec;
	SystemTimeToFileTime(&systemTime, &fileTime);
	HANDLE handle = CreateFile(m_path.c_str(), FILE_WRITE_ATTRIBUTES, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	SetFileTime(handle, NULL, NULL, &fileTime);
	CloseHandle(handle);
}

bool twFileInfo::remove()
{
	return DeleteFile(m_path.c_str());
}
twString twFileInfo::getPath()
{
	return m_path;
}
twString twFileInfo::getName(bool extension)
{
	if (extension)
		return m_fileInfo.cFileName;
	else
	{
		if (getName().find(".") == twString::npos)
			return getName();
		return getName().substr(0, getName().rfind("."));
	}
}

twString twFileInfo::getAbsolutePath()
{
	return m_fileInfo.cAlternateFileName;
}

bool twFileInfo::isFile()
{
	if (!(m_fileInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		return true;
	return false;
}

bool twFileInfo::isDir()
{
	if (m_fileInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		return true;
	return false;
}

bool twFileInfo::isSystemFile()
{
	if (m_fileInfo.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM)
		return true;
	return false;
}

bool twFileInfo::isHidden()
{
	if (m_fileInfo.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
		return true;
	return false;
}

twString twFileInfo::getExtension()
{
    if (getName().find(".") == twString::npos)
        return "";
    return getName().substr(getName().rfind(".") + 1);
}

bool twFileInfo::copy(twString path, bool overwrites)
{
	return CopyFile(m_path.c_str(), path.c_str(), !overwrites);
}

int twFileInfo::getSize()
{
	if (!m_exist)
		return -1;
	else
		return (m_fileInfo.nFileSizeHigh * (MAXDWORD+1)) + m_fileInfo.nFileSizeLow;
}

bool twFileInfo::move(twString path)
{
    return MoveFile(m_path.c_str(), path.c_str());
}

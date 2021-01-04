#ifndef TWFILEINFO_H_INCLUDED
#define TWFILEINFO_H_INCLUDED

#include "twString.h"

class twFileInfo
{
	public:
		twFileInfo(twString path);
		twString getPath();
		twString getName(bool extension = true);
		twString getAbsolutePath();
		twString getExtension();
		int getLastWrite();
		void setLastWrite(int date);
		bool copy(twString path, bool overwrites = false);
		bool isFile();
		int getSize();
		bool isExist();
		bool remove();
		bool move(twString path);
		bool isDir();
		bool isSystemFile();
		bool isHidden();

	private:
		twString m_path;
		WIN32_FIND_DATA m_fileInfo;
		bool m_exist;
};

#endif // TWFILEINFO_H_INCLUDED

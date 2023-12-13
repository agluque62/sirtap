#ifndef __CORELOGFILE_H__
#define __CORELOGFILE_H__

#include <iostream>
#include <fstream>
#include <Windows.h>

using namespace std;

class Corelogfile
{
private:
	const static string FILENAME;
	const static string FILENAME1;
	const static string FILENAME2;
	const static std::streamoff MAXFILESIZE = 2000000ll;
	const static int NUM_LINES_SIZE_FILE_IS_CHECKED = 50;

	static fstream fout;
	static CRITICAL_SECTION CriticalSection;
	static int writen_lines;

	static const std::string currentDateTime();
	static std::streamoff Corelogfile::GetFilesize();

public:
	static int Init();
	static int Open();
	static void Writelog(int level, const char* data, int len);

};

#endif

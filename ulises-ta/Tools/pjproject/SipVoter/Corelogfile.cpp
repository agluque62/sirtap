#include "Corelogfile.h"
#include <time.h>
#include <string>

const string Corelogfile::FILENAME = "Corelog.txt";
const string Corelogfile::FILENAME1 = "Corelog01.txt";
const string Corelogfile::FILENAME2 = "Corelog02.txt";

fstream Corelogfile::fout;
CRITICAL_SECTION Corelogfile::CriticalSection;
int Corelogfile::writen_lines = 0;

int Corelogfile::Init()
{
	InitializeCriticalSection(&CriticalSection);
	Open();
	writen_lines = 0;
	return 0;
}

int Corelogfile::Open()
{
	int tries = 3;
	EnterCriticalSection(&CriticalSection);

	while (fout.is_open() && tries > 0)
	{
		Sleep(40);
		tries--;
	}
	if (fout.is_open())
	{
		fout.close();
	}

	if (fout.is_open() == false)
	{
		fout.open(FILENAME, std::fstream::out | std::fstream::app);
	}
	LeaveCriticalSection(&CriticalSection);
	return 0;
}

void Corelogfile::Writelog(int level, const char* data, int len)
{
	EnterCriticalSection(&CriticalSection);
	if (fout.is_open() == false)
	{
		Open();
	}

	if (fout.is_open() == true)
	{
		fout << currentDateTime() << " " << data; // << endl;
		fout.flush();
		if (++writen_lines > NUM_LINES_SIZE_FILE_IS_CHECKED)
		{
			writen_lines = 0;
			fout.close();
			if (GetFilesize() > MAXFILESIZE)
			{					
				std::remove(FILENAME2.c_str());
				std::rename(FILENAME1.c_str(), FILENAME2.c_str());
				std::rename(FILENAME.c_str(), FILENAME1.c_str());
			}
		}
	}
	LeaveCriticalSection(&CriticalSection);

}

const std::string Corelogfile::currentDateTime() {
	string stime;
	SYSTEMTIME time;
	GetSystemTime(&time);
	stime = to_string(time.wYear) + "-" + to_string(time.wMonth) + "-" + to_string(time.wDay);

	return stime;
}

std::streamoff Corelogfile::GetFilesize()
{
	std::streamoff fsize = 0;
	EnterCriticalSection(&CriticalSection);
	fout.open("Corelogfile.txt", std::fstream::in | fstream::binary);
	if (fout.is_open() == true)
	{
		std::streamoff begin = fout.tellg();
		fout.seekg(0, ios::end);
		std::streamoff end = fout.tellg();
		fsize = (end - begin);
		fout.close();
	}
	LeaveCriticalSection(&CriticalSection);
	return fsize;
}




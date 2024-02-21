//
//	INI File routines using the STL.  
//
//	copyright August 18, 1999 by Robert Kesterson
//
//	License:  
//			    You may use this code for any project you wish, 
//				whether commercial or not, provided that 
//				the following conditions are met:
//				(a) you drop me a line (email robertk@robertk.com) to let me know
//				     you're using it, and 
//				(b) give me a mention in the credits someplace
//
//
//	Notes:
//
//	INI keys are not case sensitive in this implementation.
//
//  Lines in the INI that are > MAX_INI_LINE characters will not process correctly.  
//  (Default is MAX_INI_LINE of 500, but you can change it if you need more...)
//	
//	define TEST_INI to build a little test program.  Typical linuxcommand line:
//	g++ -DTEST_INI -O2 -Wall -o testini stlini.cpp -lstdc++
//
//  Provides the following functionality:
//
//		Loads an INI file into a single nested hash, such that for each named section
//		in the INI file, you can use the name of the section to retrieve a INISection of all
//		the key/value pairs in the section.  Within the hash of key/value pairs, you just
//		use the key name to retrieve the value. 
//
//		Provides the capability to load and save the INI file to disk files.
//	
//		To load/save the INI files, use the following:
//
//	 		INIFile LoadIni(const char *filename)
//			void SaveIni(INIFile &theINI, const char *filename)
//
//		To get/set individual keys, use the following:
//
//			string GetIniSetting(INIFile &theINI, const char *section, const char *key)
//			void PutIniSetting(INIFile &theINI, const char *section, const char *key, const char *value)
//
//  Typical usage is illustrated by the test main() function below.  
//

#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4786 4503)
#endif

#include <string.h>
#include <fstream>
#include "stlini.h"


// These pragmas are to quiet VC++ about the expanded template identifiers exceeding 255 chars.
// You won't be able to see those variables in a debug session, but the code will run normally
/**
*/
std::string GetIniSetting(INIFile &theINI, const char *section, const char *key, const char *defaultval)
{
	std::string result(defaultval);

	INIFile::iterator iSection = theINI.find(std::string(section));
	if(iSection != theINI.end())
	{
		INISection::iterator apair = iSection->second.find(std::string(key));
		if(apair != iSection->second.end())
			result = apair->second;
	}
	return result;
}

/**

void RemoveIniSetting(INIFile &theINI, const char *section, const char *key)
{
	INIFile::iterator iSection = theINI.find(std::string(section));
	if(iSection != theINI.end())
	{
		INISection::iterator apair = iSection->second.find(std::string(key));
		if(apair != iSection->second.end())
			iSection->second.erase(apair);
	}
}
*/
/**
*/
void PutIniSetting(INIFile &theINI, const char *section, const char *key, const char *value)
{     
	INIFile::iterator iniSection;
	INISection::iterator apair;
	
	if((iniSection = theINI.find(std::string(section))) == theINI.end())
	{
		// no such section?  Then add one..
		INISection newsection;
		if(key)
			newsection.insert(	std::pair<std::string, std::string> (std::string(key), std::string(value)) );
		theINI.insert( std::pair<std::string, INISection> (std::string(section), newsection) );
	}
	else if(key)
	{	// found section, make sure key isn't in there already, 
		// if it is, just drop and re-add
		apair = iniSection->second.find(std::string(key));
		if(apair != iniSection->second.end())
			iniSection->second.erase(apair);
		iniSection->second.insert( std::pair<std::string, std::string> (std::string(key), std::string(value)) );
	}
}

/**
*/
INIFile LoadIni(const char *filename)
{
	INIFile theINI;
	char  *value, *temp;
	std::string section;
	char buffer[MAX_INI_LINE];
	std::fstream file(filename, std::ios::in);
	
	while(file.good())
	{
		memset(buffer, 0, sizeof(buffer));
		file.getline(buffer, sizeof(buffer));
		if((temp = strchr(buffer, '\n')))
			*temp = '\0';		// cut off at newline
		if((temp = strchr(buffer, '\r')))
			*temp = '\0';		// cut off at linefeeds
		if((buffer[0] == '[') && (temp = strrchr(buffer, ']')))
		{     // if line is like -->   [section name]
			*temp = '\0';   // chop off the trailing ']';
			section = &buffer[1];
			PutIniSetting(theINI, &buffer[1]);	 // start new section
		}
		else if(buffer[0] && (value = strchr(buffer, '=')))
		{
			*value++ = '\0'; // assign whatever follows = sign to value, chop at "=" 
			PutIniSetting(theINI, section.c_str(), buffer, value); 	// and add both sides to INISection
		}
		else if(buffer[0])
			PutIniSetting(theINI, section.c_str(), buffer, ""); 	// must be a comment or something
	}
	return theINI;
}

/** 

void SaveIni(INIFile &theINI, const char *filename)
{
	std::fstream file(filename, std::ios::out);
	if(!file.good())
		return;
	
	// just iterate the hashes and values and dump them to a file.
	INIFile::iterator section= theINI.begin();
	while(section != theINI.end())
	{
		if(section->first > "") 
			file << std::endl << "[" << section->first << "]" << std::endl;
		INISection::iterator pair = section->second.begin();
	
		while(pair != section->second.end())
		{
			if(pair->second > "")
				file << pair->first << "=" << pair->second << std::endl;
			else
				file << pair->first << std::endl;
			pair++;
		}
		section++;
	}
	file.close();
}
*/

#ifdef _MSC_VER
#pragma warning(pop)
#endif


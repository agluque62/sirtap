#ifndef _LogConfig_h_
#define _LogConfig_h_

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>

#include "Severity.h"
#include "Util.h"

namespace plog
{
	// Mascara de Activacion 
	static struct stPlogProfile
	{
		bool toFile;
		bool toConsole;
		bool toNetwork;
	} pLogProfiles[]= {
		 {false,false,false}			// plog::none
		,{true, true, true }			// plog::fatal
		,{true, false,true }			// plog::error
		,{true, false,false}			// plog::warning
		,{true, true, false}			// plog::info
		,{false,false,false}			// plog::debug
		,{false,false,false}			// plog::verbose
	};

	/** Para el control de Nueva Configuracion */
#ifdef _WIN32
	static struct _stat _lastMod;
#else
	static struct stat _lastMod;
#endif

	/** */
	class LogConfig
	{
	public:
		LogConfig() {
			_fileformat="csv";
			_filename="plog.csv";
			_filesize=500000;
			_nfiles = 3;
			_ipcshare=false;
			_fileConfig="";

			_udpId="gw31";
			_ipUdp="127.0.0.1";
			_portUdp = 878;
		}
	public:
		inline std::string FileFormat() {return _fileformat;}
		inline std::string FileName() { return _filename;}
		inline unsigned long FileSize() { return _filesize;}
		inline int NumberOfFile() { return _nfiles;}
		inline bool IpcShare() {return _ipcshare;}
		inline std::string IpUpd(){return _ipUdp;}
		inline int PortUdp() { return _portUdp;}
		inline std::string UdpId(){return _udpId;}

	public:
		void Configure(std::string fileconfig)
		{
#ifdef _WIN32
			_stat( fileconfig.c_str(), &plog::_lastMod );
#else
			lstat( fileconfig.c_str(), &plog::_lastMod );
#endif // _WIN32

			_fileConfig = fileconfig;

			std::ifstream infile(fileconfig.c_str());
			std::string line;
			while (std::getline(infile, line))
			{
				if (line=="") continue;
				line.erase(std::remove_if(line.begin(), line.end(), &plog::LogConfig::IsSpace), line.end());
				std::vector<std::string> valores = split(line,'=');
				if (valores.size()==2)
				{
					if (valores[0]=="FILEFORMAT")
					{
						_fileformat=valores[1];
					} 
					else if (valores[0]=="FILENAME")
					{
						_filename = valores[1];
					}
					else if (valores[0]=="FILESIZE")
					{
						_filesize = atoi(valores[1].c_str());
					}
					else if (valores[0]=="NUMOFFILES")
					{
						_nfiles = atoi(valores[1].c_str());
					}
					else if (valores[0]=="IPC_SHARE")
					{
						if (valores[1]=="false")
							_ipcshare = false;
						else if (valores[1]=="true")
							_ipcshare = true;
					}
					else if (valores[0]=="UDP_IP")
					{
						_ipUdp = valores[1];
					}
					else if (valores[0]=="UDP_PORT")
					{
						_portUdp = atoi(valores[1].c_str());
					}
					else if (valores[0]=="UDP_PLOG_ID")
					{
						_udpId = valores[1];
					}
					else if (valores[0]=="TO_STANDARD_OUTPUT")
					{
						pLogProfiles[plog::info].toConsole=false;
						pLogProfiles[plog::error].toConsole=false;
						pLogProfiles[plog::debug].toConsole=false;

						std::vector<std::string> options = split(valores[1],',');
						for (int ival=0; ival < (int )options.size(); ival++)
						{
							if (options[ival]=="INFO")
								pLogProfiles[plog::info].toConsole=true;
							else if (options[ival]=="DEBUG")
								pLogProfiles[plog::debug].toConsole=true;
							else if (options[ival]=="ERROR")
								pLogProfiles[plog::error].toConsole=true;
						}
					}
					else if (valores[0]=="TO_FILE")
					{
						pLogProfiles[plog::info].toFile=false;
						pLogProfiles[plog::error].toFile=false;
						pLogProfiles[plog::debug].toFile=false;

						std::vector<std::string> options = split(valores[1],',');
						for (int ival=0; ival < (int )options.size(); ival++)
						{
							if (options[ival]=="INFO")
								pLogProfiles[plog::info].toFile=true;
							else if (options[ival]=="DEBUG")
								pLogProfiles[plog::debug].toFile=true;
							else if (options[ival]=="ERROR")
								pLogProfiles[plog::error].toFile=true;
						}
					}
					else if (valores[0]=="TO_NETWORK")
					{
						pLogProfiles[plog::info].toNetwork=false;
						pLogProfiles[plog::error].toNetwork=false;
						pLogProfiles[plog::debug].toNetwork=false;

						std::vector<std::string> options = split(valores[1],',');
						for (int ival=0; ival < (int )options.size(); ival++)
						{
							if (options[ival]=="INFO")
								pLogProfiles[plog::info].toNetwork=true;
							else if (options[ival]=="DEBUG")
								pLogProfiles[plog::debug].toNetwork=true;
							else if (options[ival]=="ERROR")
								pLogProfiles[plog::error].toNetwork=true;
						}
					}
				}
			}

		}
		/** */
		void TestCfgChange()
		{
			//static int counter=0;

			util::MutexLock lock(m_mutex);

			//counter = (counter + 1) % 5;		// Chequea cada 5 LOGS...
			//if (counter != 0)
			//	return;

			bool change = false;
#ifdef _WIN32
			struct _stat _currentMod;
			_stat( _fileConfig.c_str(), &_currentMod );
			if (_lastMod.st_mtime != _currentMod.st_mtime)
			{
				_lastMod.st_mtime = _currentMod.st_mtime;
				change = true;
			}
#else
			struct stat _currentMod;
			lstat( _fileConfig.c_str(), &plog::_lastMod );
			if (_lastMod.st_mtime != _currentMod.st_mtime)
			{
				_lastMod.st_mtime = _currentMod.st_mtime;
				change = true;
			}
#endif // _WIN32
			if (change==true)
				Configure(_fileConfig);
		}

	protected:
		std::vector<std::string> split(std::string text, char sep)
		{
			std::vector<std::string> tokens;
			unsigned int start = 0, end = 0;

			while ((end = text.find(sep, start)) != std::string::npos) 
			{
				tokens.push_back(text.substr(start, end - start));
				start = end + 1;
			}

			tokens.push_back(text.substr(start));
			return tokens;
		}
		static bool IsSpace(char c) {return c==' ';}

	private:
		std::string _fileformat;
		std::string _filename;
		unsigned long _filesize;
		int _nfiles;
		std::string _ipUdp;
		int _portUdp;
		std::string _udpId;
		bool _ipcshare;


		std::string _fileConfig;
		std::string _consoleProfile;
		std::string _fileProfile;
		std::string _networkProfile;

		/** */
		util::Mutex     m_mutex;
	};
}
#endif

#include "UG5KRecordService.h"


/** */
int main()
{
	NLOG_INFO("UG5K ED-137b4Service. ENTER para SALIR.");

	UG5KRecordService _service;
	_service.Start();

	std::string name;
	std::getline (std::cin,name);

	NLOG_INFO("UG5K ED-137b4Service. Saliendo...\n");

	_service.Dispose();
	return 0;

}

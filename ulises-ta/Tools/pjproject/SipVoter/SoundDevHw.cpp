#include "Global.h"
#include <windows.h>
#include <initguid.h>
#include <Mmdeviceapi.h>
#include <endpointvolume.h>
#include <functiondiscoverykeys_devpkey.h>
#include <propvarutil.h>
#include <iostream>

#include "Exceptions.h"
#include "SoundDevHw.h"
#include "SipAgent.h"

#define DEVHW_LOG(level,arg)	do { \
					if (SipAgent::SIP_AGENT_INITIALIZED_AND_STARTED)\
				    PJ_LOG(level,arg); \
				} while (0)

pj_bool_t SoundDevHw::IMM_Initialized = PJ_FALSE;
pj_pool_t* SoundDevHw::_Pool = NULL;
std::map<CORESIP_SndDevType, std::string> SoundDevHw::SND_devices_map;
pj_mutex_t* SoundDevHw::SND_devices_map_mutex = NULL;
char SoundDevHw::splitter[] = "<###>";

const float SoundDevHw::MinVolumeRenderDBs = -62.0f;
const float SoundDevHw::MaxVolumeRenderDBs = 0.0f;
const float SoundDevHw::MinVolumeCaptureDBs = 0.0f;
const float SoundDevHw::MaxVolumeCaptureDBs = 30.0f;

IMMDeviceEnumerator* SoundDevHw::devicesEnumerator = NULL;
IMMDeviceCollection* SoundDevHw::capture_immdevcollection = NULL;
IMMDeviceCollection* SoundDevHw::render_immdevcollection = NULL;

pj_timer_entry SoundDevHw::Winaudio_timer = { 0 };
CORESIP_WinAudioConfig SoundDevHw::WinAudioConfig = { 0 };

void SoundDevHw::Init()
{
	_Pool = pjsua_pool_create(NULL, 64, 32);
	if (_Pool == NULL)
	{
		throw PJLibException(__FILE__, PJ_ENOMEM).Msg("SoundDevHw::Init: No hay suficiente memoria");
		return;
	}

	SND_devices_map_mutex = NULL;
	pj_status_t st = pj_mutex_create_simple(_Pool, NULL, &SND_devices_map_mutex);
	if (st != PJ_SUCCESS)
	{
		pj_pool_release(_Pool);
		_Pool = NULL;
		throw PJLibException(__FILE__, PJ_ENOMEM).Msg("SoundDevHw::Init: No se puede crear SND_devices_map_mutex");
		return;
	}

	st = InitIMM();
	if (st != PJ_SUCCESS)
	{
		pj_mutex_destroy(SND_devices_map_mutex);
		SND_devices_map_mutex = NULL;
		pj_pool_release(_Pool);
		_Pool = NULL;
		PJ_CHECK_STATUS(st, ("ERROR SoundDevHw::Init: InitIMM devuelve error"));
	}
	
	IniWinAudio();

	return;
}

void SoundDevHw::Dispose()
{
	FinWinAudio();
	FinIMM();

	if (SND_devices_map_mutex != NULL)
	{
		pj_mutex_destroy(SND_devices_map_mutex);
		SND_devices_map_mutex = NULL;
	}
	if (_Pool != NULL)
	{		
		pj_pool_release(_Pool);
		_Pool = NULL;
	}
}

pj_status_t SoundDevHw::InitIMM()
{
	pj_status_t st = PJ_SUCCESS;
	HRESULT hr_coinitialize = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (hr_coinitialize != S_OK && hr_coinitialize != S_FALSE)
	{
		DEVHW_LOG(3, (__FILE__, "INFO: SoundDevHw::InitIMM: CoInitializeEx error %u", hr_coinitialize));
	}

	HRESULT hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER, __uuidof(IMMDeviceEnumerator), (LPVOID*)&devicesEnumerator);
	if (hr != S_OK)
	{
		if (hr_coinitialize == S_OK || hr_coinitialize == S_FALSE) CoUninitialize();
		DEVHW_LOG(3, (__FILE__, "ERROR SoundDevHw::InitIMM: CoCreateInstance devuelve error", "[error %ld]", hr));
		st = PJ_ENOTFOUND;
		return st;
	}

	hr = devicesEnumerator->EnumAudioEndpoints(eCapture, DEVICE_STATE_ACTIVE, &capture_immdevcollection);
	if (hr != S_OK)
	{
		devicesEnumerator->Release();
		if (hr_coinitialize == S_OK || hr_coinitialize == S_FALSE) CoUninitialize();
		DEVHW_LOG(3, (__FILE__, "ERROR SoundDevHw::InitIMM: capture EnumAudioEndpoints devuelve error", "[error %ld]", hr));
		st = PJ_ENOTFOUND;
		return st;
	}
	hr = devicesEnumerator->EnumAudioEndpoints(eRender, DEVICE_STATE_ACTIVE, &render_immdevcollection);
	if (hr != S_OK)
	{
		capture_immdevcollection->Release();
		devicesEnumerator->Release();
		if (hr_coinitialize == S_OK || hr_coinitialize == S_FALSE) CoUninitialize();
		DEVHW_LOG(3, (__FILE__, "ERROR SoundDevHw::InitIMM: render EnumAudioEndpoints devuelve error", "[error %ld]", hr));
		st = PJ_ENOTFOUND;
		return st;
	}

	if (st == PJ_SUCCESS)
		DEVHW_LOG(3, (__FILE__, "INFO: SoundDevHw::InitIMM: OK"));

	IMM_Initialized = PJ_TRUE;
	return st;
}

pj_status_t SoundDevHw::FinIMM()
{
	IMM_Initialized = PJ_FALSE;
	capture_immdevcollection->Release();
	render_immdevcollection->Release();
	devicesEnumerator->Release();
	CoUninitialize();
	DEVHW_LOG(3, (__FILE__, "INFO: SoundDevHw::FinIMM: OK"));
	return PJ_SUCCESS;
}

/*
	Asigna el literal del dispositivo de sonido que maneja la API de Windows al tipo de dispositivo que maneja el puesto de Ulises.
	@param UlisesDev. Dispositivo del puesto de Ulises
	@param DevWinName. Cadena de caracteres terminada con el caracter cero, con el nombre del dispositivo que maneja Windows
*/
void SoundDevHw::SetSNDDeviceWindowsName(CORESIP_SndDevType UlisesDev, const char *DevWinName)
{
	if (strlen(DevWinName) > (CORESIP_MAX_SOUND_NAME_LENGTH - 1))
	{
		PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR SetSNDDeviceWindowsName. Parametro DevWinName tiene longitud demasiado grande"));
	}	
	if (UlisesDev != CORESIP_SND_INSTRUCTOR_MHP && UlisesDev != CORESIP_SND_ALUMN_MHP && UlisesDev != CORESIP_SND_LC_SPEAKER &&
		UlisesDev != CORESIP_SND_RD_SPEAKER && UlisesDev != CORESIP_SND_INSTRUCTOR_RECORDER && UlisesDev != CORESIP_SND_ALUMN_RECORDER &&
		UlisesDev != CORESIP_SND_RADIO_RECORDER && UlisesDev != CORESIP_SND_LC_RECORDER && UlisesDev != CORESIP_SND_HF_SPEAKER &&
		UlisesDev != CORESIP_SND_HF_RECORDER)
	{
		PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR SetSNDDeviceWindowsName. Parametro UlisesDev no tiene un valor valido del tipo CORESIP_SndDevType"));
	}

	//Windows pone un numero, un guion y un espacio delante del nombre del dispositivo cuando se cambia de USB
	//Por tanto esos caracteres los ignoramos.
	unsigned int friendy_index = IndexIgnoringNamePrefix(DevWinName);

	pj_bool_t device_found = PJ_FALSE;
	CORESIP_SndWindowsDevices Devices;
	for (int type = 0; type < 2; type++)
	{
		GetWindowsSoundDeviceNames(type, &Devices, PJ_FALSE, NULL);
		for (unsigned int i = 0; i < Devices.ndevices_found; i++)
		{
			std::string devnames = Devices.DeviceNames;
			size_t pos = 0;
			size_t splitter_pos = 0;
			for (unsigned int i = 0; i < Devices.ndevices_found; i++)
			{
				splitter_pos = devnames.find(SoundDevHw::splitter, pos);
				std::string name;
				if (splitter_pos != std::string::npos)
				{
					name = devnames.substr(pos, splitter_pos - pos);
					pos = splitter_pos + strlen(SoundDevHw::splitter);
				}
				else
				{
					name = devnames.substr(pos);
				}
				char cname[CORESIP_MAX_SOUND_NAME_LENGTH];
				unsigned int j;
				for (j = 0; j < name.length() && j < (CORESIP_MAX_SOUND_NAME_LENGTH - 1); j++)
				{
					cname[j] = name[j];
				}
				cname[j] = '\0';

				unsigned int cname_index = IndexIgnoringNamePrefix(cname);
				if (strcmp(&cname[cname_index], &DevWinName[friendy_index]) == 0)
				{
					device_found = PJ_TRUE;
					break;
				}
			}
			if (device_found) break;
		}
		if (device_found) break;
	}

	if (!device_found)
	{
		PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR: SetSNDDeviceWindowsName. DevWinName %s no se encuentra en la lista de dispositivos de sonido de Windows", DevWinName));
	}	

	std::string devName = &DevWinName[friendy_index];
	pj_mutex_lock(SND_devices_map_mutex);
	SND_devices_map[UlisesDev] = devName;
	pj_mutex_unlock(SND_devices_map_mutex);

	char SndDevType[64];
	RecordPort::GetSndTypeString(UlisesDev, SndDevType, sizeof(SndDevType));
	DEVHW_LOG(3, (__FILE__, "INFO: SetSNDDeviceWindowsName: %s -> %s ", SndDevType, devName));
}


/*
	Funcion que retorna dispositivos de sonido. Separados en canales.
	@param captureType. Si vale distinto de cero retorna los de tipo capture, si vale cero los de tipo reproductor
	@param Devices. 
	@param only_ndevices. Solo se obtiene la cantidad de dispositivos, no los nombres
	@param ndevs. Son los dispositivos de sonido encontrados, no lo canales.
*/
void SoundDevHw::GetWindowsSoundDeviceNames(int captureType, CORESIP_SndWindowsDevices* Devices, pj_bool_t only_ndevices, unsigned int *ndevs)
{
	pj_bool_t IMM_local_initialized = PJ_FALSE;
	if (!IMM_Initialized)
	{
		//Puede que se llame desde el instalador del HMI, entonces hacemos la excepcion de inicializar aqui el IMM
		//Pero en este caso tendremos que finalizarlo tambien
		pj_status_t st = InitIMM();
		if (st != PJ_SUCCESS)
		{
			PJ_CHECK_STATUS(st, ("WARNING: GetWindowsSoundDeviceNames. IMM no inicializado. No es problema si no se trata del HMI del TA"));
		}
		IMM_local_initialized = PJ_TRUE;
	}

	HRESULT hr;
	IMMDeviceCollection* immdevcollection = NULL;	
	
	if (Devices == NULL) PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR GetSoundDevices. devices es NULL"));
	memset(Devices, 0, sizeof(CORESIP_SndWindowsDevices));

	immdevcollection = captureType != 0 ? capture_immdevcollection : render_immdevcollection;

	UINT _ndevices = 0;
	unsigned int ndevices_found = 0;
	hr = immdevcollection->GetCount(&_ndevices);
	if (hr != S_OK)
	{
		FinIMM();
		if (IMM_local_initialized) InitIMM();
		PJ_CHECK_STATUS(PJ_ENOTFOUND, ("ERROR GetSoundDevices. EnumAudioEndpoints devuelve error", "[error %ld]", hr));
	}
		
	unsigned int DevNames_index = 0;
	unsigned int Friendy_index = 0;
	unsigned int GUID_index = 0;
	for (UINT i = 0; i < _ndevices; i++)
	{
		IMMDevice* immdevice = NULL;
		hr = immdevcollection->Item(i, &immdevice);
		if (hr != S_OK)
		{
			FinIMM();
			if (IMM_local_initialized) InitIMM();
			PJ_CHECK_STATUS(PJ_ENOTFOUND, ("ERROR GetSoundDevices. Item devuelve error", "[error %ld]", hr));
		}

		IPropertyStore* ppProperties;
		hr = immdevice->OpenPropertyStore(STGM_READ, &ppProperties);
		if (hr != S_OK)
		{			
			immdevice->Release();
			FinIMM();
			if (IMM_local_initialized) InitIMM();
			PJ_CHECK_STATUS(PJ_ENOTFOUND, ("ERROR GetSoundDevices. OpenPropertyStore devuelve error", "[error %ld]", hr));
		}

		IAudioEndpointVolume* endpointVolume = NULL;
		hr = immdevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, NULL, (LPVOID*)&endpointVolume);
		if (hr != S_OK)
		{
			ppProperties->Release();
			immdevice->Release();
			FinIMM();
			if (IMM_local_initialized) InitIMM();
			PJ_CHECK_STATUS(PJ_ENOTFOUND, ("ERROR GetSoundDevices. No se puede controlar volumen del dispositivo"));
		}
		unsigned int channel_Count = 0;
		hr = endpointVolume->GetChannelCount(&channel_Count);
		if (hr != S_OK)
		{
			endpointVolume->Release();
			ppProperties->Release();
			immdevice->Release();
			FinIMM();
			if (IMM_local_initialized) InitIMM();
			PJ_CHECK_STATUS(PJ_ENOTFOUND, ("ERROR GetSoundDevices. No se puede controlar volumen del dispositivo"));
		}
		endpointVolume->Release();
		if (channel_Count > 25) channel_Count = 25;
		ndevices_found += channel_Count;

		if (!only_ndevices)
		{
			WCHAR szTitle[257];
			PROPVARIANT varName;
			PropVariantInit(&varName);
			// Get the endpoint's friendly-name property.
			//hr = ppProperties->GetValue(PKEY_Device_FriendlyName, &varName);
			hr = ppProperties->GetValue(PKEY_DeviceInterface_FriendlyName, &varName);
			hr = PropVariantToString(varName, szTitle, ARRAYSIZE(szTitle));
			if (SUCCEEDED(hr))
			{
				std::wstring ws(szTitle);
				unsigned int c;
				unsigned int h;

				if ((ws.length() + 2) >= CORESIP_MAX_SOUND_NAME_LENGTH)
				{
					PropVariantClear(&varName);
					ppProperties->Release();
					immdevice->Release();
					PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR GetSoundDevices. Nombre de dispositivo de audio mayor de ", "%ld", CORESIP_MAX_SOUND_NAME_LENGTH));
				}

				for (c = 0; c < channel_Count; c++)
				{
					for (h = 0; h < (ws.length() + 2); h++)
					{
						if (DevNames_index >= sizeof(Devices->DeviceNames))
						{
							PropVariantClear(&varName);
							ppProperties->Release();
							immdevice->Release();
							PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR GetSoundDevices. No caben todos los nombre en el array DevNames  "));
						}
						if (h < ws.length())
							Devices->DeviceNames[DevNames_index++] = (char)ws[h];
						else if (h == ws.length())
							Devices->DeviceNames[DevNames_index++] = (char)' ';
						else if (h == (ws.length() + 1))
						{
							char sc[2];							
							sprintf(sc, "%c", (char) c + 'a');
							Devices->DeviceNames[DevNames_index++] = (char)sc[0];
						}
					}

					for (unsigned int k = 0; k < strlen(splitter); k++)
					{
						if (DevNames_index >= sizeof(Devices->DeviceNames))
						{
							PropVariantClear(&varName);
							ppProperties->Release();
							immdevice->Release();
							PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR GetSoundDevices. No caben todos los nombre en el array DevNames  "));
						}
						Devices->DeviceNames[DevNames_index++] = splitter[k];
					}
				}
			}
			PropVariantClear(&varName);

			PropVariantInit(&varName);
			// Get the endpoint's friendly-name property.
			hr = ppProperties->GetValue(PKEY_Device_FriendlyName, &varName);
			hr = PropVariantToString(varName, szTitle, ARRAYSIZE(szTitle));
			if (SUCCEEDED(hr))
			{
				std::wstring ws(szTitle);
				unsigned int c;
				unsigned int h;

				if ((ws.length() + 2) >= CORESIP_MAX_SOUND_NAME_LENGTH)
				{
					PropVariantClear(&varName);
					ppProperties->Release();
					immdevice->Release();
					PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR GetSoundDevices. Nombre de dispositivo de audio mayor de ", "%ld", CORESIP_MAX_SOUND_NAME_LENGTH));
				}

				for (c = 0; c < channel_Count; c++)
				{
					for (h = 0; h < (ws.length() + 2); h++)
					{
						if (Friendy_index >= sizeof(Devices->FriendlyName))
						{
							PropVariantClear(&varName);
							ppProperties->Release();
							immdevice->Release();
							PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR GetSoundDevices. No caben todos los nombre en el array FriendlyName  "));
						}
						if (h < ws.length())
							Devices->FriendlyName[Friendy_index++] = (char)ws[h];
						else if (h == ws.length())
							Devices->FriendlyName[Friendy_index++] = (char)' ';
						else if (h == (ws.length() + 1))
						{
							char sc[2];
							sprintf(sc, "%c", (char)c + 'a');
							Devices->FriendlyName[Friendy_index++] = (char)sc[0];
						}
					}

					for (unsigned int k = 0; k < strlen(splitter); k++)
					{
						if (Friendy_index >= sizeof(Devices->FriendlyName))
						{
							PropVariantClear(&varName);
							ppProperties->Release();
							immdevice->Release();
							PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR GetSoundDevices. No caben todos los nombre en el array FriendlyName  "));
						}
						Devices->FriendlyName[Friendy_index++] = splitter[k];
					}
				}
			}
			PropVariantClear(&varName);

			PropVariantInit(&varName);
			// Get the endpoint's friendly-name property.
			hr = ppProperties->GetValue(PKEY_AudioEndpoint_GUID, &varName);
			hr = PropVariantToString(varName, szTitle, ARRAYSIZE(szTitle));
			if (SUCCEEDED(hr))
			{
				std::wstring ws(szTitle);
				unsigned int c;
				unsigned int h;

				if ((ws.length() + 2) >= CORESIP_MAX_SOUND_NAME_LENGTH)
				{
					PropVariantClear(&varName);
					ppProperties->Release();
					immdevice->Release();
					PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR GetSoundDevices. Nombre de dispositivo de audio mayor de ", "%ld", CORESIP_MAX_SOUND_NAME_LENGTH));
				}

				for (c = 0; c < channel_Count && (c + 1) < 10; c++)
				{
					for (h = 0; h < (ws.length() + 2); h++)
					{
						if (GUID_index >= sizeof(Devices->GUID))
						{
							PropVariantClear(&varName);
							ppProperties->Release();
							immdevice->Release();
							PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR GetSoundDevices. No caben todos los nombre en el array GUID  "));
						}
						if (h < ws.length())
							Devices->GUID[GUID_index++] = (char)ws[h];
						else if (h == ws.length())
							Devices->GUID[GUID_index++] = (char)' ';
						else if (h == (ws.length() + 1))
						{
							char sc[2]; 
							sprintf(sc, "%c", (char)c + 'a');
							Devices->GUID[GUID_index++] = (char)sc[0];
						}
					}

					for (unsigned int k = 0; k < strlen(splitter); k++)
					{
						if (GUID_index >= sizeof(Devices->GUID))
						{
							PropVariantClear(&varName);
							ppProperties->Release();
							immdevice->Release();
							PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR GetSoundDevices. No caben todos los nombre en el array GUID  "));
						}
						Devices->GUID[GUID_index++] = splitter[k];
					}
				}
			}
			PropVariantClear(&varName);
		}		
		
		ppProperties->Release();
		immdevice->Release();
	}

	if (!only_ndevices)
	{
		if (DevNames_index >= sizeof(Devices->DeviceNames))
		{
			PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR GetSoundDevices. No caben todos los nombre en el array DevNames  "));
		}
		if (ndevices_found > 0) Devices->DeviceNames[DevNames_index - strlen(splitter)] = '\0';
		if (Friendy_index >= sizeof(Devices->FriendlyName))
		{
			PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR GetSoundDevices. No caben todos los nombre en el array FriendlyName  "));
		}
		if (ndevices_found > 0) Devices->FriendlyName[Friendy_index - strlen(splitter)] = '\0';
		if (GUID_index >= sizeof(Devices->GUID))
		{
			PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR GetSoundDevices. No caben todos los nombre en el array GUID  "));
		}
		if (ndevices_found > 0) Devices->GUID[GUID_index - strlen(splitter)] = '\0';
	}

	if (ndevs != NULL) *ndevs = _ndevices;
	DEVHW_LOG(5, (__FILE__, "INFO: GetSoundDevices: %u", _ndevices));
	Devices->ndevices_found = ndevices_found;

	if (IMM_local_initialized) FinIMM();
}

/*
	Funcion que asigna el volumen a los dispositivos de salida (speakers) que son listener de un conf port
	@param conf_port_id. Identificador del puerto de la conferencia
	@param volume. Es el porcentaje del nivel maximo que proporciona Windows.
	@return. PJ_TRUE Si encuentra algun dispositivo.
*/
pj_bool_t SoundDevHw::SetVolumeToListenerSndPort(pjsua_conf_port_id conf_port_id, unsigned volume)
{
	pj_bool_t ret = PJ_FALSE;
	if (!IMM_Initialized)
	{
		DEVHW_LOG(3, (__FILE__, "WARNING: SetVolumeToListenerSndPort. IMM no inicializado. No es problema si no se trata del HMI del TA"));
		return ret;
	}

	pjsua_conf_port_info portInfo;
	pj_status_t st = pjsua_conf_get_port_info(conf_port_id, &portInfo);
	if (st != PJ_SUCCESS) return ret;
	for (unsigned int i = 0; i < portInfo.listener_cnt; i++)
	{
		portInfo.listeners[i];
		int dev = 0;

		for (int dev = 0; dev < PJ_ARRAY_SIZE(SipAgent::_SndPorts); dev++)
		{
			if (SipAgent::_SndPorts[dev] != NULL)
			{
				if (SipAgent::_SndPorts[dev]->_Type == CORESIP_SND_ALUMN_MHP || SipAgent::_SndPorts[dev]->_Type == CORESIP_SND_INSTRUCTOR_MHP ||
					SipAgent::_SndPorts[dev]->_Type == CORESIP_SND_RD_SPEAKER || SipAgent::_SndPorts[dev]->_Type == CORESIP_SND_LC_SPEAKER ||
					SipAgent::_SndPorts[dev]->_Type == CORESIP_SND_HF_SPEAKER)
				{
					if (SipAgent::_SndPorts[dev]->Slot == portInfo.listeners[i])
					{
						std::map<CORESIP_SndDevType, std::string>::iterator it;
						pj_mutex_lock(SND_devices_map_mutex);
						it = SND_devices_map.find(SipAgent::_SndPorts[dev]->_Type);						
						if (it == SND_devices_map.end())
						{
							pj_mutex_unlock(SND_devices_map_mutex);
							DEVHW_LOG(5, (__FILE__, "ERROR: SetVolumeToListenerSndPort: Dispositivo CORESIP_SndDevType %d no tiene asignado dispositivo de sonido de Windows",
								SipAgent::_SndPorts[dev]->_Type));
						}
						else
						{
							pj_mutex_unlock(SND_devices_map_mutex);
							SetVolumeOutputDevice(SipAgent::_SndPorts[dev]->_Type, volume);
							ret = PJ_TRUE;
						}
					}
				}
			}
		}
	}
	return ret;
}

/*
	Funcion que retorna propiedades de un dispositivo
	@param captureType. Si vale distinto de cero el dispositivo es de tipo capture, si vale cero es de tipo reproductor
	@param devIndex. Indice del dispoositivo. Es un numero comprendido entre 0 y ndevices-1. Siendo ndevices el valor retornado por GetSoundRenderDevicesCount
	@param DeviceInterface_FriendlyName. Puntero al buffer en el que se retorna la propiedad PKEY_DeviceInterface_FriendlyName finalizada con caracter cero. 
	@param DeviceInterface_FriendlyName_size. Es la longitud del buffer DeviceInterface_FriendlyName. Identificador friendly del dispositivo.
	@param AudioEndpoint_GUID. Puntero al buffer en el que se retorna la propiedad PKEY_AudioEndpoint_GUID finalizada con caracter cero.Identificador unico del dispositivo.
	@param AudioEndpoint_GUID_size. Es la longitud del buffer AudioEndpoint_GUID
*/
void SoundDevHw::GetSoundDevicePropeties(int captureType, unsigned int devIndex, char *DeviceInterface_FriendlyName, const unsigned int DeviceInterface_FriendlyName_size,
	char* AudioEndpoint_GUID, const unsigned int AudioEndpoint_GUID_size)
{
	if (!IMM_Initialized)
	{
		PJ_CHECK_STATUS(PJ_EINVALIDOP, ("WARNING: GetSoundDevicePropeties. IMM no inicializado. No es problema si no se trata del HMI del TA"));
	}

	HRESULT hr;
	IMMDeviceCollection* immdevcollection = NULL;	

	WCHAR szTitle[CORESIP_MAX_SOUND_NAME_LENGTH+1];
	if (DeviceInterface_FriendlyName == NULL)
		PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR GetSoundRenderDevicePropeties", "Parametro DeviceInterface_FriendlyName es NULL"));
	if (AudioEndpoint_GUID == NULL)
		PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR GetSoundRenderDevicePropeties", "Parametro AudioEndpoint_GUID es NULL"));
	if (DeviceInterface_FriendlyName_size >= ARRAYSIZE(szTitle))
		PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR GetSoundRenderDevicePropeties", "Parametro DeviceInterface_FriendlyName_size debe ser menor que %d", ARRAYSIZE(szTitle)));
	if (AudioEndpoint_GUID_size >= ARRAYSIZE(szTitle))
		PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR GetSoundRenderDevicePropeties", "Parametro AudioEndpoint_GUID_size debe ser menor que %d", ARRAYSIZE(szTitle)));

	immdevcollection = captureType != 0 ? capture_immdevcollection : render_immdevcollection;

	UINT _ndevices;
	hr = immdevcollection->GetCount(&_ndevices);
	if (hr != S_OK)
	{
		FinIMM();
		InitIMM();
		PJ_CHECK_STATUS(PJ_ENOTFOUND, ("ERROR GetSoundRenderDevicePropeties. EnumAudioEndpoints devuelve error", "[error %ld]", hr));
	}

	if (devIndex >= _ndevices)
	{
		PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR GetSoundRenderDevicePropeties. Parametro devIndex debe ser menor que el numero de dispositivos"));
	}

	IMMDevice* immdevice = NULL;
	hr = immdevcollection->Item(devIndex, &immdevice);
	if (hr != S_OK)
	{
		FinIMM();
		InitIMM();
		PJ_CHECK_STATUS(PJ_ENOTFOUND, ("ERROR GetSoundRenderDevicePropeties. Item devuelve error", "[error %ld]", hr));
	}

	IPropertyStore* ppProperties;
	hr = immdevice->OpenPropertyStore(STGM_READ, &ppProperties);
	if (hr != S_OK)
	{
		immdevice->Release();
		FinIMM();
		InitIMM();
		PJ_CHECK_STATUS(PJ_ENOTFOUND, ("ERROR GetSoundRenderDevicePropeties. OpenPropertyStore devuelve error", "[error %ld]", hr));
	}

	PROPVARIANT varName;
	// Initialize container for property value.
	PropVariantInit(&varName);
	// Get the endpoint's friendly-name property.
	hr = ppProperties->GetValue(PKEY_DeviceInterface_FriendlyName, &varName);
	hr = PropVariantToString(varName, szTitle, ARRAYSIZE(szTitle));
	if (SUCCEEDED(hr))
	{
		std::wstring ws(szTitle);
		unsigned int i;
		for (i = 0; i < ws.length() && i < (DeviceInterface_FriendlyName_size-1); i++)
		{
			DeviceInterface_FriendlyName[i] = (char) ws[i];
		}
		DeviceInterface_FriendlyName[i] = '\0';		
	}
	PropVariantClear(&varName);

	PropVariantInit(&varName);
	// Get the endpoint's friendly-name property.
	hr = ppProperties->GetValue(PKEY_AudioEndpoint_GUID, &varName);
	hr = PropVariantToString(varName, szTitle, ARRAYSIZE(szTitle));
	if (SUCCEEDED(hr))
	{
		std::wstring ws(szTitle);
		unsigned int i;
		for (i = 0; i < ws.length() && i < (AudioEndpoint_GUID_size - 1); i++)
		{
			AudioEndpoint_GUID[i] = (char)ws[i];
		}
		AudioEndpoint_GUID[i] = '\0';
	}
	PropVariantClear(&varName);

	DEVHW_LOG(5, (__FILE__, "INFO: GetSoundRenderDevicePropeties: dev index %u DeviceInterface_FriendlyName %s AudioEndpoint_GUID %s", 
		devIndex, DeviceInterface_FriendlyName, AudioEndpoint_GUID));

	ppProperties->Release();
	immdevice->Release();
}

/*
	Funcion que establece el volumen de un dispositivo
	@param captureType. Si vale distinto de cero el dispositivo es de tipo capture, si vale cero es de tipo reproductor
	@param item. Es el index correspondiente al un device de la lista que retorna GetWindowsSoundDeviceNames
	@param channel. El canal del dispositivo.
	@param voltype. Tipo de volumen
	@param volume. Si el tipo de scalar el valor estara comprendido entre 0.0 y 1.0. 
	Rango si es DB para tipo render (disp. salida): -62.0 y 0.0
	Rango si es DB para tipo capture (disp. salida): 0.0 y 30.0
	@param *flVolumeMindB. Retorna el volumen minimo real que soporta el dispositivo
	@param *flVolumeMaxdB. Retorna el volumen maximo real que soporta el dispositivo
	@param *flVolumeIncrementdB. Retorna el incremento en DBs que soporta el dispositivo.
	@param *current_volume_scalar. Retorna actual volumen en scalar.
	@param *current_volume_dB. Retorna actual volumen en dB.
*/
int SoundDevHw::VolumeDevByItem(int captureType, unsigned int item, unsigned int channel, SoundDevHw::volume_type voltype, float volume,
	float *flVolumeMindB, float *flVolumeMaxdB, float *flVolumeIncrementdB,
	float *current_volume_scalar, float *current_volume_dB)
{
	int ret = 0;
	if (!IMM_Initialized)
	{
		DEVHW_LOG(3, (__FILE__, "WARNING: VolumeDevByItem. IMM no inicializado. No es problema si no se trata del HMI del TA"));
		return ret;
	}

	HRESULT hr;
	IMMDeviceCollection* immdevcollection = NULL;

	if (voltype == scalar)
	{
		if (volume > 1.0f) volume = 1.0f;
		if (volume < 0.0f) volume = 0.0f;
	}
	else if (voltype == dB)
	{
		if (captureType == 0)
		{
			if (volume > MaxVolumeRenderDBs) volume = MaxVolumeRenderDBs;
			if (volume < MinVolumeRenderDBs) volume = MinVolumeRenderDBs;
		}
		else
		{
			if (volume > MaxVolumeCaptureDBs) volume = MaxVolumeCaptureDBs;
			if (volume < MinVolumeCaptureDBs) volume = MinVolumeCaptureDBs;
		}
	}

	immdevcollection = captureType != 0 ? capture_immdevcollection : render_immdevcollection;

	IMMDevice* immdevice = NULL;
	hr = immdevcollection->Item(item, &immdevice);
	if (hr == S_OK)
	{
		IAudioEndpointVolume* endpointVolume = NULL;
		hr = immdevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, NULL, (LPVOID*)&endpointVolume);
		if (hr == S_OK)
		{	
			float flMindB = 0, flMaxdB = 0, flIncrementdB = 0;
			hr = endpointVolume->GetVolumeRange(&flMindB, &flMaxdB, &flIncrementdB);
			if (hr == S_OK)
			{
				if (flVolumeMindB != NULL) *flVolumeMindB = flMindB;
				if (flVolumeMaxdB != NULL) *flVolumeMaxdB = flMaxdB;
				if (flVolumeIncrementdB != NULL) *flVolumeIncrementdB = flIncrementdB;

				if (voltype == dB)
				{
					//DEVHW_LOG(3, (__FILE__, "INFO: ScalarVolumeRenderDevByItem: flVolumeMindB %f, flVolumeMaxdB %f, flVolumeIncrementdB %f", flVolumeMindB, flVolumeMaxdB, flVolumeIncrementdB));
					if (captureType == 0)
					{
						volume = flMindB + ((volume - MinVolumeRenderDBs) / (MaxVolumeRenderDBs - MinVolumeRenderDBs)) * (flMaxdB - flMindB);
					}
					else
					{
						volume = flMindB + ((volume - MinVolumeCaptureDBs) / (MaxVolumeCaptureDBs - MinVolumeCaptureDBs)) * (flMaxdB - flMindB);
					}
				}
			}
			
			if (voltype != only_get_volume_range && hr == S_OK)
			{
				float current_vol;
				int tries = 200;				
				if (voltype == scalar)
				{
					float incremento_minimo = 0.00001f;
					hr = endpointVolume->GetChannelVolumeLevelScalar(channel, &current_vol);
					if (current_vol < volume && hr == S_OK)
					{
						while (current_vol < (volume - incremento_minimo))
						{
							float inc = ((volume - current_vol) * 5.0f) / 6.0f;
							if (inc < incremento_minimo) inc = incremento_minimo;
							current_vol += inc;
							if (current_vol > 1.0f) current_vol = 1.0f;
							hr = endpointVolume->SetChannelVolumeLevelScalar(channel, current_vol, NULL);
							if (hr != S_OK) break;							
							hr = endpointVolume->GetChannelVolumeLevelScalar(channel, &current_vol);							
							if (hr != S_OK) break;
							if (--tries < 0)
							{
								hr = S_FALSE;
								break;
							}
						}						
					}
					else
					{
						while ((current_vol > (volume + incremento_minimo)) && hr == S_OK)
						{
							float inc = ((current_vol - volume) * 5.0f) / 6.0f;
							if (inc < incremento_minimo) inc = incremento_minimo;
							current_vol -= inc;
							if (current_vol < 0.0f) current_vol = 0.0f;
							hr = endpointVolume->SetChannelVolumeLevelScalar(channel, current_vol, NULL);
							if (hr != S_OK) break;
							hr = endpointVolume->GetChannelVolumeLevelScalar(channel, &current_vol);
							if (hr != S_OK) break;
							if (--tries < 0)
							{
								hr = S_FALSE;
								break;
							}
						}
					}

					//Finalmente actualizamos el volumen con el requerido
					pj_thread_sleep(11);
					if (hr == S_OK) hr = endpointVolume->SetChannelVolumeLevelScalar(channel, volume, NULL);					
					if (hr == S_OK) hr = endpointVolume->GetChannelVolumeLevelScalar(channel, &current_vol);

					float vdbs;
					endpointVolume->GetChannelVolumeLevel(channel, &vdbs);
				}
				else if (voltype == dB)
				{
					hr = endpointVolume->GetChannelVolumeLevel(channel, &current_vol);
					if (current_vol < volume && hr == S_OK)
					{
						while (current_vol < volume)
						{
							float inc = (volume - current_vol) / 2.0f;
							if (inc < 0.01f) inc = 0.01f;
							current_vol += inc;
							if (current_vol > flMaxdB) current_vol = flMaxdB;
							hr = endpointVolume->SetChannelVolumeLevel(channel, current_vol, NULL);							
							if (hr != S_OK) break;
							hr = endpointVolume->GetChannelVolumeLevel(channel, &current_vol);
							if (hr != S_OK) break;
							if (--tries < 0)
							{
								hr = S_FALSE;
								break;
							}
						}
					}
					else
					{
						while (current_vol > volume && hr == S_OK)
						{
							float inc = (current_vol - volume) / 2.0f;
							if (inc < 0.01f) inc = 0.01f;
							current_vol -= inc;
							if (current_vol < flMindB) current_vol = flMindB;
							hr = endpointVolume->SetChannelVolumeLevel(channel, current_vol, NULL);
							if (hr != S_OK) break;
							hr = endpointVolume->GetChannelVolumeLevel(channel, &current_vol);
							if (hr != S_OK) break;
							if (--tries < 0)
							{
								hr = S_FALSE;
								break;
							}
						}
					}

					//endpointVolume->GetChannelVolumeLevel(channel, &current_vol);
					//endpointVolume->SetChannelVolumeLevel(channel, volume, NULL);
				}
			}
			if (current_volume_scalar != NULL && hr == S_OK) hr = endpointVolume->GetChannelVolumeLevelScalar(channel, current_volume_scalar);
			if (current_volume_dB != NULL && hr == S_OK) hr = endpointVolume->GetChannelVolumeLevel(channel, current_volume_dB);
			
			endpointVolume->Release();
			immdevice->Release();			
		}
		else
		{
			immdevice->Release();
		}		
	}
	else
	{
	}

	if (hr != S_OK)
	{
		FinIMM();
		InitIMM();
		ret = -1;
	}

	return ret;
}

int SoundDevHw::GetVolumeDevByItem(int captureType, unsigned int item, unsigned int channel, SoundDevHw::volume_type voltype, float *volume)
{
	int ret = 0;
	if (!IMM_Initialized)
	{
		DEVHW_LOG(3, (__FILE__, "WARNING: VolumeDevByItem. IMM no inicializado. No es problema si no se trata del HMI del TA"));
		return -1;
	}

	HRESULT hr;
	IMMDeviceCollection* immdevcollection = NULL;

	immdevcollection = captureType != 0 ? capture_immdevcollection : render_immdevcollection;

	IMMDevice* immdevice = NULL;
	hr = immdevcollection->Item(item, &immdevice);
	if (hr == S_OK)
	{
		IAudioEndpointVolume* endpointVolume = NULL;
		hr = immdevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, NULL, (LPVOID*)&endpointVolume);
		if (hr == S_OK)
		{
			if (voltype == scalar)
			{
				hr = endpointVolume->GetChannelVolumeLevelScalar(channel, volume);
			}
			else
			{
				hr = endpointVolume->GetChannelVolumeLevel(channel, volume);
			}
			endpointVolume->Release();
			immdevice->Release();
		}
		else
		{
			immdevice->Release();
		}
	}
	else
	{
	}

	if (hr != S_OK)
	{
		FinIMM();
		InitIMM();
		ret = -1;
	}

	return ret;
}

/*
	Funcion que establece volumen de un dispositivo de reproduccion(speaker)
	@param dev. dispositivo. Si vale CORESIP_SND_UNKNOWN, entonces se controla el volumen del dispositivo DEFAULT.
	@param volume. Es el porcentaje del nivel maximo 
*/
pj_status_t SoundDevHw::SetVolumeOutputDevice(CORESIP_SndDevType dev, unsigned int volume)
{
	if (!IMM_Initialized)
	{
		DEVHW_LOG(3, (__FILE__, "WARNING: SetVolumeOutputDevice. IMM no inicializado. No es problema si no se trata del HMI del TA"));
		return PJ_EINVALIDOP;
	}

	if (dev != CORESIP_SND_INSTRUCTOR_MHP && dev != CORESIP_SND_ALUMN_MHP && dev != CORESIP_SND_LC_SPEAKER &&
		dev != CORESIP_SND_RD_SPEAKER && dev != CORESIP_SND_HF_SPEAKER)
	{
		DEVHW_LOG(3, (__FILE__, "ERROR: SetVolumeOutputDevice. El dispositivo debe ser CORESIP_SND_INSTRUCTOR_MHP, CORESIP_SND_ALUMN_MHP, CORESIP_SND_LC_SPEAKER, CORESIP_SND_RD_SPEAKER o CORESIP_SND_HF_SPEAKER"));
		return PJ_EINVAL;
	}

	std::map<CORESIP_SndDevType, std::string>::iterator it;
	pj_mutex_lock(SND_devices_map_mutex);
	it = SND_devices_map.find(dev);
	if (it == SND_devices_map.end())
	{
		pj_mutex_unlock(SND_devices_map_mutex);
		DEVHW_LOG(3, (__FILE__, "ERROR: SetVolumeOutputDevice. El dispoxitivo fisico de audio no ha sido asignado"));
		return PJ_ENOTFOUND;
	}
	std::string name_to_search = SND_devices_map[dev];	
	pj_mutex_unlock(SND_devices_map_mutex);

	char c_channel = name_to_search[name_to_search.size() - 1];
	unsigned int i_channel = ((unsigned int)(c_channel - 'a'));	

	char AudioEndpoint_GUID[CORESIP_MAX_SOUND_NAME_LENGTH];

	float scalar_volume;

	CORESIP_SndWindowsDevices Devices;
	unsigned int ndevs = 0;
	GetWindowsSoundDeviceNames(0, &Devices, PJ_TRUE, &ndevs);
	for (unsigned int i = 0; i < ndevs; i++)
	{
		char DeviceInterface_FriendlyName[CORESIP_MAX_SOUND_NAME_LENGTH];
		GetSoundDevicePropeties(0, i, DeviceInterface_FriendlyName, sizeof(DeviceInterface_FriendlyName),
			AudioEndpoint_GUID, sizeof(AudioEndpoint_GUID));

		//Windows pone un numero, un guion y un espacio delante del nombre del dispositivo cuando se cambia de USB
		//Por tanto esos caracteres los ignoramos.
		unsigned int friendy_index = IndexIgnoringNamePrefix(DeviceInterface_FriendlyName);			

		//Comparamos eliminando los dos ultimos caracteres que son el espacio y 'a' o 'b', que indican el canal.
		if (strnicmp(&DeviceInterface_FriendlyName[friendy_index], name_to_search.c_str(), name_to_search.size() - 2) == 0)
		{
			//float scalar_volume;
			float winaudio_volume_scalar;
				
			switch (dev)
			{
			case CORESIP_SND_INSTRUCTOR_MHP:
				winaudio_volume_scalar = WinAudioConfig.CascoAyudante_scalar;
				break;
			case CORESIP_SND_ALUMN_MHP:
				winaudio_volume_scalar = WinAudioConfig.CascoEjecutivo_scalar;
				break;
			case CORESIP_SND_LC_SPEAKER:
				winaudio_volume_scalar = WinAudioConfig.AltavozLC_scalar;
				break;
			case CORESIP_SND_RD_SPEAKER:
				winaudio_volume_scalar = WinAudioConfig.AltavozRadioVHF_scalar;
				break;
			case CORESIP_SND_HF_SPEAKER:
				winaudio_volume_scalar = WinAudioConfig.AltavozRadioHF_scalar;
				break;
			default:
				winaudio_volume_scalar = 1.0f;
			}

			if (winaudio_volume_scalar < 0.0) winaudio_volume_scalar = 1.0;
			scalar_volume = (winaudio_volume_scalar * (float)volume) / 100.0f;

			int voltries = 3;
			for (;;)
			{
				if (VolumeDevByItem(0, i, i_channel, scalar, scalar_volume, NULL, NULL, NULL, NULL, NULL) == 0) break;
				if (--voltries <= 0)
				{
					DEVHW_LOG(3, (__FILE__, "ERROR: SetVolumeOutputDevice: dev %d error de control de volumen", dev));
					break;
				}
			}
			DEVHW_LOG(5, (__FILE__, "INFO: SetVolumeOutputDevice: found 1 dev %d, DeviceInterface_FriendlyName %s volume %u", 
				dev, DeviceInterface_FriendlyName, volume));
			break;
		}
	}

	return PJ_SUCCESS;
}

/*
	Funcion que retorna volumen de un dispositivo de reproduccion(speaker)
	@param dev. dispositivo. Si vale CORESIP_SND_UNKNOWN, entonces se controla el volumen del dispositivo DEFAULT.
	@param volume. Es el porcentaje del nivel maximo
*/
pj_status_t SoundDevHw::GetVolumeOutputDevice(CORESIP_SndDevType dev, unsigned int *volume)
{
	if (!IMM_Initialized)
	{
		DEVHW_LOG(3, (__FILE__, "WARNING: GetVolumeOutputDevice. IMM no inicializado. No es problema si no se trata del HMI del TA"));
		return PJ_EINVALIDOP;
	}

	if (dev != CORESIP_SND_INSTRUCTOR_MHP && dev != CORESIP_SND_ALUMN_MHP && dev != CORESIP_SND_LC_SPEAKER &&
		dev != CORESIP_SND_RD_SPEAKER && dev != CORESIP_SND_HF_SPEAKER)
	{
		DEVHW_LOG(3, (__FILE__, "ERROR: GetVolumeOutputDevice. El dispositivo debe ser CORESIP_SND_INSTRUCTOR_MHP, CORESIP_SND_ALUMN_MHP, CORESIP_SND_LC_SPEAKER, CORESIP_SND_RD_SPEAKER o CORESIP_SND_HF_SPEAKER"));
		return PJ_EINVAL;
	}

	std::map<CORESIP_SndDevType, std::string>::iterator it;
	pj_mutex_lock(SND_devices_map_mutex);
	it = SND_devices_map.find(dev);
	if (it == SND_devices_map.end())
	{
		pj_mutex_unlock(SND_devices_map_mutex);
		DEVHW_LOG(3, (__FILE__, "ERROR: GetVolumeOutputDevice. El dispoxitivo fisico de audio no ha sido asignado"));
		return PJ_ENOTFOUND;
	}
	std::string name_to_search = SND_devices_map[dev];
	pj_mutex_unlock(SND_devices_map_mutex);

	char c_channel = name_to_search[name_to_search.size() - 1];
	unsigned int i_channel = ((unsigned int)(c_channel - 'a'));

	char AudioEndpoint_GUID[CORESIP_MAX_SOUND_NAME_LENGTH];

	CORESIP_SndWindowsDevices Devices;
	unsigned int ndevs = 0;
	GetWindowsSoundDeviceNames(0, &Devices, PJ_TRUE, &ndevs);
	for (unsigned int i = 0; i < ndevs; i++)
	{
		char DeviceInterface_FriendlyName[CORESIP_MAX_SOUND_NAME_LENGTH];
		GetSoundDevicePropeties(0, i, DeviceInterface_FriendlyName, sizeof(DeviceInterface_FriendlyName),
			AudioEndpoint_GUID, sizeof(AudioEndpoint_GUID));

		//Windows pone un numero, un guion y un espacio delante del nombre del dispositivo cuando se cambia de USB
		//Por tanto esos caracteres los ignoramos.
		unsigned int friendy_index = IndexIgnoringNamePrefix(DeviceInterface_FriendlyName);

		//Comparamos eliminando los dos ultimos caracteres que son el espacio y 'a' o 'b', que indican el canal.
		if (strnicmp(&DeviceInterface_FriendlyName[friendy_index], name_to_search.c_str(), name_to_search.size() - 2) == 0)
		{
			float scalar_volume;
			float winaudio_volume_scalar;

			switch (dev)
			{
			case CORESIP_SND_INSTRUCTOR_MHP:
				winaudio_volume_scalar = WinAudioConfig.CascoAyudante_scalar;
				break;
			case CORESIP_SND_ALUMN_MHP:
				winaudio_volume_scalar = WinAudioConfig.CascoEjecutivo_scalar;
				break;
			case CORESIP_SND_LC_SPEAKER:
				winaudio_volume_scalar = WinAudioConfig.AltavozLC_scalar;
				break;
			case CORESIP_SND_RD_SPEAKER:
				winaudio_volume_scalar = WinAudioConfig.AltavozRadioVHF_scalar;
				break;
			case CORESIP_SND_HF_SPEAKER:
				winaudio_volume_scalar = WinAudioConfig.AltavozRadioHF_scalar;
				break;			
			}
			
			int st = GetVolumeDevByItem(0, i, i_channel, scalar, &scalar_volume);
			if (st == 0)
			{
				float fvolume = (scalar_volume * 100.0f) / winaudio_volume_scalar;
				*volume = (unsigned int)fvolume;

				DEVHW_LOG(5, (__FILE__, "INFO: GetVolumeOutputDevice: dev %d, Name %s volume %u",
					dev, DeviceInterface_FriendlyName, *volume));
			}
			else
			{
				DEVHW_LOG(3, (__FILE__, "ERROR: GetVolumeOutputDevice: GetVolumeDevByItem"));
				return PJ_EINVALIDOP;
			}
		}
	}
	return PJ_SUCCESS;
}

/*
	Retorna el ïndice del string donde empieza el nombre del dispositivo, ignorando el numero y guion que pone Windows delante 
	@param DevWinName. Friendly name del insterfaz de windows
	@return. Indice del string ignorando el numero y guion que añade windows delante
*/
unsigned int SoundDevHw::IndexIgnoringNamePrefix(const char* DevWinName)
{
	//Windows pone un numero, un guion y un espacio delante del nombre del dispositivo cuando se cambia de USB
	//Por tanto esos caracteres los ignoramos.
	unsigned int friendy_index = 0;
	while (DevWinName[friendy_index] >= '0' && DevWinName[friendy_index] <= '9')
	{
		//Se salta los primeros caracteres numericos
		friendy_index++;
		if (DevWinName[friendy_index] == '\0')
		{
			friendy_index = 0;
			break;
		}
	}
	if (friendy_index > 0)
	{
		//Si ha encontrado numeros al principio del nombre, comprueba que los dos siguientes caracteres son el guion y un espacio
		if (DevWinName[friendy_index] == '-' && DevWinName[friendy_index + 1] == ' ')
		{
			friendy_index += 2;
		}
		else
		{
			friendy_index = 0;
		}
	}
	return friendy_index;
}

/*
	Esta funcion establece los niveles volumen de los distintos dispositivos de la IAO que se actualizaran periodicamente.
	Esto se hara en los dispositivos que no se han establecido para ser controlados manualmente con los botones de
	volumen del HMI. Es decir, los que no han sido establecido con la funcion SetSNDDeviceWindowsName.
	Sirve para sustituir al servicio WinAudio	
	@return.
*/
void SoundDevHw::IniWinAudio()
{	
	if (!IMM_Initialized)
	{
		PJ_CHECK_STATUS(PJ_EINVALIDOP, ("WARNING: IniWinAudio. IMM no inicializado. No es problema si no se trata del HMI del TA"));
	}

	ReadiniFile();

	//Estos valores los iniciamos a una valor negativo no valido
	WinAudioConfig.CascoEjecutivo_scalar = -1.0f;
	WinAudioConfig.CascoAyudante_scalar = -1.0f;
	WinAudioConfig.AltavozRadioHF_scalar = -1.0f;
	WinAudioConfig.AltavozRadioVHF_scalar = -1.0f;
	WinAudioConfig.AltavozLC_scalar = -1.0f;

	if (Winaudio_timer.id == 1)
	{
		pjsua_cancel_timer(&Winaudio_timer);
	}
	pj_timer_entry_init(&Winaudio_timer, 0, NULL, Winaudio_timer_cb);
	pj_time_val	delay;
	delay.msec = 250;			//La primera actualizacion del volumen se realiza cuanto antes.
	delay.sec = (long) 0;	
	Winaudio_timer.id = 1;
	pj_status_t st = pjsua_schedule_timer(&Winaudio_timer, &delay);
	if (st != PJ_SUCCESS)
	{
		Winaudio_timer.id = 0;
		PJ_CHECK_STATUS(st, ("ERROR: SoundDevHw::IniWinAudio pjsua_schedule_timer Winaudio_timer"));
	}	
}

void SoundDevHw::FinWinAudio()
{
	if (Winaudio_timer.id == 1)
	{
		pjsua_cancel_timer(&Winaudio_timer);
	}
}

/*
	Esta funcion establece periodicamente los niveles de los dispositivos cuyo nivel no se establece manualmente
	@return. 
*/
void SoundDevHw::Winaudio_timer_cb(pj_timer_heap_t* th, pj_timer_entry* te)
{
	pjsua_cancel_timer(&Winaudio_timer);
	Winaudio_timer.id = 0;

	CORESIP_SndWindowsDevices Capture_Devices;
	unsigned int capture_ndevs = 0;
	try
	{
		GetWindowsSoundDeviceNames(1, &Capture_Devices, PJ_FALSE, &capture_ndevs);
	}
	catch (...)
	{
		DEVHW_LOG(3, (__FILE__, "ERROR: SoundDevHw::Winaudio_timer_cb GetWindowsSoundDeviceNames 1 returns error"));
		capture_ndevs = 0;
	}
	CORESIP_SndWindowsDevices Render_Devices;
	unsigned int render_ndevs = 0;
	try
	{
		GetWindowsSoundDeviceNames(0, &Render_Devices, PJ_FALSE, &render_ndevs);		
	}
	catch (...)
	{
		DEVHW_LOG(3, (__FILE__, "ERROR: SoundDevHw::Winaudio_timer_cb GetWindowsSoundDeviceNames 0 returns error"));
		render_ndevs = 0;
	}
	std::string capture_devnames = Capture_Devices.DeviceNames;
	std::string render_devnames = Render_Devices.DeviceNames;		

	/*Se establece volumen de los dispositivos de salida (render)*/
	for (unsigned int i = 0; i < render_ndevs; i++)
	{
		char DeviceInterface_FriendlyName[CORESIP_MAX_SOUND_NAME_LENGTH];
		char AudioEndpoint_GUID[CORESIP_MAX_SOUND_NAME_LENGTH];
		try
		{
			GetSoundDevicePropeties(0, i, DeviceInterface_FriendlyName, sizeof(DeviceInterface_FriendlyName),
				AudioEndpoint_GUID, sizeof(AudioEndpoint_GUID));
		}
		catch (...)
		{
			DEVHW_LOG(3, (__FILE__, "ERROR: SoundDevHw::Winaudio_timer_cb GetSoundDevicePropeties 0 returns error"));
			continue;
		}

		//Windows pone un numero, un guion y un espacio delante del nombre del dispositivo cuando se cambia de USB
		//Por tanto esos caracteres los ignoramos.
		unsigned int friendy_index = IndexIgnoringNamePrefix(DeviceInterface_FriendlyName);

		if (stricmp(&DeviceInterface_FriendlyName[friendy_index], "CWP USB Device # 01") == 0)
		{
			/*Channel 0 corresponde con GrabacionEjecutivo*/
			float volume = WinAudioConfig.GrabacionEjecutivo;
			VolumeDevByItem(0, i, 0, dB, volume, NULL, NULL, NULL, NULL, NULL);

			/*Channel 1 corresponde con CascoEjecutivo*/
			pj_bool_t found = PJ_FALSE;
			pj_mutex_lock(SND_devices_map_mutex);
			if (SND_devices_map.find(CORESIP_SND_INSTRUCTOR_MHP) != SND_devices_map.end())
			{
				if (SND_devices_map[CORESIP_SND_INSTRUCTOR_MHP].compare(&DeviceInterface_FriendlyName[friendy_index]) != 0)
				{
					found = PJ_TRUE;
				}
			}
			pj_mutex_unlock(SND_devices_map_mutex);

			if (!found)
			{
				//El volumen del dispositivo no se controla con la api de Windows. Lo establecemos aqui.
				float volume = WinAudioConfig.CascoEjecutivo;
				VolumeDevByItem(0, i, 1, dB, volume, NULL, NULL, NULL, NULL, NULL);
			}
			else if (WinAudioConfig.CascoEjecutivo_scalar < 0.0f)
			{
				//Obtenemos el volumen maximo en escalar
				float current_volume_DB_prev;
				//Obtenemos primero el valor actual
				VolumeDevByItem(0, i, 1, only_get_volume_range, WinAudioConfig.CascoEjecutivo, NULL, NULL, NULL, NULL, &current_volume_DB_prev);

				//Ponemos el volumen del winaudio.ini
				VolumeDevByItem(0, i, 1, dB, WinAudioConfig.CascoEjecutivo, NULL, NULL, NULL, &WinAudioConfig.CascoEjecutivo_scalar, NULL);

				//Ponemos el volumen previo para dejarlo igual
				VolumeDevByItem(0, i, 1, dB, current_volume_DB_prev, NULL, NULL, NULL, NULL, NULL);
			}
		}

		if (stricmp(&DeviceInterface_FriendlyName[friendy_index], "CWP USB Device # 02") == 0)
		{
			/*Channel 0 corresponde con GrabacionAyudante*/
			float volume = WinAudioConfig.GrabacionAyudante;
			VolumeDevByItem(0, i, 0, dB, volume, NULL, NULL, NULL, NULL, NULL);

			/*Channel 1 corresponde con CascoAyudante*/
			pj_bool_t found = PJ_FALSE;
			pj_mutex_lock(SND_devices_map_mutex);
			if (SND_devices_map.find(CORESIP_SND_ALUMN_MHP) != SND_devices_map.end())
			{
				if (SND_devices_map[CORESIP_SND_ALUMN_MHP].compare(&DeviceInterface_FriendlyName[friendy_index]) != 0)
				{
					found = PJ_TRUE;
				}
			}
			pj_mutex_unlock(SND_devices_map_mutex);
			if (!found)
			{
				//El volumen del dispositivo no se controla con la api de Windows. Lo establecemos aqui.
				float volume = WinAudioConfig.CascoAyudante;
				VolumeDevByItem(0, i, 1, dB, volume, NULL, NULL, NULL, NULL, NULL);
			}
			else if (WinAudioConfig.CascoAyudante_scalar < 0.0f)
			{
				//Obtenemos el volumen maximo en escalar
				float current_volume_DB_prev;
				//Obtenemos primero el valor actual
				VolumeDevByItem(0, i, 1, only_get_volume_range, WinAudioConfig.CascoAyudante, NULL, NULL, NULL, NULL, &current_volume_DB_prev);

				//Ponemos el volumen del winaudio.ini
				VolumeDevByItem(0, i, 1, dB, WinAudioConfig.CascoAyudante, NULL, NULL, NULL, &WinAudioConfig.CascoAyudante_scalar, NULL);

				//Ponemos el volumen previo para dejarlo igual
				VolumeDevByItem(0, i, 1, dB, current_volume_DB_prev, NULL, NULL, NULL, NULL, NULL);
			}
		}

		if (stricmp(&DeviceInterface_FriendlyName[friendy_index], "CWP USB Device # 03") == 0)
		{
			/*Channel 0 corresponde con AltavozRadioHF*/
			pj_bool_t found = PJ_FALSE;
			pj_mutex_lock(SND_devices_map_mutex);
			if (SND_devices_map.find(CORESIP_SND_HF_SPEAKER) != SND_devices_map.end())
			{
				if (SND_devices_map[CORESIP_SND_HF_SPEAKER].compare(&DeviceInterface_FriendlyName[friendy_index]) != 0)
				{
					found = PJ_TRUE;
				}
			}
			pj_mutex_unlock(SND_devices_map_mutex);
			if (!found)
			{
				//El volumen del dispositivo no se controla con la api de Windows. Lo establecemos aqui.
				float volume = WinAudioConfig.AltavozRadioHF;
				VolumeDevByItem(0, i, 0, dB, volume, NULL, NULL, NULL, NULL, NULL);
			}
			else if (WinAudioConfig.AltavozRadioHF_scalar < 0.0f)
			{
				//Obtenemos el volumen maximo en escalar
				float current_volume_DB_prev;
				//Obtenemos primero el valor actual
				VolumeDevByItem(0, i, 1, only_get_volume_range, WinAudioConfig.AltavozRadioHF, NULL, NULL, NULL, NULL, &current_volume_DB_prev);

				//Ponemos el volumen del winaudio.ini
				VolumeDevByItem(0, i, 1, dB, WinAudioConfig.AltavozRadioHF, NULL, NULL, NULL, &WinAudioConfig.AltavozRadioHF_scalar, NULL);

				//Ponemos el volumen previo para dejarlo igual
				VolumeDevByItem(0, i, 1, dB, current_volume_DB_prev, NULL, NULL, NULL, NULL, NULL);
			}

			/*Channel 1 corresponde con AltavozRadioVHF*/
			found = PJ_FALSE;
			pj_mutex_lock(SND_devices_map_mutex);
			if (SND_devices_map.find(CORESIP_SND_RD_SPEAKER) != SND_devices_map.end())
			{
				if (SND_devices_map[CORESIP_SND_RD_SPEAKER].compare(&DeviceInterface_FriendlyName[friendy_index]) != 0)
				{
					found = PJ_TRUE;
				}
			}
			pj_mutex_unlock(SND_devices_map_mutex);
			if (!found)
			{
				//El volumen del dispositivo no se controla con la api de Windows. Lo establecemos aqui.
				float volume = WinAudioConfig.AltavozRadioVHF;
				VolumeDevByItem(0, i, 1, dB, volume, NULL, NULL, NULL, NULL, NULL);
			}
			else if (WinAudioConfig.AltavozRadioVHF_scalar < 0.0f)
			{
				//Obtenemos el volumen maximo en escalar
				float current_volume_DB_prev;
				//Obtenemos primero el valor actual
				VolumeDevByItem(0, i, 1, only_get_volume_range, WinAudioConfig.AltavozRadioVHF, NULL, NULL, NULL, NULL, &current_volume_DB_prev);

				//Ponemos el volumen del winaudio.ini
				VolumeDevByItem(0, i, 1, dB, WinAudioConfig.AltavozRadioVHF, NULL, NULL, NULL, &WinAudioConfig.AltavozRadioVHF_scalar, NULL);

				//Ponemos el volumen previo para dejarlo igual
				VolumeDevByItem(0, i, 1, dB, current_volume_DB_prev, NULL, NULL, NULL, NULL, NULL);
			}
		}

		if (stricmp(&DeviceInterface_FriendlyName[friendy_index], "CWP USB Device # 04") == 0)
		{
			/*Channel 0 corresponde con GrabacionIntegrada*/
			float volume = WinAudioConfig.GrabacionIntegrada;
			VolumeDevByItem(0, i, 0, dB, volume, NULL, NULL, NULL, NULL, NULL);

			/*Channel 1 corresponde con AltavozLC*/
			pj_bool_t found = PJ_FALSE;
			pj_mutex_lock(SND_devices_map_mutex);
			if (SND_devices_map.find(CORESIP_SND_LC_SPEAKER) != SND_devices_map.end())
			{
				if (SND_devices_map[CORESIP_SND_LC_SPEAKER].compare(&DeviceInterface_FriendlyName[friendy_index]) != 0)
				{
					found = PJ_TRUE;						
				}
			}
			pj_mutex_unlock(SND_devices_map_mutex);
			if (!found)
			{
				//El volumen del dispositivo no se controla con la api de Windows. Lo establecemos aqui.
				float volume = WinAudioConfig.AltavozLC;
				VolumeDevByItem(0, i, 1, dB, volume, NULL, NULL, NULL, NULL, NULL);
			}
			else if (WinAudioConfig.AltavozLC_scalar < 0.0f)
			{
				//Obtenemos el volumen maximo en escalar
				float current_volume_DB_prev;
				//Obtenemos primero el valor actual
				VolumeDevByItem(0, i, 1, only_get_volume_range, WinAudioConfig.AltavozLC, NULL, NULL, NULL, NULL, &current_volume_DB_prev);

				//Ponemos el volumen del winaudio.ini
				VolumeDevByItem(0, i, 1, dB, WinAudioConfig.AltavozLC, NULL, NULL, NULL, &WinAudioConfig.AltavozLC_scalar, NULL);

				//Ponemos el volumen previo para dejarlo igual
				VolumeDevByItem(0, i, 1, dB, current_volume_DB_prev, NULL, NULL, NULL, NULL, NULL);
			}
		}
	}

	/*Se establece volumen de los dispositivos de entrada (capture)*/
	for (unsigned int i = 0; i < capture_ndevs; i++)
	{
		char DeviceInterface_FriendlyName[CORESIP_MAX_SOUND_NAME_LENGTH];
		char AudioEndpoint_GUID[CORESIP_MAX_SOUND_NAME_LENGTH];
		try
		{
			GetSoundDevicePropeties(1, i, DeviceInterface_FriendlyName, sizeof(DeviceInterface_FriendlyName),
				AudioEndpoint_GUID, sizeof(AudioEndpoint_GUID));
		}
		catch (...)
		{
			DEVHW_LOG(3, (__FILE__, "ERROR: SoundDevHw::Winaudio_timer_cb GetSoundDevicePropeties 1 returns error"));
			continue;
		}

		//Windows pone un numero, un guion y un espacio delante del nombre del dispositivo cuando se cambia de USB
		//Por tanto esos caracteres los ignoramos.
		unsigned int friendy_index = IndexIgnoringNamePrefix(DeviceInterface_FriendlyName);

		if (stricmp(&DeviceInterface_FriendlyName[friendy_index], "CWP USB Device # 01") == 0)
		{
			/*Channel 0 corresponde con RetornoEjecutivo*/
			float volume = WinAudioConfig.RetornoEjecutivo;
			VolumeDevByItem(1, i, 0, dB, volume, NULL, NULL, NULL, NULL, NULL);

			/*Channel 1 corresponde con MicrofonoEjecutivo*/
			volume = WinAudioConfig.MicrofonoEjecutivo;
			VolumeDevByItem(1, i, 1, dB, volume, NULL, NULL, NULL, NULL, NULL);
		}

		if (stricmp(&DeviceInterface_FriendlyName[friendy_index], "CWP USB Device # 02") == 0)
		{
			/*Channel 0 corresponde con RetornoAyudante*/
			float volume = WinAudioConfig.RetornoAyudante;
			VolumeDevByItem(1, i, 0, dB, volume, NULL, NULL, NULL, NULL, NULL);

			/*Channel 1 corresponde con MicrofonoAyudante*/
			volume = WinAudioConfig.MicrofonoAyudante;
			VolumeDevByItem(1, i, 1, dB, volume, NULL, NULL, NULL, NULL, NULL);
		}

		if (stricmp(&DeviceInterface_FriendlyName[friendy_index], "CWP USB Device # 03") == 0)
		{
			/*Channel 0 corresponde con RetornoAltavozRadioHF*/
			float volume = WinAudioConfig.RetornoAltavozRadioHF;
			VolumeDevByItem(1, i, 0, dB, volume, NULL, NULL, NULL, NULL, NULL);

			/*Channel 1 corresponde con RetornoAltavozRadioVHF*/
			volume = WinAudioConfig.RetornoAltavozRadioVHF;
			VolumeDevByItem(1, i, 1, dB, volume, NULL, NULL, NULL, NULL, NULL);
		}

		if (stricmp(&DeviceInterface_FriendlyName[friendy_index], "CWP USB Device # 04") == 0)
		{
			/*Channel 0 corresponde con Libre*/
			float volume = WinAudioConfig.Libre;
			VolumeDevByItem(1, i, 0, dB, volume, NULL, NULL, NULL, NULL, NULL);

			/*Channel 1 corresponde con RetornoAltavozLC*/
			volume = WinAudioConfig.RetornoAltavozLC;
			VolumeDevByItem(1, i, 1, dB, volume, NULL, NULL, NULL, NULL, NULL);
		}
	}

	if (WinAudioConfig.Period_update_volume > 0)
	{
		pj_time_val	delay;
		delay.msec = 0;
		delay.sec = (long)WinAudioConfig.Period_update_volume;
		Winaudio_timer.id = 1;
		pj_status_t st = pjsua_schedule_timer(&Winaudio_timer, &delay);
		if (st != PJ_SUCCESS)
		{
			Winaudio_timer.id = 0;
		}
	}
}

float SoundDevHw::MyStrToFloat(char *mystring)
{
	for (unsigned int i = 0; i < strlen(mystring); i++)
	{
		if (mystring[i] == ',') mystring[i] = '.';
	}
	double r = strtod(mystring, NULL);
	if (r == HUGE_VAL || r == -HUGE_VAL) r = 0.0f;
	return (float)r;
}

void SoundDevHw::ReadiniFile()
{
	//Lee el fichero ini y rellena la structura Coresip_Local_Config
	char curdir[256];
	char inipath[512];
	inipath[0] = '\0';

	if (GetCurrentDirectory(sizeof(curdir), curdir) > 0)
	{
		strcpy(inipath, curdir);
		strcat(inipath, "\\WINAUDIO\\winaudio.ini");
	}

	WinAudioConfig.Period_update_volume = GetPrivateProfileInt("GENERAL", "TSUP", 0, inipath);

	DWORD nsize;
	char svalue[16];
	nsize = GetPrivateProfileString("SALIDAS", "CascoEjecutivo", "-18,1", svalue, sizeof(svalue), inipath);
	WinAudioConfig.CascoEjecutivo = MyStrToFloat(svalue);
	nsize = GetPrivateProfileString("SALIDAS", "CascoAyudante", "-18,1", svalue, sizeof(svalue), inipath);
	WinAudioConfig.CascoAyudante = MyStrToFloat(svalue);
	nsize = GetPrivateProfileString("SALIDAS", "AltavozRadioVHF", "-5,1", svalue, sizeof(svalue), inipath);
	WinAudioConfig.AltavozRadioVHF = MyStrToFloat(svalue);
	nsize = GetPrivateProfileString("SALIDAS", "AltavozLC", "-5,1", svalue, sizeof(svalue), inipath);
	WinAudioConfig.AltavozLC = MyStrToFloat(svalue);
	nsize = GetPrivateProfileString("SALIDAS", "AltavozRadioHF", "-3,1", svalue, sizeof(svalue), inipath);
	WinAudioConfig.AltavozRadioHF = MyStrToFloat(svalue);
	nsize = GetPrivateProfileString("SALIDAS", "GrabacionEjecutivo", "-9,1", svalue, sizeof(svalue), inipath);
	WinAudioConfig.GrabacionEjecutivo = MyStrToFloat(svalue);
	nsize = GetPrivateProfileString("SALIDAS", "GrabacionAyudante", "-9,1", svalue, sizeof(svalue), inipath);
	WinAudioConfig.GrabacionAyudante = MyStrToFloat(svalue);
	nsize = GetPrivateProfileString("SALIDAS", "GrabacionIntegrada", "-9,1", svalue, sizeof(svalue), inipath);
	WinAudioConfig.GrabacionIntegrada = MyStrToFloat(svalue);

	nsize = GetPrivateProfileString("ENTRADAS", "MicrofonoEjecutivo", "16", svalue, sizeof(svalue), inipath);
	WinAudioConfig.MicrofonoEjecutivo = MyStrToFloat(svalue);
	nsize = GetPrivateProfileString("ENTRADAS", "MicrofonoAyudante", "16", svalue, sizeof(svalue), inipath);
	WinAudioConfig.MicrofonoAyudante = MyStrToFloat(svalue);
	nsize = GetPrivateProfileString("ENTRADAS", "RetornoEjecutivo", "13,4", svalue, sizeof(svalue), inipath);
	WinAudioConfig.RetornoEjecutivo = MyStrToFloat(svalue);
	nsize = GetPrivateProfileString("ENTRADAS", "RetornoAyudante", "13,4", svalue, sizeof(svalue), inipath);
	WinAudioConfig.RetornoAyudante = MyStrToFloat(svalue);
	nsize = GetPrivateProfileString("ENTRADAS", "RetornoAltavozRadioVHF", "13,4", svalue, sizeof(svalue), inipath);
	WinAudioConfig.RetornoAltavozRadioVHF = MyStrToFloat(svalue);
	nsize = GetPrivateProfileString("ENTRADAS", "RetornoAltavozRadioHF", "13,4", svalue, sizeof(svalue), inipath);
	WinAudioConfig.RetornoAltavozRadioHF = MyStrToFloat(svalue);
	nsize = GetPrivateProfileString("ENTRADAS", "RetornoAltavozLC", "13,4", svalue, sizeof(svalue), inipath);
	WinAudioConfig.RetornoAltavozLC = MyStrToFloat(svalue);		

	WinAudioConfig.Libre = 0.0f;
}





	



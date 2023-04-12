/**
 * @file SoundDevHw.h
 * @brief Gestiona los dispositivos de sonido de Windows.
 *
 *	@addtogroup CORESIP
 */
 /*@{*/

#ifndef __CORESIP_SOUNDDEVHW_H__
#define __CORESIP_SOUNDDEVHW_H__

#include <windows.h>
#include <initguid.h>
#include <Mmdeviceapi.h>
#include <endpointvolume.h>
#include <map>

/*Estructura que configura el hilo WinAudio, para establecer periódicamente los volumenes de los dispositivos*/
typedef struct CORESIP_WinAudioConfig
{
	unsigned int Period_update_volume;	//Periodo en segundos al que se actualiza el volumen de cada dispositivo.
										//Si el valor es cero, entonces los volumenes se actualizan solo an inicio
	//Volumen en dB de los dispositivos de salida. Rango -62.0 a 0.0 dB
	float GrabacionEjecutivo;
	float CascoEjecutivo;
	float GrabacionAyudante;
	float CascoAyudante;
	float AltavozRadioHF;
	float AltavozRadioVHF;
	float GrabacionIntegrada;
	float AltavozLC;

	//Volumen convertido a scalar
	float CascoEjecutivo_scalar;
	float CascoAyudante_scalar;
	float AltavozRadioHF_scalar;
	float AltavozRadioVHF_scalar;
	float AltavozLC_scalar;

	//Volumen en dB de los dispositivos de entrada. Rango 0.0 a 30.0 dB
	float RetornoEjecutivo;
	float MicrofonoEjecutivo;
	float RetornoAyudante;
	float MicrofonoAyudante;
	float RetornoAltavozRadioHF;
	float RetornoAltavozRadioVHF;
	float Libre;
	float RetornoAltavozLC;
} CORESIP_WinAudioConfig;

class SoundDevHw
{
private:
	static unsigned int IndexIgnoringNamePrefix(const char* DevWinName);

	static pj_pool_t* _Pool;
	static pj_bool_t IMM_Initialized;
	static std::map<CORESIP_SndDevType, std::string> SND_devices_map;	
	static pj_mutex_t* SND_devices_map_mutex;

	static pj_timer_entry Winaudio_timer;
	static void Winaudio_timer_cb(pj_timer_heap_t* th, pj_timer_entry* te);

	static CORESIP_WinAudioConfig WinAudioConfig;

	static void IniWinAudio();
	static void FinWinAudio();
	static void ReadiniFile();	
	static float MyStrToFloat(char* mystring);

	static IMMDeviceEnumerator* devicesEnumerator;
	static IMMDeviceCollection* capture_immdevcollection;
	static IMMDeviceCollection* render_immdevcollection;

public:	

	enum volume_type
	{
		scalar,
		dB,
		only_get_volume_range
	};

	static const float MinVolumeRenderDBs;
	static const float MaxVolumeRenderDBs;
	static const float MinVolumeCaptureDBs;
	static const float MaxVolumeCaptureDBs;

	static char splitter[];

	static void Init();
	static void Dispose();
	static pj_status_t InitIMM();
	static pj_status_t FinIMM();
	static void SetSNDDeviceWindowsName(CORESIP_SndDevType UlisesDev, const char* DevWinName);
	static void GetWindowsSoundDeviceNames(int captureType, CORESIP_SndWindowsDevices* Devices, pj_bool_t only_nedevices, unsigned int* ndevs);
	static pj_bool_t SetVolumeToListenerSndPort(pjsua_conf_port_id conf_port_id, unsigned volume);
	static void GetSoundDevicePropeties(int captureType, unsigned int devIndex, char* DeviceInterface_FriendlyName, const unsigned int DeviceInterface_FriendlyName_size,
		char* AudioEndpoint_GUID, const unsigned int AudioEndpoint_GUID_size);
	static pj_status_t SetVolumeOutputDevice(CORESIP_SndDevType dev, unsigned int volume);
	static pj_status_t GetVolumeOutputDevice(CORESIP_SndDevType dev, unsigned int* volume);
	static int VolumeDevByItem(int captureType, unsigned int item, unsigned int channel, SoundDevHw::volume_type voltype, float volume,
		float* flVolumeMindB, float* flVolumeMaxdB, float* flVolumeIncrementdB,
		float* current_volume_scalar, float* current_volume_dB);
	static int GetVolumeDevByItem(int captureType, unsigned int item, unsigned int channel, SoundDevHw::volume_type voltype, float* volume);
};

#endif

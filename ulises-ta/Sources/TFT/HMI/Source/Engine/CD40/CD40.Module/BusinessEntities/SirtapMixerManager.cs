using System;
using System.Collections.Generic;
using System.Text;
using System.Diagnostics;
using System.Timers;

using HMI.Model.Module.BusinessEntities;
using HMI.Model.Module.Messages;
using HMI.CD40.Module.Properties;
using U5ki.Infrastructure;
using Utilities;

using NLog;
using System.Runtime.InteropServices;

namespace HMI.CD40.Module.BusinessEntities
{
    /// Identificadores para la gestión de la Mezcla en puesto.
    public enum MixerDev
    {
        MhpTlf,                     // Microcasco + Mic en Telefonía
        MhpLc,                      // Microcasco + Mic en LC
        MhpRd,                      // Microcasco + Mic en Radio.
        SpkLc,                      // Altavoz de LC => Se debe cambiar en el tratamiento por MhpLc y revisar las prioridades.
        SpkRd,                      // Altavoz Radio => Se debe cambiar en el tratamienot por MhpRd y revisar las prioridades.
        SpkHf,                      // Altavoz HF = Se debería eliminar
        Ring,                       // Señal de RING.
        Aural,                      // Alarmas aurales.
        Invalid
    }
    /// Identificadores de los tipos de enlaces programados en el mezclador.
    public enum MixerDir
    {
        Send,                       // IN --> OUT Altavoz rx
        Recv,                       // IN <-- OUT micro tx
        SendRecv                    // IN <-> OUT
    }

    public enum TlfRxAudioVia
    {
        NoAudio,
        HeadPhones,
        Speaker,
    }
    public interface ITopPlatform
    {
        event GenericEventHandler<JacksStateMsg> JacksChangedHw;
        event GenericEventHandler ActivityChanged;
        event GenericEventHandler<JacksStateMsg> SpeakerExtChangedHw;
        event GenericEventHandler<JacksStateMsg> SpeakerChangedHw;
        event GenericEventHandler<ListenPickUpMsg> ListenChanged;
        event GenericEventHandler<bool> HangToneChanged;
        event GenericEventHandler<bool> UnhangToneChanged;
        void ConnectEvents();

        void LoadAudioInputsOutputsAvailables();
        int ActivateAudioDevice(bool _enabled, CORESIP_SndDevType tipo);
        void AsignAudioDevice(CORESIP_SndDevType tipo, string device);
        string GetDeviceName(int indice, string mascara = null);
        void SetVolume(int id, int volume);
        void SetVolume(CORESIP_SndDevType id, int volume);

        bool TphActivity(bool extended=false);
        bool RadioActivity();
        bool IAActivity();
    }
    public class RealTopPlatform : ITopPlatform
    {
        public event GenericEventHandler<JacksStateMsg> JacksChangedHw;
        public event GenericEventHandler ActivityChanged;
        public event GenericEventHandler<JacksStateMsg> SpeakerExtChangedHw;
        public event GenericEventHandler<JacksStateMsg> SpeakerChangedHw;
        public event GenericEventHandler<ListenPickUpMsg> ListenChanged;
        public event GenericEventHandler<bool> HangToneChanged;
        public event GenericEventHandler<bool> UnhangToneChanged;

        public void ConnectEvents()
        {
            //Top.Hw.JacksChangedHw -= OnJacksChanged;
            //Top.Tlf.ActivityChanged -= OnTlfActivityChanged;

            Top.Hw.JacksChangedHw += (origin, data) => { JacksChangedHw?.Invoke(origin, data); };
            Top.Tlf.ActivityChanged += (origin) => { ActivityChanged?.Invoke(origin); };
            Top.Hw.SpeakerExtChangedHw += (origin, data) => { SpeakerExtChangedHw?.Invoke(origin, data); };
            Top.Hw.SpeakerChangedHw += (origin, data) => { SpeakerChangedHw?.Invoke(origin, data); }; 
            Top.Tlf.Listen.ListenChanged += (origin, data) => { ListenChanged?.Invoke(origin, data); }; 
            Top.Tlf.HangToneChanged += (origin, data) => { HangToneChanged?.Invoke(origin, data); }; 
            Top.Tlf.UnhangToneChanged += (origin, data) => { UnhangToneChanged?.Invoke(origin, data); }; 
        }
        public bool IAActivity()
        {
            throw new NotImplementedException();
        }
        public bool RadioActivity()
        {
            throw new NotImplementedException();
        }
        public bool TphActivity(bool extended = false)
        {
            if (extended)
            {
                return (Top.Tlf.Listen.State == FunctionState.Executing || Top.Tlf.Activity() || Top.Tlf.ToneOn);
            }
            return Top.Tlf.Activity();
        }
        public void LoadAudioInputsOutputsAvailables()
        {
            HidGenericHwManager.LoadChannels();
        }
        public int ActivateAudioDevice(bool _enabled, CORESIP_SndDevType tipo)
        {
            return HidGenericHwManager.AddDevice(_enabled, tipo);
        }
        public void AsignAudioDevice(CORESIP_SndDevType tipo, string device)
        {
            SipAgent.Asignacion(tipo, device);
        }
        public string GetDeviceName(int indice, string mascara = null)
        {
            return SipAgent.GetNameDevice(indice, mascara);
        }
        public void SetVolume(int id, int volume)
        {
            SipAgent.SetVolume(id, volume);
        }
        public void SetVolume(CORESIP_SndDevType id, int volume)
        {
            SipAgent.SetVolume((int)id, volume);
        }
    }

    public interface IMixer
    {
        void Link(int srcId, int txPriority, int dstId, int rxPriority);
        void Link(int srcId, string topId, string rdSrvListenIp, uint rdSrvListenPort);
        void Unlink(int srcId, int dstId);
        void Unlink(int id);
    }
    public class RealMixer : IMixer
    {
        private Mixer _Mixer = new Mixer();
        void IMixer.Link(int srcId, int txPriority, int dstId, int rxPriority)
        {
            _Mixer.Link(srcId, txPriority, dstId, rxPriority);
        }
        void IMixer.Link(int srcId, string topId, string rdSrvListenIp, uint rdSrvListenPort)
        {
            _Mixer.Link(srcId, topId, rdSrvListenIp, rdSrvListenPort);
        }
        void IMixer.Unlink(int srcId, int dstId)
        {
            _Mixer.Unlink(srcId, dstId);
        }
        void IMixer.Unlink(int id)
        {
            _Mixer.Unlink((int)id);
        }
    }

    public class MixerManager
    {
        public event GenericEventHandler<SplitMode> SplitModeChanged;
        public event GenericEventHandler<string> RecorderChanged;

        public int HfSpeakerDev => -1;          // No hay Altavoz HF
        public int InstructorDev => -1;         // No hay Jack de Instructor / Ayudante.
        public int AlumnDev => _HeadPhoneDevice;

        public SplitMode SplitMode => SplitMode.Off;
        public bool BuzzerEnabled => _BuzzerEnabled;
        public TlfRxAudioVia RxTlfAudioVia => TlfRxAudioVia.HeadPhones;

        public bool ModoSoloAltavoces
        {
            get { return false; }
            set
            {
                _Logger.Error($"ModoSoloAltavoces => Funcion no disponible.");
                _OnlySpeakerMode = true;
            }
        }

        // Servicios Externos.
        ITopPlatform topPlatform = null;
        IMixer mixer = null;
        public MixerManager(ITopPlatform topPlatform = null, IMixer mixer = null)
        {
            this.topPlatform = topPlatform ?? new RealTopPlatform();
            this.mixer = mixer ?? new RealMixer();
        }

        public void Init()
        {
            this.topPlatform.JacksChangedHw += OnJacksChanged;
            this.topPlatform.ActivityChanged += OnTlfActivityChanged;
            this.topPlatform.SpeakerExtChangedHw += OnHwChanged;
            this.topPlatform.SpeakerChangedHw += OnHwChanged;
            this.topPlatform.ListenChanged += OnListenChanged;
            this.topPlatform.HangToneChanged += OnTlfToneChanged;
            this.topPlatform.UnhangToneChanged += OnTlfToneChanged;
            this.topPlatform.ConnectEvents();
            //RQF20
            AsignacionAudioVolumen();
            eAudioDeviceTypes tipoAudio = HwManager.AudioDeviceType;
            switch (tipoAudio)
            {
                case eAudioDeviceTypes.GENERIC_PTT:
                    topPlatform.LoadAudioInputsOutputsAvailables();

                    _HeadPhoneDevice = topPlatform.ActivateAudioDevice(Settings.Default.AlumnMHP, CORESIP_SndDevType.CORESIP_SND_ALUMN_MHP);
                    _AuralSpeakerDevice = topPlatform.ActivateAudioDevice(Settings.Default.RdSpeaker, CORESIP_SndDevType.CORESIP_SND_RD_SPEAKER);
                    break;

                case eAudioDeviceTypes.SIMUL:
                    _HeadPhoneDevice = 1;
                    _AuralSpeakerDevice = 2;
                    break;
                default:
                    throw new ArgumentException("Dispositivos de audio desconocidos");
            }

            _RingDev = _HeadPhoneDevice >= 0 ? MixerDev.MhpTlf : MixerDev.Invalid;
            _BuzzerEnabled = _RingDev != MixerDev.Invalid;
        }

        public void Start()
        {
        }

        public void End()
        {
        }

        public bool SetSplitMode(SplitMode mode)
        {
            _Logger.Error($"Mixermanager, SetSplitMode: Modo no permido");
            throw new InvalidOperationException("Mixermanager, SetSplitMode: Modo no permido");
        }

        public bool SetBuzzerState(bool enabled)
        {
            if (Settings.Default.AudioCardSimul == true)
            {
                _BuzzerEnabled = enabled;
                return true;
            }
            else if (_RingDev != MixerDev.Invalid)
            {
                if (enabled != _BuzzerEnabled)
                {
                    _BuzzerEnabled = enabled;

                    foreach (LinkInfo link in _LinksList)
                    {
                        if (link._Dev == MixerDev.Ring)
                        {
                            if (_BuzzerEnabled)
                            {
                                ManageRingLink(link);
                            }
                            else
                            {
                                mixer.Unlink(link._CallId);
                            }
                        }
                    }
                }
                return true;
            }
            return false;
        }

        public bool SetBuzzerLevel(int level)
        {
            if (Settings.Default.AudioCardSimul == true)
            {
                _RingVolume = CalculateVolume(level);
                return true;
            }
            // LALM 210429
            // Peticiones #4810
            // Configurar la restricción de presencia de altavoz LC
            else if (Settings.Default.LcSpeakerSimul)
            {
                _RingVolume = CalculateVolume(level);
                return true;
            }
            else if (_BuzzerEnabled)
            {
                _RingVolume = CalculateVolume(level);

                foreach (LinkInfo link in _LinksList)
                {
                    if (link._Dev == MixerDev.Ring)
                    {
                        topPlatform.SetVolume(link._CallId, _RingVolume);
                    }
                }
                return true;
            }
            return false;
        }

        public bool SetTlfHeadPhonesLevel(int level)
        {
            _TlfHeadPhonesVolume = CalculateVolume(level);

            foreach (LinkInfo link in _LinksList)
            {
                if ((link._Dev == MixerDev.MhpTlf) &&
                    ((link._Dir == MixerDir.Send) || (link._Dir == MixerDir.SendRecv)))
                {
                    topPlatform.SetVolume(link._CallId, _TlfHeadPhonesVolume);
                }
            }

            return true;
        }

        public bool SetLcSpeakerLevel(int level) // TODO... 
        {
            _Logger.Error($"SetLcSpeakerLevel: El altavoz de LC no existe");
            throw new InvalidOperationException("SetLcSpeakerLevel: El altavoz de LC no existe");
        }

        public bool SetTlfSpeakerLevel(int level)
        {
            _Logger.Error($"SetTlfSpeakerLevel: El altavoz de LC no existe");
            throw new InvalidOperationException("SetTlfSpeakerLevel: El altavoz de LC no existe");
        }

        public bool SetRdHeadPhonesLevel(int level)
        {
            _RdHeadPhonesVolume = CalculateVolume(level);

            foreach (LinkInfo link in _LinksList)
            {
                if ((link._CurrentDev == MixerDev.MhpRd) &&
                    ((link._Dir == MixerDir.Send) || (link._Dir == MixerDir.SendRecv)))
                {
                    topPlatform.SetVolume(link._CallId, _RdHeadPhonesVolume);
                    //#5829 lo hago tambien sobre el dispositivo fisico.
                    topPlatform.SetVolume(CORESIP_SndDevType.CORESIP_SND_INSTRUCTOR_MHP, _RdHeadPhonesVolume);
                }
            }

            return true;
        }

        public bool SetRdSpeakerLevel(int level)
        {
            _Logger.Error($"SetRdSpeakerLevel: No existe el altavoz radio.");
            throw new InvalidOperationException("No existe el altavoz radio.");
        }

        public bool SetHfSpeakerLevel(int level)
        {
            _Logger.Error($"SetHfSpeakerLevel: No existe el altavoz radio HF");
            throw new InvalidOperationException("No existe el altavoz radio HF");
        }

        /// <summary>
        /// Para enlaces telefónicos que involucran dispositivos de audio.
        /// </summary>
        /// <param name="id"></param>
        /// <param name="dir"></param>
        /// <param name="priority"></param>
        /// <exception cref="InvalidOperationException"></exception>
        public void LinkTlf(int id, MixerDir dir, int priority)
        {
            if (_RxTlfAudioVia == TlfRxAudioVia.HeadPhones)
            {
                Link(id, MixerDev.MhpTlf, dir, priority, FuentesGlp.Telefonia);
            }
            if (_RxTlfAudioVia == TlfRxAudioVia.Speaker)
            {
                _Logger.Error($"LinkTlf: La telefonía no puede ser dirigida a un altavoz");
                throw new InvalidOperationException("La telefonía no puede ser dirigida a un altavoz");
            }
        }
        /// <summary>
        /// Para enlaces que involucran dispositivos de audio.
        /// </summary>
        /// <param name="id"></param>
        /// <param name="dev"></param>
        /// <param name="dir"></param>
        /// <param name="priority"></param>
        /// <param name="tipoFuente"></param>
        /// <exception cref="InvalidOperationException"></exception>
        public void Link(int id, MixerDev dev, MixerDir dir, int priority, FuentesGlp tipoFuente)
        {
            if (_HeadPhoneDevice < 0 || !_HeadPhoneInserted)
            {
                _Logger.Error($"Link: Microcascos Desconectados => {_HeadPhoneDevice}, {_HeadPhoneInserted}");
                throw new InvalidOperationException("Link: Microcascos Desconectados!!");
            }

            if (dir == MixerDir.SendRecv)
            {
                PrivateLink(id, dev, MixerDir.Recv, priority, tipoFuente);
                PrivateLink(id, dev, MixerDir.Send, priority, tipoFuente);
            }
            else
            {
                PrivateLink(id, dev, dir, priority, tipoFuente);
            }
        }
        /// <summary>
        /// Para enlaces que no involucran dispositivos de audio..
        /// </summary>
        /// <param name="callId1"></param>
        /// <param name="callId2"></param>
        /// <param name="dir"></param>
        /// <param name="fuente"></param>
		public void Link(int callId1, int callId2, MixerDir dir, FuentesGlp fuente)
        {
            PrivateLink(callId1, callId2, dir, Mixer.UNASSIGNED_PRIORITY);
        }
        /// <summary>
        /// Para todos los enlaces...
        /// </summary>
        /// <param name="id"></param>
        /// <param name="temporal"></param>
        public void Unlink(int id, bool temporal = false)
        {
            List<LinkInfo> listToRemove = _LinksList.FindAll(link => link._CallId == id);
            int removed = _LinksList.RemoveAll(link => link._CallId == id);

            foreach (LinkInfo info in listToRemove)
            {
                if ((info._Dev == MixerDev.MhpTlf) && ((info._Dir == MixerDir.Recv) || (info._Dir == MixerDir.SendRecv)))
                {
                    TlfLinkRemoved();
                }

                switch (info._Dev)
                {
                    //              case MixerDev.SpkRd:
                    //              case MixerDev.SpkHf:
                    //                  /* AGL.REC La grabación del altavoz radio es continua...
                    //Top.Recorder.Rec(CORESIP_SndDevType.CORESIP_SND_RADIO_RECORDER, false);
                    //                  * */
                    //                  //_UnlinkGlpRadioTimer.Enabled = true;
                    //                  // El parametro temporal es para no cortar la grabacion en algun caso
                    //                  //lalm 220225 quit el temporal
                    //                  _UnlinkGlpRadioTimer.Enabled = true;
                    //                  if (temporal == false)
                    //                  {
                    //                      // Corto  grabacion si solo hay un recurso en squelch, tiene que ser el mio.
                    //                      Top.Recorder.SessionGlp(id, FuentesGlp.RxRadio, false, false);
                    //                      Top.WorkingThread.Enqueue("RecorderChanged", delegate ()
                    //                      {
                    //                          General.SafeLaunchEvent(RecorderChanged, this, "mensaje");
                    //                      });

                    //                  }
                    //                  else
                    //                  {
                    //                      bool finalizar = false;
                    //                      Top.Recorder.SessionGlp(id, FuentesGlp.RxRadio, false, finalizar);
                    //                      Top.Recorder.SessionGlp(id, FuentesGlp.RxRadio, true, finalizar);

                    //                  }
                    //                  Top.Rd.UpdateRadioSpeakerLed();
                    //                  break;

                    //case MixerDev.SpkLc:
                    //    ManageECHandsFreeByLC();
                    //    /*- AGL.REC La grabacion del Altavoz-LC es Continua ...
                    //     BS si es grabacion unificada. 
                    //     * En el caso de la telefonía por altavoz no unificada, no es continua */
                    //    //#3267 RQF22
                    //    if (TipoGrabacionAnalogica == 0)
                    //        //if (Settings.Default.RecordMode == 0)
                    //        Top.Recorder.Rec(CORESIP_SndDevType.CORESIP_SND_LC_RECORDER, false);

                    //    Top.Recorder.SessionGlp(info._TipoFuente, false);
                    //    // 230118 Comento esto, se comprueba todo al final de la funcion
                    //    //Top.Hw.OnOffLed(CORESIP_SndDevType.CORESIP_SND_LC_SPEAKER, HwManager.OFF);
                    //    break;

                    case MixerDev.MhpRd://cascos
                        //_UnlinkGlpRadioTimer.Enabled = true;
                        //lalm 220225
                        // MhpRd es cascos
                        //_UnlinkGlpRadioTimer.Enabled = true;
                        // LALM dejo que se corte solo por squelch
                        break;

                    case MixerDev.MhpTlf:
                        //if (_InstructorDev >= 0 && info._Dir == MixerDir.SendRecv &&
                        //    (Top.Rd.PttSource != PttSource.Instructor && Top.Rd.PttSource != PttSource.Hmi))
                        //{
                        //    Top.Recorder.Rec(CORESIP_SndDevType.CORESIP_SND_INSTRUCTOR_MHP, false);
                        //    /* AGL.REC */
                        //    _InstructorMhpTlfRecInProgress = false;
                        //}
                        //if (_AlumnDev >= 0 && info._Dir == MixerDir.SendRecv &&
                        //    (Top.Rd.PttSource != PttSource.Alumn && Top.Rd.PttSource != PttSource.Hmi))
                        //{
                        //    Top.Recorder.Rec(CORESIP_SndDevType.CORESIP_SND_ALUMN_MHP, false);
                        //    /* AGL.REC */
                        //    _AlumnMhpTlfRecInProgress = false;
                        //}
                        //Top.Recorder.SessionGlp(id, FuentesGlp.Telefonia, false);
                        break;

                    case MixerDev.MhpLc:
                        //ManageECHandsFreeByLC();
                        //if (_InstructorDev >= 0 && _SplitMode == SplitMode.Off)
                        //{
                        //    /* AGL.REC. Desactivar la grabacion del microfono, solo si no hay PTT pulsado ni llamadas telefónicas en curso ... */
                        //    if (Top.Hw.HwPtt_Activated == false && _InstructorMhpTlfRecInProgress == false)
                        //        Top.Recorder.Rec(CORESIP_SndDevType.CORESIP_SND_INSTRUCTOR_MHP, false);
                        //}
                        //if (_AlumnDev >= 0)
                        //{
                        //    /* AGL.REC. Desactivar la grabacion del microfono, solo si no hay PTT pulsado ni llamadas telefónicas en curso ... */
                        //    if (Top.Hw.HwPtt_Activated == false && _AlumnMhpTlfRecInProgress == false)
                        //        Top.Recorder.Rec(CORESIP_SndDevType.CORESIP_SND_ALUMN_MHP, false);
                        //}
                        break;
                }
            }

            if (removed > 0)
            {
                mixer.Unlink(id);
                //Top.Rd.UpdateRadioSpeakerLed();
            }
            _TlfListens.Remove(id);

            ///** */
            //RingLedToOff(id);

            ////230118 el led lo hago si esta en uso el mezclador
            //List<LinkInfo> listring = _LinksList.FindAll(link => link._CallId == id_ringing);
            //List<LinkInfo> listspklc = _LinksList.FindAll(link => link._Dev == MixerDev.SpkLc);
            //if (listspklc.Count > 0 || listring.Count > 0)
            //    Top.Hw.OnOffLed(CORESIP_SndDevType.CORESIP_SND_LC_SPEAKER, HwManager.ON);
            //else
            //    Top.Hw.OnOffLed(CORESIP_SndDevType.CORESIP_SND_LC_SPEAKER, HwManager.OFF);
        }

        public void LinkSelCal(bool on, int tone)
        {
            if (on)
                mixer.Link(tone, Top.HostId, Settings.Default.RdSrvListenIp, Settings.Default.RdSrvListenPort);
            else
                mixer.Unlink(tone, Mixer.RD_REMOTE_PORT_ID);
        }
        public void LinkRdInstructorTx()
        {
            throw new InvalidOperationException("No existe el Instructor Radio!");
        }
        public void UnlinkRdInstructorTx()
        {
            throw new InvalidOperationException("No existe el Instructor Radio!");
        }
        public void LinkRdAlumnTx()
        {
            if (_HeadPhoneDevice >= 0 && _HeadPhoneInserted)
            {
                mixer.Link(_HeadPhoneDevice, Top.HostId, Settings.Default.RdSrvListenIp, Settings.Default.RdSrvListenPort);

                //if (Top.Tlf.Activity())
                //    Top.Recorder.Rec(CORESIP_SndDevType.CORESIP_SND_ALUMN_MHP, false);  // Le quito la grabación a la telefonía

                //Top.Recorder.Rec(CORESIP_SndDevType.CORESIP_SND_ALUMN_MHP, true);
            }
        }
        public void UnlinkRdAlumnTx()
        {
            if ((_HeadPhoneDevice >= 0))
            {
                mixer.Unlink(_HeadPhoneDevice, Mixer.RD_REMOTE_PORT_ID);
                //Top.Recorder.Rec(CORESIP_SndDevType.CORESIP_SND_ALUMN_MHP, false);
            }
        }
        public void LinkTlf(int id)
        {
            _TlfListens.Add(id);

            if (_TlfRxLinks > 0)
            {
                //if ((_InstructorDev >= 0) && ((_SplitMode == SplitMode.Off) || (_SplitMode == SplitMode.LcTf)))
                //{
                //    _Mixer.Link(_InstructorDev, Mixer.TLF_PRIORITY, id, Mixer.UNASSIGNED_PRIORITY);
                //}
                if ((_HeadPhoneDevice >= 0))
                {
                    mixer.Link(_HeadPhoneDevice, Mixer.TLF_PRIORITY, id, Mixer.UNASSIGNED_PRIORITY);
                }
            }

            foreach (LinkInfo link in _LinksList)
            {
                if (link._Dev == MixerDev.MhpTlf)
                {
                    if (((link._Dir == MixerDir.Send) || (link._Dir == MixerDir.SendRecv)) &&
                        (/*(_InstructorDev >= 0) || */(_HeadPhoneDevice >= 0)))
                    {
                        mixer.Link(link._CallId, Mixer.UNASSIGNED_PRIORITY, id, Mixer.UNASSIGNED_PRIORITY);
                    }
                }
                else if (link._Dev == MixerDev.Ring)
                {
                    if (_BuzzerEnabled)
                    {
                        mixer.Link(link._CallId, Mixer.UNASSIGNED_PRIORITY, id, Mixer.UNASSIGNED_PRIORITY);
                    }
                }
            }
        }

        public void LinkRecord(CORESIP_SndDevType dev, bool on)
        {
            throw new InvalidOperationException("No hay grabación analógica");
        }
        public void LinkGlpTfl(int file)
        {
            throw new InvalidOperationException("LinkGlpTfl: No existe grabacion local en el puesto");
        }
        public void LinkGlp(int file, MixerDev device, FuentesGlp tipoFuente)
        {
            throw new InvalidOperationException("LinkGlp: No existe grabacion local en el puesto");
        }
        public void LinkReplay(int file, ViaReplay via)
        {
            throw new InvalidOperationException("LinkReplay: No existe grabacion local en el puesto");
        }
        public bool AutChangeToRdSpeaker()
        {
            throw new InvalidOperationException("AutChangeToRdSpeaker: Funcion no disponible");
        }

        // TODO. Comprobar que no se llama.
        public void SetTlfAudioVia(bool speaker)
        {
            throw new InvalidOperationException("No existe altavoz para telefonía");
        }
        public bool MatchActiveLink(MixerDev dev, int id)
        {
            bool found = false;
            foreach (LinkInfo link in _LinksList)
            {
                if ((link._CallId == id) && (link._CurrentDev == dev))
                {
                    found = true;
                    break;
                }
            }
            return found;
        }
        //LALM 211029 
        //# Error 3629 Terminal de Audio -> Señalización de Actividad en LED ALTV Intercom cuando seleccionada TF en ALTV
        public bool AltavozRingCompartidoLC => true;

        //#3267 RQF22
        public bool SetTipoGrabacionAnalogica(int tipoGrabacionAnalogica)
        {
            throw new InvalidOperationException("SetTipoGrabacionAnalogica: Funcion no disponible");
        }
        //#3267 RQF22
        public void SetGrabacionAnalogica(int tipoGrabacionAnalogica, bool enable)
        {
            throw new InvalidOperationException("SetGrabacionAnalogica: Funcion no disponible");
        }

        // debe ser uno  de estos cuatro
        //CORESIP_SndDevType.CORESIP_SND_ALUMN_MHP
        //CORESIP_SndDevType.CORESIP_SND_INSTRUCTOR_MHP
        //CORESIP_SndDevType.CORESIP_SND_RD_SPEAKER
        //CORESIP_SndDevType.CORESIP_SND_LC_SPEAKER
        //#5829 Nueva funcion para ajustar los volumenes de los dispositivos fisicos, sin idcall.
        public bool SetVolumeTones(CORESIP_SndDevType dev)
        {
            if (Settings.Default.AudioCardSimul == true)
            {
                return true;
            }
            else if (Settings.Default.LcSpeakerSimul)
            {
                return true;
            }
            else if (dev == CORESIP_SndDevType.CORESIP_SND_RD_SPEAKER)
            {
                throw new InvalidOperationException("SetVolumeTones: Altavoz Radio no disponible");
            }
            else if (dev == CORESIP_SndDevType.CORESIP_SND_ALUMN_MHP)// Habria que diferenciar en que modo estamos disgregado o agrupado
            {
                // Compruebo si se esta usando el dispositivo
                bool VolumeInUse = false;
                foreach (LinkInfo link in _LinksList) if (link._Dev == MixerDev.MhpRd) VolumeInUse = true;
                if (!VolumeInUse) topPlatform.SetVolume(dev, _TonesVolumen);//#5829
                if (_BuzzerEnabled)
                {
                    foreach (LinkInfo link in _LinksList)
                    {
                        if (link._Dev == MixerDev.Ring)
                            topPlatform.SetVolume(dev, _RingVolume);//#5829

                    }
                    return true;
                }
            }
            else if (dev == CORESIP_SndDevType.CORESIP_SND_INSTRUCTOR_MHP)// Habria que diferenciar en que modo estamos disgregado o agrupado
            {
                throw new InvalidOperationException("SetVolumeTones: Jack Instructor no disponible");
            }
            else if (dev == CORESIP_SndDevType.CORESIP_SND_LC_SPEAKER)
            {
                throw new InvalidOperationException("SetVolumeTones: Altavoz LC no disponible");
            }
            return false;
        }

        // Para ajustar indendientemente
        // volumen de ring y volumen LC
        public void SetVolumeRing()
        {
            if (id_ringing > 0)
                topPlatform.SetVolume(id_ringing, _RingVolume);
        }

        public bool SpkLcInUse()
        {
            return false;
        }

        public int VolumeTones { get => _TonesVolumen; set => _TonesVolumen = value; }
        public void setringdevice(MixerDev ringdevice)
        {
            _RingDev = ringdevice;
        }
        #region Private Members

        class LinkInfo
        {
            //Dispositivo seleccionado al que conectarse
            public MixerDev _Dev;
            public MixerDir _Dir;
            public int _Priority;
            public FuentesGlp _TipoFuente;
            //Dispositivo real al que está conectado. En casos de conmutación automática por colision con tlf
            //se conecta a otro dispositivo temporalmente
            public MixerDev _CurrentDev;
            public int _CallId;

            public LinkInfo(MixerDev dev, MixerDir dir, int priority, FuentesGlp tipoFuente, int callId)
            {
                _Dev = dev;
                _Dir = dir;
                _Priority = priority;
                _TipoFuente = tipoFuente;
                _CurrentDev = dev;
                _CallId = callId;
            }
        }

        //private bool BinAural = false;
        //private int _RdSpeakerDev = -1;
        //private int _LcSpeakerDev = -1;
        //private int _HfSpeakerDev = -1;
        //private int _InstructorDev = -1;
        //private int _InstructorDev2 = -1;
        private int _HeadPhoneDevice = -1;
        private int _AuralSpeakerDevice = -1;
        //private int _AlumnDev2 = -1;
        //private int _InstructorBinauralDev = -1;
        //private int _AlumnoBinauralDev = -1;
        private MixerDev _RingDev = MixerDev.Invalid;
        private int _TlfHeadPhonesVolume = 50;
        //private int _TlfSpeakerVolume = 50;
        private int _RdHeadPhonesVolume = 50;
        //private int _LcSpeakerVolume = 50;
        //private int _RdSpeakerVolume = 50;
        //private int _HfSpeakerVolume = 50;
        private int _RingVolume = 50;
        //private bool _InstructorJack = false;
        private bool _HeadPhoneInserted = false;
        private bool _BuzzerEnabled = false;
        //private SplitMode _SplitMode = SplitMode.Off;
        //private Mixer _Mixer = new Mixer();
        private List<LinkInfo> _LinksList { get; set; } = new List<LinkInfo>();
        private List<int> _TlfListens { get; set; } = new List<int>();
        private int _TlfRxLinks { get; set; } = 0;
        private int _TonesVolumen = 50;
        private TlfRxAudioVia _RxTlfAudioVia = TlfRxAudioVia.HeadPhones;
        private bool _OnlySpeakerMode = false;
        private int id_ringing = -1;
        private List<LinkInfo> RadioMixing { get; set; } = new List<LinkInfo>();
        private List<LinkInfo> PhoneMixing { get; set; } = new List<LinkInfo>();
        private List<LinkInfo> AuralMixing { get; set; } = new List<LinkInfo>();
        private List<LinkInfo> RingMixing { get; set; } = new List<LinkInfo>();

        private static Logger _Logger = LogManager.GetCurrentClassLogger();
        /// RQF20 aqui se añadiran todos los tipos de dispositivos.
        private void SetTipoOutWindows(CORESIP_SndDevType UlisesDev, string namewindows)
        {
            topPlatform.AsignAudioDevice(UlisesDev, namewindows);
        }
        private void AsignacionAudioVolumen()
        {
            // Para el Casco
            var headPhoneId = Settings.Default.CasAlumnoId;
            var headPhoneName = topPlatform.GetDeviceName(0, headPhoneId);
            if (headPhoneName != null)
                SetTipoOutWindows(CORESIP_SndDevType.CORESIP_SND_ALUMN_MHP, headPhoneName);
            else
            {
                _Logger.Error($"AsignacionAudioVolumen. No se localiza el dispositivo asociado al CASCO => {headPhoneId}");
            }
            // Para el Altavoz de Alarmas.
            var auralSpeakerId = Settings.Default.RdSpkWindowsId;
            var auralSpeakerName = topPlatform.GetDeviceName(0, auralSpeakerId);
            if (auralSpeakerName != null)
                SetTipoOutWindows(CORESIP_SndDevType.CORESIP_SND_RD_SPEAKER, auralSpeakerName);
            else
            {
                _Logger.Error($"AsignacionAudioVolumen. No se localiza el dispositivo asociado al Altavoz de Alarmas => {auralSpeakerName}");
            }
        }

        private int CalculateVolume(int level)
        {
            level = Math.Max(0, level);
            level = Math.Min(7, level);

            double step = (Settings.Default.MaxVolume - Settings.Default.MinVolume) / 8.0;
            return (Settings.Default.MinVolume + (int)((level + 1) * step));
        }

        private int CalculateLevel(int volume)
        {
            volume = Math.Max(Settings.Default.MinVolume, volume);
            volume = Math.Min(Settings.Default.MaxVolume, volume);

            double step = (Settings.Default.MaxVolume - Settings.Default.MinVolume) / 8.0;
            return (volume - Settings.Default.MinVolume) / (int)step - 1;
        }
        /// <summary>
        /// Para enlaces que implican dispositivos de audio y por tanto sujeto prioridades.
        /// </summary>
        /// <param name="id"></param>
        /// <param name="dev"></param>
        /// <param name="dir">Se ha prefiltrado y solo aparecerán .Send o .Recv </param>
        /// <param name="priority"></param>
        /// <param name="tipoFuente"></param>
        /// <exception cref="InvalidOperationException"></exception>
        /// <exception cref="NotSupportedException"></exception>
        /// <exception cref="NotImplementedException"></exception>
        private void PrivateLink(int id, MixerDev dev, MixerDir dir, int priority, FuentesGlp tipoFuente)
        {
            // Determinar si el dispoitivo de mezcla es correcto
            var isDevOk = dev == MixerDev.MhpRd || dev == MixerDev.MhpTlf ||
                dev == MixerDev.Aural || dev == MixerDev.Ring;

            if (!isDevOk)
            {
                _Logger.Error($"Link: Fuente de audio no soportada. {dev}");
                throw new NotSupportedException($"Link: Fuente de audio no soportada. {dev}");
            }
            // Guardo la referencia del enlace.
            LinkInfo link = new LinkInfo(dev, dir, priority, tipoFuente, id);
            _LinksList.Add(link);
            switch (dev)
            {
                case MixerDev.MhpRd:
                    ManageRadioLink(link);
                    break;
                case MixerDev.MhpTlf:
                    ManageTelephoneLink(link);
                    break;
                case MixerDev.Aural:
                    ManageAuralLink(link);
                    break;
                case MixerDev.Ring:
                    ManageRingLink(link);
                    break;
            }
            return;
            switch (dev)
            {
                case MixerDev.MhpTlf:

                    if ((_HeadPhoneDevice >= 0 && _HeadPhoneInserted))
                    {
                        PrivateLink(id, _HeadPhoneDevice, dir, priority);
                    }

                    if ((dir == MixerDir.Send) || (dir == MixerDir.SendRecv))
                    {
                        if ((_HeadPhoneDevice >= 0))
                        {
                            foreach (int listenCall in _TlfListens)
                            {
                                mixer.Link(id, Mixer.UNASSIGNED_PRIORITY, listenCall, Mixer.UNASSIGNED_PRIORITY);
                            }
                        }
                        topPlatform.SetVolume(id, _TlfHeadPhonesVolume);
                    }
                    if ((dir == MixerDir.Recv) || (dir == MixerDir.SendRecv))
                    {
                        TlfLinkAdded();
                    }
                    break;

                case MixerDev.MhpLc:
                    switch (dir)
                    {
                        case MixerDir.Send:
                            // TODO Desconectar la RX Radio.
                            // TODO Desconectar la RX TLF.
                            break;
                        case MixerDir.Recv:
                            // TODO Desconectar la TX Radio.
                            // TODO Desconectar la TX TLF.
                            break;
                        case MixerDir.SendRecv:
                            // TODO Desconectar la RX Radio.
                            // TODO Desconectar la TX Radio.
                            // TODO Desconectar la RX TLF.
                            // TODO Desconectar la TX TLF.
                            break;
                    }
                    PrivateLink(id, _HeadPhoneDevice, dir, priority);
                    break;

                case MixerDev.MhpRd:
                    if (topPlatform.IAActivity() == false)
                        return;

                    switch (dir)
                    {
                        case MixerDir.Send:
                            // TODO Desconectar la RX TLF.
                            break;
                        case MixerDir.Recv:
                            // TODO Desconectar la TX TLF.
                            break;
                        case MixerDir.SendRecv:
                            // TODO Desconectar la RX TLF.
                            // TODO Desconectar la TX TLF.
                            break;
                    }
                    PrivateLink(id, _HeadPhoneDevice, dir, priority);
                    break;

                case MixerDev.SpkLc:
                    _Logger.Error($"Link: No existe altavoz LC");
                    throw new NotSupportedException("Mixer.Link: No existe altavoz LC");

                case MixerDev.SpkRd:
                    _Logger.Error($"Link: No existe altavoz RD");
                    throw new NotSupportedException("Mixer.Link: No existe altavoz RD");

                case MixerDev.Ring:
                    Debug.Assert(dir == MixerDir.Send);
                    if (_BuzzerEnabled)
                    {
                        ManageRingLink(link);
                        bool VolumeInUse = false;
                        foreach (LinkInfo link1 in _LinksList) if (link1._Dev == MixerDev.MhpTlf) VolumeInUse = true;
                        if (!VolumeInUse) topPlatform.SetVolume(id, _TonesVolumen);//#5829
                    }

                    topPlatform.SetVolume(id, _RingVolume);
                    break;
                case MixerDev.Aural:
                    throw new NotImplementedException("Mixer.Link: No existe altavoz RD");
                default:
                    throw new NotSupportedException($"Link: Dispositivo Desconocido {dev}");
            }

        }
        /// <summary>
        /// Para enlaces que no implican dispositivos de audio.
        /// </summary>
        /// <param name="id"></param>
        /// <param name="dev"></param>
        /// <param name="dir"></param>
        /// <param name="priority"></param>
        private void PrivateLink(int id, int dev, MixerDir dir, int priority)
        {
            if ((dir == MixerDir.Send) || (dir == MixerDir.SendRecv))
            {
                mixer.Link(id, Mixer.UNASSIGNED_PRIORITY, dev, priority);
            }
            if ((dir == MixerDir.Recv) || (dir == MixerDir.SendRecv))
            {
                mixer.Link(dev, priority, id, Mixer.UNASSIGNED_PRIORITY);
            }
        }
        /// <summary>
        /// Para todos los enlaces.
        /// </summary>
        /// <param name="id"></param>
        /// <param name="dev"></param>
        /// <param name="dir"></param>
		private void PrivateUnlink(int id, int dev, MixerDir dir)
        {
            if ((dir == MixerDir.Send) || (dir == MixerDir.SendRecv))
            {
                mixer.Unlink(id, dev);
            }
            if ((dir == MixerDir.Recv) || (dir == MixerDir.SendRecv))
            {
                mixer.Unlink(dev, id);
            }
        }
        private void ManageRingLink(LinkInfo lnk)
        {
            if (_HeadPhoneDevice < 0 || !_HeadPhoneInserted)
            {
                _Logger.Error($"ManageRingLink: Microcascos Desconectados => {_HeadPhoneDevice}, {_HeadPhoneInserted}");
                throw new InvalidOperationException("Link: Microcascos Desconectados!!");
            }
            // Esto es lo que había. Revisar...
            if (BuzzerEnabled)
            {
                if (_RingDev == MixerDev.MhpTlf)
                {
                    if ((_HeadPhoneDevice >= 0))
                    {
                        mixer.Link(lnk._CallId, Mixer.UNASSIGNED_PRIORITY, _HeadPhoneDevice, lnk._Priority);
                    }
                }
                else
                {
                    // TODO.
                }

                foreach (int listenCall in _TlfListens)
                {
                    mixer.Link(lnk._CallId, Mixer.UNASSIGNED_PRIORITY, listenCall, Mixer.UNASSIGNED_PRIORITY);
                }
                id_ringing = lnk._CallId;
                bool VolumeInUse = false;
                foreach (LinkInfo link1 in _LinksList) if (link1._Dev == MixerDev.MhpTlf) VolumeInUse = true;
                if (!VolumeInUse) topPlatform.SetVolume(lnk._CallId, _TonesVolumen);//#5829
            }
        }
        private void ManageRadioLink(LinkInfo lnk)
        {
            if (_HeadPhoneDevice < 0 || !_HeadPhoneInserted)
            {
                _Logger.Error($"ManageRadioLink: Microcascos Desconectados => {_HeadPhoneDevice}, {_HeadPhoneInserted}");
                throw new InvalidOperationException("ManageRadioLink: Microcascos Desconectados!!");
            }
            // Manejar prioridades.
            // TODO Si existen AURALES establecer un volumen de radio mínimo
            // TODO Si existen TLF desconectar la Telefonía.
            PrivateLink(lnk._CallId, _HeadPhoneDevice, lnk._Dir, lnk._Priority);
        }
        private void ManageTelephoneLink(LinkInfo lnk)
        {
            if (_HeadPhoneDevice < 0 || !_HeadPhoneInserted)
            {
                _Logger.Error($"ManageTelephoneLink: Microcascos Desconectados => {_HeadPhoneDevice}, {_HeadPhoneInserted}");
                throw new InvalidOperationException("ManageTelephoneLink: Microcascos Desconectados!!");
            }
            if (topPlatform.RadioActivity()==false)
            {
                PrivateLink(lnk._CallId, _HeadPhoneDevice, lnk._Dir, lnk._Priority);

            }
            else
            {
                // Hay actividad Radio...
            }

        }
        private void ManageAuralLink(LinkInfo lnk)
        {
            if (_AuralSpeakerDevice < 0 )
            {
                _Logger.Error($"ManageAuralLink: Altavoz Desconectado => {_AuralSpeakerDevice}");
                throw new InvalidOperationException("ManageAuralLink: altavoz Desconectado!!");
            }
        }
        private void RingLedToOff(int id)
        {
            throw new InvalidOperationException("RingLedToOff: Funcion no disponible");
        }
		private void SetSplitOff()
        {
            throw new InvalidOperationException("SetSplitOff: Funcion no disponible");
        }

		private void SetSplitOn(SplitMode mode)
        {
            throw new InvalidOperationException("SetSplitOn: Funcion no disponible");
        }

		private void OnJacksChanged(object sender, JacksStateMsg st)
        {
            try
            {
                //bool instructorJack = st.RightJack;
                //bool alumnJack = st.LeftJack;

                ManageLinksJacks(st);
                _HeadPhoneInserted = st.LeftJack;

                //if ((_SplitMode != SplitMode.Off) && (!instructorJack || !alumnJack))
                //{
                //    _InstructorJack = st.RightJack;
                //    _AlumnJack = st.LeftJack;
                //    SetSplitMode(SplitMode.Off);
                //}
                //else
                //{
                //    ManageLinksJacks(st);

                //    _InstructorJack = st.RightJack;
                //    _AlumnJack = st.LeftJack;
                //}
            }
            catch (Exception e)
            {
                _Logger.Debug("Excepcion. Mensaje: {0}", e.Message);
            }
        }

		private void ManageLinksJacks(JacksStateMsg st)
        {
            bool instructorJack = st.RightJack;
            bool alumnJack = st.LeftJack;

            foreach (LinkInfo p in _LinksList)
            {
                if (p._Dev == MixerDev.MhpTlf)
                {
                    // Tratamiento jacks instructor
                    //if (instructorJack && !_InstructorJack) // Conexión jacks instructor
                    //{
                    //    Link(p._CallId, _InstructorDev, p._Dir, p._Priority);
                    //    // LALM 210922
                    //    //Errores #3909 HMI: Telefonia en Altavoz -> Grabación Enaire
                    //    if ((p._Dir == MixerDir.SendRecv) || (p._Dir == MixerDir.Send))

                    //        Top.Recorder.Rec(CORESIP_SndDevType.CORESIP_SND_INSTRUCTOR_MHP, true);
                    //}
                    //else if (!instructorJack && _InstructorJack) // Desconexión jacks instructor
                    //{
                    //    Unlink(p._CallId, _InstructorDev, p._Dir);
                    //}

                    // Tratamiento jacks alumno
                    if (alumnJack && !_HeadPhoneInserted)   // Conexión jacks alumno
                    {
                        PrivateLink(p._CallId, _HeadPhoneDevice, p._Dir, p._Priority);
                        // LALM 210922
                        //Errores #3909 HMI: Telefonia en Altavoz -> Grabación Enaire
                        //if ((p._Dir == MixerDir.SendRecv) || (p._Dir == MixerDir.Send))
                        //    Top.Recorder.Rec(CORESIP_SndDevType.CORESIP_SND_ALUMN_MHP, true);
                    }
                    else if (!alumnJack && _HeadPhoneInserted) // Desconexión jacks alumno
                    {
                        PrivateUnlink(p._CallId, _HeadPhoneDevice, p._Dir);
                    }
                }
            }

            //// Tratamiento jacks instructor
            //if (instructorJack && !_InstructorJack)	// Conexión jacks instructor
            //{
            //    _Logger.Debug("REC-ON <= Intructor-HPH");
            //    Top.Recorder.Rec(CORESIP_SndDevType.CORESIP_SND_INSTRUCTOR_RECORDER, true);
            //}
            //else if (!instructorJack && _InstructorJack) // Desconexión jacks instructor
            //{
            //    _Logger.Debug("REC-OFF <= Intructor-HPH");
            //    Top.Recorder.Rec(CORESIP_SndDevType.CORESIP_SND_INSTRUCTOR_RECORDER, false);
            //}
            //// Tratamiento jacks alumno
            //if (alumnJack && !_AlumnJack)	// Conexión jacks alumno
            //{
            //    _Logger.Debug("REC-ON <= Alumn-HPH");
            //    Top.Recorder.Rec(CORESIP_SndDevType.CORESIP_SND_ALUMN_RECORDER, true);
            //}
            //else if (!alumnJack && _AlumnJack) // Desconexión jacks alumno
            //{
            //    _Logger.Debug("REC-OFF <= Alumn-HPH");
            //    Top.Recorder.Rec(CORESIP_SndDevType.CORESIP_SND_ALUMN_RECORDER, false);
            //}
        }

		private void OnTlfActivityChanged(object sender)
        {
            ManageTogleRxAudio();
        }

        private void OnTlfToneChanged(object sender, bool toneOn)
        {
            ManageTogleRxAudio();
        }

        private void OnListenChanged(object sender, ListenPickUpMsg msg)
        {
            ManageTogleRxAudio();
        }

        private void ManageTogleRxAudio() => throw new InvalidOperationException("ManageTogleRxAudio. Funcion no disponible.");

        /// <summary>
        /// Pasa automaticamente la salida de audio de las radios seleccionadas por cascos, 
        /// al altavoz de radio cuando hay una llamada de telefonía por cascos.
        /// Si no está presente el altavoz radio principal se usa el segundo si está disponible
        /// Se mantiene en cascos si no hay altavoces
        /// </summary>
        /// <param name="mphToSpk">true si el cambio es de casco a altavoz, false en caso contrario </param>
        private void TogleRxAudioRadio(bool mphToSpk) => throw new InvalidOperationException("TogleRxAudioRadio. Funcion no disponible.");

        /// <summary>
        /// Conecta el elemento que se pasa de cascos a un altavoz disponible.
        /// Si está conectado a un altavoz y sigue disponible no se cambia de altavoz.
        /// Si no hay altavoces disponibles no se hace nada.
        /// </summary>
        /// <param name="mphToSpk">true si el cambio es de casco a altavoz, false en caso contrario </param>
        private void ConectaAltavozDisponible(ref LinkInfo link) => throw new InvalidOperationException("ConectaAltavozDisponible. Funcion no disponible.");

		private void TlfLinkAdded()
        {
            if (_TlfRxLinks++ == 0)
            {
                foreach (int listenCall in _TlfListens)
                {
                    //if ((_InstructorDev >= 0) && ((_SplitMode == SplitMode.Off) || (_SplitMode == SplitMode.LcTf)))
                    //{
                    //    _Mixer.Link(_InstructorDev, Mixer.TLF_PRIORITY, listenCall, Mixer.UNASSIGNED_PRIORITY);
                    //}
                    if ((_HeadPhoneDevice >= 0))
                    {   
                        mixer.Link(_HeadPhoneDevice, Mixer.TLF_PRIORITY, listenCall, Mixer.UNASSIGNED_PRIORITY);
                    }
                }
            }
        }

		private void TlfLinkRemoved()
        {
            if (--_TlfRxLinks == 0)
            {
                foreach (int listenCall in _TlfListens)
                {
                    //if ((_InstructorDev >= 0) && ((_SplitMode == SplitMode.Off) || (_SplitMode == SplitMode.LcTf)))
                    //{
                    //    _Mixer.Unlink(_InstructorDev, listenCall);
                    //}
                    if ((_HeadPhoneDevice >= 0))
                    {
                        mixer.Unlink(_HeadPhoneDevice, listenCall);
                    }
                }
            }
        }

        /// <summary>
        /// Se recibe cuando hay un cambio de estado de cualquiera de los altavoces radio
        /// Se utiliza para cambiar la salida radio que está seleccionada en cascos pero se ha cambiado
        /// temporalmente a altavoz. Se cambia al altavoz disponible.
        /// </summary>
        /// <param name="sender">no se usa</param>
        /// <param name="st">no se usa</param>
        private void OnHwChanged(object sender, JacksStateMsg st) 
        {
            if (topPlatform.TphActivity())
            {
                TogleRxAudioRadio(topPlatform.TphActivity());
                //Top.Rd.UpdateRadioSpeakerLed();
            }
        }

        /// <summary>
        /// Unlink con dev type. Se llama al unlink con el device
        /// </summary>
        /// <param name="id"> llamada que se quiere desconectar</param>
        /// <param name="devType">tipo de dispositivo que se quiere desconectar</param>
        /// <param name="dir"> dirección</param>
        private void Unlink(int id, MixerDev devType, MixerDir dir)
        {
            int /*devDest = -1, */devDest2 = -1;
            //int devDestRecorder = -1, devDestRecorder2 = -1;
            switch (devType)
            {
                //case MixerDev.SpkRd:
                //    devDest = _RdSpeakerDev;
                //    break;
                //case MixerDev.SpkHf:
                //    devDest = _HfSpeakerDev;
                //    break;
                case MixerDev.MhpRd:
                        if (_HeadPhoneDevice >= 0 && _HeadPhoneInserted)
                        {
                            devDest2 = _HeadPhoneDevice;
                            //devDestRecorder2 = _AlumnRecorderDevIn;
                        }
                    break;
                default:
                    _Logger.Info("devType not implemented in this procedure {0}", devType);
                    break;
            }

            //if (devDest != -1) Unlink(id, devDest, dir);
            //if (devDestRecorder != -1) Unlink(id, devDestRecorder, dir);
            if (devDest2 != -1) PrivateUnlink(id, devDest2, dir);
            //if (devDestRecorder2 != -1) Unlink(id, devDestRecorder2, dir);
        }

        #endregion


    }
}

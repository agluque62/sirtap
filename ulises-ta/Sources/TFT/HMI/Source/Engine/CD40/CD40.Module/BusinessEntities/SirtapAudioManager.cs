using HMI.Model.Module.Messages;
using System;
using System.IO.Ports;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using NAudio;
using NAudio.CoreAudioApi;

using U5ki.Infrastructure;
using Utilities;
using System.Web.Services.Description;
using static U5ki.Infrastructure.SipAgent;
using System.Diagnostics;
using System.Web;

namespace HMI.CD40.Module.BusinessEntities
{
    public enum eAudioModes { NONE, SIMUL, SIRTAP }
    public class AsioIds
    {
        public int input { get; set; } = -1;
        public int output { get; set; } = -1;
        public override string ToString()
        {
            return $"Asio IDS => input: {input}, output: {output}";
        }
    }
    public interface ISingleAudioDevice
    {
        VersionDetails.VersionDataFileItem Version { get; }
        AsioIds AsioIds { get; }
        KeyValuePair<string, List<MMDevice>>? SoundCard { get; }
        CORESIP_SndDevType Type { get; set; }
        void SetSoundCard(KeyValuePair<string, List<MMDevice>>? card);
    }
    public class SirtapAudioDevice : IDisposable, ISingleAudioDevice
    {
        class AsioManagement
        {
            public AsioIds GetIds(KeyValuePair<string, List<MMDevice>>? SoundCard)
            {
                logger.Info<AsioManagement>("GetIds");
                var ids = new AsioIds();
                Task.Run(() =>
                {
                    try
                    {
                        var aproxName = SoundCard?.Key;
                        //AsioChannels.Refresh();
                        logger.Info<AsioManagement>($"Asio channels => {Channels()}");
                        // TODO. 
                        ids = new AsioIds() { input = 0, output = 0 };
                    }
                    catch (Exception x)
                    {
                        logger.Exception<AsioManagement>(x);
                    }

                }).Wait();
                return ids;
            }
            private string Channels()
            {
                var str = $"Inputs ({AsioChannels.InChannels.Count()}: {string.Join("##", AsioChannels.InChannels)}\nOutputs ({AsioChannels.OutChannels.Count()}): {string.Join("##", AsioChannels.OutChannels)}";
                return str;
            }
            public AsioManagement(ILogger logger = null)
            {
                this.logger = logger ?? new NLogLogger();
            }
            ILogger logger;
        }
        public VersionDetails.VersionDataFileItem Version => throw new NotImplementedException("TODO");
        public AsioIds AsioIds => new AsioManagement().GetIds(SoundCard);
        public CORESIP_SndDevType Type { get; set; }
        public KeyValuePair<string, List<MMDevice>>? SoundCard { get; private set; }
        public void SetSoundCard(KeyValuePair<string, List<MMDevice>>? card)
        {
            SoundCard = card;
            // todo
            // TODO SipAgent.Asignacion(coresipId, windowsName);
        }
        public void Dispose()
        {
        }
        public SirtapAudioDevice(ILogger logger=null, 
            KeyValuePair<string, List<MMDevice>>? soundCard = null, 
            CORESIP_SndDevType coresipId = CORESIP_SndDevType.CORESIP_SND_ALUMN_MHP)
        {
            this.logger = logger ?? new NLogLogger();
            SoundCard = soundCard;
            Type = coresipId;
        }
        public override string ToString()
        {
            return $"{SoundCard?.Key}, {AsioIds}";
        }
        ILogger logger;
    }
    public interface ISingleIODevice
    {
        event GenericEventHandler<bool> StatusChanged;
        event GenericEventHandler<bool> InputChanged;
        bool IsConnected { get; }
    }
    public class SirtapPttDevice : IDisposable, ISingleIODevice
    {
        public event GenericEventHandler<bool> StatusChanged;
        public event GenericEventHandler<bool> InputChanged;
        public bool IsConnected {get; private set; } = false;
        public void Dispose()
        {
            statusSupervisorEvent.Set();
            statusSupervisor.Wait();
            sPort?.Dispose();
        }
        public SirtapPttDevice(ILogger logger = null, string commName="COM1")
        {
            this.logger = logger ?? new NLogLogger();
            usedComm = commName;
            statusSupervisor = Task.Run(() =>
            {
                bool? commPresent = null;
                while (statusSupervisorEvent.WaitOne(TimeSpan.FromSeconds(1))==false) 
                {
                    // Supervisa si el COMM está presente
                    PresenceCheck(commPresent, (change) =>
                    {
                        ResetPort(change);
                        commPresent = change;
                        if (change == false)
                        {
                            IsConnected = false;
                            General.SafeLaunchEvent(StatusChanged, this, IsConnected);
                        }
                    });
                    // Supervisa que está conectado
                    if (commPresent == true)
                    {
                        if (!sPort.IsOpen)
                        {
                            sPort.Open();
                            IsConnected = sPort.IsOpen;
                            General.SafeLaunchEvent(StatusChanged, this, IsConnected);
                        }
                    }
                }
            });
        }

        private void SPort_PinChanged(object sender, SerialPinChangedEventArgs e)
        {
            if (e.EventType == SerialPinChange.CtsChanged)
            {
                General.SafeLaunchEvent(InputChanged, this, sPort?.CtsHolding ?? false);
            }
        }
        void SetupPort()
        {
            sPort = new SerialPort(usedComm);
            sPort.PinChanged += SPort_PinChanged;
        }
        void ResetPort(bool present)
        {
           sPort?.Dispose();
           if (present == true)
               SetupPort();
        }
        void PresenceCheck(bool? actual, Action<bool> notifyChange)
        {
            var current = IsPresent;
            if (current != actual)
            {
                notifyChange(current);
            }
        }
        bool IsPresent => SerialPort.GetPortNames().Contains(usedComm);
        string usedComm = null;
        SerialPort sPort = null;
        ManualResetEvent statusSupervisorEvent = new ManualResetEvent(false);
        Task statusSupervisor;
        ILogger logger;
    }
    public interface IOldHwManager
    {
        bool InstructorJack { get; }
        bool AlumnJack { get; }
        bool RdSpeaker { get; }
        bool LCSpeaker { get; }
        bool HfSpeaker { get; }
        bool HwPtt_Activated { get; }
        void OnOffLed(CORESIP_SndDevType tipo, byte val);
        List<object> ListaDispositivos {  get; }
    }
    public interface IHwAudioManager : IOldHwManager
    {
        event GenericEventHandler<ISingleIODevice, bool> PttDeviceStatusChanged;
        event GenericEventHandler<ISingleAudioDevice, bool> HeadSetStatusChanged;
        event GenericEventHandler<ISingleAudioDevice, bool> SpeakerStatusChanged;
        event GenericEventHandler<JacksStateMsg> JacksChanged;
        event GenericEventHandler<bool> PttPulsed;
        event GenericEventHandler<SnmpIntMsg<string, int>> SetSnmpInt;

        ISingleAudioDevice HeadSet { get; set; }
        ISingleAudioDevice AuralAlarmsSpeaker { get; set; }
        ISingleIODevice Ptt { get; set; }
        List<VersionDetails.VersionDataFileItem> Version { get; set; } 
        void Init();
        void Start(EventQueue workingThread = null);
        void End();
    }
    public class SirtapAudioManager : IDisposable, IHwAudioManager
    {
        #region IHwAudioManager
        public ISingleAudioDevice HeadSet { get; set ; }
        public ISingleAudioDevice AuralAlarmsSpeaker { get; set; }
        public ISingleIODevice Ptt { get; set; }
        public List<VersionDetails.VersionDataFileItem> Version { get => throw new NotImplementedException(); set => throw new NotImplementedException(); }

        public event GenericEventHandler<ISingleAudioDevice, bool> HeadSetStatusChanged;
        public event GenericEventHandler<ISingleAudioDevice, bool> SpeakerStatusChanged;
        public event GenericEventHandler<ISingleIODevice, bool> PttDeviceStatusChanged;
        public event GenericEventHandler<JacksStateMsg> JacksChanged;
        public event GenericEventHandler<bool> PttPulsed;
        public event GenericEventHandler<SnmpIntMsg<string, int>> SetSnmpInt;

        public void Dispose()
        {
            if (Ptt != null)
            {
                Ptt.StatusChanged -= OnPttDeviceStatusChanged;
                Ptt.InputChanged -= OnPttDeviceInputChanged;
            }
            statusSupervisorEvent.Set();
            statusSupervisor.Wait();
        }
        public void End()
        {
            Dispose();
        }
        public void Init()
        {
        }
        public void Start(EventQueue workingThread = null)
        {
            BackwardQueue = workingThread ?? new EventQueue();
            if (workingThread == null)
                BackwardQueue.Start();
            statusSupervisor = Task.Run(SupervisorTask);
        }
        #endregion

        public SirtapAudioManager(ILogger logger = null, ISingleIODevice ptt = null)
        {
            this.logger = logger ?? new NLogLogger();
            HeadSet = new SirtapAudioDevice(this.logger, null, CORESIP_SndDevType.CORESIP_SND_ALUMN_MHP);
            Ptt = ptt ?? new SirtapPttDevice(null, "COM1");
            AuralAlarmsSpeaker = new SirtapAudioDevice(this.logger, null, CORESIP_SndDevType.CORESIP_SND_RD_SPEAKER);
            if (Ptt != null)
            {
                Ptt.StatusChanged += OnPttDeviceStatusChanged;
                Ptt.InputChanged += OnPttDeviceInputChanged;
            }
        }
        private void OnPttDeviceStatusChanged(object from, bool actualStatus)
        {
            BackwardLaunchEvent(() => General.SafeLaunchEvent(PttDeviceStatusChanged, this, Ptt, actualStatus));
            // NotifyJackChanged(HeadSet.SoundCard != null, actualStatus);
        }
        private void OnPttDeviceInputChanged(object from, bool actualStatus)
        {
            BackwardLaunchEvent(() => General.SafeLaunchEvent(PttPulsed, this, actualStatus));
        }
        private void SupervisorTask()
        {
            do
            {
                // Supervisa la presencia de los dispositivos de Audio.
                FindHeadSetAndSpeaker((headset, speaker) =>
                {
                    ManageSoundCarSatus(HeadSet.SoundCard, headset, (change) =>
                    {
                        HeadSet.SetSoundCard(change == true ? headset : null);
                        BackwardLaunchEvent(() => General.SafeLaunchEvent(HeadSetStatusChanged, this, HeadSet, change));
                        NotifyJackChanged(change, /*Ptt.IsConnected*/true);
                    });
                    ManageSoundCarSatus(AuralAlarmsSpeaker.SoundCard, speaker, (change) =>
                    {
                        AuralAlarmsSpeaker.SetSoundCard(change == true ? speaker : null);
                        BackwardLaunchEvent(() => General.SafeLaunchEvent(SpeakerStatusChanged, this, AuralAlarmsSpeaker, change));
                    });
                });
            } while (statusSupervisorEvent.WaitOne(TimeSpan.FromSeconds(2)) == false);
        }
        void FindHeadSetAndSpeaker(Action<KeyValuePair<string, List<MMDevice>>?, KeyValuePair<string, List<MMDevice>>?> notify)
        {
            var activeDevices = new MMDeviceEnumerator()
                .EnumerateAudioEndPoints(DataFlow.All, DeviceState.Active)
                .GroupBy(dev => dev.DeviceFriendlyName)
                .ToDictionary(g => g.Key, g => g.ToList());
            var HeadSetCandidate = activeDevices.Where(c => MeetIOPattern(c, true, true)).FirstOrDefault();
            var SpeakerCandidate = activeDevices.Where(c => MeetIOPattern(c, false, true)).FirstOrDefault();
            notify(HeadSetCandidate, SpeakerCandidate);
        }
        bool MeetIOPattern(KeyValuePair<string, List<MMDevice>> candidate, bool withInputs, bool withOutputs)
        {
            var inputs = candidate.Value.Where(d => d.DataFlow== DataFlow.Capture ).Count();
            var outputs = candidate.Value.Where(d => d.DataFlow== DataFlow.Render).Count();
            var patternInputs = withInputs ? inputs > 0 : inputs <= 0;
            var patternOutput = withOutputs ? outputs > 0 : outputs <= 0;  
            return patternInputs && patternOutput;
        }
        void ManageSoundCarSatus(KeyValuePair<string, List<MMDevice>>? actual, KeyValuePair<string, List<MMDevice>>? candidate, Action<bool> notifychange)
        {
            bool candidateNull = candidate == null || candidate?.Key == null;
            if (actual == null) 
            {
                if (candidateNull == false)
                    notifychange(true);
            }
            else
            {
                if (candidateNull==true)
                    notifychange(false);
                else
                {
                    if (actual?.Key != candidate?.Key)
                    {
                        notifychange(false);
                    }
                }
            }
        }
        void NotifyJackChanged(bool audioDevice, bool pttdevice)
        {
            var jackstate = audioDevice && pttdevice;
            var msg = new JacksStateMsg(jackstate, jackstate);
            BackwardLaunchEvent(() => General.SafeLaunchEvent(JacksChanged, this, msg));
        }
        void BackwardLaunchEvent(Action launch)
        {
            if (BackwardQueue == null) launch();
            else 
                BackwardQueue.Enqueue("SirtapAudioManagerEvent", () => launch());
        }
        bool IsAvailble = false;
        ManualResetEvent statusSupervisorEvent = new ManualResetEvent(false);
        Task statusSupervisor;
        EventQueue BackwardQueue = null;
        ILogger logger = null;

        #region IOldHwManager
        public bool InstructorJack => false;
        public bool AlumnJack => false;         // TODO.
        public bool LCSpeaker => false;
        public bool RdSpeaker => false;
        public bool HfSpeaker => false;
        public bool HwPtt_Activated => false;   // TODO.
        public List<object> ListaDispositivos => new List<object>();
        public void OnOffLed(CORESIP_SndDevType tipo, byte val) 
        {
            logger.Error<SirtapAudioManager>("OnOffLed => Rutina obsoleta");
        }

        #endregion
    }
}

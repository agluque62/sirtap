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
using NAudio.Wave.Asio;

using U5ki.Infrastructure;
using Utilities;
using System.Web.Services.Description;
using static U5ki.Infrastructure.SipAgent;
using System.Diagnostics;
using System.Web;
using NAudio.Wave;
using System.Runtime.CompilerServices;

namespace HMI.CD40.Module.BusinessEntities
{
    public enum eAudioModes { NONE, SIMUL, SIRTAP }
    public interface IWindowsAudioDevice
    {
        string Name { get; }
        MMDevice MMDevice { get; }
    }
    public interface ISingleAudioDevice
    {
        VersionDetails.VersionDataFileItem Version { get; }
        AudioIds AudioIds { get; }
        KeyValuePair<string, List<IWindowsAudioDevice>>? SoundCard { get; }
        CORESIP_SndDevType Type { get; set; }
        void SetSoundCard(KeyValuePair<string, List<IWindowsAudioDevice>>? card);
        void Init(Object initObject);
    }
    public interface ISingleIODevice
    {
        event GenericEventHandler<bool> StatusChanged;
        event GenericEventHandler<bool> InputChanged;
        bool IsConnected { get; }
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
        List<object> ListaDispositivos { get; }
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

    static class SirtapAudioExtensions
    {
        public static bool SmartCompare(this string str, string other, int limit,  ILogger logger = null)
        {
            Func<string, string, int> LevenshteinDistance = (str1, str2) =>
            {
                int len1 = str1.Length;
                int len2 = str2.Length;
                var matrix = new int[len1 + 1, len2 + 1];

                for (int i = 0; i <= len1; i++)
                    matrix[i, 0] = i;

                for (int j = 0; j <= len2; j++)
                    matrix[0, j] = j;

                for (int i = 1; i <= len1; i++)
                {
                    for (int j = 1; j <= len2; j++)
                    {
                        int cost = (str2[j - 1] == str1[i - 1]) ? 0 : 1;
                        matrix[i, j] = Math.Min(
                            Math.Min(matrix[i - 1, j] + 1, matrix[i, j - 1] + 1),
                            matrix[i - 1, j - 1] + cost);
                    }
                }
                return matrix[len1, len2];
            };
            var distance = LevenshteinDistance(str, other);
            return distance <= limit;
        }
    }

    public class AudioIds
    {
        public int input { get; set; } = -1;
        public int output { get; set; } = -1;
        public override string ToString()
        {
            return $"input: {input}, output: {output}";
        }
    }
    public class WindowsMMDevice : IWindowsAudioDevice
    {
        public string Name { get; }
        public MMDevice MMDevice { get; }
        public WindowsMMDevice(string name, MMDevice mMDevice)
        {
            Name = name;
            MMDevice = mMDevice;
        }
    }
    public class SirtapAudioDevice : IDisposable, ISingleAudioDevice
    {
        public VersionDetails.VersionDataFileItem Version => throw new NotImplementedException("TODO");
        public AudioIds AudioIds => IsthereASIO ? GetAsioIds() : GetWindowsAudioIds();
        public CORESIP_SndDevType Type { get; set; }
        public KeyValuePair<string, List<IWindowsAudioDevice>>? SoundCard { get; private set; }
        public void SetSoundCard(KeyValuePair<string, List<IWindowsAudioDevice>>? card)
        {
            SoundCard = card;
            // todo
            // TODO SipAgent.Asignacion(coresipId, windowsName);
        }
        public void Init(Object initObject)
        {
            if (initObject != null && initObject is AsioDriver)
            {
                asioDriver = initObject as AsioDriver;
            }
        }
        public void Dispose()
        {
        }
        public SirtapAudioDevice(ILogger logger=null, 
            KeyValuePair<string, List<IWindowsAudioDevice>>? soundCard = null, 
            CORESIP_SndDevType coresipId = CORESIP_SndDevType.CORESIP_SND_ALUMN_MHP)
        {
            this.logger = logger ?? new NLogLogger();
            SoundCard = soundCard;
            Type = coresipId;
        }
        public override string ToString()
        {
            return $"{SoundCard?.Key}, {AudioIds}";
        }

        private AudioIds GetWindowsAudioIds()
        {
            // TODO.
            return new AudioIds();
        }
        private AudioIds GetAsioIds()
        {
            logger.Info<SirtapAudioDevice>("GetIds");
            var ids = new AudioIds();
            Task.Run(() =>
            {
                try
                {
                    var deviceName = SoundCard?.Key;
                    // Obtenemos los datos del driver...
                    asioDriver.Init(IntPtr.Zero);
                    asioDriver.GetChannels(out int inputs, out int outputs);
                    var asioInputs = new Dictionary<int, AsioChannelInfo>();
                    var asioOutputs = new Dictionary<int, AsioChannelInfo>();
                    for (int i = 0; i < inputs; i++)
                        asioInputs.Add(i, asioDriver.GetChannelInfo(i, true));
                    for (int i = 0; i < outputs; i++)
                        asioOutputs.Add(i, asioDriver.GetChannelInfo((int)i, false));
                    // Obtenemos los candidatos comparando el nombre del dispositivo con el id asio.
                    var inputCandidates = asioInputs.Where(e => deviceName.SmartCompare(e.Value.name, 10))
                        .Select(e => new Pair<int,string>(e.Key, e.Value.name)).ToArray();
                    var outputCandidates = asioOutputs.Where(e => deviceName.SmartCompare(e.Value.name, 10))
                        .Select(e => new Pair<int, string>(e.Key, e.Value.name)).ToArray();
                    // Seleccionamos las primeras si existen.
                    if (inputCandidates.Length > 0 || outputCandidates.Length > 0)
                    {
                        ids.input = inputCandidates.Length > 0 ? inputCandidates[0].First : -1;
                        ids.output = outputCandidates.Length > 0 ? outputCandidates[0].First : -1;

                        logger.Info<SirtapAudioDevice>($"Asio IDS found for device {deviceName} => {ids}");
                    }
                    else
                    {
                        logger.Info<SirtapAudioDevice>($"No Asio IDS found for device {deviceName}");
                    }
                }
                catch (Exception x)
                {
                    logger.Exception<SirtapAudioDevice>(x);
                }

            }).Wait();
            return ids;
        }
        private bool IsthereASIO => asioDriver != null;
        ILogger logger;
        AsioDriver asioDriver = null;
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
    public class SirtapAudioManagerBase
    {
        protected void FindHeadSetAndSpeaker(Action<KeyValuePair<string, List<IWindowsAudioDevice>>?, KeyValuePair<string, List<IWindowsAudioDevice>>?> notify)
        {
            var activeDevices = new MMDeviceEnumerator()
                .EnumerateAudioEndPoints(DataFlow.All, DeviceState.Active)
                .Select(d => new WindowsMMDevice(d.DeviceFriendlyName, d) as IWindowsAudioDevice)
                .GroupBy(dev => dev.Name)
                .ToDictionary(g => g.Key, g => g.ToList());
            var HeadSetCandidate = activeDevices.Where(c => MeetIOPattern(c, true, true)).FirstOrDefault();
            var SpeakerCandidate = activeDevices.Where(c => MeetIOPattern(c, false, true)).FirstOrDefault();
            notify(HeadSetCandidate, SpeakerCandidate);
        }
        protected bool MeetIOPattern(KeyValuePair<string, List<IWindowsAudioDevice>> candidate, bool withInputs, bool withOutputs)
        {
            var inputs = candidate.Value.Where(d => d.MMDevice.DataFlow == DataFlow.Capture).Count();
            var outputs = candidate.Value.Where(d => d.MMDevice.DataFlow == DataFlow.Render).Count();
            var patternInputs = withInputs ? inputs > 0 : inputs <= 0;
            var patternOutput = withOutputs ? outputs > 0 : outputs <= 0;
            return patternInputs && patternOutput;
        }
        protected void ManageSoundCarSatus(KeyValuePair<string, List<IWindowsAudioDevice>>? actual, KeyValuePair<string, List<IWindowsAudioDevice>>? candidate, Action<bool> notifychange)
        {
            bool candidateNull = candidate == null || candidate?.Key == null;
            if (actual == null)
            {
                if (candidateNull == false)
                    notifychange(true);
            }
            else
            {
                if (candidateNull == true)
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
        protected void BackwardLaunchEvent(Action launch)
        {
            if (BackwardQueue == null) launch();
            else
                BackwardQueue.Enqueue("SirtapAudioManagerEvent", () => launch());
        }
        protected bool IsAvailble = false;
        protected ManualResetEvent statusSupervisorEvent = new ManualResetEvent(false);
        protected Task statusSupervisor;
        protected EventQueue BackwardQueue = null;
        protected ILogger logger = null;
    }
    public class SirtapAudioManagerWithASIO : SirtapAudioManagerBase, IDisposable, IHwAudioManager
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
            if (asioDriver != null && asioDriver is AsioDriver)
            {
                (asioDriver as AsioDriver).ReleaseComAsioDriver();
                asioDriver = null;
            }
            Dispose();
        }
        public void Init()
        {
            var drivers = AsioOut.GetDriverNames();
            asioDriver = AsioDriver.GetAsioDriverByName(drivers[0]);
            HeadSet.Init(asioDriver);
            AuralAlarmsSpeaker.Init(asioDriver);
        }
        public void Start(EventQueue workingThread = null)
        {
            BackwardQueue = workingThread ?? new EventQueue();
            if (workingThread == null)
                BackwardQueue.Start();
            statusSupervisor = Task.Run(SupervisorTask);
        }
        #endregion

        public SirtapAudioManagerWithASIO(ILogger logger = null, ISingleIODevice ptt = null)
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
                        BackwardLaunchEvent(() => General.SafeLaunchEvent(JacksChanged, this, new JacksStateMsg(change, change)));
                    });
                    ManageSoundCarSatus(AuralAlarmsSpeaker.SoundCard, speaker, (change) =>
                    {
                        AuralAlarmsSpeaker.SetSoundCard(change == true ? speaker : null);
                        BackwardLaunchEvent(() => General.SafeLaunchEvent(SpeakerStatusChanged, this, AuralAlarmsSpeaker, change));
                    });
                });
            } while (statusSupervisorEvent.WaitOne(TimeSpan.FromSeconds(2)) == false);
        }
        AsioDriver asioDriver = null;
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
            logger.Error<SirtapAudioManagerWithASIO>("OnOffLed => Rutina obsoleta");
        }
        #endregion
    }
    public class SirtapAudioManagerWithoutASIO : SirtapAudioManagerBase, IDisposable, IHwAudioManager
    {
        #region IHwAudioManager
        public ISingleAudioDevice HeadSet { get; set; }
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

        public SirtapAudioManagerWithoutASIO(ILogger logger = null, ISingleIODevice ptt = null)
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
                        BackwardLaunchEvent(() => General.SafeLaunchEvent(JacksChanged, this, new JacksStateMsg(change, change)));
                    });
                    ManageSoundCarSatus(AuralAlarmsSpeaker.SoundCard, speaker, (change) =>
                    {
                        AuralAlarmsSpeaker.SetSoundCard(change == true ? speaker : null);
                        BackwardLaunchEvent(() => General.SafeLaunchEvent(SpeakerStatusChanged, this, AuralAlarmsSpeaker, change));
                    });
                });
            } while (statusSupervisorEvent.WaitOne(TimeSpan.FromSeconds(2)) == false);
        }

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
            logger.Error<SirtapAudioManagerWithASIO>("OnOffLed => Rutina obsoleta");
        }
        #endregion
    }

}

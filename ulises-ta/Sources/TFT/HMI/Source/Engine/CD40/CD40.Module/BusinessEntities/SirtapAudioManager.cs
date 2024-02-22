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
            public AsioIds GetIds(string asioName, CORESIP_SndDevType tp)
            {
                try
                {
                    AsioChannels.Init();
                    var asioIds = new AsioIds()
                    {
                        input = AsioChannels.InChannels.FindIndex(name => name == asioName),
                        output = AsioChannels.OutChannels.FindIndex((name) => name == asioName)
                    };
                    return asioIds;
                }
                catch(Exception ex)
                {
                    // TODO Logger...
                    return new AsioIds();
                }
            }
            public AsioIds GetIds(KeyValuePair<string, List<MMDevice>>? SoundCard)
            {
                // TODO
                return new AsioIds();
            }
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
        public SirtapAudioDevice(KeyValuePair<string, List<MMDevice>>? soundCard, CORESIP_SndDevType coresipId)
        {
            SoundCard = soundCard;
            Type = coresipId;
        }
        public override string ToString()
        {
            return $"{SoundCard?.Key}, {AsioIds}";
        }
    }
    public interface ISingleIODevice
    {
        event GenericEventHandler<bool> StatusChanged;
        event GenericEventHandler<bool> InputChanged;
    }
    public class SirtapPttDevice : IDisposable, ISingleIODevice
    {
        public event GenericEventHandler<bool> StatusChanged;
        public event GenericEventHandler<bool> InputChanged;

        public void Dispose()
        {
            statusSupervisorEvent.Set();
            statusSupervisor.Wait();
            sPort?.Dispose();
        }
        public SirtapPttDevice(string commName)
        {
            usedComm = commName;
            statusSupervisor = Task.Run(() =>
            {
                bool? commPresent = null;
                while (statusSupervisorEvent.WaitOne(TimeSpan.FromSeconds(1))==false) 
                {
                    // Supervisa si el COMM está presente
                    PresenceCheck(commPresent, (change) =>
                    {
                        General.SafeLaunchEvent(StatusChanged, this, change);
                        ResetPort(change);
                        commPresent = change;
                    });
                    // Supervisa que está conectado
                    if (commPresent == true)
                    {
                        if (!sPort.IsOpen)
                        {
                            sPort.Open();
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
    }
    public interface IHwAudioManager
    {
        event GenericEventHandler<ISingleIODevice, bool> DeviceStatusChanged;
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
        void Start();
        void End();
    }
    public class SirtapAudioManager : IDisposable, IHwAudioManager
    {
        public ISingleAudioDevice HeadSet { get; set ; }
        public ISingleAudioDevice AuralAlarmsSpeaker { get; set; }
        public ISingleIODevice Ptt { get; set; }
        public List<VersionDetails.VersionDataFileItem> Version { get => throw new NotImplementedException(); set => throw new NotImplementedException(); }

        public event GenericEventHandler<ISingleAudioDevice, bool> HeadSetStatusChanged;
        public event GenericEventHandler<ISingleAudioDevice, bool> SpeakerStatusChanged;
        public event GenericEventHandler<ISingleIODevice, bool> DeviceStatusChanged;
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
        public void Start()
        {
            statusSupervisor = Task.Run(SupervisorTask);
        }

        public SirtapAudioManager(ISingleIODevice ptt = null)
        {
            HeadSet = new SirtapAudioDevice(null, CORESIP_SndDevType.CORESIP_SND_ALUMN_MHP);
            Ptt = ptt ?? new SirtapPttDevice("COM1");
            AuralAlarmsSpeaker = new SirtapAudioDevice(null, CORESIP_SndDevType.CORESIP_SND_RD_SPEAKER);
            if (Ptt != null)
            {
                Ptt.StatusChanged += OnPttDeviceStatusChanged;
                Ptt.InputChanged += OnPttDeviceInputChanged;
            }
        }
        private void OnPttDeviceStatusChanged(object from, bool actualStatus)
        {
        }
        private void OnPttDeviceInputChanged(object from, bool actualStatus)
        {
        }
        private void SupervisorTask()
        {
            while (statusSupervisorEvent.WaitOne(TimeSpan.FromSeconds(2)) == false)
            {
                FindHeadSetAndSpeaker((headset, speaker) =>
                {
                    ManageSoundCarSatus(HeadSet.SoundCard, headset, (change) =>
                    {
                        HeadSet.SetSoundCard(change==true ? headset : null);
                        General.SafeLaunchEvent(HeadSetStatusChanged, this, HeadSet, change);
                    });
                    ManageSoundCarSatus(AuralAlarmsSpeaker.SoundCard, speaker, (change) =>
                    {
                        AuralAlarmsSpeaker.SetSoundCard(change == true ? speaker : null);
                        General.SafeLaunchEvent(SpeakerStatusChanged, this, AuralAlarmsSpeaker, change);
                    });
                });
            }
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
        ManualResetEvent statusSupervisorEvent = new ManualResetEvent(false);
        Task statusSupervisor;
    }
}

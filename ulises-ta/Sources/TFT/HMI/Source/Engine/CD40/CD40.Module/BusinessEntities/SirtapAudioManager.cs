using HMI.Model.Module.Messages;
using System;
using System.IO.Ports;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

using U5ki.Infrastructure;
using Utilities;
using System.Web.Services.Description;
using static U5ki.Infrastructure.SipAgent;
using System.Diagnostics;

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
        event GenericEventHandler<bool> StatusChanged;
        VersionDetails.VersionDataFileItem Version { get; }
        AsioIds AsioIds { get; }
        string WindowsName { get; set; }
        string AsioName { get; set; }
        CORESIP_SndDevType Type { get; set; }
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
        }
        public VersionDetails.VersionDataFileItem Version => throw new NotImplementedException("TODO");
        public AsioIds AsioIds => new AsioManagement().GetIds(AsioName, Type);
        public string AsioName { get; set; }
        public string WindowsName { get; set; }
        public CORESIP_SndDevType Type { get; set; }

        public event GenericEventHandler<bool> StatusChanged;
        public void Dispose()
        {
            statusSupervisorEvent.Set();
            statusSupervisor.Wait();
            statusSupervisor = null;
            statusSupervisorEvent = null;
        }
        public SirtapAudioDevice(string windowsName, string asioName, CORESIP_SndDevType coresipId)
        {
            Type = coresipId;
            AsioName = asioName;
            WindowsName = windowsName;
            SipAgent.Asignacion(coresipId, windowsName);
            statusSupervisor = Task.Run(() =>
            {
                bool? actualStatus = null;
                while (statusSupervisorEvent.WaitOne(TimeSpan.FromSeconds(1))== false) 
                {
                    // Detectar si hay tarjeta de audio o no y generar los eventos correspondientes.
                    var currentStatus = IstheDevicePresent();
                    if (currentStatus != actualStatus)
                    {
                        General.SafeLaunchEvent(StatusChanged, this, currentStatus);
                        actualStatus = currentStatus;
                    }
                }
            });
        }
        private bool IstheDevicePresent()
        {
            CORESIP_Error err;
            CORESIP_SndWindowsDevices Devices;
            if (CORESIP_GetWindowsSoundDeviceNames(0, out Devices, out err) == 0)
            {
                string[] separatingStrings = { "<###>" };
                string[] DevWinName = Devices.DeviceNames.Split(separatingStrings, System.StringSplitOptions.RemoveEmptyEntries);
                return DevWinName.Contains(WindowsName);
            }
            else
            {
                return false;
            }
        }
        ManualResetEvent statusSupervisorEvent = new ManualResetEvent(false);
        Task statusSupervisor;
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
        event GenericEventHandler<JacksStateMsg> JacksChanged;
        event GenericEventHandler<bool> PttPulsed;
        event GenericEventHandler<ISingleIODevice> DeviceStatusChanged;
        event GenericEventHandler<SnmpIntMsg<string, int>> SetSnmpInt;

        ISingleAudioDevice ExecutiveHeadPhone { get; set; }
        ISingleAudioDevice AssistantHeadPhone { get; set; }
        ISingleAudioDevice RadioSpeaker { get; set; }
        ISingleAudioDevice PhoneSpeaker { get; set; }
        ISingleAudioDevice AuxiliarRadioSpeaker { get; set; }
        ISingleAudioDevice AuralAlarmsSpeaker { get; set; }
        ISingleIODevice ExecutivePtt { get; set; }
        ISingleIODevice AssistantPtt { get; set; }
        List<VersionDetails.VersionDataFileItem> Version { get; set; } 
        void Init();
        void Start();
        void End();
    }
    public class SirtapAudioManager : IDisposable, IHwAudioManager
    {
        public ISingleAudioDevice ExecutiveHeadPhone { get => throw new NotImplementedException(); set => throw new NotImplementedException(); }
        public ISingleAudioDevice AssistantHeadPhone { get => throw new NotImplementedException(); set => throw new NotImplementedException(); }
        public ISingleAudioDevice RadioSpeaker { get => throw new NotImplementedException(); set => throw new NotImplementedException(); }
        public ISingleAudioDevice PhoneSpeaker { get => throw new NotImplementedException(); set => throw new NotImplementedException(); }
        public ISingleAudioDevice AuxiliarRadioSpeaker { get => throw new NotImplementedException(); set => throw new NotImplementedException(); }
        public ISingleAudioDevice AuralAlarmsSpeaker { get => throw new NotImplementedException(); set => throw new NotImplementedException(); }
        public ISingleIODevice ExecutivePtt { get => throw new NotImplementedException(); set => throw new NotImplementedException(); }
        public ISingleIODevice AssistantPtt { get => throw new NotImplementedException(); set => throw new NotImplementedException(); }
        public List<VersionDetails.VersionDataFileItem> Version { get => throw new NotImplementedException(); set => throw new NotImplementedException(); }

        public event GenericEventHandler<JacksStateMsg> JacksChanged;
        public event GenericEventHandler<bool> PttPulsed;
        public event GenericEventHandler<ISingleIODevice> DeviceStatusChanged;
        public event GenericEventHandler<SnmpIntMsg<string, int>> SetSnmpInt;

        public void Dispose()
        {
            if (ExecutiveHeadPhone != null)
            {
                ExecutiveHeadPhone.StatusChanged -= OnHeadphoneStatusChanged;
            }
            if (AuralAlarmsSpeaker != null)
            {
                AuralAlarmsSpeaker.StatusChanged -= OnAuralSpeakerStatusChanged;
            }
            if (ExecutivePtt != null)
            {
                ExecutivePtt.StatusChanged -= OnPttDeviceStatusChanged;
                ExecutivePtt.InputChanged -= OnPttDeviceInputChanged;
            }
        }

        public void End()
        {
            throw new NotImplementedException();
        }

        public void Init()
        {
            throw new NotImplementedException();
        }

        public void Start()
        {
            throw new NotImplementedException();
        }

        public SirtapAudioManager(ISingleAudioDevice headphone = null, ISingleIODevice ptt = null, ISingleAudioDevice speaker = null)
        {
            ExecutiveHeadPhone = headphone ?? new SirtapAudioDevice(
                Properties.Settings.Default.CasAlumnoId, 
                Properties.Settings.Default.CasAsioId,
                CORESIP_SndDevType.CORESIP_SND_ALUMN_MHP);
            ExecutivePtt = ptt ?? new SirtapPttDevice("COM1");
            AuralAlarmsSpeaker = speaker ?? new SirtapAudioDevice(
                Properties.Settings.Default.RdSpkWindowsId, 
                Properties.Settings.Default.RdSpkAsioId, 
                CORESIP_SndDevType.CORESIP_SND_RD_SPEAKER);

            AssistantHeadPhone = null;
            RadioSpeaker = null;
            PhoneSpeaker = null;
            AuxiliarRadioSpeaker = null;
            AssistantPtt = null;

            if (ExecutiveHeadPhone != null)
            {
                ExecutiveHeadPhone.StatusChanged += OnHeadphoneStatusChanged;
            }
            if (AuralAlarmsSpeaker != null)
            {
                AuralAlarmsSpeaker.StatusChanged += OnAuralSpeakerStatusChanged;
            }
            if (ExecutivePtt != null)
            {
                ExecutivePtt.StatusChanged += OnPttDeviceStatusChanged;
                ExecutivePtt.InputChanged += OnPttDeviceInputChanged;
            }
        }

        private void OnPttDeviceStatusChanged(object from, bool actualStatus)
        {
        }

        private void OnPttDeviceInputChanged(object from, bool actualStatus)
        {
        }

        private void OnHeadphoneStatusChanged(object from, bool actualStatus)
        {
        }

        private void OnAuralSpeakerStatusChanged(object from, bool actualStatus)
        {
        }

    }
}

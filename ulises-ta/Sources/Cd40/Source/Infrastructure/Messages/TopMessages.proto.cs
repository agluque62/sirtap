//------------------------------------------------------------------------------
// <auto-generated>
//     This code was generated by a tool.
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
// </auto-generated>
//------------------------------------------------------------------------------

// Generated from: TopMessages.proto
// Note: requires additional types generated from: Common.proto
namespace U5ki.Infrastructure
{
  [global::System.Serializable, global::ProtoBuf.ProtoContract(Name=@"TopRs")]
  public partial class TopRs : global::ProtoBuf.IExtensible
  {
    public TopRs() {}
    
    private global::ProtoBuf.IExtension extensionObject;
    global::ProtoBuf.IExtension global::ProtoBuf.IExtensible.GetExtensionObject(bool createIfMissing)
      { return global::ProtoBuf.Extensible.GetExtensionObject(ref extensionObject, createIfMissing); }
  }
  
  [global::System.Serializable, global::ProtoBuf.ProtoContract(Name=@"FrRxChangeAsk")]
  public partial class FrRxChangeAsk : global::ProtoBuf.IExtensible
  {
    public FrRxChangeAsk() {}
    
    private string _HostId;
    [global::ProtoBuf.ProtoMember(1, IsRequired = true, Name=@"HostId", DataFormat = global::ProtoBuf.DataFormat.Default)]
    public string HostId
    {
      get { return _HostId; }
      set { _HostId = value; }
    }
    private string _Frecuency;
    [global::ProtoBuf.ProtoMember(2, IsRequired = true, Name=@"Frecuency", DataFormat = global::ProtoBuf.DataFormat.Default)]
    public string Frecuency
    {
      get { return _Frecuency; }
      set { _Frecuency = value; }
    }
    private bool _Rx;
    [global::ProtoBuf.ProtoMember(3, IsRequired = true, Name=@"Rx", DataFormat = global::ProtoBuf.DataFormat.Default)]
    public bool Rx
    {
      get { return _Rx; }
      set { _Rx = value; }
    }
    private global::ProtoBuf.IExtension extensionObject;
    global::ProtoBuf.IExtension global::ProtoBuf.IExtensible.GetExtensionObject(bool createIfMissing)
      { return global::ProtoBuf.Extensible.GetExtensionObject(ref extensionObject, createIfMissing); }
  }
  
  [global::System.Serializable, global::ProtoBuf.ProtoContract(Name=@"FrTxChangeAsk")]
  public partial class FrTxChangeAsk : global::ProtoBuf.IExtensible
  {
    public FrTxChangeAsk() {}
    
    private string _HostId;
    [global::ProtoBuf.ProtoMember(1, IsRequired = true, Name=@"HostId", DataFormat = global::ProtoBuf.DataFormat.Default)]
    public string HostId
    {
      get { return _HostId; }
      set { _HostId = value; }
    }
    private string _Frecuency;
    [global::ProtoBuf.ProtoMember(2, IsRequired = true, Name=@"Frecuency", DataFormat = global::ProtoBuf.DataFormat.Default)]
    public string Frecuency
    {
      get { return _Frecuency; }
      set { _Frecuency = value; }
    }
    private bool _Tx;
    [global::ProtoBuf.ProtoMember(3, IsRequired = true, Name=@"Tx", DataFormat = global::ProtoBuf.DataFormat.Default)]
    public bool Tx
    {
      get { return _Tx; }
      set { _Tx = value; }
    }

    private uint _PttType = default(uint);
    [global::ProtoBuf.ProtoMember(4, IsRequired = false, Name=@"PttType", DataFormat = global::ProtoBuf.DataFormat.TwosComplement)]
    [global::System.ComponentModel.DefaultValue(default(uint))]
    public uint PttType
    {
      get { return _PttType; }
      set { _PttType = value; }
    }

    private bool _CheckAlreadyAssigned = default(bool);
    [global::ProtoBuf.ProtoMember(5, IsRequired = false, Name=@"CheckAlreadyAssigned", DataFormat = global::ProtoBuf.DataFormat.Default)]
    [global::System.ComponentModel.DefaultValue(default(bool))]
    public bool CheckAlreadyAssigned
    {
      get { return _CheckAlreadyAssigned; }
      set { _CheckAlreadyAssigned = value; }
    }
    private global::ProtoBuf.IExtension extensionObject;
    global::ProtoBuf.IExtension global::ProtoBuf.IExtensible.GetExtensionObject(bool createIfMissing)
      { return global::ProtoBuf.Extensible.GetExtensionObject(ref extensionObject, createIfMissing); }
  }
  
  [global::System.Serializable, global::ProtoBuf.ProtoContract(Name=@"FrTxAssigned")]
  public partial class FrTxAssigned : global::ProtoBuf.IExtensible
  {
    public FrTxAssigned() {}
    
    private string _Frecuency;
    [global::ProtoBuf.ProtoMember(1, IsRequired = true, Name=@"Frecuency", DataFormat = global::ProtoBuf.DataFormat.Default)]
    public string Frecuency
    {
      get { return _Frecuency; }
      set { _Frecuency = value; }
    }
    private string _UserId;
    [global::ProtoBuf.ProtoMember(2, IsRequired = true, Name=@"UserId", DataFormat = global::ProtoBuf.DataFormat.Default)]
    public string UserId
    {
      get { return _UserId; }
      set { _UserId = value; }
    }
    private global::ProtoBuf.IExtension extensionObject;
    global::ProtoBuf.IExtension global::ProtoBuf.IExtensible.GetExtensionObject(bool createIfMissing)
      { return global::ProtoBuf.Extensible.GetExtensionObject(ref extensionObject, createIfMissing); }
  }
  
  [global::System.Serializable, global::ProtoBuf.ProtoContract(Name=@"PttChangeAsk")]
  public partial class PttChangeAsk : global::ProtoBuf.IExtensible
  {
    public PttChangeAsk() {}
    
    private string _HostId;
    [global::ProtoBuf.ProtoMember(1, IsRequired = true, Name=@"HostId", DataFormat = global::ProtoBuf.DataFormat.Default)]
    public string HostId
    {
      get { return _HostId; }
      set { _HostId = value; }
    }
    private U5ki.Infrastructure.PttSource _Src;
    [global::ProtoBuf.ProtoMember(2, IsRequired = true, Name=@"Src", DataFormat = global::ProtoBuf.DataFormat.TwosComplement)]
    public U5ki.Infrastructure.PttSource Src
    {
      get { return _Src; }
      set { _Src = value; }
    }
    private global::ProtoBuf.IExtension extensionObject;
    global::ProtoBuf.IExtension global::ProtoBuf.IExtensible.GetExtensionObject(bool createIfMissing)
      { return global::ProtoBuf.Extensible.GetExtensionObject(ref extensionObject, createIfMissing); }
  }
  
  [global::System.Serializable, global::ProtoBuf.ProtoContract(Name=@"RtxGroupChangeAsk")]
  public partial class RtxGroupChangeAsk : global::ProtoBuf.IExtensible
  {
    public RtxGroupChangeAsk() {}
    
    private string _HostId;
    [global::ProtoBuf.ProtoMember(1, IsRequired = true, Name=@"HostId", DataFormat = global::ProtoBuf.DataFormat.Default)]
    public string HostId
    {
      get { return _HostId; }
      set { _HostId = value; }
    }
    private uint _GroupId;
    [global::ProtoBuf.ProtoMember(2, IsRequired = true, Name=@"GroupId", DataFormat = global::ProtoBuf.DataFormat.TwosComplement)]
    public uint GroupId
    {
      get { return _GroupId; }
      set { _GroupId = value; }
    }
    private readonly global::System.Collections.Generic.List<string> _FrIds = new global::System.Collections.Generic.List<string>();
    [global::ProtoBuf.ProtoMember(3, Name=@"FrIds", DataFormat = global::ProtoBuf.DataFormat.Default)]
    public global::System.Collections.Generic.List<string> FrIds
    {
      get { return _FrIds; }
    }
  
    private readonly global::System.Collections.Generic.List<U5ki.Infrastructure.RtxGroupChangeAsk.ChangeType> _Changes = new global::System.Collections.Generic.List<U5ki.Infrastructure.RtxGroupChangeAsk.ChangeType>();
    [global::ProtoBuf.ProtoMember(4, Name=@"Changes", DataFormat = global::ProtoBuf.DataFormat.TwosComplement)]
    public global::System.Collections.Generic.List<U5ki.Infrastructure.RtxGroupChangeAsk.ChangeType> Changes
    {
      get { return _Changes; }
    }
  
    [global::ProtoBuf.ProtoContract(Name=@"ChangeType")]
    public enum ChangeType
    {
            
      [global::ProtoBuf.ProtoEnum(Name=@"NoChanged", Value=0)]
      NoChanged = 0,
            
      [global::ProtoBuf.ProtoEnum(Name=@"Add", Value=1)]
      Add = 1,
            
      [global::ProtoBuf.ProtoEnum(Name=@"Delete", Value=2)]
      Delete = 2
    }
  
    private global::ProtoBuf.IExtension extensionObject;
    global::ProtoBuf.IExtension global::ProtoBuf.IExtensible.GetExtensionObject(bool createIfMissing)
      { return global::ProtoBuf.Extensible.GetExtensionObject(ref extensionObject, createIfMissing); }
  }
  
  [global::System.Serializable, global::ProtoBuf.ProtoContract(Name=@"SelcalPrepareMsg")]
  public partial class SelcalPrepareMsg : global::ProtoBuf.IExtensible
  {
    public SelcalPrepareMsg() {}
    
    private string _HostId;
    [global::ProtoBuf.ProtoMember(1, IsRequired = true, Name=@"HostId", DataFormat = global::ProtoBuf.DataFormat.Default)]
    public string HostId
    {
      get { return _HostId; }
      set { _HostId = value; }
    }
    private bool _OnOff;
    [global::ProtoBuf.ProtoMember(2, IsRequired = true, Name=@"OnOff", DataFormat = global::ProtoBuf.DataFormat.Default)]
    public bool OnOff
    {
      get { return _OnOff; }
      set { _OnOff = value; }
    }
    private string _Code;
    [global::ProtoBuf.ProtoMember(3, IsRequired = true, Name=@"Code", DataFormat = global::ProtoBuf.DataFormat.Default)]
    public string Code
    {
      get { return _Code; }
      set { _Code = value; }
    }
    private global::ProtoBuf.IExtension extensionObject;
    global::ProtoBuf.IExtension global::ProtoBuf.IExtensible.GetExtensionObject(bool createIfMissing)
      { return global::ProtoBuf.Extensible.GetExtensionObject(ref extensionObject, createIfMissing); }
  }
  
  [global::System.Serializable, global::ProtoBuf.ProtoContract(Name=@"SelcalPrepareRsp")]
  public partial class SelcalPrepareRsp : global::ProtoBuf.IExtensible
  {
    public SelcalPrepareRsp() {}
    
    private bool _resultado;
    [global::ProtoBuf.ProtoMember(1, IsRequired = true, Name=@"resultado", DataFormat = global::ProtoBuf.DataFormat.Default)]
    public bool resultado
    {
      get { return _resultado; }
      set { _resultado = value; }
    }
    private string _Frecuency;
    [global::ProtoBuf.ProtoMember(2, IsRequired = true, Name=@"Frecuency", DataFormat = global::ProtoBuf.DataFormat.Default)]
    public string Frecuency
    {
      get { return _Frecuency; }
      set { _Frecuency = value; }
    }
    private string _mensaje;
    [global::ProtoBuf.ProtoMember(3, IsRequired = true, Name=@"mensaje", DataFormat = global::ProtoBuf.DataFormat.Default)]
    public string mensaje
    {
      get { return _mensaje; }
      set { _mensaje = value; }
    }
    private string _Code;
    [global::ProtoBuf.ProtoMember(4, IsRequired = true, Name=@"Code", DataFormat = global::ProtoBuf.DataFormat.Default)]
    public string Code
    {
      get { return _Code; }
      set { _Code = value; }
    }
    private global::ProtoBuf.IExtension extensionObject;
    global::ProtoBuf.IExtension global::ProtoBuf.IExtensible.GetExtensionObject(bool createIfMissing)
      { return global::ProtoBuf.Extensible.GetExtensionObject(ref extensionObject, createIfMissing); }
  }
  
  [global::System.Serializable, global::ProtoBuf.ProtoContract(Name=@"ChangeSiteMsg")]
  public partial class ChangeSiteMsg : global::ProtoBuf.IExtensible
  {
    public ChangeSiteMsg() {}
    
    private string _HostId;
    [global::ProtoBuf.ProtoMember(1, IsRequired = true, Name=@"HostId", DataFormat = global::ProtoBuf.DataFormat.Default)]
    public string HostId
    {
      get { return _HostId; }
      set { _HostId = value; }
    }
    private string _Frequency;
    [global::ProtoBuf.ProtoMember(2, IsRequired = true, Name=@"Frequency", DataFormat = global::ProtoBuf.DataFormat.Default)]
    public string Frequency
    {
      get { return _Frequency; }
      set { _Frequency = value; }
    }
    private string _Alias;
    [global::ProtoBuf.ProtoMember(3, IsRequired = true, Name=@"Alias", DataFormat = global::ProtoBuf.DataFormat.Default)]
    public string Alias
    {
      get { return _Alias; }
      set { _Alias = value; }
    }
    private global::ProtoBuf.IExtension extensionObject;
    global::ProtoBuf.IExtension global::ProtoBuf.IExtensible.GetExtensionObject(bool createIfMissing)
      { return global::ProtoBuf.Extensible.GetExtensionObject(ref extensionObject, createIfMissing); }
  }
  
  [global::System.Serializable, global::ProtoBuf.ProtoContract(Name=@"ChangeSiteRsp")]
  public partial class ChangeSiteRsp : global::ProtoBuf.IExtensible
  {
    public ChangeSiteRsp() {}
    
    private bool _resultado;
    [global::ProtoBuf.ProtoMember(1, IsRequired = true, Name=@"resultado", DataFormat = global::ProtoBuf.DataFormat.Default)]
    public bool resultado
    {
      get { return _resultado; }
      set { _resultado = value; }
    }
    private string _Frecuency;
    [global::ProtoBuf.ProtoMember(2, IsRequired = true, Name=@"Frecuency", DataFormat = global::ProtoBuf.DataFormat.Default)]
    public string Frecuency
    {
      get { return _Frecuency; }
      set { _Frecuency = value; }
    }
    private string _Alias;
    [global::ProtoBuf.ProtoMember(3, IsRequired = true, Name=@"Alias", DataFormat = global::ProtoBuf.DataFormat.Default)]
    public string Alias
    {
      get { return _Alias; }
      set { _Alias = value; }
    }
    private global::ProtoBuf.IExtension extensionObject;
    global::ProtoBuf.IExtension global::ProtoBuf.IExtensible.GetExtensionObject(bool createIfMissing)
      { return global::ProtoBuf.Extensible.GetExtensionObject(ref extensionObject, createIfMissing); }
  }
  
  [global::System.Serializable, global::ProtoBuf.ProtoContract(Name=@"FrChangeAsk")]
  public partial class FrChangeAsk : global::ProtoBuf.IExtensible
  {
    public FrChangeAsk() {}
    
    private string _HostId;
    [global::ProtoBuf.ProtoMember(1, IsRequired = true, Name=@"HostId", DataFormat = global::ProtoBuf.DataFormat.Default)]
    public string HostId
    {
      get { return _HostId; }
      set { _HostId = value; }
    }
    private string _IdFrecuency;
    [global::ProtoBuf.ProtoMember(2, IsRequired = true, Name=@"IdFrecuency", DataFormat = global::ProtoBuf.DataFormat.Default)]
    public string IdFrecuency
    {
      get { return _IdFrecuency; }
      set { _IdFrecuency = value; }
    }
    private string _NewFrecuency;
    [global::ProtoBuf.ProtoMember(3, IsRequired = true, Name=@"NewFrecuency", DataFormat = global::ProtoBuf.DataFormat.Default)]
    public string NewFrecuency
    {
      get { return _NewFrecuency; }
      set { _NewFrecuency = value; }
    }
    private global::ProtoBuf.IExtension extensionObject;
    global::ProtoBuf.IExtension global::ProtoBuf.IExtensible.GetExtensionObject(bool createIfMissing)
      { return global::ProtoBuf.Extensible.GetExtensionObject(ref extensionObject, createIfMissing); }
  }
  
  [global::System.Serializable, global::ProtoBuf.ProtoContract(Name=@"FrChangeRsp")]
  public partial class FrChangeRsp : global::ProtoBuf.IExtensible
  {
    public FrChangeRsp() {}
    
    private bool _resultado;
    [global::ProtoBuf.ProtoMember(1, IsRequired = true, Name=@"resultado", DataFormat = global::ProtoBuf.DataFormat.Default)]
    public bool resultado
    {
      get { return _resultado; }
      set { _resultado = value; }
    }
    private string _IdFrecuency;
    [global::ProtoBuf.ProtoMember(2, IsRequired = true, Name=@"IdFrecuency", DataFormat = global::ProtoBuf.DataFormat.Default)]
    public string IdFrecuency
    {
      get { return _IdFrecuency; }
      set { _IdFrecuency = value; }
    }
    private string _AssignedFrecuency;
    [global::ProtoBuf.ProtoMember(3, IsRequired = true, Name=@"AssignedFrecuency", DataFormat = global::ProtoBuf.DataFormat.Default)]
    public string AssignedFrecuency
    {
      get { return _AssignedFrecuency; }
      set { _AssignedFrecuency = value; }
    }
    private uint _Code;
    [global::ProtoBuf.ProtoMember(4, IsRequired = true, Name=@"Code", DataFormat = global::ProtoBuf.DataFormat.TwosComplement)]
    public uint Code
    {
      get { return _Code; }
      set { _Code = value; }
    }
    private global::ProtoBuf.IExtension extensionObject;
    global::ProtoBuf.IExtension global::ProtoBuf.IExtensible.GetExtensionObject(bool createIfMissing)
      { return global::ProtoBuf.Extensible.GetExtensionObject(ref extensionObject, createIfMissing); }
  }
  
  [global::System.Serializable, global::ProtoBuf.ProtoContract(Name=@"ConferenceStatus")]
  public partial class ConferenceStatus : global::ProtoBuf.IExtensible
  {
    public ConferenceStatus() {}
    
    private string _RoomName;
    [global::ProtoBuf.ProtoMember(1, IsRequired = true, Name=@"RoomName", DataFormat = global::ProtoBuf.DataFormat.Default)]
    public string RoomName
    {
      get { return _RoomName; }
      set { _RoomName = value; }
    }
    private U5ki.Infrastructure.ConferenceStatus.ConfStatus _Status;
    [global::ProtoBuf.ProtoMember(2, IsRequired = true, Name=@"Status", DataFormat = global::ProtoBuf.DataFormat.TwosComplement)]
    public U5ki.Infrastructure.ConferenceStatus.ConfStatus Status
    {
      get { return _Status; }
      set { _Status = value; }
    }
    private readonly global::System.Collections.Generic.List<string> _ActiveParticipants = new global::System.Collections.Generic.List<string>();
    [global::ProtoBuf.ProtoMember(3, Name=@"ActiveParticipants", DataFormat = global::ProtoBuf.DataFormat.Default)]
    public global::System.Collections.Generic.List<string> ActiveParticipants
    {
      get { return _ActiveParticipants; }
    }
  
    [global::ProtoBuf.ProtoContract(Name=@"ConfStatus")]
    public enum ConfStatus
    {
            
      [global::ProtoBuf.ProtoEnum(Name=@"Ok", Value=0)]
      Ok = 0,
            
      [global::ProtoBuf.ProtoEnum(Name=@"Error", Value=1)]
      Error = 1
    }
  
    private global::ProtoBuf.IExtension extensionObject;
    global::ProtoBuf.IExtension global::ProtoBuf.IExtensible.GetExtensionObject(bool createIfMissing)
      { return global::ProtoBuf.Extensible.GetExtensionObject(ref extensionObject, createIfMissing); }
  }
  
  [global::System.Serializable, global::ProtoBuf.ProtoContract(Name=@"SirTapAudAlarmMsg")]
  public partial class SirTapAudAlarmMsg : global::ProtoBuf.IExtensible
  {
    public SirTapAudAlarmMsg() {}
    
    private string _identity;
    [global::ProtoBuf.ProtoMember(1, IsRequired = true, Name=@"identity", DataFormat = global::ProtoBuf.DataFormat.Default)]
    public string identity
    {
      get { return _identity; }
      set { _identity = value; }
    }
    private U5ki.Infrastructure.SirTapAudAlarmMsg.EventType _eventtype;
    [global::ProtoBuf.ProtoMember(2, IsRequired = true, Name=@"eventtype", DataFormat = global::ProtoBuf.DataFormat.TwosComplement)]
    public U5ki.Infrastructure.SirTapAudAlarmMsg.EventType eventtype
    {
      get { return _eventtype; }
      set { _eventtype = value; }
    }
    private U5ki.Infrastructure.SirTapAudAlarmMsg.Category _category;
    [global::ProtoBuf.ProtoMember(3, IsRequired = true, Name=@"category", DataFormat = global::ProtoBuf.DataFormat.TwosComplement)]
    public U5ki.Infrastructure.SirTapAudAlarmMsg.Category category
    {
      get { return _category; }
      set { _category = value; }
    }
    [global::ProtoBuf.ProtoContract(Name=@"EventType")]
    public enum EventType
    {
            
      [global::ProtoBuf.ProtoEnum(Name=@"Stop", Value=0)]
      Stop = 0,
            
      [global::ProtoBuf.ProtoEnum(Name=@"Start", Value=1)]
      Start = 1,
            
      [global::ProtoBuf.ProtoEnum(Name=@"Cancel", Value=2)]
      Cancel = 2
    }
  
    [global::ProtoBuf.ProtoContract(Name=@"Category")]
    public enum Category
    {
            
      [global::ProtoBuf.ProtoEnum(Name=@"Alert", Value=0)]
      Alert = 0,
            
      [global::ProtoBuf.ProtoEnum(Name=@"Alarm", Value=1)]
      Alarm = 1
    }
  
    private global::ProtoBuf.IExtension extensionObject;
    global::ProtoBuf.IExtension global::ProtoBuf.IExtensible.GetExtensionObject(bool createIfMissing)
      { return global::ProtoBuf.Extensible.GetExtensionObject(ref extensionObject, createIfMissing); }
  }
  
}
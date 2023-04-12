using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Nucleo.CheckIO
{
    class CMediaDevManager : ISndDevManager
    {
        protected List<GenericHid.HidDeviceManagement.DeviceIdentification> _did = new List<GenericHid.HidDeviceManagement.DeviceIdentification>()
        {
            new GenericHid.HidDeviceManagement.DeviceIdentification(){ id="C-Media", vid="VID_0D8C", pid="PID_0178"},
            new GenericHid.HidDeviceManagement.DeviceIdentification(){ id="C-Media-1", vid="VID_0D8C", pid="PID_01A4"}
        };
        protected List<GenericHid.HidDeviceManagement.DeviceDescription> _dev_desc = null;
        
        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public override byte Init()
        {
            _dev_desc = GenericHid.HidDeviceManagement.DeviceList(_did);

            foreach (GenericHid.HidDeviceManagement.DeviceDescription _dd in _dev_desc)
            {
                ISoundDevice _dev = new CMediaSoundDevice(_dd);
                if (_dev.IsValid)
                {
                    // _dev.Id = _id++;
                    _sndDevs[_dev.Id] = _dev;
                }
            }            
            
            return (byte )_sndDevs.Count;
        }
    }
}

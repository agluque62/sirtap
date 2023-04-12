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
            new GenericHid.HidDeviceManagement.DeviceIdentification(){ id="C-Media", vid="VID_0D8C", pid="*"}
        };
        protected List<GenericHid.HidDeviceManagement.DeviceDescription> _dev_desc = null;
        
        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public override byte Init()
        {
            int orden = 0;
            _dev_desc = GenericHid.HidDeviceManagement.DeviceList(_did);

            foreach (GenericHid.HidDeviceManagement.DeviceDescription _dd in _dev_desc)
            {
                ISoundDevice _dev = new CMediaSoundDevice(_dd, orden++);
                if (_dev.IsValid)
                {
                    // _dev.Id = _id++;
                    _sndDevs[_dev.Id] = _dev;
                }
            }            
            
            return (byte )_sndDevs.Count;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public string DevList()
        {
            StringBuilder str = new StringBuilder();

            if (_dev_desc.Count == 0)
                str.Append("None");
            else
            {
                int orden = 0;
                str.Append(String.Format("{0}", _dev_desc.Count));
                foreach (GenericHid.HidDeviceManagement.DeviceDescription _devdes in _dev_desc)
                {
                    String[] val = _devdes.path.Split('#');
                    str.Append(String.Format("\n\t{0}: {1}-{2}", _devdes.id, orden++, val[1]));
                }
            }

            return str.ToString();
        }
    }
}

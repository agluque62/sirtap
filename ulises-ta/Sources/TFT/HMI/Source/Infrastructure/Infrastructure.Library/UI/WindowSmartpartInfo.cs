//----------------------------------------------------------------------------------------
// patterns & practices - Smart Client Software Factory - Guidance Package
//
// This file was generated by this guidance package as part of the solution template
//
// The WindowSmartPartInfo class allows passing the extra information to the WindowWorkpsace
// 
// For more information see: 
// ms-help://MS.VSCC.v80/MS.VSIPCC.v80/ms.practices.scsf.2007may/SCSF/html/03-01-010-How_to_Create_Smart_Client_Solutions.htm
//
// Latest version of this Guidance Package: http://go.microsoft.com/fwlink/?LinkId=62182
//----------------------------------------------------------------------------------------

using System;
using System.Collections.Generic;
using System.Text;

namespace HMI.Infrastructure.Library.UI
{
    public class WindowSmartPartInfo : Microsoft.Practices.CompositeUI.WinForms.WindowSmartPartInfo
    {
        private Dictionary<string, object> _settings = new Dictionary<string, object>();

        public IDictionary<string, object> Keys
        {
            get { return _settings; }
        }
    }
}

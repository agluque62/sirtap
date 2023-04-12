﻿//----------------------------------------------------------------------------------------
// patterns & practices - Smart Client Software Factory - Guidance Package
//
// This file was generated by this guidance package as part of the solution template
//
// The ISolutionProfileParser interface is used along with the XmlStreamDependentModuleEnumerator
// to provide parsing services of a profile catalog xml. It returns an array of IModuleInfo
// 
// For more information see: 
// ms-help://MS.VSCC.v80/MS.VSIPCC.v80/ms.practices.scsf.2007may/SCSF/html/03-01-010-How_to_Create_Smart_Client_Solutions.htm
//
// Latest version of this Guidance Package: http://go.microsoft.com/fwlink/?LinkId=62182
//----------------------------------------------------------------------------------------

using Microsoft.Practices.CompositeUI.Configuration;

namespace HMI.Infrastructure.Library.Services
{
    public interface ISolutionProfileParser
    {
        IModuleInfo[] Parse(string xml);
    }
}

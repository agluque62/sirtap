//----------------------------------------------------------------------------------------
// patterns & practices - Smart Client Software Factory - Guidance Package
//
// This file was generated by the "Add Business Module" recipe.
//
// For more information see: 
// ms-help://MS.VSCC.v80/MS.VSIPCC.v80/ms.practices.scsf.2007may/SCSF/html/02-08-060-Add_Business_Module_Next_Steps.htm
//
// Latest version of this Guidance Package: http://go.microsoft.com/fwlink/?LinkId=62182
//----------------------------------------------------------------------------------------

namespace HMI.Presentation.Sirtap.Constants
{
    /// <summary>
    /// Constants for event topic names.
    /// </summary>
    //LALM 210224 Errores #4755 confirmación de cambio de página radio
    public class EventTopicNames : HMI.Model.Module.Constants.EventTopicNames
    {
        public const string DependencesNumberCalled = "DependencesNumberCalled";
        public const string CambioPaginaRadioUp = "CambioPaginaRadioUp";
        public const string CambioPaginaRadioDown = "CambioPaginaRadioDown";
        public const string CambioConferenciaPreprogramada = "CambioConferenciaPreprogramada";
    }
}

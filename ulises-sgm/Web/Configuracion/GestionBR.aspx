<%@ Page Language="C#"  MasterPageFile="~/CD40.master" AutoEventWireup="true" CodeFile="GestionBR.aspx.cs" Inherits="GestionBR"
    CodeFileBaseClass="PageBaseCD40.PageCD40" Title="GESTIÓN DE BACKUP/RESTORE" EnableEventValidation="false" StylesheetTheme="TemaPaginasConfiguracion" meta:resourcekey="PageResource1" %>

<%@ Register Assembly="AjaxControlToolkit" Namespace="AjaxControlToolkit" TagPrefix="asp" %>

<asp:Content ID="Content1" ContentPlaceHolderID="ContentPlaceHolder1" Runat="Server">
    <asp:Label ID="Label6" runat="server" Text="GESTIÓN DE BACKUP/RESTORE" 
		 CssClass="labelPagina" meta:resourcekey="Label6Resource1"></asp:Label>

	<asp:ListBox ID="LBIDBR" runat="server" AutoPostBack="True" 
		 OnSelectedIndexChanged="LBIDBR_SelectedIndexChanged" 
		 SkinID="MascaraListaElementos" meta:resourcekey="LBIDBRResource1" />

	<asp:LinkButton ID="BtnBackup" runat="server" OnClick="BtnBackup_Click" 
		 CausesValidation="False" Text="BackUp" SkinID="MascaraBotonNuevo" 
		 meta:resourcekey="BtnNuevoResource1"/>

<asp:LinkButton ID="BtnEliminar" runat="server" OnClick="BtnEliminar_Click" 
		 CausesValidation="False" Text="Eliminar" SkinID="MascaraBotonEliminar" 
		 meta:resourcekey="BtnEliminarResource1"/>

	<ajaxToolKit:ConfirmButtonExtender ID="BtnEliminar_ConfirmButtonExtender" 
        runat="server" ConfirmText="Eliminar" Enabled="True" TargetControlID="BtnEliminar"> 
    </ajaxToolKit:ConfirmButtonExtender>

	<asp:LinkButton ID="BtnRestore" runat="server" OnClick="BtnRestore_Click" 
		 CausesValidation="False" Text="Restore" SkinID="MascaraBotonModificar" 
		 meta:resourcekey="BtnRestoreResource1"/>

	<asp:LinkButton ID="BtnCancelar" runat="server" CausesValidation="False" 
		 OnClick="BtnCancelar_Click" Text="Cancelar" SkinID="MascaraBotonCancelar" 
		 meta:resourcekey="BtnCancelarResource1"/>

	<ajaxToolKit:ConfirmButtonExtender ID="BtnCancelar_ConfirmButtonExtender" 
        runat="server" ConfirmText="" Enabled="True" TargetControlID="BtnCancelar">
    </ajaxToolKit:ConfirmButtonExtender>

	<asp:Panel ID="Panel1" runat="server" BorderStyle="Inset" Height="256px" Style="z-index: 105;
		left: 238px; position: absolute; top: 60px" Width="529px" Enabled="False" 
		 meta:resourcekey="Panel1Resource1">

		<asp:Label style="Z-INDEX: 100; LEFT: 12px; POSITION: absolute; TOP: 14px" 
			id="LBLPATH" runat="server" Text="Directorio Servidor Activo" meta:resourcekey="LBLPATHResource1"></asp:Label>

		<asp:TextBox style="Z-INDEX: 203; LEFT: 12px; POSITION: absolute; TOP: 34px" 
			id="TBPATH" MaxLength="64" runat="server" meta:resourcekey="TBPATHResource1"></asp:TextBox>

		<asp:Label style="Z-INDEX: 100; LEFT: 180px; POSITION: absolute; TOP: 78px" 
			id="LBLIDBR" runat="server" Text="Identificador de Fichero" meta:resourcekey="LBLIDBRResource1"></asp:Label>

		<asp:TextBox style="Z-INDEX: 203; LEFT: 180px; POSITION: absolute; TOP: 98px" 
			id="TBIDBR" MaxLength="20" runat="server" meta:resourcekey="TBIDBRResource1"></asp:TextBox>  

        <asp:RequiredFieldValidator ID="RequiredFieldIdentificador" runat="server" ControlToValidate="TBIDBR"
                ErrorMessage="Rellene identificador de Backup" Style="z-index: 120; left: 12px; position: absolute; top: 36px; width: 10px; height: 19px;"
                Visible="False" SetFocusOnError="True"
                meta:resourcekey="RequiredFieldIdentificadorResource1">*</asp:RequiredFieldValidator>

        <asp:RequiredFieldValidator ID="RequiredFieldValidator1" runat="server" ControlToValidate="TBIDBR"
                ErrorMessage="Identificador de Backup invalido" Style="z-index: 121; left: 12px; position: absolute; top: 38px; width: 10px; height: 19px;"
                Visible="False" SetFocusOnError="True"
                meta:resourcekey="RequiredFieldIdentificadorResource2">*</asp:RequiredFieldValidator>
		&nbsp;
		<asp:Label ID="LblErrorConfirmacion" runat="server" ForeColor="Red" Style="z-index: 117;
			left: 420px; position: absolute; top: 82px" Text="*" Visible="False" 
			Width="11px" meta:resourcekey="LblErrorConfirmacionResource"></asp:Label>

		<asp:Label style="Z-INDEX: 100; LEFT: 12px; POSITION: absolute; TOP: 70px" 
			id="progreso" runat="server" ></asp:Label>
		<asp:Label style="Z-INDEX: 100; LEFT: 12px; POSITION: absolute; TOP: 70px" 
			id="porciento" runat="server" ></asp:Label>

	</asp:Panel>

	&nbsp;&nbsp;
	<asp:Button ID="BtnAceptar" runat="server" OnClick="BtnAceptar_Click" Style="z-index: 106;
		left: 429px; position: absolute; top: 335px" Text="Aceptar" Visible="False" 
		 meta:resourcekey="BtnAceptarResource1" />

	<asp:ValidationSummary ID="ValidationSummary1" runat="server" Style="z-index: 108;
		left: 228px; position: absolute; top: 403px" 
		 meta:resourcekey="ValidationSummary1Resource1" />

	<asp:Label ID="LblMensajeConfirmacion" runat="server" ForeColor="Red" Style="z-index: 109;
		left: 235px; position: absolute; top: 380px" Text="Confirme la clave." Visible="False"
		Width="551px" meta:resourcekey="LblMensajeConfirmacionResource1"></asp:Label>
	&nbsp;

</asp:Content>


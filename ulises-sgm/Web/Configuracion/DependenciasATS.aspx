<%@ Page Language="C#"  MasterPageFile="~/CD40.master" AutoEventWireup="true" CodeFile="DependenciasATS.aspx.cs" Inherits="DependenciasATS"
    CodeFileBaseClass="PageBaseCD40.PageCD40" Title="Gestión de Dependencias ATS" EnableEventValidation="false" StylesheetTheme="TemaPaginasConfiguracion" meta:resourcekey="PageResource1" %>
<asp:Content ID="Content1" ContentPlaceHolderID="ContentPlaceHolder1" Runat="Server">
   
    <asp:Label ID="Label6" runat="server" Text="GESTIÓN DE DEPENDENCIAS ATS" 
		 CssClass="labelPagina" meta:resourcekey="Label6Resource1"></asp:Label>

	<asp:ListBox ID="LBDependenciasATS" runat="server" AutoPostBack="True" 
		 OnSelectedIndexChanged="LBDependenciasATS_SelectedIndexChanged" 
		 SkinID="MascaraListaElementos" meta:resourcekey="LBDependenciasATSResource1" />

	<asp:LinkButton ID="BtnNuevo" runat="server" OnClick="BtnNuevo_Click" 
		 CausesValidation="False" Text="Nuevo" SkinID="MascaraBotonNuevo" 
		 meta:resourcekey="BtnNuevoResource1"/>

	<asp:LinkButton ID="BtnEliminar" runat="server" OnClick="BtnEliminar_Click" 
		 CausesValidation="False" Text="Eliminar" SkinID="MascaraBotonEliminar" 
		 meta:resourcekey="BtnEliminarResource1"/>

	<ajaxToolKit:ConfirmButtonExtender ID="BtnEliminar_ConfirmButtonExtender" 
        runat="server" ConfirmText="" Enabled="True" TargetControlID="BtnEliminar"> 
    </ajaxToolKit:ConfirmButtonExtender>

	<asp:LinkButton ID="BtnModificar" runat="server" OnClick="BtnModificar_Click" 
		 CausesValidation="False" Text="Modificar" SkinID="MascaraBotonModificar" 
		 meta:resourcekey="BtnModificarResource1"/>

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
			id="Label1" runat="server" Text="Dependencia ATS" meta:resourcekey="Label1Resource1"></asp:Label>

		<asp:TextBox style="Z-INDEX: 103; LEFT: 12px; POSITION: absolute; TOP: 34px" 
			id="TBDependencia" MaxLength="32" runat="server" meta:resourcekey="TBDependenciaResource1"></asp:TextBox>  
          		
        <asp:RequiredFieldValidator ID="RequiredFieldIdentificador" runat="server" ControlToValidate="TBDependencia"
                ErrorMessage="Rellene el campo Dependencia ATS" Style="z-index: 120; left: 12px; position: absolute; top: 36px; width: 10px; height: 19px;"
                Visible="False" SetFocusOnError="True"
                meta:resourcekey="RequiredFieldIdentificadorResource1">*</asp:RequiredFieldValidator>

        <asp:RequiredFieldValidator ID="RequiredFieldValidator1" runat="server" ControlToValidate="TBDependencia"
                ErrorMessage="Identificador de Dependencia ATS invalido" Style="z-index: 121; left: 12px; position: absolute; top: 38px; width: 10px; height: 19px;"
                Visible="False" SetFocusOnError="True"
                meta:resourcekey="RequiredFieldIdentificadorResource2">*</asp:RequiredFieldValidator>
		&nbsp;
		<asp:Label ID="LblErrorConfirmacion" runat="server" ForeColor="Red" Style="z-index: 117;
			left: 420px; position: absolute; top: 82px" Text="*" Visible="False" 
			Width="11px" meta:resourcekey="LblErrorConfirmacionResource"></asp:Label>
	</asp:Panel>

	&nbsp;&nbsp;
	<asp:Button ID="BtnAceptar" runat="server" OnClick="BtnAceptar_Click" Style="z-index: 106;
		left: 429px; position: absolute; top: 335px" Text="Aceptar" Visible="False" 
		 meta:resourcekey="BtnAceptarResource1" />

	<ajaxToolKit:ConfirmButtonExtender ID="BtnAceptar_ConfirmButtonExtender" 
        runat="server" ConfirmText="" Enabled="True" TargetControlID="BtnAceptar">
    </ajaxToolKit:ConfirmButtonExtender>

	<asp:ValidationSummary ID="ValidationSummary1" runat="server" Style="z-index: 108;
		left: 228px; position: absolute; top: 403px" 
		 meta:resourcekey="ValidationSummary1Resource1" />

	<asp:Label ID="LblMensajeConfirmacion" runat="server" ForeColor="Red" Style="z-index: 109;
		left: 235px; position: absolute; top: 380px" Text="Confirme la clave." Visible="False"
		Width="551px" meta:resourcekey="LblMensajeConfirmacionResource1"></asp:Label>
	&nbsp;

<%--Control actividad de sesión    --%>
    <script type="text/javascript">
        function PingSessionSrv() {
            var CONTROLADOR = "PingSrv.aspx";
            var head = document.getElementsByTagName("head").item(0);
            script = document.createElement("script");
            script.src = CONTROLADOR;
            script.setAttribute("type", "text/javascript");
            script.defer = true;
            head.appendChild(script);
        }
    </script>
    <script language="javascript" type="text/javascript">
        setInterval("PingSessionSrv()", 5000);
    </script>
<%--Control actividad de sesión FIN    --%>

</asp:Content>




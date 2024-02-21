<%@ Page Language="C#" MasterPageFile="~/CD40.master" AutoEventWireup="true" CodeFile="Operadores.aspx.cs" Inherits="Operadores" 
CodeFileBaseClass="PageBaseCD40.PageCD40" Title="Gestión de operadores" EnableEventValidation="false" StylesheetTheme="TemaPaginasConfiguracion" meta:resourcekey="PageResource1"%>
<asp:Content ID="Content1" ContentPlaceHolderID="ContentPlaceHolder1" Runat="Server">

<%--    <script type="text/javascript">
        if ("<%=GetWindowName()%>" != "Wizard") {
            if (window.name != "<%=GetWindowName()%>") {
                window.name = "invalidAccess";
                window.open("../BloqueoAplicacion.aspx", "_self");
            }
        }
	</script>
--%>     
    <asp:Label ID="Label6" runat="server" Text="GESTIÓN DE OPERADORES" 
		 CssClass="labelPagina" meta:resourcekey="Label6Resource1"></asp:Label>
	<asp:ListBox ID="LBOperadores" runat="server" AutoPostBack="True" 
		 OnSelectedIndexChanged="LBOperadores_SelectedIndexChanged" 
		 SkinID="MascaraListaElementos" meta:resourcekey="LBOperadoresResource1" />
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
	<asp:Panel ID="Panel1" runat="server" BorderStyle="Inset" Height="289px" Style="z-index: 105;
		left: 238px; position: absolute; top: 60px" Width="529px" Enabled="False" 
		 meta:resourcekey="Panel1Resource1">
		<asp:Label style="Z-INDEX: 100; LEFT: 12px; POSITION: absolute; TOP: 14px" 
			id="Label1" runat="server" Text="Usuario" meta:resourcekey="Label1Resource1"></asp:Label>
		<asp:TextBox style="Z-INDEX: 103; LEFT: 12px; POSITION: absolute; TOP: 34px" MaxLength="32"
			id="TBUsuario" runat="server" meta:resourcekey="TBUsuarioResource1"></asp:TextBox>

		<asp:Label style="Z-INDEX: 102; LEFT: 12px; POSITION: absolute; TOP: 60px" 
			id="Label3" runat="server" Text="Perfil" meta:resourcekey="Label3Resource1"></asp:Label>
		<asp:DropDownList style="Z-INDEX: 105; LEFT: 12px; POSITION: absolute; TOP: 80px" 
			id="DDLPerfil" runat="server"  AutoPostBack="True" OnSelectedIndexChanged="DDLPerfil_SelectedIndexChanged"  Width="140"  class="select"
			meta:resourcekey="DDLPerfilResource1">
			<asp:ListItem Value="0" meta:resourcekey="ListItemResource1">Operador</asp:ListItem>
			<asp:ListItem Value="1" meta:resourcekey="ListItemResource2">T&#233;cnico 1</asp:ListItem>
			<asp:ListItem Value="2" meta:resourcekey="ListItemResource3">T&#233;cnico 2</asp:ListItem>
			<asp:ListItem Value="3" meta:resourcekey="ListItemResource4">T&#233;cnico 3</asp:ListItem>
            <asp:ListItem Value="4" meta:resourcekey="ListItemResource5">Supervisor</asp:ListItem>
            <asp:ListItem Value="5" meta:resourcekey="ListItemResource6">Operador de Misión</asp:ListItem>
			</asp:DropDownList>

		<asp:Label style="Z-INDEX: 108; LEFT: 12px; POSITION: absolute; TOP: 106px" 
			id="Label4" runat="server" Text="Nombre" meta:resourcekey="Label4Resource1"></asp:Label>
		<asp:TextBox style="Z-INDEX: 111; LEFT: 12px; POSITION: absolute; TOP: 126px" MaxLength="32"
			id="TBNombre" runat="server" Width="131px" meta:resourcekey="TBNombreResource1"></asp:TextBox>

		<asp:Label style="Z-INDEX: 109; LEFT: 12px; POSITION: absolute; TOP: 152px" 
			id="Label5" runat="server" Text="Apellidos" meta:resourcekey="Label5Resource1"></asp:Label>
		<asp:TextBox style="Z-INDEX: 112; LEFT: 12px; POSITION: absolute; TOP: 172px" 
			id="TBApellidos" runat="server" Width="131px" MaxLength="32"
			meta:resourcekey="TBApellidosResource1"></asp:TextBox>

		<asp:Label style="Z-INDEX: 110; LEFT: 12px; POSITION: absolute; TOP: 198px" 
			id="Label7" runat="server" Text="Teléfono" meta:resourcekey="Label7Resource1"></asp:Label>
		<asp:TextBox style="Z-INDEX: 113; LEFT: 12px; POSITION: absolute; TOP: 218px" 
			id="TBTelefono" runat="server" Width="131px" 
			meta:resourcekey="TBTelefonoResource1"></asp:TextBox>

		<asp:Label style="Z-INDEX: 110; LEFT: 12px; POSITION: absolute; TOP: 244px" 
			id="LBLTBTimeoutSesion" runat="server" Text="Timeout Sesión" meta:resourcekey="LBLTBTimeoutSesionResource1"></asp:Label>
		<asp:TextBox style="Z-INDEX: 113; LEFT: 12px; POSITION: absolute; TOP: 264px" 
			id="TBTimeoutSesion" runat="server" Width="131px" 
			meta:resourcekey="TBTimeoutSesionResource1"></asp:TextBox>

        <asp:CheckBox ID="CBSeguro" runat="server" Style="z-index: 105; left: 204px; position: absolute; width: 100px;
            top: 80px" Text="Seguro: " TextAlign="Left" Enabled="False" Visible="false"
            AutoPostBack="True" oncheckedchanged="CBSeguro_OnCheckedChanged" Font-Size="Medium" Height="14px"  Font-Names="Arial" 
            meta:resourcekey="LSeguroResource1"/>

		<asp:LinkButton ID="LkBCambiarClave" runat="server" Style="z-index: 116; left: 360px;
			position: absolute; top: 14px" Visible="False" OnClick="LkBCambiarClave_Click" 
			meta:resourcekey="LkBCambiarClaveResource1">Cambiar clave</asp:LinkButton>

		<asp:Label ID="Label9" runat="server" Style="z-index: 118; left: 360px; position: absolute;
			top: 60px" Text="Clave antigua" Visible="False" 
			meta:resourcekey="Label9Resource1"></asp:Label>
		<asp:TextBox ID="TBClaveAntigua" runat="server" Style="z-index: 119; left: 360px; position: absolute;
			top: 80px" Visible="False" Width="145px" TextMode="Password" MaxLength="32"
			meta:resourcekey="TBClaveAntiguaResource1"></asp:TextBox>

		<asp:Label style="Z-INDEX: 101; left: 360px; POSITION: absolute; TOP: 106px" 
			id="Label2" runat="server" Text="Clave" Visible="False" 
			meta:resourcekey="Label2Resource1"></asp:Label>
		<asp:TextBox style="Z-INDEX: 104; left: 360px; POSITION: absolute; TOP: 126px" 
			id="TBClave" runat="server" Visible="False" Width="145px" TextMode="Password" MaxLength="32"
			meta:resourcekey="TBClaveResource1"></asp:TextBox>

		<asp:Label style="Z-INDEX: 106; left: 360px; POSITION: absolute; TOP: 152px" 
			id="Label8" runat="server" Text="Confirma clave" Visible="False" 
			meta:resourcekey="Label8Resource1"></asp:Label>
		<asp:TextBox style="Z-INDEX: 107; left: 360px; POSITION: absolute; TOP: 172px" 
			id="TBConfirmaClave" runat="server" 
				Width="145px" Visible="False" TextMode="Password" MaxLength="32"
			meta:resourcekey="TBConfirmaClaveResource1"></asp:TextBox>	
        		
		<asp:RequiredFieldValidator ID="RequiredFieldValidator2" runat="server" ControlToValidate="TBClave"
			ErrorMessage="Es obligatorio introducir una clave." Style="z-index: 114; left: 509px;
			position: absolute; top: 82px" Width="12px" Visible="False" 
			meta:resourcekey="RequiredFieldValidator2Resource1">*</asp:RequiredFieldValidator>
		<asp:RequiredFieldValidator ID="RequiredFieldValidator1" runat="server" ControlToValidate="TBUsuario"
			ErrorMessage="Es obligatorio introducir un identificador de usuario" Style="z-index: 115;
			left: 230px; position: absolute; top: 14px" Width="10px" 
			meta:resourcekey="RequiredFieldValidator1Resource1">*</asp:RequiredFieldValidator>
		&nbsp;
		<asp:Label ID="LblErrorConfirmacion" runat="server" ForeColor="Red" Style="z-index: 117;
			left: 420px; position: absolute; top: 82px" Text="*" Visible="False" 
			Width="11px" meta:resourcekey="LblErrorConfirmacionResource1"></asp:Label>
		<asp:Label ID="LblErrorClaveAntigua" runat="server" ForeColor="Red" Style="z-index: 121;
			left: 420px; position: absolute; top: 82px" Text="*" Visible="False" 
			meta:resourcekey="LblErrorClaveAntiguaResource1"></asp:Label>

	</asp:Panel>

             <asp:Label ID="LabelDepATS" runat="server" Style="z-index: 122; left:  790px; position: absolute;
                top: 40px" Text="Asignación Dependencias ATS" Visible="true" 
		          meta:resourcekey="Label10Resource1"></asp:Label>

	<asp:Panel ID="PanelDepATS" runat="server" BorderStyle="Inset" Height="289px" Style="z-index: 123;
		        left: 790px; position: absolute; top: 60px" Width="180px" Enabled="False" 
		         meta:resourcekey="Panel10Resource1">
                           
            <asp:Label ID="LabelAsig" runat="server" Style="z-index: 124; left: 14px; position: absolute; top: 1px"
                    Text="Asignados:" Visible="False"
                    meta:resourcekey="LabelAsigResource1"></asp:Label>
            
            <asp:ListBox ID="ListAsignadas" runat="server" Height="100px" Style="z-index: 125; left: 14px; position: absolute; top: 17px"
                    Visible="False" Width="144px"
                    SelectionMode="Multiple" meta:resourcekey="ListDependenciasAsignadas"></asp:ListBox>

            <asp:Label ID="LabelExist" runat="server" Style="z-index: 126; left: 14px; position: absolute; top: 159px"
                    Text="Existentes:" Visible="False"
                    meta:resourcekey="LabelExistResource1"></asp:Label>

            <asp:ListBox ID="ListExistentes" runat="server" Height="100px" Style="z-index:127; left: 14px; position: absolute; top: 175px"
                    Visible="False" Width="144px"
                    SelectionMode="Multiple" meta:resourcekey="ListDependenciasExistentes"></asp:ListBox>

            <asp:ImageButton ID="IButAsignar" runat="server" OnClick="IButAsignar_Click" Style="z-index: 128; left: 32px; position: absolute; top: 120px; -webkit-transform: rotate(180deg); -moz-transform: rotate(180); -o-transform: rotate(180); transform: rotate(180);"
                    CausesValidation="False"
                    ImageUrl="~/Configuracion/Images/arrow.png" Visible="True"
                    meta:resourcekey="IButAsignarResource1" />
                        &nbsp;
            <asp:ImageButton ID="IButQuitar" runat="server" Style="z-index: 129; left: 98px; position: absolute; top: 120px; -webkit-transform: rotate(90deg); -moz-transform: rotate(0deg); -o-transform: rotate(0deg); transform: rotate(0deg);"
                    CausesValidation="False"
                    ImageUrl="~/Configuracion/Images/arrow.png" OnClick="IButQuitar_Click"
                    Visible="True" meta:resourcekey="IButQuitarResource1" />

        </asp:Panel>

	&nbsp;&nbsp;
	<asp:Button ID="BtnAceptar" runat="server" OnClick="BtnAceptar_Click" Style="z-index: 106;
		left: 429px; position: absolute; top: 360px" Text="Aceptar" Visible="False" 
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
	<asp:HiddenField ID="HFClave" runat="server" />

</asp:Content>


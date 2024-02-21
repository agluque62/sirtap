<%@ Page Language="C#" CodeFile="TFTTelefonia.aspx.cs" MasterPageFile="~/CD40.master" Inherits="TFTTelefonia"
	CodeFileBaseClass="PageBaseCD40.PageCD40" Title="Panel Telefon�a" StylesheetTheme="TemaPaginasConfiguracion" meta:resourcekey="PageResource1"%>

<asp:Content ID="Content1" ContentPlaceHolderID="ContentPlaceHolder1" Runat="Server">
<%--     <script type="text/javascript">
        if (window.name != "<%=GetWindowName()%>") {
            window.name = "invalidAccess";
            window.open("../BloqueoAplicacion.aspx", "_self");
        }
    </script>
--%>
<asp:Label ID="Label6" runat="server" Text="TELEFON�A" CssClass="labelPagina" 
		meta:resourcekey="Label6Resource1"></asp:Label>

        <asp:CheckBox ID="CBSeguro" runat="server" Style="z-index: 105; left: 516px; position: absolute;
            top: 75px" Text="Seguro: " TextAlign="Left" Enabled="True"  Visible ="false"
            AutoPostBack="True" oncheckedchanged="CBSeguro_OnCheckedChanged" Font-Size="Medium" Height="14px" Font-Names="Arial"
            meta:resourcekey="LSeguroResource1"/>
       
<asp:ImageButton ID="IButPagAbajo" runat="server"  Style="z-index: 100; -webkit-transform: rotate(90deg); -moz-transform: rotate(90deg); -o-transform: rotate(90deg); transform: rotate(90deg);
    left: 162px; position: absolute; top: 73px" CausesValidation="False" 
		ImageUrl="~/Configuracion/Images/arrow.png" OnClick="IButPagAbajo_Click" 
		meta:resourcekey="IButPagAbajoResource1" />
<asp:ImageButton ID="IButPagArriba" runat="server" Style="z-index: 101; left: 309px; -webkit-transform: rotate(-90deg); -moz-transform: rotate(-90deg); -o-transform: rotate(-90deg); transform: rotate(-90deg);
    position: absolute; top: 73px" CausesValidation="False" 
		ImageUrl="~/Configuracion/Images/arrow.png" OnClick="IButPagArriba_Click" 
		meta:resourcekey="IButPagArribaResource1" />

<asp:Label ID="LabelPag" runat="server" Font-Bold="True" Style="z-index: 102; left: 216px;
    position: absolute; top: 79px" Text="PAGINA " 
		meta:resourcekey="LabelPagResource1"></asp:Label>
            
<asp:Table ID="TEnlacesInternos" runat="server"  Height="76px" Style="left: 53px;
        position: absolute; top: 135px; z-index: 104;" BackColor="Transparent"   BorderStyle="Solid" BorderColor="#eeb44f">

    <asp:TableRow ID="TableEnlacesInternos" runat="server" >
        <asp:TableCell ID="TableCell1" runat="server" Visible="false">
            <asp:Button ID="Button1" runat="server"
                SkinId="BotonPanelTelefonia"
                OnClick="CeldaEnlaceTelefonia_OnClick" CausesValidation="false" />
        </asp:TableCell>

        <asp:TableCell ID="TableCell2" runat="server" Visible="false">
            <asp:Button ID="Button2" runat="server"
                SkinId="BotonPanelTelefonia"
                OnClick="CeldaEnlaceTelefonia_OnClick" CausesValidation="false" />
        </asp:TableCell>        
        <asp:TableCell ID="TableCell3" runat="server" Visible="false">
            <asp:Button ID="Button3" runat="server"
                SkinId="BotonPanelTelefonia"
                OnClick="CeldaEnlaceTelefonia_OnClick" CausesValidation="false" />
        </asp:TableCell>

        <asp:TableCell ID="TableCell4" runat="server" Visible="false">
            <asp:Button ID="Button4" runat="server"
                SkinId="BotonPanelTelefonia"
                OnClick="CeldaEnlaceTelefonia_OnClick" CausesValidation="false" />
        </asp:TableCell>

        <asp:TableCell ID="TableCell5" runat="server" Visible="false">
            <asp:Button ID="Button5" runat="server"
                SkinId="BotonPanelTelefonia"
                OnClick="CeldaEnlaceTelefonia_OnClick" CausesValidation="false" />
        </asp:TableCell>

        <asp:TableCell ID="TableCell6" runat="server" Visible="false">
            <asp:Button ID="Button6" runat="server"
                SkinId="BotonPanelTelefonia"
                OnClick="CeldaEnlaceTelefonia_OnClick" CausesValidation="false" />
        </asp:TableCell>
    </asp:TableRow>

</asp:Table>
    <asp:ListBox ID="LBoxDestinos" runat="server" Height="335px" Style="z-index: 105; left: 516px;
        position: absolute; top: 128px" Width="175px" Rows="100" 
		meta:resourcekey="LBoxDestinosResource1"></asp:ListBox>
    <asp:Label ID="Label1" runat="server" Style="z-index: 106; left: 516px; position: absolute;
        top: 106px" Text="Destinos disponibles:" 
		meta:resourcekey="Label1Resource1"></asp:Label>
    <asp:LinkButton ID="BtRadio" runat="server" Style="z-index: 107; left: 40px; position: absolute;
        top: 430px; height: 19px; width: auto;" Text="Panel Radio" SkinId="LinkButtonCabecera" 
		OnClick="BtRadio_Click" meta:resourcekey="BtRadioResource1" />
    <asp:LinkButton ID="BtLC" runat="server" Style="z-index: 108; left: 210px; position: absolute;
        top: 430px; height: 19px; width: auto;" Text="Panel Linea Caliente" SkinId="LinkButtonCabecera" 
		OnClick="BtLC_Click" meta:resourcekey="BtLCResource1" />

    <asp:LinkButton ID="BtSector" runat="server" Style="z-index: 103; left: 80px; position: absolute;
        top: 550px; width: 180px; height: 35px; background: url('../Configuracion/Images/arrowback.gif') no-repeat left top; 
        padding-left:18px;margin-left: 10px;padding-right:10px;" SkinId="LinkButtonCabecera" 
        Visible="True" 
		OnClick="BtSector_Click" CausesValidation="False"  
		  meta:resourcekey="BtVolverSectorRes1" >
        <asp:Label ID="EnlaceVolverSector" runat="server" Style="display:table;margin:10px auto; vertical-align: middle;"
                Text=" Volver al Sector" Visible="True" meta:resourcekey="EnlaceVolverSectorResource1"></asp:Label>
     </asp:LinkButton>
    <asp:Panel ID="Panel1" runat="server" BackColor="White" BorderColor="Black" BorderStyle="Groove"
        BorderWidth="1px" Height="126px" Style="z-index: 109; left: 163px; position: absolute;
        top: 246px" Visible="False" Width="93px" 
		meta:resourcekey="Panel1Resource1">
        <asp:Button ID="BtLiberar" runat="server" Style="z-index: 100; left: 7px; position: absolute;
            top: 37px" Text="Liberar" Width="80px" OnClick="BtLiberar_Click" 
			  meta:resourcekey="BtLiberarResource1" />
        <asp:Button ID="BModificar" runat="server" Style="z-index: 104; left: 7px; position: absolute;
            top: 65px" Text="Modificar" Width="80px" OnClick="BtModificar_Click" 
			  meta:resourcekey="BModificarResource1" />
        <asp:Button ID="BtCancelar" runat="server" Style="z-index: 101; left: 7px; position: absolute;
            top: 93px" Text="Cancelar" Width="80px" OnClick="BtCancelar_Click" 
			  meta:resourcekey="BtCancelarResource1" />
        <asp:Button ID="BtAsignar" runat="server" Style="z-index: 103; left: 7px; position: absolute;
            top: 9px" Text="Asignar" Width="80px" OnClick="BtAsignar_Click" 
			  meta:resourcekey="BtAsignarResource1" />
    </asp:Panel>
    <asp:Panel ID="Panel2" runat="server" BackColor="White" BorderColor="Black" BorderStyle="Groove"
        BorderWidth="1px" Height="250px" Style="z-index: 111; left: 50px; position: absolute;
        top: 150px" Visible="False" Width="330px" 
		meta:resourcekey="Panel2Resource1">

        <asp:Label ID="Label2" runat="server" Style="z-index: 102; left: 20px; position: absolute;
            top: 23px" Text="Literal:" meta:resourcekey="Label2Resource1"></asp:Label>
        <asp:TextBox ID="TBoxLiteral" runat="server" MaxLength="32" Style="z-index: 101; left: 20px;
            position: absolute; top: 43px" Width="144px" 
			 meta:resourcekey="TBoxLiteralResource1"></asp:TextBox>

        <asp:Label ID="Label4" runat="server" Style="z-index: 107; left: 20px; position: absolute;
            top: 83px" Text="Prioridad SIP:" meta:resourcekey="Label4Resource1"></asp:Label>
        <asp:DropDownList ID="DListPrioridadSIP" runat="server" Style="z-index: 105; left: 20px;
            position: absolute; top: 103px" Width="117px"  class="select"
			 meta:resourcekey="DListPrioridadSIPResource1">
            <asp:ListItem Value="4" meta:resourcekey="ListItemResource1">No urgente</asp:ListItem>
			  <asp:ListItem Value="3" meta:resourcekey="ListItemResource2">Normal</asp:ListItem>
			  <asp:ListItem Value="2" meta:resourcekey="ListItemResource3">Urgente</asp:ListItem>
			  <asp:ListItem Value="1" meta:resourcekey="ListItemResource4">Emergencia</asp:ListItem>
        </asp:DropDownList>

        <asp:Label ID="Label5" runat="server" Style="z-index: 108; left: 20px; position: absolute;
            top: 143px" Text="Prioridad de descarte en Sectorizaci�n:" meta:resourcekey="Label5Resource1"></asp:Label>
        <asp:DropDownList ID="DListPrioridadTecla" runat="server" Style="z-index: 106; left: 20px;
            position: absolute; top: 163px" Width="275px"  class="select"
			 meta:resourcekey="DListPrioridadTeclaResource1">
            <asp:ListItem Value="1" meta:resourcekey="ListItemResource5">No descartable</asp:ListItem>
            <asp:ListItem Value="2" meta:resourcekey="ListItemResource6">Alta</asp:ListItem>
            <asp:ListItem Value="3" meta:resourcekey="ListItemResource7">Media</asp:ListItem>
            <asp:ListItem Value="4" meta:resourcekey="ListItemResource8">Baja</asp:ListItem>
        </asp:DropDownList>

      <asp:Button ID="BtAceptar" runat="server" Style="z-index: 100; left: 59px; position: absolute;
            top: 210px" Text="Aceptar" Width="80px" OnClick="BtAceptar_Click" 
			 meta:resourcekey="BtAceptarResource1" UseSubmitBehavior="true" />
		<asp:Button ID="BCancelar" runat="server" Style="z-index: 115; left: 175px; position: absolute;
			 top: 210px" Text="Cancelar" Width="80px" OnClick="BCancelar_Click" 
			 CausesValidation="False" meta:resourcekey="BCancelarResource1" />


        <asp:RequiredFieldValidator ID="RequiredFieldValidator1" runat="server" ControlToValidate="TBoxLiteral"
            ErrorMessage="*" 
			 Style="z-index: 110; left: 180px; position: absolute; top: 45px" 
			 meta:resourcekey="RequiredFieldValidator1Resource1">*</asp:RequiredFieldValidator>
    </asp:Panel>

    <asp:Panel ID="PanelNoPermissions" runat="server" ForeColor="#00C000" 
		Style="z-index: -1; left: 50px; position: absolute; top: 460px; height: 60px; width: 797px;" 
		meta:resourcekey="PanelNoPermissions" Visible="false">
        <asp:Label ID="LblPermisos" runat="server" ForeColor="Red" Style="z-index: 100; left: 10px; width:auto; position : absolute; top: 15px" 
        Text="No tiene permisos para ver esta secci�n."
        meta:resourcekey="LblPermisosResource1">
        </asp:Label>
    </asp:Panel>

</asp:Content>
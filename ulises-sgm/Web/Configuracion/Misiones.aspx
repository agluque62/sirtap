<%@ Page Language="C#" MasterPageFile="~/CD40.master" AutoEventWireup="true" CodeFile="Misiones.aspx.cs" Inherits="Misiones" 
		CodeFileBaseClass="PageBaseCD40.PageCD40" Title="Gestión de Misiones" EnableEventValidation="false" StyleSheetTheme="TemaPaginasConfiguracion" meta:resourcekey="PageResource1" %>
<asp:Content ID="Content1" ContentPlaceHolderID="ContentPlaceHolder1" Runat="Server">
     
    <script type="text/javascript">
        function AbreVentana(ventana) {
            window.open(ventana);
        }
	</script>

    <asp:Label ID="Label1" runat="server" Style="z-index: 112; position: absolute; " 
			Text="GESTIÓN DE MISIONES" CssClass="labelPagina" 
		  meta:resourcekey="Label1Resource1"></asp:Label>
	 <asp:ListBox ID="ListBox1" runat="server" Height="252px" Style="z-index: 105; left: 14px; position: absolute; top: 50px"
            Width="171px" Enabled="True" Visible="true"
		  OnSelectedIndexChanged="ListBox1_SelectedIndexChanged" AutoPostBack="True"
					SkinID="MascaraListaElementos" meta:resourcekey="ListBox1Resource1">
    </asp:ListBox>

        <asp:Panel ID="Panel1" runat="server" BorderStyle="Inset" Height="252px" Style="z-index: 105; left: 200px; position: absolute; top: 50px"
            Width="247px" Enabled="true"
            meta:resourcekey="Panel1Resource1">

            <asp:Label ID="Label5" runat="server" Style="z-index: 116; left: 14px; position: absolute; top: 15px"
                Text="Identificador:" Visible="True"
                meta:resourcekey="Label5Resource1"></asp:Label>
            <asp:TextBox ID="TxtMision" runat="server" Style="z-index: 117; left: 14px; position: absolute; top: 33px"
                Visible="True" MaxLength="45"
                meta:resourcekey="TxtMisionResource1"></asp:TextBox>

            <asp:Label ID="LabelTipo" runat="server" Style="z-index: 101; left: 14px; position: absolute; top: 68px"
                Text="Tipo:" Visible="True" meta:resourcekey="LabelTipoResource1"></asp:Label>
            <asp:DropDownList ID="DListTipo" runat="server" Style="z-index: 105; left: 14px; position: absolute; top: 88px"
                Width="120px" Visible="True" OnSelectedIndexChanged="DListTipo_SelectedIndexChanged" AutoPostBack="true"
                meta:resourcekey="DListTipoResource1" >
                <asp:ListItem Value="0" meta:resourcekey="ListItemResource1">Real</asp:ListItem>
                <asp:ListItem Value="1" meta:resourcekey="ListItemResource2">Entrenamiento</asp:ListItem>
            </asp:DropDownList>

            <asp:Label ID="LabelSeguro" runat="server" Style="z-index: 101; left: 42px; position: absolute; top: 132px"
                Text="Clasificación de la Misión" Visible="false" meta:resourcekey="LabelSeguroResource1"></asp:Label>
             <asp:Label ID="LabelTipoSeguro" runat="server" Style="z-index: 101; left: 25px; position: absolute; top: 157px"
                Text="Segura" Visible="false" meta:resourcekey="LabelTipoSeguroResource1"></asp:Label>
             <asp:Label ID="LabelTipoNoSeguro" runat="server" Style="z-index: 101; left: 142px; position: absolute; top: 157px"
                Text="No Segura" Visible="false" meta:resourcekey="LabelTipoSeguroResource1"></asp:Label>
           <asp:Image ID="ImagenSegura" runat="server"   Style="z-index: 102;left: 31px; position: absolute; top: 177px;height: 36px; width: 36px"
                 ImageUrl="~/Configuracion/Images/ModoSeguro.png" />
           <asp:Image ID="ImagenNoSegura" runat="server"   Style="z-index: 102;left: 154px; position: absolute; top: 177px;height: 36px; width: 36px"
                 ImageUrl="~/Configuracion/Images/ModoNoSeguro.png" />

            <asp:Label ID="LabelMSAS" runat="server" Style="z-index: 101; left: 2px; position: absolute; top: 132px"
                Text="** Misión con operadores asignados **" Visible="false" meta:resourcekey="LabelMSASResource1"></asp:Label>



        </asp:Panel>

        <asp:Label ID="LabelSenalesAcusticas" runat="server" Style="z-index: 122; left:  14px; position: absolute;
                top: 336px" Text="SEÑALES ACUSTICAS" Visible="true" 
		        meta:resourcekey="Label10Resource1"></asp:Label>

	<asp:Panel ID="PanelSenalesAcusticas" runat="server" BorderStyle="Inset" Height="307px" Style="z-index: 123;
		        left: 14px; position: absolute; top: 353px" Width="429px" Enabled="true" 
		         meta:resourcekey="Panel10Resource1"> 
                                         
            <asp:Label ID="LabelAsignadas" runat="server" Style="z-index: 124; left: 20px; position: absolute; top: 22px"
                    Text="Asignadas:" Visible="True"
                    meta:resourcekey="LabelAsigResource1"></asp:Label> 
               
            <asp:ListBox ID="ListaAsignadas" runat="server" OnSelectedIndexChanged="ListaAsignadas_Click" Height="100px" Style="z-index: 125; left: 20px; position: absolute; top: 39px"
                    Visible="True" Width="390px"  AutoPostBack="true"
                    SelectionMode ="Single" meta:resourcekey="ListAsignadas_resource1">                  
            </asp:ListBox>

            <asp:Label ID="LabelExist" runat="server" Style="z-index: 126; left: 20px; position: absolute; top: 172px"
                    Text="Existentes:" Visible="True"
                    meta:resourcekey="LabelExistResource1"></asp:Label>

            <asp:ListBox ID="ListaExistentes" runat="server" OnSelectedIndexChanged="ListaExistentes_Click"  Height="100px" Style="z-index:127; left: 20px; position: absolute; top: 190px"
                    Visible="True" Width="390px"  AutoPostBack="true"
                    SelectionMode="Single" meta:resourcekey="ListExistentes_resource1">
            </asp:ListBox>

            <asp:ImageButton ID="IButSAAsignar" runat="server" OnClick="IButSAAsignar_Click" Style="z-index: 128; left: 150px; position: absolute; top: 144px; -webkit-transform: rotate(180deg); -moz-transform: rotate(180); -o-transform: rotate(180); transform: rotate(180);"
                    CausesValidation="False"
                    ImageUrl="~/Configuracion/Images/btmas.png" Visible="True"
                    meta:resourcekey="IButAsignarSAResource1" />
                        &nbsp;
            <asp:ImageButton ID="IButSADesAsignar" runat="server" Style="z-index: 129; left: 256px; position: absolute; top: 144px; -webkit-transform: rotate(90deg); -moz-transform: rotate(0deg); -o-transform: rotate(0deg); transform: rotate(0deg);"
                    CausesValidation="False"
                    ImageUrl="~/Configuracion/Images/btmenos.png" OnClick="IButSADesasignar_Click"
                    Visible="True" meta:resourcekey="IButQuitarSAResource1" />

        </asp:Panel>

        <asp:Label ID="LabelRadio" runat="server" Style="z-index: 122; left:  480px; position: absolute;
            top: 6px" Text="RADIO:" Visible="true" 
		    meta:resourcekey="LabelRadioResource1"></asp:Label>

	    <asp:Panel ID="PanelRadio" runat="server" BorderStyle="Inset" Height="267px" Style="z-index: 123;
		        left: 480px; position: absolute; top: 20px" Width="536px" Enabled="true" 
		         meta:resourcekey="PanelRadioResource1"> 

                <asp:CheckBox ID="CBSeguroRadio" runat="server" Style="z-index: 105; left: 47px; position: absolute;
                top: 19px" Text="Seguro: " TextAlign="Left" Enabled="False"  Visible ="false"
                AutoPostBack="false"  Font-Size="Medium" Height="14px" Font-Names="Arial"
                meta:resourcekey="LSeguroResource1"/>

            	<asp:ImageButton ID="IButPagAbajo" runat="server"  Style="z-index: 100; -webkit-transform: rotate(90deg); -moz-transform: rotate(90deg); -o-transform: rotate(90deg); transform: rotate(90deg);
                left: 190px; position: absolute; top: 6px" CausesValidation="False" 
		            ImageUrl="~/Configuracion/Images/arrow.png" OnClick="IButPagAbajoR_Click" 
		            meta:resourcekey="IButPagAbajoResource1" />

	            <asp:ImageButton ID="IButPagArriba" runat="server" Style="z-index: 101; left: 328px; -webkit-transform: rotate(-90deg); -moz-transform: rotate(-90deg); -o-transform: rotate(-90deg); transform: rotate(-90deg);
                position: absolute; top: 6px" CausesValidation="False" 
		            ImageUrl="~/Configuracion/Images/arrow.png" OnClick="IButPagArribaR_Click" 
		            meta:resourcekey="IButPagArribaResource1" />

	<asp:Label ID="LabelPag" runat="server" Font-Bold="True" Style="z-index: 102; left: 244px;
    position: absolute; top: 14px" Text="PAGINA " 
		meta:resourcekey="LabelPagResource1"></asp:Label>

    <asp:Table ID="TEnlacesRadio" runat="server" Style="left: 108px; position: absolute; top: 44px; z-index: 104;"
        BackColor="Transparent" BorderStyle="Solid" BorderColor="#eeb44f"
        meta:resourcekey="TEnlacesRadioResource1">
        <asp:TableRow ID="TableRow1" runat="server" BackColor="Transparent">
            <asp:TableCell ID="TableCell1" runat="server" Visible="false" Width="103px"
                BorderStyle="Solid" BorderColor="#eeb44f" BorderWidth="1px">
                <div id="Div1" runat="server" style="height: 30px; width: 103px">
                    <asp:TextBox runat="server" ID="TextBox1" ReadOnly="True"
                        SkinID="LiteralTeclaPanelRadio"></asp:TextBox>
                </div>
                <div id="Div2" runat="server" style="height: 50px; width: 103px">
                    <asp:Button ID="Button1" runat="server" SkinId="BotonPanelRadio" Text="Tx Rx"
                        OnClick="CeldaEnlaceRadio_OnClick" CausesValidation="false" />
                </div>
            </asp:TableCell>
            <asp:TableCell ID="TableCell2" runat="server" Visible="false" Width="103px"
                BorderStyle="Solid" BorderColor="#eeb44f" BorderWidth="1px">
                <div id="Div3" runat="server" style="height: 30px; width: 103px">
                    <asp:TextBox ID="TextBox2" runat="server" ReadOnly="True"
                        SkinID="LiteralTeclaPanelRadio"></asp:TextBox>
                </div>
                <div id="Div4" runat="server" style="height: 50px; width: 103px">
                    <asp:Button ID="Button2" runat="server" SkinId="BotonPanelRadio" Text="Tx Rx"
                        OnClick="CeldaEnlaceRadio_OnClick" CausesValidation="false" />
                </div>
            </asp:TableCell>
            <asp:TableCell ID="TableCell3" runat="server" Visible="false" Width="103px"
                BorderStyle="Solid" BorderColor="#eeb44f" BorderWidth="1px">
                <div id="Div5" runat="server" style="height: 30px; width: 103px">
                    <asp:TextBox ID="TextBox3" runat="server" ReadOnly="True" Rows="3"
                        MaxLength="32" SkinID="LiteralTeclaPanelRadio"></asp:TextBox>
                </div>
                <div id="Div6" runat="server" style="height: 50px; width: 103px">
                    <asp:Button ID="Button3" runat="server"
                        SkinId="BotonPanelRadio" Text="Tx Rx"
                        OnClick="CeldaEnlaceRadio_OnClick" CausesValidation="false" />
                </div>
            </asp:TableCell>
            <asp:TableCell ID="TableCell4" runat="server" Visible="false" Width="103px"
                BorderStyle="Solid" BorderColor="#eeb44f" BorderWidth="1px">
                <div id="Div7" runat="server" style="height: 30px; width: 103px">
                    <asp:TextBox ID="TextBox4" runat="server" ReadOnly="True" Rows="3"
                        MaxLength="32" SkinID="LiteralTeclaPanelRadio"></asp:TextBox>
                </div>
                <div id="Div9" runat="server" style="height: 50px; width: 103px">
                    <asp:Button ID="Button4" runat="server"
                        SkinId="BotonPanelRadio" Text="Tx Rx"
                        OnClick="CeldaEnlaceRadio_OnClick" CausesValidation="false" />
                </div>
            </asp:TableCell>
            <asp:TableCell ID="TableCell5" runat="server" Visible="false" Width="103px"
                BorderStyle="Solid" BorderColor="#eeb44f" BorderWidth="1px">
                <div id="Div8" runat="server" style="height: 30px; width: 103px">
                    <asp:TextBox ID="TextBox5" runat="server" ReadOnly="True" Rows="3"
                        MaxLength="32" SkinID="LiteralTeclaPanelRadio"></asp:TextBox>
                </div>
                <div id="Div11" runat="server" style="height: 50px; width: 103px">
                    <asp:Button ID="Button5" runat="server"
                        SkinId="BotonPanelRadio" Text="Tx Rx"
                        OnClick="CeldaEnlaceRadio_OnClick" CausesValidation="false" />
                </div>
            </asp:TableCell>
        </asp:TableRow>
        <asp:TableRow ID="TableRow2" runat="server" BackColor="Transparent">
            <asp:TableCell ID="TableCell6" runat="server" Visible="false" Width="103px"
                BorderStyle="Solid" BorderColor="#eeb44f" BorderWidth="1px">
                <div id="Div10" runat="server" style="height: 30px; width: 103px">
                    <asp:TextBox ID="TextBox6" runat="server" ReadOnly="True" Rows="3"
                        MaxLength="32" SkinID="LiteralTeclaPanelRadio"></asp:TextBox>
                </div>
                <div id="Div13" runat="server" style="height: 50px; width: 103px">
                    <asp:Button ID="Button6" runat="server"
                        SkinId="BotonPanelRadio" Text="Tx Rx"
                        OnClick="CeldaEnlaceRadio_OnClick" CausesValidation="false" />
                </div>
            </asp:TableCell>
            <asp:TableCell ID="TableCell7" runat="server" Visible="false" Width="103px"
                BorderStyle="Solid" BorderColor="#eeb44f" BorderWidth="1px">
                <div id="Div12" runat="server" style="height: 30px; width: 103px">
                    <asp:TextBox ID="TextBox7" runat="server" ReadOnly="True" Rows="3"
                        MaxLength="32" SkinID="LiteralTeclaPanelRadio"></asp:TextBox>
                </div>
                <div id="Div15" runat="server" style="height: 50px; width: 103px">
                    <asp:Button ID="Button7" runat="server"
                        SkinId="BotonPanelRadio" Text="Tx Rx"
                        OnClick="CeldaEnlaceRadio_OnClick" CausesValidation="false" />
                </div>
            </asp:TableCell>
            <asp:TableCell ID="TableCell8" runat="server" Visible="false" Width="103px"
                BorderStyle="Solid" BorderColor="#eeb44f" BorderWidth="1px">
                <div id="Div14" runat="server" style="height: 30px; width: 103px">
                    <asp:TextBox ID="TextBox8" runat="server" ReadOnly="True" Rows="3"
                        MaxLength="32" SkinID="LiteralTeclaPanelRadio"></asp:TextBox>
                </div>
                <div id="Div17" runat="server" style="height: 50px; width: 103px">
                    <asp:Button ID="Button8" runat="server"
                        SkinId="BotonPanelRadio" Text="Tx Rx"
                        OnClick="CeldaEnlaceRadio_OnClick" CausesValidation="false" />
                </div>
            </asp:TableCell>
            <asp:TableCell ID="TableCell9" runat="server" Visible="false" Width="103px"
                BorderStyle="Solid" BorderColor="#eeb44f" BorderWidth="1px">
                <div id="Div16" runat="server" style="height: 30px; width: 103px">
                    <asp:TextBox ID="TextBox9" runat="server" ReadOnly="True" Rows="3"
                        MaxLength="32" SkinID="LiteralTeclaPanelRadio"></asp:TextBox>
                </div>
                <div id="Div19" runat="server" style="height: 50px; width: 103px">
                    <asp:Button ID="Button9" runat="server"
                        SkinId="BotonPanelRadio" Text="Tx Rx"
                        OnClick="CeldaEnlaceRadio_OnClick" CausesValidation="false" />
                </div>
            </asp:TableCell>
            <asp:TableCell ID="TableCell10" runat="server" Visible="false" Width="103px"
                BorderStyle="Solid" BorderColor="#eeb44f" BorderWidth="1px">
                <div id="Div18" runat="server" style="height: 30px; width: 103px">
                    <asp:TextBox ID="TextBox10" runat="server" ReadOnly="True" Rows="3"
                        MaxLength="32" SkinID="LiteralTeclaPanelRadio"></asp:TextBox>
                </div>
                <div id="Div21" runat="server" style="height: 50px; width: 103px">
                    <asp:Button ID="Button10" runat="server"
                        SkinId="BotonPanelRadio" Text="Tx Rx"
                        OnClick="CeldaEnlaceRadio_OnClick" CausesValidation="false" />
                </div>
            </asp:TableCell>
        </asp:TableRow>
    </asp:Table>
            <asp:ImageButton ID="IButRDAsignar" runat="server" OnClick="IButRDAsignar_Click" Style="z-index: 102; left: 62px; position: absolute; top: 101px;"
                CausesValidation="False"
                ImageUrl="~/Configuracion/Images/btmas.png" 
                Visible="true"
                meta:resourcekey="IButRDAsignarResource1" />

            <asp:ImageButton ID="IButRDDesAsignar" runat="server" OnClick="IButRDDesAsignar_Click" Style="z-index: 102; left: 62px; position: absolute; top: 146px;"
                CausesValidation="False"
                ImageUrl="~/Configuracion/Images/btmenos.png" 
                Visible="true"
                meta:resourcekey="IButMFAsignarResource1" />

            <asp:Image ID="ImagePosicionR" runat="server"   Style="z-index: 102;left: 16px; position: absolute; top: 236px;height: 17px; width: 504px"
                 ImageUrl="~/Configuracion/Images/Posiciones.png" />

             <asp:CheckBox ID="CBR1" runat="server" Style="z-index: 105; left: 16px; position: absolute;
                top: 250px" Text="" TextAlign="Left" Enabled="False" 
                AutoPostBack="false"  Font-Size="Medium" Height="14px" Font-Names="Arial"
                meta:resourcekey="LSeguroResource1"/>
             <asp:CheckBox ID="CBR2" runat="server" Style="z-index: 105; left: 34px; position: absolute;
                top: 250px" Text="" TextAlign="Left" Enabled="False" 
                AutoPostBack="false"  Font-Size="Medium" Height="14px" Font-Names="Arial"
                meta:resourcekey="LSeguroResource1"/>
             <asp:CheckBox ID="CBR3" runat="server" Style="z-index: 105; left: 52px; position: absolute;
                top: 250px" Text="" TextAlign="Left" Enabled="False" 
                AutoPostBack="false"  Font-Size="Medium" Height="14px" Font-Names="Arial"
                meta:resourcekey="LSeguroResource1"/>
             <asp:CheckBox ID="CBR4" runat="server" Style="z-index: 105; left: 70px; position: absolute;
                top: 250px" Text="" TextAlign="Left" Enabled="False" 
                AutoPostBack="false"  Font-Size="Medium" Height="14px" Font-Names="Arial"
                meta:resourcekey="LSeguroResource1"/>
             <asp:CheckBox ID="CBR5" runat="server" Style="z-index: 105; left: 88px; position: absolute;
                top: 250px" Text="" TextAlign="Left" Enabled="False" 
                AutoPostBack="false"  Font-Size="Medium" Height="14px" Font-Names="Arial"
                meta:resourcekey="LSeguroResource1"/>
             <asp:CheckBox ID="CBR6" runat="server" Style="z-index: 105; left: 106px; position: absolute;
                top: 250px" Text="" TextAlign="Left" Enabled="False" 
                AutoPostBack="false"  Font-Size="Medium" Height="14px" Font-Names="Arial"
                meta:resourcekey="LSeguroResource1"/>
             <asp:CheckBox ID="CBR7" runat="server" Style="z-index: 105; left: 124px; position: absolute;
                top: 250px" Text="" TextAlign="Left" Enabled="False" 
                AutoPostBack="false"  Font-Size="Medium" Height="14px" Font-Names="Arial"
                meta:resourcekey="LSeguroResource1"/>

             <asp:CheckBox ID="CBR8" runat="server" Style="z-index: 105; left: 142px; position: absolute;
                top: 250px" Text="" TextAlign="Left" Enabled="False" 
                AutoPostBack="false"  Font-Size="Medium" Height="14px" Font-Names="Arial"
                meta:resourcekey="LSeguroResource1"/>
             <asp:CheckBox ID="CBR9" runat="server" Style="z-index: 105; left: 160px; position: absolute;
                top: 250px" Text="" TextAlign="Left" Enabled="False"
                AutoPostBack="false"  Font-Size="Medium" Height="14px" Font-Names="Arial"
                meta:resourcekey="LSeguroResource1"/>
             <asp:CheckBox ID="CBR10" runat="server" Style="z-index: 105; left: 178px; position: absolute;
                top: 250px" Text="" TextAlign="Left" Enabled="False" 
                AutoPostBack="false"  Font-Size="Medium" Height="14px" Font-Names="Arial"
                meta:resourcekey="LSeguroResource1"/>
             <asp:CheckBox ID="CBR11" runat="server" Style="z-index: 105; left: 196px; position: absolute;
                top: 250px" Text="" TextAlign="Left" Enabled="False" 
                AutoPostBack="false"  Font-Size="Medium" Height="14px" Font-Names="Arial"
                meta:resourcekey="LSeguroResource1"/>
             <asp:CheckBox ID="CBR12" runat="server" Style="z-index: 105; left: 214px; position: absolute;
                top: 250px" Text="" TextAlign="Left" Enabled="False" 
                AutoPostBack="false"  Font-Size="Medium" Height="14px" Font-Names="Arial"
                meta:resourcekey="LSeguroResource1"/>
             <asp:CheckBox ID="CBR13" runat="server" Style="z-index: 105; left: 232px; position: absolute;
                top: 250px" Text="" TextAlign="Left" Enabled="False" 
                AutoPostBack="false"  Font-Size="Medium" Height="14px" Font-Names="Arial"
                meta:resourcekey="LSeguroResource1"/>
             <asp:CheckBox ID="CBR14" runat="server" Style="z-index: 105; left: 250px; position: absolute;
                top: 250px" Text="" TextAlign="Left" Enabled="False" 
                AutoPostBack="false"  Font-Size="Medium" Height="14px" Font-Names="Arial"
                meta:resourcekey="LSeguroResource1"/>
             <asp:CheckBox ID="CBR15" runat="server" Style="z-index: 105; left: 268px; position: absolute;
                top: 250px" Text="" TextAlign="Left" Enabled="False" 
                AutoPostBack="false"  Font-Size="Medium" Height="14px" Font-Names="Arial"
                meta:resourcekey="LSeguroResource1"/>
             <asp:CheckBox ID="CBR16" runat="server" Style="z-index: 105; left: 286px; position: absolute;
                top: 250px" Text="" TextAlign="Left" Enabled="False" 
                AutoPostBack="false"  Font-Size="Medium" Height="14px" Font-Names="Arial"
                meta:resourcekey="LSeguroResource1"/>
             <asp:CheckBox ID="CBR17" runat="server" Style="z-index: 105; left: 304px; position: absolute;
                top: 250px" Text="" TextAlign="Left" Enabled="False" 
                AutoPostBack="false"  Font-Size="Medium" Height="14px" Font-Names="Arial"
                meta:resourcekey="LSeguroResource1"/>
             <asp:CheckBox ID="CBR18" runat="server" Style="z-index: 105; left: 322px; position: absolute;
                top: 250px" Text="" TextAlign="Left" Enabled="False" 
                AutoPostBack="false"  Font-Size="Medium" Height="14px" Font-Names="Arial"
                meta:resourcekey="LSeguroResource1"/>
             <asp:CheckBox ID="CBR19" runat="server" Style="z-index: 105; left: 340px; position: absolute;
                top: 250px" Text="" TextAlign="Left" Enabled="False"
                AutoPostBack="false"  Font-Size="Medium" Height="14px" Font-Names="Arial"
                meta:resourcekey="LSeguroResource1"/>
             <asp:CheckBox ID="CBR20" runat="server" Style="z-index: 105; left: 358px; position: absolute;
                top: 250px" Text="" TextAlign="Left" Enabled="False" 
                AutoPostBack="false"  Font-Size="Medium" Height="14px" Font-Names="Arial"
                meta:resourcekey="LSeguroResource1"/> 
              <asp:CheckBox ID="CBR21" runat="server" Style="z-index: 105; left: 376px; position: absolute;
                top: 250px" Text="" TextAlign="Left" Enabled="False" 
                AutoPostBack="false"  Font-Size="Medium" Height="14px" Font-Names="Arial"
                meta:resourcekey="LSeguroResource1"/>
             <asp:CheckBox ID="CBR22" runat="server" Style="z-index: 105; left: 394px; position: absolute;
                top: 250px" Text="" TextAlign="Left" Enabled="False" 
                AutoPostBack="false"  Font-Size="Medium" Height="14px" Font-Names="Arial"
                meta:resourcekey="LSeguroResource1"/>
             <asp:CheckBox ID="CBR23" runat="server" Style="z-index: 105; left: 412px; position: absolute;
                top: 250px" Text="" TextAlign="Left" Enabled="False" 
                AutoPostBack="false"  Font-Size="Medium" Height="14px" Font-Names="Arial"
                meta:resourcekey="LSeguroResource1"/>
             <asp:CheckBox ID="CBR24" runat="server" Style="z-index: 105; left: 430px; position: absolute;
                top: 250px" Text="" TextAlign="Left" Enabled="False" 
                AutoPostBack="false"  Font-Size="Medium" Height="14px" Font-Names="Arial"
                meta:resourcekey="LSeguroResource1"/>
             <asp:CheckBox ID="CBR25" runat="server" Style="z-index: 105; left: 448px; position: absolute;
                top: 250px" Text="" TextAlign="Left" Enabled="False" 
                AutoPostBack="false"  Font-Size="Medium" Height="14px" Font-Names="Arial"
                meta:resourcekey="LSeguroResource1"/>
             <asp:CheckBox ID="CBR26" runat="server" Style="z-index: 105; left: 466px; position: absolute;
                top: 250px" Text="" TextAlign="Left" Enabled="False" 
                AutoPostBack="false"  Font-Size="Medium" Height="14px" Font-Names="Arial"
                meta:resourcekey="LSeguroResource1"/>
             <asp:CheckBox ID="CBR27" runat="server" Style="z-index: 105; left: 484px; position: absolute;
                top: 250px" Text="" TextAlign="Left" Enabled="False" 
                AutoPostBack="false"  Font-Size="Medium" Height="14px" Font-Names="Arial"
                meta:resourcekey="LSeguroResource1"/>
             <asp:CheckBox ID="CBR28" runat="server" Style="z-index: 105; left: 502px; position: absolute;
                top: 250px" Text="" TextAlign="Left" Enabled="False" 
                AutoPostBack="false"  Font-Size="Medium" Height="14px" Font-Names="Arial"
                meta:resourcekey="LSeguroResource1"/>
        </asp:Panel>

        <asp:Label ID="LabelTelefonia" runat="server" Style="z-index: 122; left:  480px; position: absolute;
                top: 304px" Text="TELEFONÍA:" Visible="true" 
		        meta:resourcekey="LabelTelefoniaResource1"></asp:Label>

	    <asp:Panel ID="PanelTelefonia" runat="server" BorderStyle="Inset" Height="154px" Style="z-index: 123;
		        left: 480px; position: absolute; top: 318px" Width="536px" Enabled="true" Visible="true" 
		         meta:resourcekey="PanelTelefoniaResource1"> 

                <asp:CheckBox ID="CBSeguroTelefonia" runat="server" Style="z-index: 105; left: 47px; position: absolute;
                top: 19px" Text="Seguro: " TextAlign="Left" Enabled="False"  Visible ="false"
                AutoPostBack="false"  Font-Size="Medium" Height="14px" Font-Names="Arial"
                meta:resourcekey="LSeguroResource1"/>

            	<asp:ImageButton ID="IButPagTAbajo" runat="server"  Style="z-index: 100; -webkit-transform: rotate(90deg); -moz-transform: rotate(90deg); -o-transform: rotate(90deg); transform: rotate(90deg);
                left: 193px; position: absolute; top: 6px" CausesValidation="False" 
		            ImageUrl="~/Configuracion/Images/arrow.png" OnClick="IButPagTAbajo_Click" 
		            meta:resourcekey="IButPagAbajoResource1" />

	            <asp:ImageButton ID="IButPagTArriba" runat="server" Style="z-index: 101; left: 331px; -webkit-transform: rotate(-90deg); -moz-transform: rotate(-90deg); -o-transform: rotate(-90deg); transform: rotate(-90deg);
                position: absolute; top: 6px" CausesValidation="False" 
		            ImageUrl="~/Configuracion/Images/arrow.png" OnClick="IButPagTArriba_Click" 
		            meta:resourcekey="IButPagArribaResource1" />
	    <asp:Label ID="LabelPagT" runat="server" Font-Bold="True" Style="z-index: 102; left: 247px;
             position: absolute; top: 14px" Text="PAGINA " 
		meta:resourcekey="LabelPagResource1"></asp:Label>

            <asp:ImageButton ID="IButTLFAsignar" runat="server" OnClick="IButTLFAsignar_Click" Style="z-index: 102; left: 45px; position: absolute; top: 47px;"
                CausesValidation="False"
                ImageUrl="~/Configuracion/Images/btmas.png" 
                Visible="true"
                meta:resourcekey="IButRDAsignarResource1" />

            <asp:ImageButton ID="IButTLFDesAsignar" runat="server" OnClick="IButTLFDesAsignar_Click" Style="z-index: 102; left: 45px; position: absolute; top: 91px;"
                CausesValidation="False"
                ImageUrl="~/Configuracion/Images/btmenos.png" 
                Visible="true"
                meta:resourcekey="IButMFAsignarResource1" />

    <asp:Table ID="TEnlacesInternos" runat="server"  Height="68px" Style="left: 95px;
        position: absolute; top: 50px; z-index: 104;" BackColor="Transparent"   BorderStyle="Solid" BorderColor="#eeb44f">

    <asp:TableRow ID="TableEnlacesInternos" runat="server" >
        <asp:TableCell ID="TableCellTI1" runat="server" Visible="true">
            <asp:Button ID="ButtonTI1" runat="server"
                SkinId="BotonPanelTelefonia"
                 CausesValidation="false" />
        </asp:TableCell>
        <asp:TableCell ID="TableCellTI2" runat="server" Visible="true">
            <asp:Button ID="ButtonTI2" runat="server"
                SkinId="BotonPanelTelefonia"
                 CausesValidation="false" />
        </asp:TableCell>        
        <asp:TableCell ID="TableCellTI3" runat="server" Visible="true">
            <asp:Button ID="ButtonTI3" runat="server"
                SkinId="BotonPanelTelefonia"
                 CausesValidation="false" />
        </asp:TableCell>
        <asp:TableCell ID="TableCellTI4" runat="server" Visible="true">
            <asp:Button ID="ButtonTI4" runat="server"
                SkinId="BotonPanelTelefonia"
                 CausesValidation="false" />
        </asp:TableCell>
        <asp:TableCell ID="TableCellTI5" runat="server" Visible="true">
            <asp:Button ID="ButtonTI5" runat="server"
                SkinId="BotonPanelTelefonia"
                 CausesValidation="false" />
        </asp:TableCell>
        <asp:TableCell ID="TableCellTI6" runat="server" Visible="false">
            <asp:Button ID="ButtonTI6" runat="server" 
                SkinId="BotonPanelTelefonia"
                 CausesValidation="false" />
        </asp:TableCell>
    </asp:TableRow>
</asp:Table>
            <asp:Image ID="ImagenPosicionT" runat="server"   Style="z-index: 102;left: 214px; position: absolute; top: 126px;height: 17px; width: 161px"
                 ImageUrl="~/Configuracion/Images/PagSel_1_9.png" />

             <asp:CheckBox ID="CBT1" runat="server" Style="z-index: 105; left: 214px; position: absolute;
                top: 136px" Text="" TextAlign="Left" Enabled="False" 
                AutoPostBack="false"  Font-Size="Medium" Height="14px" Font-Names="Arial"
                meta:resourcekey="LSeguroResource1"/>
             <asp:CheckBox ID="CBT2" runat="server" Style="z-index: 105; left: 232px; position: absolute;
                top: 136px" Text="" TextAlign="Left" Enabled="False"
                AutoPostBack="false"  Font-Size="Medium" Height="14px" Font-Names="Arial"
                meta:resourcekey="LSeguroResource1"/>
             <asp:CheckBox ID="CBT3" runat="server" Style="z-index: 105; left: 250px; position: absolute;
                top: 136px" Text="" TextAlign="Left" Enabled="False" 
                AutoPostBack="false"  Font-Size="Medium" Height="14px" Font-Names="Arial"
                meta:resourcekey="LSeguroResource1"/>
             <asp:CheckBox ID="CBT4" runat="server" Style="z-index: 105; left: 268px; position: absolute;
                top: 136px" Text="" TextAlign="Left" Enabled="False" 
                AutoPostBack="false"  Font-Size="Medium" Height="14px" Font-Names="Arial"
                meta:resourcekey="LSeguroResource1"/>
             <asp:CheckBox ID="CBT5" runat="server" Style="z-index: 105; left: 286px; position: absolute;
                top: 136px" Text="" TextAlign="Left" Enabled="False" 
                AutoPostBack="false"  Font-Size="Medium" Height="14px" Font-Names="Arial"
                meta:resourcekey="LSeguroResource1"/>
             <asp:CheckBox ID="CBT6" runat="server" Style="z-index: 105; left: 304px; position: absolute;
                top: 136px" Text="" TextAlign="Left" Enabled="False" 
                AutoPostBack="false"  Font-Size="Medium" Height="14px" Font-Names="Arial"
                meta:resourcekey="LSeguroResource1"/>
             <asp:CheckBox ID="CBT7" runat="server" Style="z-index: 105; left: 322px; position: absolute;
                top: 136px" Text="" TextAlign="Left" Enabled="False" 
                AutoPostBack="false"  Font-Size="Medium" Height="14px" Font-Names="Arial"
                meta:resourcekey="LSeguroResource1"/>
             <asp:CheckBox ID="CBT8" runat="server" Style="z-index: 105; left: 340px; position: absolute;
                top: 136px" Text="" TextAlign="Left" Enabled="False" 
                AutoPostBack="false"  Font-Size="Medium" Height="14px" Font-Names="Arial"
                meta:resourcekey="LSeguroResource1"/>
             <asp:CheckBox ID="CBT9" runat="server" Style="z-index: 105; left: 358px; position: absolute;
                top: 136px" Text="" TextAlign="Left" Enabled="False" 
                AutoPostBack="false"  Font-Size="Medium" Height="14px" Font-Names="Arial"
                meta:resourcekey="LSeguroResource1"/>

        </asp:Panel>

        <asp:Label ID="LabelLCEN" runat="server" Style="z-index: 122; left:  480px; position: absolute;
                top: 486px" Text="LÍNEA CALIENTE:" Visible="true" 
		        meta:resourcekey="LabelTelefoniaResource1"></asp:Label>

	    <asp:Panel ID="PanelLCEN" runat="server" BorderStyle="Inset" Height="197px" Style="z-index: 123;
		        left: 480px; position: absolute; top: 500px" Width="536px" Enabled="False" Visible="true" 
		         meta:resourcekey="PanelLCENResource1"> 

           <asp:Table ID="TEnlacesLC" runat="server" Height="80px" Style="left: 26px;
                position: absolute; top: 26px; z-index: 99;" BackColor="Transparent" BorderStyle="Solid" BorderColor="#eeb44f">
               <asp:TableRow ID="TBREnalcesLC" runat="server" BackColor="Transparent" Visible="true">
                   <asp:TableCell ID="TableCellLC1" runat="server" BorderStyle="Solid" BorderColor="#eeb44f" BorderWidth="1px">
                       <asp:Button ID="ButtonLC1" runat="server" Visible="true" Enabled="false"
                           SkinId="BotonPanelLc" 
                           OnClick="CeldaEnlaceLineaCaliente_OnClick" />

                   </asp:TableCell>
                   <asp:TableCell ID="TableCellLC2" runat="server" BorderStyle="Solid" BorderColor="#eeb44f" BorderWidth="1px">
                       <asp:Button ID="ButtonLC2" runat="server" Visible="true" Enabled="false"
                           SkinId="BotonPanelLc" 
                           OnClick="CeldaEnlaceLineaCaliente_OnClick" />

                   </asp:TableCell>
                   <asp:TableCell ID="TableCellLC3" runat="server" BorderStyle="Solid" BorderColor="#eeb44f" BorderWidth="1px">
                       <asp:Button ID="ButtonLC3" runat="server" Visible="true" Enabled="false"
                           SkinId="BotonPanelLc"
                           OnClick="CeldaEnlaceLineaCaliente_OnClick" />

                   </asp:TableCell>
                   <asp:TableCell ID="TableCellLC4" runat="server" BorderStyle="Solid" BorderColor="#eeb44f" BorderWidth="1px">
                       <asp:Button ID="ButtonLC4" runat="server" Visible="true" Enabled="false"
                           SkinId="BotonPanelLc"
                           OnClick="CeldaEnlaceLineaCaliente_OnClick" />

                   </asp:TableCell>
                   <asp:TableCell ID="TableCellLC5" runat="server" BorderStyle="Solid" BorderColor="#eeb44f" BorderWidth="1px">
                       <asp:Button ID="ButtonLC5" runat="server" Visible="true" Enabled="false"
                           SkinId="BotonPanelLc"
                           OnClick="CeldaEnlaceLineaCaliente_OnClick" />

                   </asp:TableCell>
                   <asp:TableCell ID="TableCellLC6" runat="server" BorderStyle="Solid" BorderColor="#eeb44f" BorderWidth="1px">
                       <asp:Button ID="ButtonLC6" runat="server" Visible="true" Enabled="false"
                           SkinId="BotonPanelLc"
                           OnClick="CeldaEnlaceLineaCaliente_OnClick" />

                   </asp:TableCell>
                   <asp:TableCell ID="TableCellLC7" runat="server" BorderStyle="Solid" BorderColor="#eeb44f" BorderWidth="1px" Visible="false" Enabled="false">
                       <asp:Button ID="ButtonLC7" runat="server" Visible="true" Enabled="false"
                           SkinId="BotonPanelLc"
                           OnClick="CeldaEnlaceLineaCaliente_OnClick" />
                   </asp:TableCell>

                   <asp:TableCell ID="TableCellLC8" runat="server" BorderStyle="Solid" BorderColor="#eeb44f" BorderWidth="1px" Visible="false" Enabled="false">
                       <asp:Button ID="ButtonLC8" runat="server" Visible="true" Enabled="false"
                           SkinId="BotonPanelLc"
                           OnClick="CeldaEnlaceLineaCaliente_OnClick" />
                        </asp:TableCell>
               </asp:TableRow>
           </asp:Table> 

       </asp:Panel>




    <asp:Button ID="BtAceptar" runat="server" Style="z-index: 113; left: 99px; position: absolute; top: 688px"
        Text="Aceptar" Width="80px" Visible="True" UseSubmitBehavior="true"
        OnClick="BtAceptar_Click"  ForeColor="#616365" meta:resourcekey="BtAceptarResource1" />
    <ajaxToolkit:ConfirmButtonExtender ID="BtAceptar_ConfirmButtonExtender"
        runat="server" ConfirmText="" Enabled="True" TargetControlID="BtAceptar">
    </ajaxToolkit:ConfirmButtonExtender>

    <asp:LinkButton ID="BtCancelar"  runat="server"   OnClick="BtCancelar_Click" Style="z-index: 113; left: 299px; position: absolute; top: 688px"
        Text="Cancelar" Width="80px" Visible="False"
		  CausesValidation="False"  meta:resourcekey="BtCancelarResource1"/>
      <ajaxToolKit:ConfirmButtonExtender ID="BtCancelar_ConfirmButtonExtender" 
          runat="server" ConfirmText="" Enabled="True" TargetControlID="BtCancelar">
      </ajaxToolKit:ConfirmButtonExtender>

    <asp:LinkButton ID="BtModificar" runat="server" OnClick="BtModificar_Click"
                    ForeColor="#616365" Style="position: absolute; left: 132px; top: 688px; height: 19px; width: 100px;" 
				    Font-Names  ="Arial" Font-Size="14px" Visible="false"
		            CausesValidation="False" Text="Modificar"  meta:resourcekey="BtModificarResource1"/>
    
    <asp:LinkButton ID="BtNuevo" runat="server"	OnClick="BtNuevo_Click" 
		  CausesValidation="False" Text="Nuevo"  ForeColor="#616365" Style="position: absolute; left: 29px; top: 688px; height: 19px; width: 62px;" 
				 Font-Names="Arial" Font-Size="14px" 
		  meta:resourcekey="BtNuevoResource1"/>

    <asp:LinkButton ID="BtEliminar" runat="server" OnClick="BtEliminar_Click" 
		  CausesValidation="False" Text="Eliminar"  ForeColor="#616365" Style="position: absolute; left: 236px; top: 688px;  height: 19px; width: 62px;" 
				 Font-Names="Arial" Font-Size="14px" Visible="false" 
		  meta:resourcekey="BtEliminarResource1"/>
    <ajaxToolkit:ConfirmButtonExtender ID="BtEliminar_ConfirmButtonExtender"
        runat="server" ConfirmText="¿Esta usted seguro de eliminar la misión seleccionada?" Enabled="True" TargetControlID="BtEliminar">
    </ajaxToolkit:ConfirmButtonExtender>


  <script type="text/javascript">

      var waitafewseconds;
      function waitafewseconds() {
          waitafewseconds = setInterval("OnTimer()", 5000);
      }

      function OnTimer() {
          hideSloader();
      }

      function displaySloader() {
          document.getElementById("initSloader").style.display = "block";
          waitafewseconds();
      }

      function hideSloader() {
          document.getElementById("initSloader").style.display = "none";
      }
  </script>


     <div   id="initSloader"  style="position:absolute;margin-top:2px;background-color:white;z-index:1000; display:none" >
         <div id="Desactivador"  style="position: absolute;margin-top: 0px;background-color:transparent;width:900px; height:650px" > </div>
         <div id="loader" style="display:block;position:absolute;top:50%;left:50%;width:150px;height:150px;margin:500px 0 0 470px;background-image: url('../Cluster/Images/progress.gif');background-repeat: no-repeat;background-size: 100% 100%">
        </div>  
     </div>

</asp:Content>
<%@ Page Language="C#" MasterPageFile="~/CD40.master" AutoEventWireup="true" CodeFile="Conferencias.aspx.cs" Inherits="Conferencias" 
		CodeFileBaseClass="PageBaseCD40.PageCD40" Title="Gestión de Conferencias Preprogramadas" EnableEventValidation="false" StyleSheetTheme="TemaPaginasConfiguracion" meta:resourcekey="PageResource1" %>
<asp:Content ID="Content1" ContentPlaceHolderID="ContentPlaceHolder1" Runat="Server">
     
    <script type="text/javascript">
        function AbreVentana(ventana) {
            window.open(ventana);
        }

	</script>

    <asp:Label ID="Label1" runat="server" Style="z-index: 112; position: absolute; " 
			Text="GESTIÓN DE CONFERENCIAS PREPROGRAMADAS" CssClass="labelPagina" 
		  meta:resourcekey="Label1Resource1"></asp:Label>
	 <asp:ListBox ID="LBConferencias" runat="server"
         OnSelectedIndexChanged="LBConferencias_SelectedIndexChanged" Visible="True"  AutoPostBack="True"
         SkinID="MascaraListaElementos"  Style="z-index: 100; left: 21px; width:172px; height:460px; position: absolute; top: 58px" meta:resourcekey="LBConferenciasResource">
    </asp:ListBox>
        <asp:Panel ID="Panel1" runat="server" BorderStyle="Inset" Height="455px" Style="z-index: 105; left: 200px; position: absolute; top: 60px"
            Width="800px" Enabled="False"
            meta:resourcekey="Panel1Resource1">

            <asp:Label ID="LblIdConferencia" runat="server" meta:resourcekey="LblIdConferenciaResource" 
                Style="z-index: 116; left: 15px; position: absolute; top: 22px; left: 14px; width:165px;" Text="Id. Conferencia:" Visible="True"></asp:Label>
            <asp:TextBox ID="TxtIdConferencia" runat="server" Style="z-index: 117; left: 14px; position: absolute; top: 42px; height: 22px; width: 140px;"
                Visible="True" MaxLength="32"
                meta:resourcekey="TxtIdConferencia"></asp:TextBox>
             <asp:RequiredFieldValidator ID="RequiredFieldIdentificador" runat="server" ControlToValidate="TxtIdConferencia"
                ErrorMessage="Rellene el campo Identificador"
                Style="left: 172px; position: absolute; top: 42px" Enabled="False"
                meta:resourcekey="RequiredFieldIdentificadorResource">*</asp:RequiredFieldValidator>

             <asp:Label ID="LblSalaConferencia" runat="server" meta:resourcekey="LblSalaConferenciaResource" 
                        Style="z-index: 116; left: 15px; position: absolute; top: 22px; left: 205px; width:165px;" 
                        Text="Id. Sala de Conferencia:" Visible="True"></asp:Label>
            <asp:TextBox ID="TxtIdSalaConferencia" runat="server" Style="z-index: 117; left: 205px; position: absolute; top: 42px; height: 22px; width: 140px;"
                Visible="True" MaxLength="18" meta:resourcekey="TxtIdSalaConferenciaResource"></asp:TextBox>
             <asp:RequiredFieldValidator ID="RFVSalaConferencia" runat="server" ControlToValidate="TxtIdSalaConferencia"
                ErrorMessage="Rellene el campo Sala"
                Style="left: 172px; position: absolute; top: 42px" Enabled="False"
                meta:resourcekey="RRFVSalaConferenciaResource">*</asp:RequiredFieldValidator>
             
                <asp:Label ID="LblModoConferencia" runat="server" Style="z-index: 111; left: 400px; position: absolute; top: 22px"
                    Text="Modo de Conferencia"  Visible = "True" meta:resourcekey="LblModoConferenciaResource"></asp:Label>
				<asp:DropDownList ID="DDModoConferencia" runat="server" Style="z-index: 201; left: 400px; position: absolute; top: 42px; height:auto"
                    Width="180px" AppendDataBoundItems="True" 
                    Visible = "True" AutoPostBack="True">
                    <asp:ListItem Value="1" meta:resourcekey="ListItemModoConferenciaResource1">Meet Me</asp:ListItem>
                    <asp:ListItem Value="3" meta:resourcekey="ListItemModoConferenciaResource3">Preset</asp:ListItem>
                    </asp:DropDownList>

                <asp:Label ID="LblPosHMI"  runat="server" Style="z-index: 104; left: 630px; position: absolute; top: 22px; height: auto; width: auto;"
                    Text="Posición HMI"  Visible = "True" meta:resourcekey="LblPosHMIResource"></asp:Label>
                <asp:DropDownList ID="DDLPosHMI" runat="server"   Visible = "True"
                    meta:resourcekey="DDLPosHMIResource" Style="z-index: 105; left: 630px; 
                    position: absolute; top: 42px; height: 19px; width: 50px;">
                </asp:DropDownList>

            <asp:Label ID="LblAlias" runat="server" meta:resourcekey="LblAliasResource" 
                Style="z-index: 116; left: 15px; position: absolute; top: 79px; width:165px;" 
                Text="Alias:" Visible="True"></asp:Label>
            <asp:TextBox ID="TxtAlias" runat="server" 
                Style="z-index: 117; left: 52px; position: absolute; top: 77px; height: 22px; width: 140px;"
                Visible="True" MaxLength="16"
                meta:resourcekey="TxtAliasResource"></asp:TextBox>

            <asp:Label ID="LblDescripcion" runat="server" meta:resourcekey="LblDescripcionResource" 
                Style="z-index: 116; left: 210px; position: absolute; top: 79px; width:165px;" 
                Text="Descripción:" Visible="True"></asp:Label>
            <asp:TextBox ID="TxtDescripcion" runat="server" 
                Style="z-index: 117; left: 296px; position: absolute; top: 77px; height: 22px; width: 500px;"
                Visible="True" MaxLength="80"
                meta:resourcekey="TxtDescripcionResource"></asp:TextBox> 

            <asp:Label ID="LblSectores" runat="server" Style="z-index: 110; left:15px; position: absolute; top: 114px;height: 17px;"
                Text="Internos:"  Visible="False"
                meta:resourcekey="LblSectoresResource1">
            </asp:Label>
           <asp:ListBox ID="LBSectores" runat="server" Visible="False"  AutoPostBack="True"  SelectionMode="Multiple"
             SkinID="MascaraListaElementos"  Style="z-index: 100; left: 15px; width:240px; height:320px; position: absolute; top: 132px" meta:resourcekey="LBSectoresResource">
            </asp:ListBox>

            <asp:Label ID="LblParticipantes" runat="server" meta:resourcekey="LblParticipantesnResource" 
                Style="z-index: 116; left: 447px; position: absolute; top: 114px; width:165px;" 
                Text="Participantes:" Visible="True"></asp:Label>
           <asp:ListBox ID="LBParticipantes" runat="server" Visible="True"  AutoPostBack="True"  OnSelectedIndexChanged="LBParticipantes_SelectedIndexChanged"
             SkinID="MascaraListaElementos"  Style="z-index: 100; left: 447px; width:338px; height:320px; position: absolute; top: 132px" meta:resourcekey="LBParticipantessResource">
            </asp:ListBox>

            <asp:ImageButton ID="IButAsignar" runat="server" OnClick="IButAsignar_Click" Style="z-index: 102; left: 331px; position: absolute; top: 209px; -webkit-transform: rotate(-90deg); -moz-transform: rotate(-90deg); -o-transform: rotate(-90deg); transform: rotate(-90deg);"
                CausesValidation="False"
                ImageUrl="~/Configuracion/Images/arrow.png" Visible="True"
                meta:resourcekey="IButAsignarResource1" />
            <asp:ImageButton ID="IButQuitar" runat="server" Style="z-index: 103; left: 331px; position: absolute; top: 281px;-webkit-transform: rotate(90deg); -moz-transform: rotate(90deg); -o-transform: rotate(90deg); transform: rotate(90deg); "
                CausesValidation="False"
                ImageUrl="~/Configuracion/Images/arrow.png" OnClick="IButQuitar_Click"
                Visible="True" meta:resourcekey="IButQuitarResource1" />

            <asp:Panel ID="Panelbtn" runat="server" BackColor="White" BorderColor="Black" BorderStyle="Groove"
                    BorderWidth="0px" Height="70px" Style="left: 300px; position: absolute;
                    top: 378px; z-index: 113;" Visible="true" Width="104px" 
                    meta:resourcekey="PanelbtnResource">
                <asp:Button ID="BtInternos" runat="server" Style="left: 7px; position: absolute;
                    top: 9px" Text="Internos" Width="90px" OnClick="BtInternos_Click" 
                    meta:resourcekey="BtInternosResource"  />
                <asp:Button ID="BtExternos" runat="server" Style="left: 7px; position: absolute;
                    top: 37px" Text="Externos" Width="90px" OnClick="BtExternos_Click" 
                    meta:resourcekey="BtExternosResource"  />
            </asp:Panel>

            <asp:Label ID="LblExternos" runat="server" Style="z-index: 110; left:15px; position: absolute; top: 114px;height: 17px;"
                Text="Externos:"  Visible="True"
                meta:resourcekey="LblExternosResource1">
            </asp:Label> 
            <asp:Panel ID="PanelExternos" runat="server" BackColor="White" BorderColor="Black" BorderStyle="Groove"
                    BorderWidth="1px" Height="237px" Style="left: 15px; position: absolute;
                    top: 132px; z-index: 113;" Visible="true" Width="288px" Enabled="false"  
                    meta:resourcekey="PanelbtnResource">

                 <asp:Label ID="LblUsuario" runat="server" meta:resourcekey="LblUsuarioResource" 
                            Style="z-index: 116; left: 2px; position: absolute; top: 10px; right: 631px;width:165px;" 
                            Text="Usuario:" Visible="True"></asp:Label>
                <asp:TextBox ID="TBUsuario" runat="server" Style="z-index: 117; left: 2px; position: absolute; top: 25px; height: 22px; width: 140px;"
                    Visible="True"  Enabled="False" MaxLength="32" meta:resourcekey="TBUsuarioResource"></asp:TextBox>

                <asp:Label ID="LblIP" runat="server" Style="z-index: 107; left: 2px; position: absolute;
                    top: 57px" Text="IP:" Visible="True"
		              meta:resourcekey="LblIPResource"></asp:Label>

                <asp:TextBox ID="TBIP" runat="server" Style="z-index: 103; left: 2px; position: absolute;
                    top: 72px" Visible="True"  Enabled="False" meta:resourcekey="TBIPResource"></asp:TextBox>

                <asp:Label ID="LblPort" runat="server" Style="z-index: 107; left: 2px; position: absolute;
                    top: 106px" Text=" Puerto:" Visible="True"
		              meta:resourcekey="LblPuertoResource"></asp:Label>

                <asp:TextBox ID="TBPort" runat="server" Style="z-index: 154;top: 121px; left: 2px; position: absolute;"  
                    Visible="True"  Enabled="False" Width="51px" Height="22px" MaxLength="5"></asp:TextBox>

                <asp:Label ID="LblDescripcionExt" runat="server" meta:resourcekey="LblDescripcionResource" 
                Style="z-index: 116; left: 2px; position: absolute; top: 157px; width:165px;" 
                Text="Desc.:" Visible="True"></asp:Label>
                <asp:TextBox ID="TxtDescripcionExt" runat="server" 
                Style="z-index: 117; left: 2px; position: absolute; top: 172px; height: 22px; width: 277px;"
                Visible="True" MaxLength="80"
                meta:resourcekey="TxtDescripcionResource"></asp:TextBox>

                <asp:Panel ID="PanelbtnExternos" runat="server" BackColor="White" BorderColor="Black" BorderStyle="Groove"
                        BorderWidth="0px" Height="30px" Style="left: 7px; position: absolute;
                        top: 207px; z-index: 113;" Visible="true" Width="104px" 
                        meta:resourcekey="PanelbtnResource">
                    <asp:Button ID="BtExternoNuevo" runat="server" Style="left: 7px; position: absolute;
                        top: 1px" Text="Añadir" Width="80px" OnClick="BtExternoNuevo_Click" 
                        meta:resourcekey="BtNuevoExternoResource"  />
                    <asp:Button ID="BtExternoModificar" runat="server" Style="left: 95px; position: absolute;
                        top: 1px" Text="Modificar" Width="80px" OnClick="BtExternoModificar_Click" 
                        meta:resourcekey="BtModificarExternosResource"  />
                    <asp:Button ID="BtExternoCancelar" runat="server" Style="left: 180px; position: absolute;
                        top: 1px" Text="Cancelar" Width="80px" OnClick="BtExternoCancelar_Click" 
                        meta:resourcekey="BtCancelarExternosResource"  />                    
                </asp:Panel> 
            </asp:Panel>                     
       </asp:Panel>

    <asp:Button ID="BtAceptar" runat="server" Style="z-index: 113; left: 366px; position: absolute; top: 533px"
        Text="Aceptar" Width="80px" Visible="False" UseSubmitBehavior="true"
        OnClick="BtAceptar_Click" meta:resourcekey="BtAceptarResource1" />
      <ajaxToolKit:ConfirmButtonExtender ID="BtAceptar_ConfirmButtonExtender" 
          runat="server" ConfirmText="" Enabled="True" TargetControlID="BtAceptar">
      </ajaxToolKit:ConfirmButtonExtender>

    <asp:LinkButton ID="BtCancelar" runat="server" OnClick="BtCancelar_Click" 
		  CausesValidation="False" Text="Cancelar" SkinID="MascaraBotonCancelar" 
		  meta:resourcekey="BtCancelarResource1"/>
      <ajaxToolKit:ConfirmButtonExtender ID="BtCancelar_ConfirmButtonExtender" 
          runat="server" ConfirmText="" Enabled="True" TargetControlID="BtCancelar">
      </ajaxToolKit:ConfirmButtonExtender>

    <asp:LinkButton ID="BtModificar" runat="server" OnClick="BtModificar_Click" 
		  CausesValidation="False" Text="Modificar" SkinID="MascaraBotonModificar" 
		  meta:resourcekey="BtModificarResource1"/>
    <asp:LinkButton ID="BtNuevo" runat="server"	OnClick="BtNuevo_Click" 
		  CausesValidation="False" Text="Nuevo" SkinID="MascaraBotonNuevo" 
		  meta:resourcekey="BtNuevoResource1"/>

    <asp:LinkButton ID="BtEliminar" runat="server" OnClick="BtEliminar_Click" 
		  CausesValidation="False" Text="Eliminar" SkinID="MascaraBotonEliminar" 
		  meta:resourcekey="BtEliminarResource1"/>
    <ajaxToolkit:ConfirmButtonExtender ID="BtEliminar_ConfirmButtonExtender"
        runat="server" ConfirmText="" Enabled="True" TargetControlID="BtEliminar">
    </ajaxToolkit:ConfirmButtonExtender>
</asp:Content>


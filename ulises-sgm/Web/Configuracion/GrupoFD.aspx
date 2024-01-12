<%@ Page Language="C#" MasterPageFile="~/CD40.master" AutoEventWireup="true" CodeFile="GrupoFD.aspx.cs" Inherits="GrupoFD" 
		CodeFileBaseClass="PageBaseCD40.PageCD40" Title="Gestión de Destinos Radio" EnableEventValidation="false" StyleSheetTheme="TemaPaginasConfiguracion" meta:resourcekey="PageResource1" %>
<asp:Content ID="Content1" ContentPlaceHolderID="ContentPlaceHolder1" Runat="Server">
     
    <script type="text/javascript"> 
			function AbreVentana(ventana) {
			window.open(ventana);
			}

	</script>

    <asp:Label ID="Label1" runat="server" Style="z-index: 112; position: absolute; " 
			Text="GESTIÓN DESTINOS RADIO" CssClass="labelPagina" 
		  meta:resourcekey="Label1Resource1"></asp:Label>
	 <asp:ListBox ID="ListBox1" runat="server"
         OnSelectedIndexChanged="ListBox1_SelectedIndexChanged" Visible="True"  AutoPostBack="True"
         SkinID="MascaraListaElementos"  Style="z-index: 100; left: 21px; width:172px; height:460px; position: absolute; top: 58px" meta:resourcekey="ListBox1Resource1">
    </asp:ListBox>
        <asp:Panel ID="Panel1" runat="server" BorderStyle="Inset" Height="455px" Style="z-index: 105; left: 200px; position: absolute; top: 60px"
            Width="800px" Enabled="False"
            meta:resourcekey="Panel1Resource1">
            <asp:Label ID="Label5" runat="server" meta:resourcekey="Label5Resource1" Style="z-index: 116; left: 15px; position: absolute; top: 22px; right: 631px;width:165px;" Text="Id. Destino:" Visible="True"></asp:Label>
            <asp:TextBox ID="TxtIdEnlace" runat="server" Style="z-index: 117; left: 14px; position: absolute; top: 42px; height: 22px; width: 140px;"
                Visible="True" MaxLength="32"
                meta:resourcekey="TxtIdDestino"></asp:TextBox>
             <asp:RequiredFieldValidator ID="RequiredFieldIdentificador" runat="server" ControlToValidate="TxtIdEnlace"
                ErrorMessage="Rellene el campo Identificador"
                Style="left: 172px; position: absolute; top: 42px" Enabled="False"
                meta:resourcekey="RequiredFieldIdentificadorResource1">*</asp:RequiredFieldValidator>

			<asp:CheckBox ID="CBSeguro" runat="server" Style="z-index: 119; left: 14px; position: absolute;
					        top: 120px" Text="Seguro: " TextAlign="Left" Enabled="False" 
				AutoPostBack="True" oncheckedchanged="CBSeguro_OnCheckedChanged" Font-Size="Medium" Height="14px" Font-Names="Arial"
				meta:resourcekey="LSeguroResource1"/>

          <asp:CheckBox ID="CheckFrecNoDesasignable" runat="server" Style="z-index: 122; left: 14px; position: absolute; top: 139px"
                Text="Frec. no desasignable" TextAlign="Left" Width="153px"
                Visible="false" Enabled="false" meta:resourcekey="CheckFrecNoDesasignableResource1" Font-Size="Medium" Height="14px" Font-Names="Arial" />

          <asp:CheckBox ID="CheckPasivoRetransmision" runat="server" Style="z-index: 122; left: 14px; position: absolute; top: 184px"
                Text="Pasivo Retrans." TextAlign="Left" Width="153px"
                Visible="false" Enabled="false" meta:resourcekey="CheckPasivoRetransmisionResource1" Font-Size="Medium" Height="14px" Font-Names="Arial" />

            <asp:Label ID="Label6" runat="server" Style="z-index: 111; left: 10px; position: absolute; top: 211px"
                Text="Emplazamientos" Visible="false" meta:resourcekey="Label6Resource1"></asp:Label>

            <asp:ListBox ID="ListEmplazamientos" runat="server" meta:resourcekey="ListRecursosResource1"
                Enabled="False" Style="overflow-x:auto; z-index: 120; left: 11px; position: absolute; top: 231px; width: 120px; height: 195px;" class="listbox" EnableViewState="true" SelectionMode="Multiple" Rows="10">
            </asp:ListBox>

            <asp:Label ID="Label8" runat="server" Style="z-index: 110; left: 140px; position: absolute; top: 211px;  height: 17px;"
                Text="Tipo:" Visible="false" meta:resourcekey="Label8Resource1"></asp:Label>
            <asp:ListBox ID="ListTipos" runat="server" Enabled="false"
                meta:resourcekey="LisTipoResource1" Style="overflow-x:auto; z-index: 120; left: 140px; position: absolute; top: 231px;width: 120px; height: 195px;" SelectionMode="Multiple"></asp:ListBox>

            <asp:Label ID="Label3" runat="server" Style="z-index: 110; left: 11px; position: absolute; top: 214px;height: 17px;"
                Text="Rec. Asignados"
                meta:resourcekey="Label3Resource1"></asp:Label>

            <asp:ListBox ID="ListRecursos" runat="server"
                meta:resourcekey="ListRecursosResource1"  Style="overflow-x:auto; z-index: 120; left: 11px; position: absolute; top: 231px; width: 400px; height: 195px;font-family: Courier New;font-size:13px;" Visible="true" SelectionMode="Multiple"></asp:ListBox>
           <asp:Label ID="Label4" runat="server" Style="z-index: 111; left: 480px; position: absolute; top: 214px"
                Text="Recursos Libres " Visible="False"
                meta:resourcekey="Label4Resource1"></asp:Label>
            <asp:ListBox ID="ListRecursosLibres" runat="server" Style="overflow-x:auto; z-index: 121; left: 480px; position: absolute; top: 231px; width: 305px; height: 190px; font-family: Courier New;font-size:13px;"
                Visible="False" SelectionMode="Multiple"  meta:resourcekey="ListRecursosLibresResource1"></asp:ListBox>


            <asp:ListBox ID="ListRecursosMF" runat="server" Style="overflow-x:auto; z-index: 121; left: 480px; position: absolute; top: 231px; width: 305px; height: 190px; font-family: Courier New;font-size:13px;"
                Visible="False" meta:resourcekey="ListRecursosLibresResource1"></asp:ListBox>

            <asp:ListBox ID="ListRecursosMFLibres" runat="server" Style="overflow-x:auto; z-index: 121; left: 480px; position: absolute; top: 231px; width: 305px; height: 190px; font-family: Courier New;font-size:13px;"
                Visible="False" meta:resourcekey="ListRecursosLibresResource1"></asp:ListBox>

            <asp:Label ID="LblFiltroEmplazamiento" runat="server" Style="z-index: 111; left: 290px; position: absolute; top: 429px"
                Text="Filtrar por emplazamiento" Visible="False"
                meta:resourcekey="FiltrarPorEmplazamientoResource"></asp:Label>

            <asp:DropDownList ID="DListEmplazamiento" runat="server" Enabled="False" Style="z-index: 107; left: 510px; position: absolute; top: 427px; height: 20px; width: 120px; "  class="select" OnSelectedIndexChanged="DListEmplazamiento_SelectedIndexChanged" AutoPostBack="true"
                        AppendDataBoundItems="True" meta:resourcekey="DListEmplazamientoResource1" Visible="False">
                        <asp:ListItem Value="0" meta:resourcekey="ListEmplazamientoResource" Text="Todos">&lt;Selecciona&gt;</asp:ListItem>
            </asp:DropDownList>

            <asp:DropDownList ID="DropDownFiltro" runat="server" Style="z-index: 107; left: 650px; position: absolute; top:427px; height: 20px; width: 120px;"  class="select" OnSelectedIndexChanged="DFiltro_SelectedIndexChanged" AutoPostBack="true"
                        AppendDataBoundItems="True" Visible="False">
                <asp:ListItem Value="0" meta:resourcekey="DropDownFiltroItemResource1">Tipo</asp:ListItem>
                <asp:ListItem Value="1" meta:resourcekey="DropDownFiltroItemResource2">Emplazamiento</asp:ListItem>
            </asp:DropDownList>
            
             <asp:ListBox ID="ListTiposLibres" runat="server" Style="z-index: 121; left: 650px; position: absolute; top: 231px; width: 120px; height: 190px; font-family: Courier New;font-size:13px;"
                Visible="False" SelectionMode="Multiple"  meta:resourcekey="ListTiposLibresResource1"></asp:ListBox>

            <asp:ListBox ID="ListEmplazamientosLibres" runat="server" Style="overflow-x:auto; z-index: 121; left: 650px; position: absolute; top: 231px; width: 120px; height: 190px; font-family: Courier New;font-size:13px;"
                Visible="False" SelectionMode="Multiple" meta:resourcekey="ListTiposLibresResource1"></asp:ListBox>
  
            <asp:ImageButton ID="IButAsignar" runat="server" OnClick="IButAsignar_Click" Style="z-index: 102; left: 427px; position: absolute; top: 281px; -webkit-transform: rotate(90deg); -moz-transform: rotate(90deg); -o-transform: rotate(90deg); transform: rotate(90deg);"
                CausesValidation="False"
                ImageUrl="~/Configuracion/Images/arrow.png" Visible="True"
                meta:resourcekey="IButAsignarResource1" />
                    &nbsp;
            <asp:ImageButton ID="IButQuitar" runat="server" Style="z-index: 103; left: 427px; position: absolute; top: 341px;-webkit-transform: rotate(-90deg); -moz-transform: rotate(-90deg); -o-transform: rotate(-90deg); transform: rotate(-90deg); "
                CausesValidation="False"
                ImageUrl="~/Configuracion/Images/arrow.png" OnClick="IButQuitar_Click"
                Visible="True" meta:resourcekey="IButQuitarResource1" />

             <asp:Label ID="Label2" runat="server" Style="z-index: 101; left: 14px; position: absolute; top: 75px"
                Text="Tipo:" Visible="True" meta:resourcekey="Label2Resource1"></asp:Label>
            <asp:DropDownList ID="DListTipo" runat="server" Style="z-index: 105; left: 14px; height: 22px; position: absolute; top: 95px"
                Width="90px" Visible="True" OnSelectedIndexChanged="DListTipo_SelectedIndexChanged"
                meta:resourcekey="DListTipoResource1" class="select" AutoPostBack="True">
                <asp:ListItem Value="0" meta:resourcekey="ListItemResource1">VHF</asp:ListItem>
                <asp:ListItem Value="1" meta:resourcekey="ListItemResource2">UHF</asp:ListItem>
                <asp:ListItem Value="2" meta:resourcekey="ListItemResource3">HF</asp:ListItem>
            </asp:DropDownList>

             <asp:Table runat="server" ID="TblTunedFreq" Style="left: 14px; position: absolute; top: 115px; right: 460px;" Visible="false">
                <asp:TableRow ID="TableRow1" runat="server" >
                    <asp:TableCell ID="TableCell1" runat="server"  Style="width:40%;">
                        <asp:Label runat="server" ID="LblTunedFreq" meta:resourcekey="LblTunedFreqResource1"></asp:Label>
                    </asp:TableCell>
                    <asp:TableCell ID="TableCell2" runat="server" Style="width:40%;" >
                        <asp:TextBox ID="TBTunedFrequency" runat="server" Width="100px" Style="left:50px" ></asp:TextBox>
                    </asp:TableCell>
                    <asp:TableCell ID="TableCell3" runat="server">
                        <asp:Label ID="Label10" runat="server">Hz</asp:Label>
                    </asp:TableCell>
                </asp:TableRow>
            </asp:Table>

            <asp:Label ID="LbModoTransmision" runat="server" Style="z-index: 101; left: 188px; position: absolute; top: 165px"
                Text="Modo de Transmisión:" Visible="false" Enabled="false" meta:resourcekey="LbModoTransmisionResource"></asp:Label>
            <asp:DropDownList ID="DListModoTransmision" runat="server" Style="z-index: 105; left: 188px; height: 22px; position: absolute; top: 183px"
                Width="145px" Visible="false" Enabled="false" OnSelectedIndexChanged="DListModoTransmision_SelectedIndexChanged" AutoPostBack="True"
                meta:resourcekey="DListModoTransmisionRes" class="select">
                <asp:ListItem Value="C" meta:resourcekey="ListItemModoTransmisionClimaxRes">Climax</asp:ListItem>
                <asp:ListItem Value="R" meta:resourcekey="ListItemModoTransmisionUltRecepcionRes">Última Recepción</asp:ListItem>
                <%--<asp:ListItem Value="M" meta:resourcekey="ListItemModoTransmisionManualRes">Manual</asp:ListItem>  --%> 
            </asp:DropDownList>

            <asp:Label ID="LbEmplazamientoDefecto" runat="server" Style="z-index: 101; left: 363px; position: absolute; top: 165px"
                Text="Emplazamiento por Defecto:" Visible="False" meta:resourcekey="LbEmplazamientoDefectoResource"></asp:Label>
            <asp:DropDownList ID="DListEmplazamientoDefecto" runat="server" Style="z-index: 105; left: 363px; height: 22px; position: absolute; top: 183px"
                Width="200px" Visible="False" OnSelectedIndexChanged="DListEmplazamientoDefecto_SelectedIndexChanged" 
                meta:resourcekey="DListEmplazamientoDefectoRes" class="select" AutoPostBack="True">
                <asp:ListItem Value="0" meta:resourcekey="ListItemEmplDefNinguno">Ninguno</asp:ListItem>
            </asp:DropDownList>

            <asp:Label ID="LbTiempoVueltaADefecto" runat="server" Text="Tiempo Vuelta a Defecto" Style="left:604px; top:165px; position:absolute; width:auto"
                Visible="False" meta:resourcekey="LbTiempoVueltaADefectoResource"></asp:Label>

            <asp:TextBox ID="TxtTiempoVueltaADefecto" runat="server" Width="88px" Style="z-index: auto; top: 183px; left: 604px; position: absolute;"
                Visible="False" meta:resourcekey="TxtTiempoVueltaADefectoResource">0</asp:TextBox>

             <asp:RequiredFieldValidator ID="RFV_TBTunedFrequency" runat="server" ControlToValidate="TBTunedFrequency"
                ErrorMessage="La Frecuencia Sintonizada es requerida"
                Style="left: 330px; position: absolute; top: 132px" Enabled="False"
                meta:resourcekey="RFV_TBTunedFrequencyResource1">*</asp:RequiredFieldValidator>
            
            <asp:CustomValidator 
                 ID="valCustom" Style="left: 330px; position: absolute; top: 132px" Enabled="False"
                  runat="server">*</asp:CustomValidator>


             <asp:Label ID="Label41" runat="server" Style="z-index: 102; left:170px; position: absolute; top: 22px; height: 18px; width: 150px;"
             Text="Modo Destino:" meta:resourcekey="Label41Resource1"></asp:Label>
             <asp:DropDownList ID="DListModoDestino" runat="server" Style="z-index: 103; left: 170px; position: absolute; top: 42px; height: 22px; width: 88px;" 
                 OnSelectedIndexChanged="DListModoDestino_SelectedIndexChanged" AutoPostBack="True"
                 meta:resourcekey="DListModoDestinoResource1" class="select" ToolTip="Normal, FD">
                        <asp:ListItem Value="0"  meta:resourcekey="DListModoDestinoItemRes1">Normal</asp:ListItem>
                       <%-- <asp:ListItem Value="2" meta:resourcekey="DListModoDestinoItemRes2">MultiEmp</asp:ListItem> --%>
             </asp:DropDownList>

              <asp:Label ID="Label7" runat="server" Style="z-index: 102; left: 382px; position: absolute; top: 22px; height: 18px; width: 155px;"
             Text="Prioridad Sesión SIP:" Visible="false" Enabled="false" meta:resourcekey="Label7Resource1"></asp:Label>
             <asp:DropDownList ID="DListPrioridadSIP" Visible="false" Enabled="false" runat="server" Style="z-index: 103; left: 382px; position: absolute; top: 42px; height: 22px; width: 105px;" 
                        OnSelectedIndexChanged="DListPrioridadSIP_SelectedIndexChanged" AutoPostBack="True"
                        meta:resourcekey="DListGrupoResource1" class="select" ToolTip="Normal, Emergencia">
                        <asp:ListItem Value="2" meta:resourcekey="DListPrioridadSIPItemRes1">Normal</asp:ListItem>
                        <asp:ListItem Value="0" meta:resourcekey="DListPrioridadSIPItemRes2">Emergencia</asp:ListItem>
             </asp:DropDownList>

            <asp:Label ID="LbRedundancia" runat="server" Style="z-index: 102; left: 543px; position: absolute; top: 22px; height: 18px; width: 155px;"
             Text="1+1" Visible="false" Enabled="false" meta:resourcekey="CheckedRedundanciaResource1"></asp:Label>
             <asp:CheckBox ID="CheckBoxRedundancia" runat="server" Style="z-index: 103; left: 543px; position: absolute; top: 42px; width: 20px;"
                 OnCheckedChanged="CBRedundancia_CheckedChanged" AutoPostBack="true"
                    Visible="false" Enabled="false"/>

            <asp:Label ID="LbMultiFrecuencia" runat="server" Style="z-index: 102; left: 282px; position: absolute; top: 22px; height: 18px; width: 155px;"
             Text="MultiFrec."   Visible="False" Enabled="false" meta:resourcekey="CheckedMultiFrecuenciaResource1"></asp:Label>
             <asp:CheckBox ID="CheckMultiFrecuencia" runat="server" Style="z-index: 103; left: 302px; position: absolute; top: 42px; width: 20px;"
                 OnCheckedChanged="CBMultiFrecuencia_CheckedChanged" AutoPostBack="true"
                    Visible="False" Enabled=" False"/>

            <asp:Label ID="LbMFGestionMF" runat="server" Style="z-index: 111; left: 173px; position: absolute; top: 72px"
                Text="Gestión Multifrecuencia" Visible="False" meta:resourcekey="LbMFGestionMFResource1"></asp:Label>

            <asp:Label ID="LbMFrecuencia" runat="server" Style="z-index: 111; left: 173px; position: absolute; top: 127px"
                Text="F. (MHz)" Visible="False" meta:resourcekey="LbMFrecuenciaResource1"></asp:Label>
            <asp:TextBox ID="TBMFrecuencia" runat="server" Style="z-index: 117; left: 173px; position: absolute; top: 147px; height: 22px; width: 80px;"
                Visible="False" MaxLength="7" meta:resourcekey="TBMFrecuenciaResource1"></asp:TextBox>

            <asp:Label ID="LbMFrecuenciaDefecto" runat="server" Style="z-index: 111; left: 173px; position: absolute; top: 172px"
                Text="F. Defecto" Visible="False" meta:resourcekey="LbMFrecuenciaDefectoResource1"></asp:Label>
            <asp:TextBox ID="TBMFrecuenciaDefecto" runat="server" Style="z-index: 117; left: 173px; position: absolute; top: 192px; height: 22px; width: 80px;"
                Visible="False" MaxLength="7" meta:resourcekey="TBMFrecuenciaDefectoResource1"></asp:TextBox>

            <asp:Label ID="LbMFrecuencias" runat="server" Style="z-index: 111; left: 276px; position: absolute; top: 127px"
                Text="Frecuencias" Visible="False" meta:resourcekey="LbMFrecuenciasResource1"></asp:Label>
	         <asp:ListBox ID="ListBoxMFrecuencias" runat="server"
                 OnSelectedIndexChanged="ListBoxMFrecuencias_SelectedIndexChanged" Visible="False"  AutoPostBack="True"
                 SkinID="MascaraListaElementos"  Style="z-index: 100; left: 276px; width:80px; height:74px; position: absolute; top: 145px" meta:resourcekey="ListBoxMFrecuenciasResource1">
            </asp:ListBox>

            <asp:ImageButton ID="IButMFLimpia" runat="server" OnClick="IButMFLimpia_Click" Style="z-index: 102; left: 186px; position: absolute; top: 89px;"
                CausesValidation="false"
                ImageUrl="~/Configuracion/Images/btlimpia.png" 
                Visible="False"
                meta:resourcekey="IButMFLimpiaResource1" />
            
            <asp:ImageButton ID="IButMFAsignar" runat="server" OnClick="IButMFAsignar_Click" Style="z-index: 102; left: 226px; position: absolute; top: 89px;"
                CausesValidation="False"
                ImageUrl="~/Configuracion/Images/btmas.png" 
                Visible="False"
                meta:resourcekey="IButMFAsignarResource1" />

            <asp:ImageButton ID="IButMFDesAsignar" runat="server" OnClick="IButMFDesAsignar_Click" Style="z-index: 102; left: 266px; position: absolute; top: 89px;"
                CausesValidation="False"
                ImageUrl="~/Configuracion/Images/btmenos.png" 
                Visible="False"
                meta:resourcekey="IButMFAsignarResource1" />

            <asp:ImageButton ID="IButMFDefecto" runat="server" OnClick="IButMFDefecto_Click" Style="z-index: 102; left: 306px; position: absolute; top: 89px;"
                CausesValidation="False"
                ImageUrl="~/Configuracion/Images/btdefecto.png" 
                Visible="False"
                meta:resourcekey="IButMFDefectoResource1" />

             <asp:Label ID="Label9" runat="server" Style="z-index: 102; left: 598px; position: absolute; top: 22px; height: 18px; width: 155px;"
             Text="Método Climax:" Visible="false" Enabled="false" meta:resourcekey="Label10Resource"></asp:Label>
             <asp:DropDownList ID="DListMetodoClimax" Visible="false" Enabled="false" runat="server" Style="z-index: 103; left: 600px; position: absolute; top: 42px; height: 22px; width: 88px;" OnSelectedIndexChanged="DListMetodoClimax_SelectedIndexChanged"
                        meta:resourcekey="DListMetodoClimaxResource1" class="select" ToolTip="Relativo, Absoluto">
                        <asp:ListItem Value="0"  meta:resourcekey="DListMetodoClimaxItemRes1">Relativo</asp:ListItem>
                        <asp:ListItem Value="1"  meta:resourcekey="DListMetodoClimaxItemRes2">Absoluto</asp:ListItem>                        
             </asp:DropDownList>

            <asp:CheckBox ID="CheckedSincro" runat="server" Enabled="False" Text="Sinc. Recepción en Grupo" Style="z-index: 103; left: 359px; position: absolute; top: 127px; width: 176px; margin-right: 9px;"
                  meta:resourcekey="CheckedSincroResource1" Visible="False"/>
            <asp:CheckBox ID="CheckBox1Squelch" runat="server" Enabled="False" Text="Audio en Primer Squelch" Style="z-index: 103; left: 548px; position: absolute; top: 127px; width: 176px; margin-right: 9px;" 
                   meta:resourcekey="CheckBox1SquelchResource1" Visible="False" />


            <asp:Label ID="Label44" runat="server" Visible="false" Enabled="false" Text="Periodo cálculo de retardo (s):" Style="left:382px; top:129px; position:absolute; width:auto"
                meta:resourcekey="Label44Resource1"></asp:Label>
                <asp:TextBox ID="TextBoxCLD" Visible="false" Enabled="false" runat="server" Width="88px" Style="z-index: auto; top: 129px; left: 610px; position: absolute;"
                meta:resourcekey="TxtTiempoCLDResource1">1</asp:TextBox>
            <asp:RequiredFieldValidator ID="RequiredFieldValidator18" runat="server" ControlToValidate="TextBoxCLD" Style="z-index: auto; top: 98px; left: 280px; position: absolute;"
            ErrorMessage="El periodo de cálculo CLD debe estar comprendido entre 0 y 60 segundos."
            meta:resourcekey="RequiredFieldValidator18Resource1">*</asp:RequiredFieldValidator>
            <asp:CompareValidator ID="CompareValidator15" runat="server" ControlToValidate="TextBoxCLD"
            ErrorMessage="El campo GRS Delay debe ser numérico." Operator="DataTypeCheck"
            Style="z-index: 207; left: 269px; position: absolute; top: 94px"
            Type="Integer"  meta:resourcekey="CompareValidator15Resource1">*</asp:CompareValidator> 
            <asp:RangeValidator ID="RangeTextBoxCLD" runat="server" ControlToValidate="TextBoxCLD"
			ErrorMessage="El periodo de cálculo CLD debe estar comprendido entre 0 y 60 segundos." MaximumValue="60"
			MinimumValue="0" Style="z-index: 207; left: 280px; position: absolute; top: 98px; width: 8px;"
			Type="Double" meta:resourcekey="RVTxtCLDResource1">*</asp:RangeValidator>

            <asp:Label ID="Label11" runat="server" Visible="false" Enabled="false" meta:resourcekey="Label11Resource1" Style="z-index: 116; left: 382px; position: absolute; top: 75px; right: 208px; height: 19px;" Text="Ventana BSS (ms):"></asp:Label>
            <asp:TextBox ID="TextVentanaBSS" Visible="false" Enabled="false" runat="server" Style="z-index: 117; left: 382px; position: absolute; top: 95px; height: 22px; width: 87px;" MaxLength="32" meta:resourcekey="TxtVentanaBSS" Text="50"></asp:TextBox>

              <asp:RequiredFieldValidator ID="RequiredFieldValidator4" runat="server" Visible="false" Enabled="false"
							     ControlToValidate="TextVentanaBSS" Style="z-index: 117; left: 470px; position: absolute; top: 98px"
							     meta:resourcekey="RequiredFieldValidator4Resource1">*</asp:RequiredFieldValidator>
					<asp:CompareValidator ID="CompareValidator2" runat="server" ControlToValidate="TextVentanaBSS"
							    Operator="DataTypeCheck" Type="Integer" Width="17px"  Style="z-index: 117; left: 480px; position: absolute; top: 95px"
							     meta:resourcekey="CompareValidator2Resource1">*</asp:CompareValidator>
                    <asp:RangeValidator ID="RVTexVentanaBSS" runat="server" ControlToValidate="TextVentanaBSS"
					    ErrorMessage="La ventana de selección BSS debe estar comprendido entre 200 y 2000 ms" MaximumValue="2000"
					    MinimumValue="200" Style="z-index: 117; left: 490px; position: absolute; top: 95px"
					    Type="Double" meta:resourcekey="RVTxtVentanaBSS">*</asp:RangeValidator>
             <asp:Label ID="Label12" Visible="false" Enabled="false" runat="server" Style="z-index: 102; left: 543px; position: absolute; top: 75px; width: 180px;"
                Text="Métodos BSS:" meta:resourcekey="Label12Resource"></asp:Label>
             <asp:DropDownList ID="DDLMetodosBssOfrecidos"  Visible="false" Enabled="false" runat="server" AutoPostBack="True" Style="z-index: 103; left: 543px; position: absolute; top: 95px; width: 145px;" 
                        class="select" OnSelectedIndexChanged="DDLMetodosBssOfrecidos_SelectedIndexChanged" meta:resourcekey="DDLMetodosBsssResource1">
                        <asp:ListItem Value="0"  meta:resourcekey="DDLMetodosBssItemRes1">Ninguno</asp:ListItem>
                        <asp:ListItem Value="1"  meta:resourcekey="DDLMetodosBssItemRes2">RSSI</asp:ListItem>
                        <asp:ListItem Value="2"  meta:resourcekey="DDLMetodosBssItemRes3">RSSI y NUCLEO</asp:ListItem>
                        <asp:ListItem Value="3"  meta:resourcekey="DDLMetodosBssItemRes4">CENTRAL</asp:ListItem>
             </asp:DropDownList>
            <asp:DropDownList ID="DListTipoRec" runat="server" AutoPostBack="True" class="select" Enabled="False" meta:resourcekey="DListTipoResource1" Style="z-index: 103; left: 232px; position: absolute; top: 407px; height: 19px; width: 99px;" Visible="False">
                <asp:ListItem Value="0" meta:resourcekey="ListTipoRec_Audio_RX">Audio RX</asp:ListItem>
                <asp:ListItem Value="1" meta:resourcekey="ListTipoRec_Audio_TX">Audio TX</asp:ListItem>
                <asp:ListItem Value="2" meta:resourcekey="ListTipoRec_Audio_RXTX">Audio RX TX</asp:ListItem>
                <asp:ListItem Value="3" meta:resourcekey="ListTipoRec_Audio_HF">Audio HF-TX</asp:ListItem>
                <asp:ListItem Value="4" meta:resourcekey="ListTipoRec_Audio_MN">Audio M+N</asp:ListItem>
                <asp:ListItem Value="10" meta:resourcekey="ListTipoRec_Audio_RXTX_SIRTAP">Audio RX TX SIRTAP</asp:ListItem>
                <%--//20200525 JOI  #4470 Version 2.6.0 no se implementa--%>
            <%--<asp:ListItem Value="7" meta:resourcekey="ListTipoRec_Audio_EE_RX">
                Audio EE RX</asp:ListItem>
                <asp:ListItem Value="8" meta:resourcekey="ListTipoRec_Audio_EE_TX">Audio EE TX</asp:ListItem>
                <asp:ListItem Value="9" meta:resourcekey="ListTipoRec_Audio_EE_RXTX">Audio EE RX TX</asp:ListItem>--%>

            <%--// 20211210 #2857 Analisis centralizado QIDX--%>
            </asp:DropDownList>
            <asp:Label ID="LBPorCentral"  Visible="false" Enabled="false" runat="server" Style="z-index: 104; left: 723px; position: absolute; top: 75px; height: auto; width: auto;"
                Text="%RSSI Ext." meta:resourcekey="LBPorCentralResource"></asp:Label>

            <asp:DropDownList ID="DLPorcentajeRSSI" runat="server" class="select" Visible="false" Enabled="false" 
                meta:resourcekey="DLPorcentajeRSSIResource" Style="z-index: 105; left: 723px; 
                position: absolute; top: 95px; height: 19px; width: 50px;">
                <asp:ListItem Value="0">0</asp:ListItem>
                <asp:ListItem Value="10">10</asp:ListItem>
                <asp:ListItem Value="20">20</asp:ListItem>
                <asp:ListItem Value="30">30</asp:ListItem>
                <asp:ListItem Value="40">40</asp:ListItem>
                <asp:ListItem Value="50">50</asp:ListItem>
                <asp:ListItem Value="60">60</asp:ListItem>
                <asp:ListItem Value="70">70</asp:ListItem>
                <asp:ListItem Value="80">80</asp:ListItem>
                <asp:ListItem Value="90">90</asp:ListItem>
                <asp:ListItem Value="100">100</asp:ListItem>
            </asp:DropDownList>   
        </asp:Panel>





    <asp:Button ID="BtAceptar" runat="server" Style="z-index: 113; left: 366px; position: absolute; top: 533px"
        Text="Aceptar" Width="80px" Visible="False" UseSubmitBehavior="true"
        OnClick="BtAceptar_Click" meta:resourcekey="BtAceptarResource1" />

    <asp:ValidationSummary ID="errores" runat="server" HeaderText="Resumen de Errores:" ForeColor="Red"
        Style="z-index: 114; left: 240px; position: absolute; top: 580px"
        Visible="False" meta:resourcekey="erroresResource1" />
    <asp:Label ID="LblErrorMismatchFrequency" runat="server" Style="z-index: 115; left: 240px; position: absolute; top: 580px; height: 12px; width: 535px;" ForeColor="Red"
        Visible="False" meta:resourcekey="LblMismatchFrequency">Los recursos asignados tienen configurada distinta frecuencia</asp:Label> 

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



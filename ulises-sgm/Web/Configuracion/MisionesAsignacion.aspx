
<%@ Page Language="C#" MasterPageFile="~/CD40.master" AutoEventWireup="true" CodeFile="MisionesAsignacion.aspx.cs" Inherits="MisionesAsignacion" 
		CodeFileBaseClass="PageBaseCD40.PageCD40" Title="Asignación de Operadores a Misiones" EnableEventValidation="false" StyleSheetTheme="TemaPaginasConfiguracion" meta:resourcekey="PageResource1" %>
<asp:Content ID="Content1" ContentPlaceHolderID="ContentPlaceHolder1" Runat="Server">
     
    <script type="text/javascript">
        function AbreVentana(ventana) {
            window.open(ventana);
        }
	</script>

    <asp:Label ID="Label1" runat="server" Style="z-index: 112; position: absolute; " 
			Text="ASIGNACIÓN DE MISIONES" CssClass="labelPagina" 
		  meta:resourcekey="Label1Resource1"></asp:Label>
	 <asp:ListBox ID="ListBox1" runat="server" Height="423px" Style="z-index: 105; left: 14px; position: absolute; top: 50px"
            Width="171px" Enabled="True" Visible="true"
		  OnSelectedIndexChanged="ListBox1_SelectedIndexChanged" AutoPostBack="True"
					SkinID="MascaraListaElementos" meta:resourcekey="ListBox1Resource1">
    </asp:ListBox>

        <asp:Panel ID="Panel1" runat="server" BorderStyle="Inset" Height="420px" Style="z-index: 105; left: 200px; position: absolute; top: 50px"
            Width="677px" Enabled="true"
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
                Width="120px" Visible="True" Enabled="false"
                meta:resourcekey="DListTipoResource1">
                <asp:ListItem Value="0" meta:resourcekey="ListItemResource1">Real</asp:ListItem>
                <asp:ListItem Value="1" meta:resourcekey="ListItemResource2">Entrenamiento</asp:ListItem>
            </asp:DropDownList>



            <asp:Label ID="Label3" runat="server" Style="z-index: 110; left: 121px; position: absolute; top: 142px;height: 17px;" Visible="true"
                Text="Operadores asignados" 
                meta:resourcekey="Label3Resource1"></asp:Label>
            <asp:ListBox ID="ListOperadoresAsignados" runat="server" Visible="true"
                meta:resourcekey="ListOperadoresAsignadosResource1"  Style="overflow-x:auto; z-index: 120; left: 121px; position: absolute; top: 165px; width: 178px; height: 195px;font-family: Courier New;font-size:13px;"  SelectionMode="Multiple"></asp:ListBox>
          
            <asp:Label ID="Label4" runat="server" Style="z-index: 111; left: 404px; position: absolute; top: 142px"
                Text="Operadores no asignados" Visible="true"
                meta:resourcekey="Label4Resource1"></asp:Label>
            <asp:ListBox ID="ListOperadoresNoAsignados" runat="server" Style="overflow-x:auto; z-index: 121; left: 404px; position: absolute; top: 165px; width: 178px; height: 195px; font-family: Courier New;font-size:13px;"
                Visible="true" SelectionMode="Multiple"  meta:resourcekey="ListOperadoresNoAsignadosResource1"></asp:ListBox>

            <asp:ImageButton ID="IButAsignar" runat="server" OnClick="IButAsignar_Click" Style="z-index: 102; left: 334px; position: absolute; top: 202px; -webkit-transform: rotate(90deg); -moz-transform: rotate(90deg); -o-transform: rotate(90deg); transform: rotate(90deg);"
                CausesValidation="False"
                ImageUrl="~/Configuracion/Images/arrow.png" Visible="True"
                meta:resourcekey="IButAsignarResource1" />
                    &nbsp;
            <asp:ImageButton ID="IButQuitar" runat="server" Style="z-index: 103; left: 334px; position: absolute; top: 262px;-webkit-transform: rotate(-90deg); -moz-transform: rotate(-90deg); -o-transform: rotate(-90deg); transform: rotate(-90deg); "
                CausesValidation="False"
                ImageUrl="~/Configuracion/Images/arrow.png" OnClick="IButQuitar_Click"
                Visible="True" meta:resourcekey="IButQuitarResource1" />

        </asp:Panel>

    <asp:Button ID="BtAceptar" runat="server" Style="z-index: 113; left: 365px; position: absolute; top: 700px"
        Text="Aceptar" Width="80px" Visible="True" UseSubmitBehavior="true"
        OnClick="BtAceptar_Click" meta:resourcekey="BtAceptarResource1" />
    <ajaxToolkit:ConfirmButtonExtender ID="BtAceptar_ConfirmButtonExtender"
        runat="server" ConfirmText="" Enabled="True" TargetControlID="BtAceptar">
    </ajaxToolkit:ConfirmButtonExtender>

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
        runat="server"  ConfirmText="¿Esta usted seguro de eliminar los operadores asignados a la misión?" Enabled="True" TargetControlID="BtEliminar">
    </ajaxToolkit:ConfirmButtonExtender>
</asp:Content>
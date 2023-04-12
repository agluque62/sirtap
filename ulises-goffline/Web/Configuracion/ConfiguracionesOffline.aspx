<%@ Page Language="C#"  MasterPageFile="~/CD40.master" AutoEventWireup="true" CodeFile="ConfiguracionesOffline.aspx.cs" Inherits="ConfiguracionesOffline"
  CodeFileBaseClass="PageBaseCD40.PageCD40" Title="Gestión de configuraciones offline" EnableEventValidation="false" StylesheetTheme="TemaPaginasConfiguracion" meta:resourcekey="PageResource1" %> 

<asp:Content ID="Content1" ContentPlaceHolderID="ContentPlaceHolder1" Runat="Server">
    <asp:Label ID="lblTitulo" runat="server" Text="GESTIÓN DE CONFIGURACIONES OFFLINE" CssClass="labelPagina" meta:resourcekey="lblTituloResource"></asp:Label>
    <br />
    <br />
    <br />
    <div id="configuraciones" style="padding-left:15px; padding-right:15px">
        <asp:GridView ID="gvConfiguraciones" runat="server" SkinID="GridViewSkin" AutoGenerateColumns="False" AllowPaging="True" CellPadding="4" PageSize="10"
            OnPageIndexChanging="gvConfiguraciones_PageIndexChanging"
            OnRowCommand="gvConfiguraciones_RowCommand"
            meta:resourcekey="gvConfiguracionesResource">
	        <Columns>
                <asp:BoundField ReadOnly="true" DataField="Nombre" HeaderText="Configuración" meta:resourcekey="gvColConfiguracionResource">
                    <ItemStyle Width="150px"></ItemStyle>
                </asp:BoundField>
                <asp:BoundField ReadOnly="true" DataField="Conf_Edicion" meta:resourcekey="gvColConfiguracionResource">
                </asp:BoundField>
                <asp:BoundField ReadOnly="true" DataField="Tipo" meta:resourcekey="gvColConfiguracionResource">
                </asp:BoundField>
                <asp:BoundField ReadOnly="true" DataField="Descripcion" HeaderText="Descripción" meta:resourcekey="gvColDescripcionnResource">
                    <ItemStyle Width="999px"></ItemStyle>
                </asp:BoundField>

		        <asp:TemplateField ShowHeader="False">
                    <ItemTemplate>
                        <asp:LinkButton ID="lnkEditar" runat="server" CausesValidation="false" CommandName="Editar" Text="Editar" Visible='<%# Eval("Tipo").ToString() == "ONLINE" ? false : true %>' ForeColor="#616365" />
		            </ItemTemplate>
                </asp:TemplateField>
                <asp:TemplateField ShowHeader="False">
                    <ItemTemplate>
                        <asp:LinkButton ID="lnkEliminar" runat="server" CausesValidation="false" CommandName="Eliminar" Text="Eliminar"
                            Visible='<%# Eval("Tipo").ToString() == "ONLINE" ? false : true %>' ForeColor="#616365" />
	                    <ajaxToolKit:ConfirmButtonExtender ID="lnkEliminar_ConfirmButtonExtender" 
                            runat="server" ConfirmText='<%# " ¿Desea eliminar la configuración " + Eval("Nombre") + "? " %>' Enabled="True" TargetControlID="lnkEliminar"> 
                        </ajaxToolKit:ConfirmButtonExtender>
		            </ItemTemplate>
                </asp:TemplateField>
                <asp:TemplateField ShowHeader="False">
                    <ItemTemplate>
                        <asp:LinkButton ID="lnkExportar" runat="server" CausesValidation="false" CommandName="Exportar" Text="Exportar" ForeColor="#616365" />
		            </ItemTemplate>
                </asp:TemplateField>
                <asp:TemplateField ShowHeader="False">
                    <ItemTemplate>
                        <asp:LinkButton ID="lnkCopiar" runat="server" CausesValidation="false" CommandName="Copiar" Text="Copiar" ForeColor="#616365" />
		            </ItemTemplate>
                </asp:TemplateField>
                <asp:TemplateField ShowHeader="False">
                    <ItemTemplate>
                        <asp:LinkButton ID="lnkActivar" runat="server" CausesValidation="false" CommandName="Activar" Text="Activar" Visible='<%# Eval("Tipo").ToString() == "ONLINE" ? false : true %>' ForeColor="#616365" />
	                    <ajaxToolKit:ConfirmButtonExtender ID="lnkActivar_ConfirmButtonExtender" 
                            runat="server" ConfirmText='<%# "Antes de ejecutar este proceso es conveniente hacer un backup de la configuración en producción para poder deshacer los cambios en caso de ser necesario. En el manual de usuario se puede consultar el procedimiento.\n ¿Desea pasar la configuración " + Eval("Conf_Edicion") + " a producción?" %>' Enabled="True" TargetControlID="lnkActivar"> 
                        </ajaxToolKit:ConfirmButtonExtender>
                    </ItemTemplate>
                </asp:TemplateField>
                

            </Columns>
		
        </asp:GridView>

        <br />
        <table align = "left">
        <tr>
            <td>
        	    <asp:LinkButton ID="lnkNueva" runat="server" OnClick="lnkNueva_Click" CausesValidation="False" Text="Nueva"  ForeColor="#616365" meta:resourcekey="BtnNuevoResource1"/>
            </td>
            <td style="width:10px"/>
            <td>
        	    <asp:LinkButton ID="lnkImportar" runat="server" OnClick="lnkImportar_Click" CausesValidation="False" Text="Importar"  ForeColor="#616365" meta:resourcekey="BtnModificarResource1"/>
            </td>
            <td style="width:10px"/>
            <td>
                <asp:LinkButton ID="lnkReplicar" runat="server" OnClick="lnkReplicar_Click" CausesValidation="False" Text="Sincronizar"  ForeColor="#616365" meta:resourcekey="BtnModificarResource1"/>
	            <ajaxToolKit:ConfirmButtonExtender ID="lnkReplicar_ConfirmButtonExtender" 
                    runat="server" ConfirmText="¿Desea sincronizar las configuraciones offline al otro servidor?" Enabled="True" TargetControlID="lnkReplicar"> 
                </ajaxToolKit:ConfirmButtonExtender>
            </td>
            <td style="width:10px"/>
        </tr>
        </table>
    </div>
    <br />
    <br />
    <br />

    <div id="logProcesos" style="padding-left:15px; padding-right:15px">
        <asp:Timer ID="timerRefrescoLogProcesos" runat="server" Interval="5000" Enabled="False" EnableViewState="True" OnTick="timerRefrescoLogProcesos_Tick"></asp:Timer>
        <asp:Label ID = "lblProcesando" runat = "server" Text = "Log de procesos" style = "font-family: Arial; font-weight: bold; font-size: 16px; color: #616365; background-color: Transparent;" ></asp:Label>
        <br />
        <br />
        <asp:GridView ID="gvLogProcesos" runat="server" AutoGenerateColumns="False" AllowPaging="True" CellPadding="4" PageSize="5" ShowHeader="false" GridLines="Horizontal" 
            OnPageIndexChanging="gvLogProcesos_PageIndexChanging"
            OnRowCommand="gvLogProcesos_RowCommand">
            <PagerStyle BackColor="#D5D6D7" ForeColor="Black" HorizontalAlign="Center"/>
	        <Columns>
                <asp:BoundField ReadOnly="true" DataField="Id" NullDisplayText=" "></asp:BoundField>
                <asp:BoundField ReadOnly="true" DataField="PID" NullDisplayText=" "></asp:BoundField>
                <asp:BoundField ReadOnly="true" DataField="Activo" NullDisplayText=" "></asp:BoundField>
                <asp:BoundField ReadOnly="true" DataField="FechaHora" NullDisplayText=" ">
                    <ItemStyle Width="200px" ForeColor="#616365"></ItemStyle>
                </asp:BoundField>
                <asp:BoundField ReadOnly="true" DataField="Descripcion" NullDisplayText="" meta:resourcekey="gvColDescripcionnResource">
                    <ItemStyle Width="999px" ForeColor="#616365" Font-Italic="true"></ItemStyle>
                </asp:BoundField>

                <asp:TemplateField ShowHeader="False">
                    <ItemTemplate>
                        <asp:LinkButton ID="lnkLogProcesos" runat="server" CausesValidation="false"  Width="130px" ForeColor="#616365" 
                            CommandName='<%# Eval("Activo").ToString() == "False" ? "Leido" : "Cancelar" %>' Text='<%# Eval("Activo").ToString() == "False" ? "Marcar como leído" : "Cancelar" %>'/>
	                    <ajaxToolKit:ConfirmButtonExtender ID="lnkLogProcesos_ConfirmButtonExtender" 
                            runat="server" ConfirmText='<%# Eval("Activo").ToString() == "False" ? "¿Desea marcar el proceso como leído?" : "¿Desea cancelar el proceso?" %>' Enabled="True" TargetControlID="lnkLogProcesos"> 
                        </ajaxToolKit:ConfirmButtonExtender>
                    </ItemTemplate>
                </asp:TemplateField>
            </Columns>		
        </asp:GridView>
    </div>

    <asp:Panel ID="pnlNuevaConfiguracion" runat="server" DefaultButton="btnNuevaConfAceptar" style = "background-color: #FFFFFF; border: 3px solid #e6e6e6; display:none" >
        <div style="background-color: #4A7729; height: 30px; line-height: 30px; text-align: center; font-weight: bold;">
            <asp:Label ID="Label4" runat="server" Text="Nueva Configuración" Visible="true" ForeColor="White"></asp:Label>
        </div>
        <br />        
        <div style="min-height: 50px; line-height: 30px; text-align: center; padding:5px">
            <table align = "center" width="auto">
            <tr>
                <td>
                    <asp:Label ID = "lblNuevaConfNombre" runat = "server" Text = "Nombre configuración" ></asp:Label>
                </td>
                <td>
                    <asp:TextBox ID="txtNuevaConfNombre" Width = "200px" runat="server"></asp:TextBox>
                </td>
            </tr>
            <tr>
                <td>
                    <asp:Label ID = "lblNuevaConfDescripcion" runat = "server" Text = "Descripción" ></asp:Label>
                </td>
                <td>
                    <asp:TextBox ID="txtNuevaConfDescripcion" Width="350px" runat="server"></asp:TextBox>   
                </td>
            </tr>
            <tr>
                <td/>
                <td/>
            </tr>
            <tr>
                <td/>
                <td>
                    <asp:Label ID = "lblNuevaConfFicheros" runat = "server" Text = "Ficheros disponibles" ></asp:Label>
                </td>
            </tr>
            <tr>
                <td/>
                <td>
        		    <asp:DropDownList id="lstNuevaConfFicheros" runat="server" Height="25px" Width="350px"  class="select"/>
                </td>
            </tr>
            </table>

            <div>
                <asp:RequiredFieldValidator ID="rNuevaConfNombre" runat="server" ErrorMessage="El nombre de la nueva configuración es obligatorio" ControlToValidate="txtNuevaConfNombre" Display="None"></asp:RequiredFieldValidator>
                <asp:RegularExpressionValidator ID="regExpNuevaConfNombre" ControlToValidate="txtNuevaConfNombre" runat="server" ErrorMessage="El nombre solo puede contener caracteres alfanuméricos y _" ValidationExpression="[\w_]+" Display="None"></asp:RegularExpressionValidator>
                <asp:ValidationSummary ID="ValidationNuevaConfNombre" runat="server" ShowMessageBox="true" ShowSummary="false" />
            </div>
            <div id="divImage" style="display:none">
                <asp:Image ID="imgProgress" runat="server" ImageUrl="~/images/progress.gif" />
                     Ejecutando...
            </div> 
            <div style="text-align: right; width: 100%;padding-top:10px;">
                <asp:TextBox ID="txtNuevaConfAccion" Visible="false" runat="server"></asp:TextBox>
                <asp:TextBox ID="txtNuevaConfOrigen" Visible="false" runat="server"></asp:TextBox>
                <asp:Button ID="btnNuevaConfAceptar" runat="server" Text="Aceptar" Style="height: 32px; width: 90px;" OnClick="btnNuevaConfAceptar_Click"/>
                <asp:Button ID="btnNuevaConfCancelar" runat="server" Text="Cancelar" Style="height: 32px; width: 90px;" OnClick="btnNuevaConfCancelar_Click" CausesValidation="false"/>
             </div>
        </div>
    </asp:Panel>

	<asp:Panel ID="pnlConfirmarNuevaConf" runat="server" DefaultButton="btnConfirmarNuevaConfAceptar" style = "background-color: #FFFFFF; border: 3px solid #e6e6e6; display:none">
        <div style="background-color: #4A7729; height: 30px; line-height: 30px; text-align: center; font-weight: bold;">
            <asp:Label ID="Label1" runat="server" Text="Nueva Configuración" Visible="true" ForeColor="White"></asp:Label>
        </div>
		<div style = "background-color: #FFFFFF; border: 3px;padding:5px">
		    <asp:Label ID="lblConfirmarNuevaConf" runat="server" Width="370px" 
                Text="La configuración ya existe. Al continuar se eliminará y volverá a crear ¿Desesa continuar?" meta:resourcekey="lblConfirmarNuevaConf"
			    Style="text-align:center;padding:15px"></asp:Label>
            <div style="text-align: right; width: 100%;">
                <asp:Button ID="btnConfirmarNuevaConfAceptar" runat="server" Text="Aceptar" Style="height: 32px; width: 90px;" OnClick="btnConfirmarNuevaConfAceptar_Click"/>
                <asp:Button ID="btnConfirmarNuevaConfCancelar" runat="server" Text="Cancelar" Style="height: 32px; width: 90px;" OnClick="btnConfirmarNuevaConfCancelar_Click"/>
             </div>
        </div>
	</asp:Panel>

    <asp:LinkButton ID="lnkFake" runat="server"></asp:LinkButton>
    <ajaxToolkit:ModalPopupExtender ID="modalNuevaConfiguracion" runat="server" DropShadow="false" BackgroundCssClass="modalBackground" 
        PopupControlID="pnlNuevaConfiguracion" TargetControlID="lnkFake">
    </ajaxToolkit:ModalPopupExtender>

	<asp:Button ID="btnFake" runat="server" Text="Button" Style="display:none;" />
	<ajaxToolKit:ModalPopupExtender ID="modalConfirmarNuevaConf" runat="server" BackgroundCssClass="modalBackground" DropShadow="false" 
			TargetControlId="btnFake" PopupControlID="pnlConfirmarNuevaConf">
	</ajaxToolKit:ModalPopupExtender>

</asp:Content>
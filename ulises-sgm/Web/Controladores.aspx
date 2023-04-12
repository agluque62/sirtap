


<%@ Page Language="C#" AutoEventWireup="true" CodeFile="Controladores.aspx.cs" Inherits="Controladores" 
		Title="Sectorizaciones" EnableEventValidation="false" meta:resourcekey="PageResource1"%>

<%@ Register Assembly="AjaxControlToolkit" Namespace="AjaxControlToolkit" TagPrefix="asp" %>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
    <head runat="server">
    <link rel="shortcut icon" href="~/favicon.ico"/>
    <title></title>
    </head>
<%--<asp:Content ID="Content1" ContentPlaceHolderID="ContentPlaceHolder1" Runat="Server">--%>
<body>
    <form id="form1" runat="server" style="background-color: #666699">

    <script type="text/javascript">
        setInterval("OnTimer()", 1000);

        function OnTimer() {
            CallServer("State", "");
            
            <%--// 20211104 #4942--%>
          
            hideNloader();
        }

        function ClientCallbackError(result, context) {
            alert(result);
        }

        function ReceiveServerData(arg) {
            var info = arg;
            if (info.length > 0) {
                var lblEstadoSacta = document.getElementById('<%=LBLEnlaceSacta.ClientID%>');
                if (lblEstadoSacta != null) {
                    if (info == "0") {
                        lblEstadoSacta.style.color = 'red';
                        lblEstadoSacta.innerText = 'SACTA OFF';
                    }
                    else if (info == "1") {
                        lblEstadoSacta.style.color = 'green';
                        lblEstadoSacta.innerText = 'SACTA ON';
                    }
                }
            }
        }
        function DisableButton() {

            // Intercambio  20211104 #4942
            if (document.activeElement.id == "<%=Button5.ClientID %>") {
                document.getElementById("<%=Button5.ClientID %>").disabled = true;
                document.getElementById("<%=Button6.ClientID %>").disabled = true;
                document.getElementById("<%=LbEnProcesoIntercambio.ClientID %>").visible = true;
                document.getElementById("<%=LbEnProcesoIntercambio.ClientID %>").textContent = "En proceso intercambio TOP. Espere...";

                document.getElementById("initloader").style.display = "block";
            }
            // Agrupación 20211104 #4942
            if (document.activeElement.id == "<%=Button7.ClientID %>") {
                document.getElementById("<%=Button7.ClientID %>").disabled = true;
                document.getElementById("<%=Button8.ClientID %>").disabled = true;
                document.getElementById("<%=LbEnProcesoAgrupar.ClientID %>").textContent="En proceso agrupación sectores. Espere...";
                document.getElementById("initloader").style.display = "block";
            }
            // Sectorización 20211104 #4942
            if (document.activeElement.id == "<%=Button9.ClientID %>") {
                document.getElementById("<%=Button9.ClientID %>").disabled = true;
                document.getElementById("<%=Button10.ClientID %>").disabled = true;
                document.getElementById("<%=LbEnProcesoSectorizar.ClientID %>").visible = true;
                document.getElementById("<%=LbEnProcesoSectorizar.ClientID %>").textContent = "En proceso sectorización. Espere..";
                document.getElementById("initloader").style.display = "block";
            }
      }

      window.onbeforeunload = DisableButton;

                    <%--// 20211104 #4942--%>
        function displayNloader() {
            document.getElementById("Nloader").style.display = "block";
        }
                    <%--// 20211104 #4942--%>
        function hideNloader() {
            document.getElementById("Nloader").style.display = "none";
        }
	</script>


 		<ajaxToolKit:ToolkitScriptManager ID="ToolkitScriptManager1" runat="server" 
			CombineScripts="True">
		</ajaxToolKit:ToolkitScriptManager>
    
	 <asp:ScriptManagerProxy ID="ScriptManagerProxy1" runat="server">
	</asp:ScriptManagerProxy>

	<asp:UpdatePanel ID="UpdatePanel3" runat="server" UpdateMode="Conditional" ChildrenAsTriggers="true">
		<ContentTemplate>
			<ajaxToolKit:ModalPopupExtender ID="ModalPopupExtender3" runat="server" 
					TargetControlId="Button13" PopupControlID="Panel10" 
					BackgroundCssClass="modalBackground" DropShadow="true">
			</ajaxToolKit:ModalPopupExtender>
			 <asp:Panel ID="Panel10" runat="server" CssClass="modalBox" Height="174px" Width="396px"
				BackColor="#666699" BorderColor="Black" BorderStyle="Solid">
		 <asp:Label ID="LBNoRenActiva" runat="server" Width="370px"
			 Text="No puede nombrar sectorización con el identificador de la actualmente activa." meta:resourcekey="LBNoRenActivaResource"
			 BorderStyle="Solid" Style="position:absolute; top: 5px; left: 11px; text-align:center;" Enabled="true"
			 BackColor="#9999FF" Height="42px"></asp:Label>
		 <asp:Button ID="Button12" runat="server" Text="Aceptar" onclick="BtnNoRenActiva_Click"
			 Style="position:absolute; top: 106px; left: 152px; height: 32px; width: 90px;"/>
    	</asp:Panel>
			<asp:Button ID="Button13" runat="server" Text="Button" Style="display:none;" />
		</ContentTemplate>
	</asp:UpdatePanel>

	<asp:UpdatePanel ID="UpdatePanel2" runat="server" UpdateMode="Conditional" ChildrenAsTriggers="true">
		<ContentTemplate>
			<ajaxToolKit:ModalPopupExtender ID="ModalPopupExtender2" runat="server" 
					TargetControlId="Button1" PopupControlID="Panel3" 
					BackgroundCssClass="modalBackground" DropShadow="true">
			</ajaxToolKit:ModalPopupExtender>
			 <asp:Panel ID="Panel3" runat="server" CssClass="modalBox" Height="174px" Width="396px"
				BackColor="#666699" BorderColor="Black" BorderStyle="Solid">
		 <asp:Label ID="LBNoAgrupar" runat="server" Width="370px"
			 Text="Los sectores de mantenimiento no se pueden agrupar con otros sectores." meta:resourcekey="LBNoAgruparResource"
			 BorderStyle="Solid" Style="position:absolute; top: 5px; left: 11px; text-align:center;" Enabled="true"
			 BackColor="#9999FF" Height="42px"></asp:Label>
		 <asp:Button ID="BtnAceptar" runat="server" Text="Aceptar" onclick="BtnCancelarMantto_Click"
			 Style="position:absolute; top: 106px; left: 152px; height: 32px; width: 90px;"/>
    	</asp:Panel>
			<asp:Button ID="Button1" runat="server" Text="Button" Style="display:none;" />
		</ContentTemplate>
	</asp:UpdatePanel>

	<asp:UpdatePanel ID="UpdatePanel1" runat="server" UpdateMode="Conditional" ChildrenAsTriggers="true">
		<ContentTemplate>
			<ajaxToolKit:ModalPopupExtender ID="ModalPopupExtender1" runat="server" 
					TargetControlId="Button3" PopupControlID="Panel4" 
					BackgroundCssClass="modalBackground" DropShadow="true">
			</ajaxToolKit:ModalPopupExtender>
			 <asp:Panel ID="Panel4" runat="server" CssClass="modalBox" Height="174px" Width="396px"
				BackColor="#666699" BorderColor="Black" BorderStyle="Solid">
		 <asp:Label ID="LBNoBorrarActiva" runat="server" Width="370px"
			 Text="La Sectorización Activa no puede ser borrada" meta:resourcekey="LBNoBorrarActivaResource"
			 BorderStyle="Solid" Style="position:absolute; top: 5px; left: 11px; text-align:center;" Enabled="true"
			 BackColor="#9999FF" Height="42px"></asp:Label>
		 <asp:Button ID="BtnAceptarNoActiva" runat="server" Text="Aceptar" onclick="BtnCancelarBorradoActiva_Click"
			 Style="position:absolute; top: 106px; left: 152px; height: 32px; width: 90px;"/>
    	</asp:Panel>
			<asp:Button ID="Button3" runat="server" Text="Button" Style="display:none;" />
		</ContentTemplate>
	</asp:UpdatePanel>


	<asp:Panel ID="Panel1" runat="server" CssClass="modalBox" Height="174px" Width="416px"
				BackColor="#666699" BorderColor="Black" BorderStyle="Solid" Visible="false" Style="position:absolute; top:300px; left: 400px">
		 <asp:Label ID="LBGuardarSectorizacion" runat="server" Width="370px" 
			 Text="Sectorización sin grabar. Se debe grabar con un identificador" meta:resourcekey="LBGuardarSectorizacionResource"
			 BorderStyle="Solid" Style="position:absolute; top: 5px; left: 11px; width:auto" Enabled="false"
			 BackColor="#9999FF" Height="21px"></asp:Label>
		 <asp:Label ID="LBIdentificadorSectorizacion" runat="server" Text="Identificador sectorización:" 
			 Style="position:absolute; top: 53px; left: 14px;"></asp:Label>
		 <asp:TextBox ID="TBNomSectorizacion" runat="server" 
			 Style="position:absolute; top: 52px; left: 195px; height: 22px; width: 201px;"></asp:TextBox>
		 <asp:Button ID="BtnAceptarGuardar" runat="server" Text="Grabar" 
			 Style="position:absolute; top: 106px; left: 92px; height: 32px; width: 90px;" 
			 onclick="BtnAceptarGuardar_Click"/>
		 <asp:Button ID="BtnGuardarSectorizacion" runat="server" Text="Grabar" Visible="false"
			 Style="position:absolute; top: 106px; left: 92px; height: 32px; width: 90px;"
			 onclick="BtnGuardarSectorizacion_Click"/>
		 <asp:LinkButton ID="LnkBCancelar" runat="server" ForeColor="White"
			 Style="position:absolute; top: 116px; left: 325px;" OnClick="LnkBCancelar_OnClick">Cancelar</asp:LinkButton>
	</asp:Panel>

	<asp:Panel ID="Panel5" runat="server" CssClass="modalBox" Height="174px" Width="460px"
				BackColor="#666699" BorderColor="Black" BorderStyle="Solid" Visible="false" Style="position:absolute; top:300px; left: 400px">
		 <asp:Label ID="LBEliminarSectorizacion" runat="server" Width="450px" 
			 Text="Sectorización almacenada. ¿Esta seguro de eliminar Sectorización?"  meta:resourcekey="LBEliminarSectorizacionResource"
			 BorderStyle="Solid" Style="position:absolute; top: 10px; left: 11px; width:auto" Enabled="false"
			 BackColor="#9999FF" Height="21px"></asp:Label>
		 <asp:Label ID="Label9" runat="server" Text="Identificador de Sectorización:" 
			 Style="position:absolute; top: 63px; left: 44px;"></asp:Label>
		 <asp:Label ID="TextBoxSector" runat="server" 
			 Style="position:absolute; top: 63px; left: 244px; height: 22px; width: 201px;"></asp:Label>
		 <asp:Button ID="Button2" runat="server" Text="Eliminar" 
			 Style="position:absolute; top: 106px; left: 92px; height: 32px; width: 90px;" 
			 onclick="BtnAceptarBorrar_Click"/>
        <asp:Button ID="Button4" runat="server" Text="Cancelar"
			 Style="position:absolute; top: 106px; left: 325px; height: 32px; width: 90px;" 
			 onclick="BtnCancelarBorrar_OnClick"/>
	</asp:Panel>

	<asp:Panel ID="Panel6" runat="server" CssClass="modalBox" Height="174px" Width="460px"
				BackColor="#666699" BorderColor="Black" BorderStyle="Solid" Visible="false" Style="position:absolute; top:300px; left: 400px">
		 <asp:Label ID="LBIntercambio" runat="server" Width="450px" 
			 Text="Intercambio de TOP. ¿Esta usted seguro?"  meta:resourcekey="LBIntercambioResource"
			 BorderStyle="Solid" Style="position:absolute; top: 10px; left: 115px; width:auto" Enabled="false"
			 BackColor="#9999FF" Height="21px"></asp:Label>

		 <asp:Label ID="LBTopOrigen" runat="server" Text="TOP Origen:" 
			 Style="position:absolute; top: 55px; left: 145px;"></asp:Label>
		 <asp:Label ID="TextBoxTopOrigen" runat="server" 
			 Style="position:absolute; top: 55px; left: 286px; height: 22px; width: 201px;"></asp:Label>

		 <asp:Label ID="LBTopDestino" runat="server" Text="TOP Destino:" 
			 Style="position:absolute; top: 79px; left: 145px;"></asp:Label>
		 <asp:Label ID="TextBoxTopDestino" runat="server" 
			 Style="position:absolute; top: 79px; left: 286px; height: 22px; width: 201px;"></asp:Label>

		 <asp:Button ID="Button5" runat="server" Text="Aceptar" 
			 Style="position:absolute; top: 106px; left: 70px; height: 32px; width: 90px;" 
			 onclick="BtnAceptarIntercambiar_Click"/>
        <asp:Button ID="Button6" runat="server" Text="Cancelar"
			 Style="position:absolute; top: 106px; left: 303px; height: 32px; width: 90px;" 
			 onclick="BtnCancelarIntercambiar_OnClick"/>
		 <asp:Label ID="LbEnProcesoIntercambio" runat="server" Text="" Enabled="true" Visible="true" Font-Bold="True" ForeColor="White"
			 Style="position:absolute; top: 146px; left: 124px;"></asp:Label>
	</asp:Panel>
 

	<asp:Panel ID="Panel7" runat="server" CssClass="modalBox" Height="174px" Width="460px"
				BackColor="#666699" BorderColor="Black" BorderStyle="Solid" Visible="false" Style="position:absolute; top:300px; left: 400px">
		 <asp:Label ID="LBAgrupacion" runat="server" Width="450px" 
			 Text="Agrupación de Sectores en TOP. ¿Esta usted seguro?" meta:resourcekey="LBAgrupacionResource"
			 BorderStyle="Solid" Style="position:absolute; top: 10px; left: 81px; width:auto" Enabled="false"
			 BackColor="#9999FF" Height="21px"></asp:Label>

		 <asp:Label ID="LBTopSeleccionado" runat="server" Text="TOP Seleccionado:" 
			 Style="position:absolute; top: 63px; left: 130px;"></asp:Label>
		 <asp:Label ID="TextBoxTopDestinoAgrupacion" runat="server" 
			 Style="position:absolute; top: 63px; left: 274px; height: 22px; width: 201px;"></asp:Label>

		 <asp:Button ID="Button7" runat="server" Text="Aceptar" 
			 Style="position:absolute; top: 106px; left: 67px; height: 32px; width: 90px;" 
			 onclick="BtnAceptarAgrupar_Click"/>
        <asp:Button ID="Button8" runat="server" Text="Cancelar"
			 Style="position:absolute; top: 106px; left: 299px; height: 32px; width: 90px;" 
			 onclick="BtnCancelarAgrupar_OnClick"/>
		 <asp:Label ID="LbEnProcesoAgrupar" runat="server" Text="" Enabled="true" Visible="true" Font-Bold="True" ForeColor="White"
			 Style="position:absolute; top: 146px; left: 122px;"></asp:Label>

	</asp:Panel>


	<asp:Panel ID="Panel8" runat="server" CssClass="modalBox" Height="174px" Width="460px"
				BackColor="#666699" BorderColor="Black" BorderStyle="Solid" Visible="false" Style="position:absolute; top:300px; left: 400px">
		 <asp:Label ID="LBPreguntaSectorizacion" runat="server" Width="450px" 
			 Text="Activación de Sectorización. ¿Esta usted seguro?" meta:resourcekey="LBPreguntaSectorizacionResource"
			 BorderStyle="Solid" Style="position:absolute; top: 10px; left: 80px; width:auto" Enabled="false"
			 BackColor="#9999FF" Height="21px"></asp:Label>

		 <asp:Label ID="LBSectorizacion" runat="server" Text="Sectorización:" 
			 Style="position:absolute; top: 63px; left: 104px;"></asp:Label>
		 <asp:Label ID="TBSectorizacion" runat="server" 
			 Style="position:absolute; top: 63px; left: 246px; height: 22px; width: 201px;"></asp:Label>

		 <asp:Button ID="Button9" runat="server" Text="Aceptar" 
			 Style="position:absolute; top: 106px; left: 71px; height: 32px; width: 90px;" 
			 onclick="BtnAceptarSectorizar_Click"/>
        <asp:Button ID="Button10" runat="server" Text="Cancelar"
			 Style="position:absolute; top: 106px; left: 304px; height: 32px; width: 90px;" 
			 onclick="BtnCancelarSectorizar_OnClick"/>
             <asp:Label ID="LbEnProcesoSectorizar" runat="server" Text="" Enabled="true" Visible="true" Font-Bold="True" ForeColor="White"
			 Style="position:absolute; top: 146px; left: 122px; "></asp:Label>
	</asp:Panel>

	<asp:Panel ID="Panel9" runat="server" CssClass="modalBox" Height="174px" Width="720px"
				BackColor="#666699" BorderColor="Black" BorderStyle="Solid" Visible="false" Style="position:absolute; top:300px; left: 400px">
		 <asp:Label ID="LbNoSectorizacion" runat="server" Width="700px" 
			 Text="Activación de Sectorización no realizada" 
			 BorderStyle="Solid" Style="position:absolute; top: 10px; left: 62px; width:auto" Enabled="false"
			 BackColor="#9999FF" Height="21px"></asp:Label>

		 <asp:Label ID="LBSectorizacionFallida" runat="server" Text="Sectorización:" 
			 Style="position:absolute; top: 63px; left: 280px;"></asp:Label>
		 <asp:Label ID="TBSectorizacionFallida" runat="server" 
			 Style="position:absolute; top: 63px; left: 400px; height: 22px; width: 201px;"></asp:Label>

		 <asp:Button ID="Button11" runat="server" Text="Aceptar" 
			 Style="position:absolute; top: 106px; left: 300px; height: 32px; width: 90px;" 
			 onclick="BtnAceptarSectorizarFallida_Click"/>
	</asp:Panel>

	<asp:Panel ID="Panel11" runat="server" CssClass="modalBox" Height="174px" Width="460px"
				BackColor="#666699" BorderColor="Black" BorderStyle="Solid" Visible="false" Style="position:absolute; top:300px; left: 400px">
		 <asp:Label ID="LBPreguntaSectorizacionSacta" runat="server" Width="450px" 
			 Text="Solicitud de Sectorización Sacta. ¿Esta usted seguro?" meta:resourcekey="LBPreguntaSectorizacionSactaResource"
			 BorderStyle="Solid" Style="position:absolute; top: 10px; left: 62px; width:auto" Enabled="false"
			 BackColor="#9999FF" Height="21px"></asp:Label>

		 <asp:Label ID="LBSectorizacionSacta" runat="server" Text="Sectorización:" 
			 Style="position:absolute; top: 63px; left: 130px;"></asp:Label>
		 <asp:Label ID="TBSectorizacionSacta" runat="server" 
			 Style="position:absolute; top: 63px; left: 274px; height: 22px; width: 201px;"></asp:Label>

		 <asp:Button ID="Button14" runat="server" Text="Aceptar" 
			 Style="position:absolute; top: 106px; left: 82px; height: 32px; width: 90px;" 
			 onclick="BtnAceptarSectorizarSacta_Click"/>
        <asp:Button ID="Button15" runat="server" Text="Cancelar"
			 Style="position:absolute; top: 106px; left: 315px; height: 32px; width: 90px;" 
			 onclick="BtnCancelarSectorizarSacta_OnClick"/>
	</asp:Panel>

	<asp:Panel ID="Panel12" runat="server" CssClass="modalBox" Height="174px" Width="720px"
				BackColor="#666699" BorderColor="Black" BorderStyle="Solid" Visible="false" Style="position:absolute; top:300px; left: 400px">
		 <asp:Label ID="LbNoSectorizacionSACTA" runat="server" Width="300px" 
			 Text="Activación de Sectorización SACTA no realizada" meta:resourcekey="LbNoSectorizacionSACTAResource"
			 BorderStyle="Solid" Style="position:absolute; top: 10px; left: 220px; width:auto" Enabled="false"
			 BackColor="#9999FF" Height="21px"></asp:Label>

		 <asp:Label ID="LBSectorizacionFallidaSACTA" runat="server" Text="Sectorización:" 
			 Style="position:absolute; top: 63px; left: 280px;"></asp:Label>
		 <asp:Label ID="TBSectorizacionFallidaSACTA" runat="server" 
			 Style="position:absolute; top: 63px; left: 400px; height: 22px; width: 201px;"></asp:Label>

		 <asp:Button ID="Button16" runat="server" Text="Aceptar" 
			 Style="position:absolute; top: 106px; left: 300px; height: 32px; width: 90px;" 
			 onclick="BtnAceptarSectorizarFallidaSACTA_Click"/>
	</asp:Panel>

	<asp:Panel ID="Panel13" runat="server" CssClass="modalBox" Height="174px" Width="720px"
				BackColor="#666699" BorderColor="Black" BorderStyle="Solid" Visible="false" Style="position:absolute; top:300px; left: 400px">
		 <asp:Label ID="LblSectorizacionRechazada" runat="server" Width="300px" 
			 Text="El estado del sistema SACTA no permite la solicitud de Sectorización" meta:resourcekey="LblSectorizacionRechazadaResource"
			 BorderStyle="Solid" Style="position:absolute; top: 43px; left: 130px; width:auto" Enabled="false"
			 BackColor="#9999FF" Height="21px"></asp:Label>

		 <asp:Button ID="Button17" runat="server" Text="Aceptar" 
			 Style="position:absolute; top: 83px; left: 300px; height: 32px; width: 90px;" 
			 onclick="BtnSectorizacionRechazada_Click"/>
	</asp:Panel>


		<div style="border: thin solid #C0C0C0; background-color: #CCCCFF; height: 80px; ">
		<asp:Table runat="server" Width="1180px">
			<asp:TableRow ID="TableRow21" runat="server">
				<asp:TableCell ID="TableCell97" runat="server" RowSpan="2" Width="265px">
					 <asp:Image ID="Image1" runat="server" Width="203px" Height="74px"
						 ImageUrl="~/Images/GRUPO_AMPER_PQ.png" />
				</asp:TableCell>
				<asp:TableCell ID="TableCell98" runat="server" VerticalAlign="Top" HorizontalAlign="Left" Width="160px">
    				 <asp:Label ID="LBSEC" runat="server" BorderStyle="Inset" Font-Bold="True" 
						 style="height: 19px" 
						 Text="SECTORIZACIÓN:">
					</asp:Label>
				</asp:TableCell>
				<asp:TableCell ID="TableCell111" runat="server" VerticalAlign="Top" Width="160px">
					<asp:Label ID="LblIdSectorizacion" runat="server" 
						 Font-Bold="True" ForeColor="White"></asp:Label>
				</asp:TableCell>
				<asp:TableCell ID="TableCell99" runat="server" VerticalAlign="Top" Width="160px">
    				 <asp:Label ID="LBSAC" runat="server" BorderStyle="Inset" Font-Bold="True" 
						 style="height: 19px;" 
						 Text="ENLACE SACTA:"></asp:Label>
				</asp:TableCell>
				<asp:TableCell ID="TableCell199" runat="server" VerticalAlign="Top" Width="160px" >
					<asp:Label ID="LBLEnlaceSacta" runat="server"></asp:Label>
				</asp:TableCell>
				<asp:TableCell ID="TableCell100" runat="server" HorizontalAlign="Right">
					<asp:LinkButton ID="LinkButton1" runat="server"  
						 Height="21px" Width="134px" onclick="BtnCerrar_OnClick" >Cerrar Sesión</asp:LinkButton>
				</asp:TableCell>
			</asp:TableRow>
			<asp:TableRow ID="TableRow22" runat="server">
				<asp:TableCell ID="TableCell102" runat="server" VerticalAlign="Top" HorizontalAlign="Left">
    				 <asp:Label ID="LBEST" runat="server" BorderStyle="Inset" Font-Bold="True" 
						 style="height: 19px;" 
						 Text="ESTADO:">
					</asp:Label>
				</asp:TableCell>
				<asp:TableCell runat="server" VerticalAlign="Top">
					<asp:Label ID="LblEstado" runat="server" Font-Bold="True" ForeColor="White">
					</asp:Label>
				</asp:TableCell>
				<asp:TableCell ID="TableCell103" runat="server">
				</asp:TableCell>
				<asp:TableCell ID="TableCell104" runat="server">
				</asp:TableCell>
			</asp:TableRow>
		</asp:Table>
		</div>
		<div runat="server" style="border: thin #000000; background-color: #666699" >
			<asp:Table ID="TableCabecera" runat="server" BorderColor="Black" BorderStyle="Solid" 
				 Height="20px" style="margin-top: 3px" Width="1186px" ForeColor="#FFFFCC">
				<asp:TableHeaderRow runat="server">
					<asp:TableHeaderCell ID="TableHeaderCell1" runat="server" Font-Size="Large" ForeColor="#66FF66" Width="25px"></asp:TableHeaderCell>
					<asp:TableHeaderCell ID="THCPOS" runat="server" Font-Size="Large" ForeColor="#66FF66" Width="105px" HorizontalAlign="Left">Posiciones</asp:TableHeaderCell>
					<asp:TableHeaderCell ID="THCNUC" runat="server" Font-Size="Large" ForeColor="#66FF66" Width="105px" HorizontalAlign="Left">Núcleo</asp:TableHeaderCell>
					<asp:TableHeaderCell ID="THCAGR" runat="server" Font-Size="Large" ForeColor="#66FF66" Width="105px" HorizontalAlign="Left">Agrupación</asp:TableHeaderCell>
					<asp:TableHeaderCell ID="THCSEC" runat="server" Font-Size="Large" ForeColor="#66FF66" HorizontalAlign="Left">Sectores asignados</asp:TableHeaderCell>
				</asp:TableHeaderRow>
			</asp:Table>
		</div>
		<div style="border: thin #000000; background-color: #666699; height: 475px; overflow:auto; ">
    	<asp:Table ID="Table1" runat="server" BorderColor="Silver" BorderStyle="Double" 
				style="margin-top: 3px" Width="1225px" GridLines="Both">
			<asp:TableRow runat="server" Height="30px">
				<asp:TableCell ID="TableCell1" runat="server" Font-Size="Medium" ForeColor="Black" Width="25px">
					<asp:RadioButton ID="RadioButton1" runat="server" GroupName="Selected" OnCheckedChanged="RBCheckedChanged" AutoPostBack="true"/>
				</asp:TableCell>
				<asp:TableCell ID="TableCell101" runat="server" Font-Size="Medium" ForeColor="Black" Width="115px">
					<asp:CheckBox runat="server" ID="CBTop1" OnCheckedChanged="CBTop_CheckedChanged" AutoPostBack="true"/>
				</asp:TableCell>
				<asp:TableCell ID="TableCell26" runat="server" Font-Size="Medium" ForeColor="White" Width="115px"></asp:TableCell>
				<asp:TableCell ID="TableCell27" runat="server" Font-Size="Medium" ForeColor="White" Width="115px"></asp:TableCell>
				<asp:TableCell ID="TableCell28" runat="server" Font-Size="Medium" ForeColor="White">
					<div runat="server" style="overflow:auto">
						<asp:CheckBoxList ID="CBLSectores1" runat="server" Width="750px" RepeatDirection="Horizontal"></asp:CheckBoxList>
					</div>
				</asp:TableCell>
				<asp:TableCell ID="TableCell129" runat="server" Font-Size="Medium" ForeColor="White" Width="60px">
					<asp:CheckBox ID="CBTodos1" runat="server" Text="Todos" AutoPostBack="true" OnCheckedChanged="CBTodos_CheckedChanged"/>
				</asp:TableCell>
			</asp:TableRow>


			<asp:TableRow runat="server" Height="30px">
				<asp:TableCell ID="TableCell105" runat="server" Font-Size="Medium" ForeColor="Black" Width="25px">
					<asp:RadioButton ID="RadioButton2" runat="server" GroupName="Selected" OnCheckedChanged="RBCheckedChanged" AutoPostBack="true"/></asp:TableCell>
				<asp:TableCell ID="TableCell2" runat="server" Font-Size="Medium" ForeColor="Black">
					<asp:CheckBox runat="server" ID="CBTop2" OnCheckedChanged="CBTop_CheckedChanged" AutoPostBack="true" />
				</asp:TableCell>
				<asp:TableCell ID="TableCell29" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell30" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell31" runat="server" Font-Size="Medium" ForeColor="White">
					<div id="Div1" runat="server" style="overflow:auto">
						<asp:CheckBoxList ID="CBLSectores2" runat="server"  Width="800px" RepeatDirection="Horizontal"></asp:CheckBoxList>
					</div>
				</asp:TableCell>
				<asp:TableCell ID="TableCell130" runat="server" Font-Size="Medium" ForeColor="White" Width="60px">
					<asp:CheckBox ID="CBTodos2" runat="server" Text="Todos" AutoPostBack="true" OnCheckedChanged="CBTodos_CheckedChanged" />
				</asp:TableCell>
				</asp:TableRow>
				<asp:TableRow runat="server" Height="30px">
				<asp:TableCell ID="TableCell106" runat="server" Font-Size="Medium" ForeColor="Black" Width="25px">
					<asp:RadioButton ID="RadioButton3" runat="server" GroupName="Selected" OnCheckedChanged="RBCheckedChanged" AutoPostBack="true"/></asp:TableCell>
				<asp:TableCell ID="TableCell3" runat="server" Font-Size="Medium" ForeColor="Black">
					<asp:CheckBox runat="server" ID="CBTop3" OnCheckedChanged="CBTop_CheckedChanged" AutoPostBack="true" />
				</asp:TableCell>
				<asp:TableCell ID="TableCell32" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell33" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell34" runat="server" Font-Size="Medium" ForeColor="White">
					<div id="Div2" runat="server" style="overflow:auto">
						<asp:CheckBoxList ID="CBLSectores3" runat="server"  Width="800px" RepeatDirection="Horizontal"></asp:CheckBoxList>
					

					

					

					</div>
				

</asp:TableCell>
				<asp:TableCell ID="TableCell131" runat="server" Font-Size="Medium" ForeColor="White" Width="60px">
					<asp:CheckBox ID="CBTodos3" runat="server" Text="Todos" AutoPostBack="true" OnCheckedChanged="CBTodos_CheckedChanged" />
				</asp:TableCell>
			</asp:TableRow>
			<asp:TableRow runat="server" Height="30px">
				<asp:TableCell ID="TableCell107" runat="server" Font-Size="Medium" ForeColor="Black" Width="25px">
					<asp:RadioButton ID="RadioButton4" runat="server" GroupName="Selected" OnCheckedChanged="RBCheckedChanged" AutoPostBack="true"/></asp:TableCell>
				<asp:TableCell ID="TableCell4" runat="server" Font-Size="Medium" ForeColor="Black">
					<asp:CheckBox runat="server" ID="CBTop4" OnCheckedChanged="CBTop_CheckedChanged" AutoPostBack="true" />
				</asp:TableCell>
				<asp:TableCell ID="TableCell35" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell36" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell37" runat="server" Font-Size="Medium" ForeColor="White">
					<div id="Div3" runat="server" style="overflow:auto">
						<asp:CheckBoxList ID="CBLSectores4" runat="server"  Width="800px" RepeatDirection="Horizontal"></asp:CheckBoxList>
					

					

					

					</div>
				

</asp:TableCell>
				<asp:TableCell ID="TableCell132" runat="server" Font-Size="Medium" ForeColor="White" Width="60px">
					<asp:CheckBox ID="CBTodos4" runat="server" Text="Todos" AutoPostBack="true" OnCheckedChanged="CBTodos_CheckedChanged" />
				</asp:TableCell>
			</asp:TableRow>
			<asp:TableRow ID="TableRow1" runat="server" Height="30px">
				<asp:TableCell ID="TableCell108" runat="server" Font-Size="Medium" ForeColor="Black" Width="25px">
					<asp:RadioButton ID="RadioButton5" runat="server" GroupName="Selected" OnCheckedChanged="RBCheckedChanged" AutoPostBack="true"/></asp:TableCell>
				<asp:TableCell ID="TableCell5" runat="server" Font-Size="Medium" ForeColor="Black">
					<asp:CheckBox runat="server" ID="CBTop5" OnCheckedChanged="CBTop_CheckedChanged" AutoPostBack="true" />
				</asp:TableCell>
				<asp:TableCell ID="TableCell6" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell7" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell8" runat="server" Font-Size="Medium" ForeColor="White">
					<div id="Div4" runat="server" style="overflow:auto">
						<asp:CheckBoxList ID="CBLSectores5" runat="server"  Width="800px" RepeatDirection="Horizontal"></asp:CheckBoxList>
					

					

					

					</div>
				

</asp:TableCell>
				<asp:TableCell ID="TableCell133" runat="server" Font-Size="Medium" ForeColor="White" Width="60px">
					<asp:CheckBox ID="CBTodos5" runat="server" Text="Todos" AutoPostBack="true" OnCheckedChanged="CBTodos_CheckedChanged" />
				</asp:TableCell>
			</asp:TableRow>
			<asp:TableRow ID="TableRow2" runat="server" Height="30px">
				<asp:TableCell ID="TableCell109" runat="server" Font-Size="Medium" ForeColor="Black" Width="25px">
					<asp:RadioButton ID="RadioButton6" runat="server" GroupName="Selected" OnCheckedChanged="RBCheckedChanged" AutoPostBack="true"/></asp:TableCell>
				<asp:TableCell ID="TableCell9" runat="server" Font-Size="Medium" ForeColor="Black">
					<asp:CheckBox runat="server" ID="CBTop6" OnCheckedChanged="CBTop_CheckedChanged" AutoPostBack="true" />
				</asp:TableCell>
				<asp:TableCell ID="TableCell10" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell11" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell12" runat="server" Font-Size="Medium" ForeColor="White">
					<div id="Div5" runat="server" style="overflow:auto">
						<asp:CheckBoxList ID="CBLSectores6" runat="server"  Width="800px" RepeatDirection="Horizontal"></asp:CheckBoxList>
					

					

					

					</div>
				

</asp:TableCell>
				<asp:TableCell ID="TableCell134" runat="server" Font-Size="Medium" ForeColor="White" Width="60px">
					<asp:CheckBox ID="CBTodos6" runat="server" Text="Todos" AutoPostBack="true" OnCheckedChanged="CBTodos_CheckedChanged" />
				</asp:TableCell>
			</asp:TableRow>
			<asp:TableRow ID="TableRow3" runat="server" Height="30px">
				<asp:TableCell ID="TableCell110" runat="server" Font-Size="Medium" ForeColor="Black" Width="25px">
					<asp:RadioButton ID="RadioButton7" runat="server" GroupName="Selected" OnCheckedChanged="RBCheckedChanged" AutoPostBack="true"/></asp:TableCell>
				<asp:TableCell ID="TableCell13" runat="server" Font-Size="Medium" ForeColor="Black">
					<asp:CheckBox runat="server" ID="CBTop7" OnCheckedChanged="CBTop_CheckedChanged" AutoPostBack="true" />
				</asp:TableCell>
				<asp:TableCell ID="TableCell14" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell15" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell16" runat="server" Font-Size="Medium" ForeColor="White">
					<div id="Div6" runat="server" style="overflow:auto">
						<asp:CheckBoxList ID="CBLSectores7" runat="server"  Width="800px" RepeatDirection="Horizontal"></asp:CheckBoxList>
					

					

					

					</div>
				

</asp:TableCell>
				<asp:TableCell ID="TableCell135" runat="server" Font-Size="Medium" ForeColor="White" Width="60px">
					<asp:CheckBox ID="CBTodos7" runat="server" Text="Todos" AutoPostBack="true" OnCheckedChanged="CBTodos_CheckedChanged" />
				</asp:TableCell>
			</asp:TableRow>
			<asp:TableRow ID="TableRow4" runat="server" Height="30px">
				<asp:TableCell ID="TableCell112" runat="server" Font-Size="Medium" ForeColor="Black" Width="25px">
					<asp:RadioButton ID="RadioButton8" runat="server" GroupName="Selected" OnCheckedChanged="RBCheckedChanged" AutoPostBack="true"/></asp:TableCell>
				<asp:TableCell ID="TableCell17" runat="server" Font-Size="Medium" ForeColor="Black">
					<asp:CheckBox runat="server" ID="CBTop8" OnCheckedChanged="CBTop_CheckedChanged" AutoPostBack="true" />
				</asp:TableCell>
				<asp:TableCell ID="TableCell18" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell19" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell20" runat="server" Font-Size="Medium" ForeColor="White">
					<div id="Div7" runat="server" style="overflow:auto">
						<asp:CheckBoxList ID="CBLSectores8" runat="server"  Width="800px" RepeatDirection="Horizontal"></asp:CheckBoxList>
					

					

					

					</div>
				

</asp:TableCell>
				<asp:TableCell ID="TableCell136" runat="server" Font-Size="Medium" ForeColor="White" Width="60px">
					<asp:CheckBox ID="CBTodos8" runat="server" Text="Todos" AutoPostBack="true" OnCheckedChanged="CBTodos_CheckedChanged" />
				</asp:TableCell>
			</asp:TableRow>
			<asp:TableRow ID="TableRow5" runat="server" Height="30px">
				<asp:TableCell ID="TableCell113" runat="server" Font-Size="Medium" ForeColor="Black" Width="25px">
					<asp:RadioButton ID="RadioButton9" runat="server" GroupName="Selected" OnCheckedChanged="RBCheckedChanged" AutoPostBack="true"/></asp:TableCell>
				<asp:TableCell ID="TableCell21" runat="server" Font-Size="Medium" ForeColor="Black">
					<asp:CheckBox runat="server" ID="CBTop9" OnCheckedChanged="CBTop_CheckedChanged" AutoPostBack="true" />
				</asp:TableCell>
				<asp:TableCell ID="TableCell22" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell23" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell24" runat="server" Font-Size="Medium" ForeColor="White">
					<div id="Div8" runat="server" style="overflow:auto">
						<asp:CheckBoxList ID="CBLSectores9" runat="server"  Width="800px" RepeatDirection="Horizontal"></asp:CheckBoxList>
					

					

					

					</div>
				

</asp:TableCell>
				<asp:TableCell ID="TableCell137" runat="server" Font-Size="Medium" ForeColor="White" Width="60px">
					<asp:CheckBox ID="CBTodos9" runat="server" Text="Todos" AutoPostBack="true" OnCheckedChanged="CBTodos_CheckedChanged" />
				</asp:TableCell>
			</asp:TableRow>
			<asp:TableRow ID="TableRow7" runat="server" Height="30px">
				<asp:TableCell ID="TableCell114" runat="server" Font-Size="Medium" ForeColor="Black" Width="25px">
					<asp:RadioButton ID="RadioButton10" runat="server" GroupName="Selected" OnCheckedChanged="RBCheckedChanged" AutoPostBack="true"/></asp:TableCell>
				<asp:TableCell ID="TableCell41" runat="server" Font-Size="Medium" ForeColor="Black">
					<asp:CheckBox runat="server" ID="CBTop10" OnCheckedChanged="CBTop_CheckedChanged" AutoPostBack="true" />
				</asp:TableCell>
				<asp:TableCell ID="TableCell42" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell43" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell44" runat="server" Font-Size="Medium" ForeColor="White">
					<div id="Div10" runat="server" style="overflow:auto">
						<asp:CheckBoxList ID="CBLSectores10" runat="server"  Width="800px" RepeatDirection="Horizontal"></asp:CheckBoxList>
					

					

					

					</div>
				

</asp:TableCell>
				<asp:TableCell ID="TableCell138" runat="server" Font-Size="Medium" ForeColor="White" Width="60px">
					<asp:CheckBox ID="CBTodos10" runat="server" Text="Todos" AutoPostBack="true" OnCheckedChanged="CBTodos_CheckedChanged" />
				</asp:TableCell>
			</asp:TableRow>
			<asp:TableRow ID="TableRow8" runat="server" Height="30px">
				<asp:TableCell ID="TableCell115" runat="server" Font-Size="Medium" ForeColor="Black" Width="25px">
					<asp:RadioButton ID="RadioButton11" runat="server" GroupName="Selected" OnCheckedChanged="RBCheckedChanged" AutoPostBack="true"/></asp:TableCell>
				<asp:TableCell ID="TableCell45" runat="server" Font-Size="Medium" ForeColor="Black">
					<asp:CheckBox runat="server" ID="CBTop11" OnCheckedChanged="CBTop_CheckedChanged" AutoPostBack="true" />
				</asp:TableCell>
				<asp:TableCell ID="TableCell46" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell47" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell48" runat="server" Font-Size="Medium" ForeColor="White">
					<div id="Div11" runat="server" style="overflow:auto">
						<asp:CheckBoxList ID="CBLSectores11" runat="server"  Width="800px" RepeatDirection="Horizontal"></asp:CheckBoxList>
					

					

					

					</div>
				

</asp:TableCell>
				<asp:TableCell ID="TableCell139" runat="server" Font-Size="Medium" ForeColor="White" Width="60px">
					<asp:CheckBox ID="CBTodos11" runat="server" Text="Todos" AutoPostBack="true" OnCheckedChanged="CBTodos_CheckedChanged" />
				</asp:TableCell>
			</asp:TableRow>
			<asp:TableRow ID="TableRow9" runat="server" Height="30px">
				<asp:TableCell ID="TableCell116" runat="server" Font-Size="Medium" ForeColor="Black" Width="25px">
					<asp:RadioButton ID="RadioButton12" runat="server" GroupName="Selected" OnCheckedChanged="RBCheckedChanged" AutoPostBack="true"/></asp:TableCell>
				<asp:TableCell ID="TableCell49" runat="server" Font-Size="Medium" ForeColor="Black">
					<asp:CheckBox runat="server" ID="CBTop12" OnCheckedChanged="CBTop_CheckedChanged" AutoPostBack="true" />
				</asp:TableCell>
				<asp:TableCell ID="TableCell50" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell51" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell52" runat="server" Font-Size="Medium" ForeColor="White">
					<div id="Div12" runat="server" style="overflow:auto">
						<asp:CheckBoxList ID="CBLSectores12" runat="server"  Width="800px" RepeatDirection="Horizontal"></asp:CheckBoxList>
					

					

					

					</div>
				

</asp:TableCell>
				<asp:TableCell ID="TableCell140" runat="server" Font-Size="Medium" ForeColor="White" Width="60px">
					<asp:CheckBox ID="CBTodos12" runat="server" Text="Todos" AutoPostBack="true" OnCheckedChanged="CBTodos_CheckedChanged" />
				</asp:TableCell>
			</asp:TableRow>
			<asp:TableRow ID="TableRow10" runat="server" Height="30px">
				<asp:TableCell ID="TableCell117" runat="server" Font-Size="Medium" ForeColor="Black" Width="25px">
					<asp:RadioButton ID="RadioButton13" runat="server" GroupName="Selected" OnCheckedChanged="RBCheckedChanged" AutoPostBack="true"/></asp:TableCell>
				<asp:TableCell ID="TableCell53" runat="server" Font-Size="Medium" ForeColor="Black">
					<asp:CheckBox runat="server" ID="CBTop13" OnCheckedChanged="CBTop_CheckedChanged" AutoPostBack="true" />
				</asp:TableCell>
				<asp:TableCell ID="TableCell54" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell55" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell56" runat="server" Font-Size="Medium" ForeColor="White">
					<div id="Div13" runat="server" style="overflow:auto">
						<asp:CheckBoxList ID="CBLSectores13" runat="server"  Width="800px" RepeatDirection="Horizontal"></asp:CheckBoxList>
					

					

					

					</div>
				

</asp:TableCell>
				<asp:TableCell ID="TableCell141" runat="server" Font-Size="Medium" ForeColor="White" Width="60px">
					<asp:CheckBox ID="CBTodos13" runat="server" Text="Todos" AutoPostBack="true" OnCheckedChanged="CBTodos_CheckedChanged" />
				</asp:TableCell>
			</asp:TableRow>
			<asp:TableRow ID="TableRow11" runat="server" Height="30px">
				<asp:TableCell ID="TableCell118" runat="server" Font-Size="Medium" ForeColor="Black" Width="25px">
					<asp:RadioButton ID="RadioButton14" runat="server" GroupName="Selected" OnCheckedChanged="RBCheckedChanged" AutoPostBack="true"/></asp:TableCell>
				<asp:TableCell ID="TableCell57" runat="server" Font-Size="Medium" ForeColor="Black">
					<asp:CheckBox runat="server" ID="CBTop14" OnCheckedChanged="CBTop_CheckedChanged" AutoPostBack="true" />
				</asp:TableCell>
				<asp:TableCell ID="TableCell58" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell59" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell60" runat="server" Font-Size="Medium" ForeColor="White">
					<div id="Div14" runat="server" style="overflow:auto">
						<asp:CheckBoxList ID="CBLSectores14" runat="server"  Width="800px" RepeatDirection="Horizontal"></asp:CheckBoxList>
					

					

					

					</div>
				

</asp:TableCell>
				<asp:TableCell ID="TableCell142" runat="server" Font-Size="Medium" ForeColor="White" Width="60px">
					<asp:CheckBox ID="CBTodos14" runat="server" Text="Todos" AutoPostBack="true" OnCheckedChanged="CBTodos_CheckedChanged" />
				</asp:TableCell>
			</asp:TableRow>
			<asp:TableRow ID="TableRow12" runat="server" Height="30px">
				<asp:TableCell ID="TableCell119" runat="server" Font-Size="Medium" ForeColor="Black" Width="25px">
					<asp:RadioButton ID="RadioButton15" runat="server" GroupName="Selected" OnCheckedChanged="RBCheckedChanged" AutoPostBack="true"/></asp:TableCell>
				<asp:TableCell ID="TableCell61" runat="server" Font-Size="Medium" ForeColor="Black">
					<asp:CheckBox runat="server" ID="CBTop15" OnCheckedChanged="CBTop_CheckedChanged" AutoPostBack="true" />
				</asp:TableCell>
				<asp:TableCell ID="TableCell62" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell63" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell64" runat="server" Font-Size="Medium" ForeColor="White">
					<div id="Div15" runat="server" style="overflow:auto">
						<asp:CheckBoxList ID="CBLSectores15" runat="server"  Width="800px" RepeatDirection="Horizontal"></asp:CheckBoxList>
					

					

					

					</div>
				

</asp:TableCell>
				<asp:TableCell ID="TableCell143" runat="server" Font-Size="Medium" ForeColor="White" Width="60px">
					<asp:CheckBox ID="CBTodos15" runat="server" Text="Todos" AutoPostBack="true" OnCheckedChanged="CBTodos_CheckedChanged" />
				</asp:TableCell>
			</asp:TableRow>
			<asp:TableRow ID="TableRow13" runat="server" Height="30px">
				<asp:TableCell ID="TableCell120" runat="server" Font-Size="Medium" ForeColor="Black" Width="25px">
					<asp:RadioButton ID="RadioButton16" runat="server" GroupName="Selected" OnCheckedChanged="RBCheckedChanged" AutoPostBack="true"/></asp:TableCell>
				<asp:TableCell ID="TableCell65" runat="server" Font-Size="Medium" ForeColor="Black">
					<asp:CheckBox runat="server" ID="CBTop16" OnCheckedChanged="CBTop_CheckedChanged" AutoPostBack="true" />
				</asp:TableCell>
				<asp:TableCell ID="TableCell66" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell67" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell68" runat="server" Font-Size="Medium" ForeColor="White">
					<div id="Div16" runat="server" style="overflow:auto">
						<asp:CheckBoxList ID="CBLSectores16" runat="server"  Width="800px" RepeatDirection="Horizontal"></asp:CheckBoxList>
					

					

					

					</div>
				

</asp:TableCell>
				<asp:TableCell ID="TableCell144" runat="server" Font-Size="Medium" ForeColor="White" Width="60px">
					<asp:CheckBox ID="CBTodos16" runat="server" Text="Todos" AutoPostBack="true" OnCheckedChanged="CBTodos_CheckedChanged" />
				</asp:TableCell>
			</asp:TableRow>
			<asp:TableRow ID="TableRow14" runat="server" Height="30px">
				<asp:TableCell ID="TableCell121" runat="server" Font-Size="Medium" ForeColor="Black" Width="25px">
					<asp:RadioButton ID="RadioButton17" runat="server" GroupName="Selected" OnCheckedChanged="RBCheckedChanged" AutoPostBack="true"/></asp:TableCell>
				<asp:TableCell ID="TableCell69" runat="server" Font-Size="Medium" ForeColor="Black">
					<asp:CheckBox runat="server" ID="CBTop17" OnCheckedChanged="CBTop_CheckedChanged" AutoPostBack="true" />
				</asp:TableCell>
				<asp:TableCell ID="TableCell70" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell71" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell72" runat="server" Font-Size="Medium" ForeColor="White">
					<div id="Div17" runat="server" style="overflow:auto">
						<asp:CheckBoxList ID="CBLSectores17" runat="server"  Width="800px" RepeatDirection="Horizontal"></asp:CheckBoxList>
					

					

					

					</div>
				

</asp:TableCell>
				<asp:TableCell ID="TableCell145" runat="server" Font-Size="Medium" ForeColor="White" Width="60px">
					<asp:CheckBox ID="CBTodos17" runat="server" Text="Todos" AutoPostBack="true" OnCheckedChanged="CBTodos_CheckedChanged" />
				</asp:TableCell>
			</asp:TableRow>
			<asp:TableRow ID="TableRow15" runat="server" Height="30px">
				<asp:TableCell ID="TableCell122" runat="server" Font-Size="Medium" ForeColor="Black" Width="25px">
					<asp:RadioButton ID="RadioButton18" runat="server" GroupName="Selected" OnCheckedChanged="RBCheckedChanged" AutoPostBack="true"/></asp:TableCell>
				<asp:TableCell ID="TableCell73" runat="server" Font-Size="Medium" ForeColor="Black">
					<asp:CheckBox runat="server" ID="CBTop18" OnCheckedChanged="CBTop_CheckedChanged" AutoPostBack="true" />
				</asp:TableCell>
				<asp:TableCell ID="TableCell74" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell75" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell76" runat="server" Font-Size="Medium" ForeColor="White">
					<div id="Div18" runat="server" style="overflow:auto">
						<asp:CheckBoxList ID="CBLSectores18" runat="server"  Width="800px" RepeatDirection="Horizontal"></asp:CheckBoxList>
					

					

					

					</div>
				

</asp:TableCell>
				<asp:TableCell ID="TableCell146" runat="server" Font-Size="Medium" ForeColor="White" Width="60px">
					<asp:CheckBox ID="CBTodos18" runat="server" Text="Todos" AutoPostBack="true" OnCheckedChanged="CBTodos_CheckedChanged" />
				</asp:TableCell>
			</asp:TableRow>
			<asp:TableRow ID="TableRow16" runat="server" Height="30px">
				<asp:TableCell ID="TableCell123" runat="server" Font-Size="Medium" ForeColor="Black" Width="25px">
					<asp:RadioButton ID="RadioButton19" runat="server" GroupName="Selected" OnCheckedChanged="RBCheckedChanged" AutoPostBack="true"/></asp:TableCell>
				<asp:TableCell ID="TableCell77" runat="server" Font-Size="Medium" ForeColor="Black">
					<asp:CheckBox runat="server" ID="CBTop19" OnCheckedChanged="CBTop_CheckedChanged" AutoPostBack="true" />
				</asp:TableCell>
				<asp:TableCell ID="TableCell78" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell79" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell80" runat="server" Font-Size="Medium" ForeColor="White">
					<div id="Div19" runat="server" style="overflow:auto">
						<asp:CheckBoxList ID="CBLSectores19" runat="server"  Width="800px" RepeatDirection="Horizontal"></asp:CheckBoxList>
					

					

					

					</div>
				

</asp:TableCell>
				<asp:TableCell ID="TableCell147" runat="server" Font-Size="Medium" ForeColor="White" Width="60px">
					<asp:CheckBox ID="CBTodos19" runat="server" Text="Todos" AutoPostBack="true" OnCheckedChanged="CBTodos_CheckedChanged" />
				</asp:TableCell>
			</asp:TableRow>
			<asp:TableRow ID="TableRow17" runat="server" Height="30px">
				<asp:TableCell ID="TableCell124" runat="server" Font-Size="Medium" ForeColor="Black" Width="25px">
					<asp:RadioButton ID="RadioButton20" runat="server" GroupName="Selected" OnCheckedChanged="RBCheckedChanged" AutoPostBack="true"/></asp:TableCell>
				<asp:TableCell ID="TableCell81" runat="server" Font-Size="Medium" ForeColor="Black">
					<asp:CheckBox runat="server" ID="CBTop20" OnCheckedChanged="CBTop_CheckedChanged" AutoPostBack="true" />
				</asp:TableCell>
				<asp:TableCell ID="TableCell82" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell83" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell84" runat="server" Font-Size="Medium" ForeColor="White">
					<div id="Div20" runat="server" style="overflow:auto">
						<asp:CheckBoxList ID="CBLSectores20" runat="server"  Width="800px" RepeatDirection="Horizontal"></asp:CheckBoxList>
					

					

					

					</div>
				

</asp:TableCell>
				<asp:TableCell ID="TableCell148" runat="server" Font-Size="Medium" ForeColor="White" Width="60px">
					<asp:CheckBox ID="CBTodos20" runat="server" Text="Todos" AutoPostBack="true" OnCheckedChanged="CBTodos_CheckedChanged" />
				</asp:TableCell>
			</asp:TableRow>
			<asp:TableRow ID="TableRow18" runat="server" Height="30px">
				<asp:TableCell ID="TableCell125" runat="server" Font-Size="Medium" ForeColor="Black" Width="25px">
					<asp:RadioButton ID="RadioButton21" runat="server" GroupName="Selected" OnCheckedChanged="RBCheckedChanged" AutoPostBack="true"/></asp:TableCell>
				<asp:TableCell ID="TableCell85" runat="server" Font-Size="Medium" ForeColor="Black">
					<asp:CheckBox runat="server" ID="CBTop21" OnCheckedChanged="CBTop_CheckedChanged" AutoPostBack="true" />
				</asp:TableCell>
				<asp:TableCell ID="TableCell86" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell87" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell88" runat="server" Font-Size="Medium" ForeColor="White">
					<div id="Div21" runat="server" style="overflow:auto">
						<asp:CheckBoxList ID="CBLSectores21" runat="server"  Width="800px" RepeatDirection="Horizontal"></asp:CheckBoxList>
					

					

					

					</div>
				

</asp:TableCell>
				<asp:TableCell ID="TableCell149" runat="server" Font-Size="Medium" ForeColor="White" Width="60px">
					<asp:CheckBox ID="CBTodos21" runat="server" Text="Todos" AutoPostBack="true" OnCheckedChanged="CBTodos_CheckedChanged" />
				</asp:TableCell>
			</asp:TableRow> 
			<asp:TableRow ID="TableRow19" runat="server" Height="30px">
				<asp:TableCell ID="TableCell126" runat="server" Font-Size="Medium" ForeColor="Black" Width="25px">
					<asp:RadioButton ID="RadioButton22" runat="server" GroupName="Selected" OnCheckedChanged="RBCheckedChanged" AutoPostBack="true"/></asp:TableCell>
				<asp:TableCell ID="TableCell89" runat="server" Font-Size="Medium" ForeColor="Black">
					<asp:CheckBox runat="server" ID="CBTop22" OnCheckedChanged="CBTop_CheckedChanged" AutoPostBack="true" />
				</asp:TableCell>
				<asp:TableCell ID="TableCell90" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell91" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell92" runat="server" Font-Size="Medium" ForeColor="White">
					<div id="Div22" runat="server" style="overflow:auto">
						<asp:CheckBoxList ID="CBLSectores22" runat="server"  Width="800px" RepeatDirection="Horizontal"></asp:CheckBoxList>
					

					

					

					</div>
				

</asp:TableCell>
				<asp:TableCell ID="TableCell150" runat="server" Font-Size="Medium" ForeColor="White" Width="60px">
					<asp:CheckBox ID="CBTodos22" runat="server" Text="Todos" AutoPostBack="true" OnCheckedChanged="CBTodos_CheckedChanged" />
				</asp:TableCell>
			</asp:TableRow>
			<asp:TableRow ID="TableRow20" runat="server" Height="30px">
				<asp:TableCell ID="TableCell127" runat="server" Font-Size="Medium" ForeColor="Black" Width="25px">
					<asp:RadioButton ID="RadioButton23" runat="server" GroupName="Selected" OnCheckedChanged="RBCheckedChanged" AutoPostBack="true"/></asp:TableCell>
				<asp:TableCell ID="TableCell93" runat="server" Font-Size="Medium" ForeColor="Black">
					<asp:CheckBox runat="server" ID="CBTop23" OnCheckedChanged="CBTop_CheckedChanged" AutoPostBack="true" />
				</asp:TableCell>
				<asp:TableCell ID="TableCell94" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell95" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell96" runat="server" Font-Size="Medium" ForeColor="White">
					<div id="Div23" runat="server" style="overflow:auto">
						<asp:CheckBoxList ID="CBLSectores23" runat="server"  Width="800px" RepeatDirection="Horizontal"></asp:CheckBoxList>
					

					

					

					</div>
				

</asp:TableCell>
				<asp:TableCell ID="TableCell151" runat="server" Font-Size="Medium" ForeColor="White" Width="60px">
					<asp:CheckBox ID="CBTodos23" runat="server" Text="Todos" AutoPostBack="true" OnCheckedChanged="CBTodos_CheckedChanged" />
				</asp:TableCell>
			</asp:TableRow>
			<asp:TableRow ID="TableRow6" runat="server" Height="30px">
				<asp:TableCell ID="TableCell128" runat="server" Font-Size="Medium" ForeColor="Black" Width="25px">
					<asp:RadioButton ID="RadioButton24" runat="server" GroupName="Selected" OnCheckedChanged="RBCheckedChanged" AutoPostBack="true"/></asp:TableCell>
				<asp:TableCell ID="TableCell25" runat="server" Font-Size="Medium" ForeColor="Black">
					<asp:CheckBox runat="server" ID="CBTop24" OnCheckedChanged="CBTop_CheckedChanged" AutoPostBack="true" />
				</asp:TableCell>
				<asp:TableCell ID="TableCell38" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell39" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell40" runat="server" Font-Size="Medium" ForeColor="White">
					<div id="Div9" runat="server" style="overflow:auto">
						<asp:CheckBoxList ID="CBLSectores24" runat="server"  Width="800px" RepeatDirection="Horizontal"></asp:CheckBoxList>
					

					

					

					</div>
				

</asp:TableCell>
				<asp:TableCell ID="TableCell152" runat="server" Font-Size="Medium" ForeColor="White" Width="60px">
					<asp:CheckBox ID="CBTodos24" runat="server" Text="Todos" AutoPostBack="true" OnCheckedChanged="CBTodos_CheckedChanged" />
				</asp:TableCell>
			</asp:TableRow>

<%-- INICIO TOP25 // 20210505 #4815--%>
			<asp:TableRow ID="TableRow23" runat="server" Height="30px">
				<asp:TableCell ID="TableCell157" runat="server" Font-Size="Medium" ForeColor="Black" Width="25px">
					<asp:RadioButton ID="RadioButton25" runat="server" GroupName="Selected" OnCheckedChanged="RBCheckedChanged" AutoPostBack="true"/></asp:TableCell>
				<asp:TableCell ID="TableCell158" runat="server" Font-Size="Medium" ForeColor="Black">
					<asp:CheckBox runat="server" ID="CBTop25" OnCheckedChanged="CBTop_CheckedChanged" AutoPostBack="true" />
				</asp:TableCell>
				<asp:TableCell ID="TableCell159" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell160" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell161" runat="server" Font-Size="Medium" ForeColor="White">
					<div id="Div24" runat="server" style="overflow:auto">
						<asp:CheckBoxList ID="CBLSectores25" runat="server"  Width="800px" RepeatDirection="Horizontal"></asp:CheckBoxList>
					

					

					

					</div>
				

</asp:TableCell>
				<asp:TableCell ID="TableCell162" runat="server" Font-Size="Medium" ForeColor="White" Width="60px">
					<asp:CheckBox ID="CBTodos25" runat="server" Text="Todos" AutoPostBack="true" OnCheckedChanged="CBTodos_CheckedChanged" />
				</asp:TableCell>
			</asp:TableRow>
<%-- FIN TOP25  // 20210505 #4815--%>
<%-- INICIO TOP26 // 20210505 #4815--%>
			<asp:TableRow ID="TableRow24" runat="server" Height="30px">
				<asp:TableCell ID="TableCell163" runat="server" Font-Size="Medium" ForeColor="Black" Width="25px">
					<asp:RadioButton ID="RadioButton26" runat="server" GroupName="Selected" OnCheckedChanged="RBCheckedChanged" AutoPostBack="true"/></asp:TableCell>
				<asp:TableCell ID="TableCell164" runat="server" Font-Size="Medium" ForeColor="Black">
					<asp:CheckBox runat="server" ID="CBTop26" OnCheckedChanged="CBTop_CheckedChanged" AutoPostBack="true" />
				</asp:TableCell>
				<asp:TableCell ID="TableCell165" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell166" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell167" runat="server" Font-Size="Medium" ForeColor="White">
					<div id="Div25" runat="server" style="overflow:auto">
						<asp:CheckBoxList ID="CBLSectores26" runat="server"  Width="800px" RepeatDirection="Horizontal"></asp:CheckBoxList>
					

					

					

					</div>
				

</asp:TableCell>
				<asp:TableCell ID="TableCell168" runat="server" Font-Size="Medium" ForeColor="White" Width="60px">
					<asp:CheckBox ID="CBTodos26" runat="server" Text="Todos" AutoPostBack="true" OnCheckedChanged="CBTodos_CheckedChanged" />
				</asp:TableCell>
			</asp:TableRow>
<%-- FIN TOP26  // 20210505 #4815--%>

<%-- INICIO TOP27 // 20210505 #4815--%>
			<asp:TableRow ID="TableRow25" runat="server" Height="30px">
				<asp:TableCell ID="TableCell169" runat="server" Font-Size="Medium" ForeColor="Black" Width="25px">
					<asp:RadioButton ID="RadioButton27" runat="server" GroupName="Selected" OnCheckedChanged="RBCheckedChanged" AutoPostBack="true"/></asp:TableCell>
				<asp:TableCell ID="TableCell170" runat="server" Font-Size="Medium" ForeColor="Black">
					<asp:CheckBox runat="server" ID="CBTop27" OnCheckedChanged="CBTop_CheckedChanged" AutoPostBack="true" />
				</asp:TableCell>
				<asp:TableCell ID="TableCell171" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell172" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell173" runat="server" Font-Size="Medium" ForeColor="White">
					<div id="Div26" runat="server" style="overflow:auto">
						<asp:CheckBoxList ID="CBLSectores27" runat="server"  Width="800px" RepeatDirection="Horizontal"></asp:CheckBoxList>
					

					

					

					</div>
				

</asp:TableCell>
				<asp:TableCell ID="TableCell174" runat="server" Font-Size="Medium" ForeColor="White" Width="60px">
					<asp:CheckBox ID="CBTodos27" runat="server" Text="Todos" AutoPostBack="true" OnCheckedChanged="CBTodos_CheckedChanged" />
				</asp:TableCell>
			</asp:TableRow>
<%-- FIN TOP27  // 20210505 #4815--%>

<%-- INICIO TOP28 // 20210505 #4815--%>
			<asp:TableRow ID="TableRow26" runat="server" Height="30px">
				<asp:TableCell ID="TableCell175" runat="server" Font-Size="Medium" ForeColor="Black" Width="25px">
					<asp:RadioButton ID="RadioButton28" runat="server" GroupName="Selected" OnCheckedChanged="RBCheckedChanged" AutoPostBack="true"/></asp:TableCell>
				<asp:TableCell ID="TableCell176" runat="server" Font-Size="Medium" ForeColor="Black">
					<asp:CheckBox runat="server" ID="CBTop28" OnCheckedChanged="CBTop_CheckedChanged" AutoPostBack="true" />
				</asp:TableCell>
				<asp:TableCell ID="TableCell177" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell178" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell179" runat="server" Font-Size="Medium" ForeColor="White">
					<div id="Div27" runat="server" style="overflow:auto">
						<asp:CheckBoxList ID="CBLSectores28" runat="server"  Width="800px" RepeatDirection="Horizontal"></asp:CheckBoxList>
					

					

					

					</div>
				

</asp:TableCell>
				<asp:TableCell ID="TableCell180" runat="server" Font-Size="Medium" ForeColor="White" Width="60px">
					<asp:CheckBox ID="CBTodos28" runat="server" Text="Todos" AutoPostBack="true" OnCheckedChanged="CBTodos_CheckedChanged" />
				</asp:TableCell>
			</asp:TableRow>
<%-- FIN TOP28  // 20210505 #4815--%>
<%-- INICIO TOP29 // 20210505 #4815--%>
			<asp:TableRow ID="TableRow27" runat="server" Height="30px">
				<asp:TableCell ID="TableCell181" runat="server" Font-Size="Medium" ForeColor="Black" Width="25px">
					<asp:RadioButton ID="RadioButton29" runat="server" GroupName="Selected" OnCheckedChanged="RBCheckedChanged" AutoPostBack="true"/></asp:TableCell>
				<asp:TableCell ID="TableCell182" runat="server" Font-Size="Medium" ForeColor="Black">
					<asp:CheckBox runat="server" ID="CBTop29" OnCheckedChanged="CBTop_CheckedChanged" AutoPostBack="true" />
				</asp:TableCell>
				<asp:TableCell ID="TableCell183" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell184" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell185" runat="server" Font-Size="Medium" ForeColor="White">
					<div id="Div28" runat="server" style="overflow:auto">
						<asp:CheckBoxList ID="CBLSectores29" runat="server"  Width="800px" RepeatDirection="Horizontal"></asp:CheckBoxList>
					

					

					

					</div>
				

</asp:TableCell>
				<asp:TableCell ID="TableCell186" runat="server" Font-Size="Medium" ForeColor="White" Width="60px">
					<asp:CheckBox ID="CBTodos29" runat="server" Text="Todos" AutoPostBack="true" OnCheckedChanged="CBTodos_CheckedChanged" />
				</asp:TableCell>
			</asp:TableRow>
<%-- FIN TOP29  // 20210505 #4815--%>
<%-- INICIO TOP30 // 20210505 #4815--%>
			<asp:TableRow ID="TableRow28" runat="server" Height="30px">
				<asp:TableCell ID="TableCell187" runat="server" Font-Size="Medium" ForeColor="Black" Width="25px">
					<asp:RadioButton ID="RadioButton30" runat="server" GroupName="Selected" OnCheckedChanged="RBCheckedChanged" AutoPostBack="true"/></asp:TableCell>
				<asp:TableCell ID="TableCell188" runat="server" Font-Size="Medium" ForeColor="Black">
					<asp:CheckBox runat="server" ID="CBTop30" OnCheckedChanged="CBTop_CheckedChanged" AutoPostBack="true" />
				</asp:TableCell>
				<asp:TableCell ID="TableCell189" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell190" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell191" runat="server" Font-Size="Medium" ForeColor="White">
					<div id="Div29" runat="server" style="overflow:auto">
						<asp:CheckBoxList ID="CBLSectores30" runat="server"  Width="800px" RepeatDirection="Horizontal"></asp:CheckBoxList>
					

					

					

					</div>
				

</asp:TableCell>
				<asp:TableCell ID="TableCell192" runat="server" Font-Size="Medium" ForeColor="White" Width="60px">
					<asp:CheckBox ID="CBTodos30" runat="server" Text="Todos" AutoPostBack="true" OnCheckedChanged="CBTodos_CheckedChanged" />
				</asp:TableCell>
			</asp:TableRow>
<%-- FIN TOP30  // 20210505 #4815--%>

<%-- INICIO TOP31 // 20210505 #4815--%>
			<asp:TableRow ID="TableRow29" runat="server" Height="30px">
				<asp:TableCell ID="TableCell193" runat="server" Font-Size="Medium" ForeColor="Black" Width="25px">
					<asp:RadioButton ID="RadioButton31" runat="server" GroupName="Selected" OnCheckedChanged="RBCheckedChanged" AutoPostBack="true"/></asp:TableCell>
				<asp:TableCell ID="TableCell194" runat="server" Font-Size="Medium" ForeColor="Black">
					<asp:CheckBox runat="server" ID="CBTop31" OnCheckedChanged="CBTop_CheckedChanged" AutoPostBack="true" />
				</asp:TableCell>
				<asp:TableCell ID="TableCell195" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell196" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell197" runat="server" Font-Size="Medium" ForeColor="White">
					<div id="Div30" runat="server" style="overflow:auto">
						<asp:CheckBoxList ID="CBLSectores31" runat="server"  Width="800px" RepeatDirection="Horizontal"></asp:CheckBoxList>
					

					

					

					</div>
				

</asp:TableCell>
				<asp:TableCell ID="TableCell198" runat="server" Font-Size="Medium" ForeColor="White" Width="60px">
					<asp:CheckBox ID="CBTodos31" runat="server" Text="Todos" AutoPostBack="true" OnCheckedChanged="CBTodos_CheckedChanged" />
				</asp:TableCell>
			</asp:TableRow>
<%-- FIN TOP31  // 20210505 #4815--%>
<%-- INICIO TOP32 // 20210505 #4815--%>
			<asp:TableRow ID="TableRow30" runat="server" Height="30px">
				<asp:TableCell ID="TableCell200" runat="server" Font-Size="Medium" ForeColor="Black" Width="25px">
					<asp:RadioButton ID="RadioButton32" runat="server" GroupName="Selected" OnCheckedChanged="RBCheckedChanged" AutoPostBack="true"/></asp:TableCell>
				<asp:TableCell ID="TableCell201" runat="server" Font-Size="Medium" ForeColor="Black">
					<asp:CheckBox runat="server" ID="CBTop32" OnCheckedChanged="CBTop_CheckedChanged" AutoPostBack="true" />
				</asp:TableCell>
				<asp:TableCell ID="TableCell202" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell203" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell204" runat="server" Font-Size="Medium" ForeColor="White">
					<div id="Div31" runat="server" style="overflow:auto">
						<asp:CheckBoxList ID="CBLSectores32" runat="server"  Width="800px" RepeatDirection="Horizontal"></asp:CheckBoxList>
					

					

					

					</div>
				

</asp:TableCell>
				<asp:TableCell ID="TableCell205" runat="server" Font-Size="Medium" ForeColor="White" Width="60px">
					<asp:CheckBox ID="CBTodos32" runat="server" Text="Todos" AutoPostBack="true" OnCheckedChanged="CBTodos_CheckedChanged" />
				</asp:TableCell>
			</asp:TableRow>
<%-- FIN TOP32  // 20210505 #4815--%>

<%-- INICIO TOP33 // 20210505 #4815--%>
			<asp:TableRow ID="TableRow31" runat="server" Height="30px">
				<asp:TableCell ID="TableCell206" runat="server" Font-Size="Medium" ForeColor="Black" Width="25px">
					<asp:RadioButton ID="RadioButton33" runat="server" GroupName="Selected" OnCheckedChanged="RBCheckedChanged" AutoPostBack="true"/></asp:TableCell>
				<asp:TableCell ID="TableCell207" runat="server" Font-Size="Medium" ForeColor="Black">
					<asp:CheckBox runat="server" ID="CBTop33" OnCheckedChanged="CBTop_CheckedChanged" AutoPostBack="true" />
				</asp:TableCell>
				<asp:TableCell ID="TableCell208" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell209" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell210" runat="server" Font-Size="Medium" ForeColor="White">
					<div id="Div32" runat="server" style="overflow:auto">
						<asp:CheckBoxList ID="CBLSectores33" runat="server"  Width="800px" RepeatDirection="Horizontal"></asp:CheckBoxList>
					

					

					

					</div>
				

</asp:TableCell>
				<asp:TableCell ID="TableCell211" runat="server" Font-Size="Medium" ForeColor="White" Width="60px">
					<asp:CheckBox ID="CBTodos33" runat="server" Text="Todos" AutoPostBack="true" OnCheckedChanged="CBTodos_CheckedChanged" />
				</asp:TableCell>
			</asp:TableRow>
<%-- FIN TOP33  // 20210505 #4815--%>

<%-- INICIO TOP34 // 20210505 #4815--%>
			<asp:TableRow ID="TableRow32" runat="server" Height="30px">
				<asp:TableCell ID="TableCell212" runat="server" Font-Size="Medium" ForeColor="Black" Width="25px">
					<asp:RadioButton ID="RadioButton34" runat="server" GroupName="Selected" OnCheckedChanged="RBCheckedChanged" AutoPostBack="true"/></asp:TableCell>
				<asp:TableCell ID="TableCell213" runat="server" Font-Size="Medium" ForeColor="Black">
					<asp:CheckBox runat="server" ID="CBTop34" OnCheckedChanged="CBTop_CheckedChanged" AutoPostBack="true" />
				</asp:TableCell>
				<asp:TableCell ID="TableCell214" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell215" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell216" runat="server" Font-Size="Medium" ForeColor="White">
					<div id="Div33" runat="server" style="overflow:auto">
						<asp:CheckBoxList ID="CBLSectores34" runat="server"  Width="800px" RepeatDirection="Horizontal"></asp:CheckBoxList>
					

					

					

					</div>
				

</asp:TableCell>
				<asp:TableCell ID="TableCell217" runat="server" Font-Size="Medium" ForeColor="White" Width="60px">
					<asp:CheckBox ID="CBTodos34" runat="server" Text="Todos" AutoPostBack="true" OnCheckedChanged="CBTodos_CheckedChanged" />
				</asp:TableCell>
			</asp:TableRow>
<%-- FIN TOP34  // 20210505 #4815--%>
<%-- INICIO TOP35 // 20210505 #4815--%>
			<asp:TableRow ID="TableRow33" runat="server" Height="30px">
				<asp:TableCell ID="TableCell218" runat="server" Font-Size="Medium" ForeColor="Black" Width="25px">
					<asp:RadioButton ID="RadioButton35" runat="server" GroupName="Selected" OnCheckedChanged="RBCheckedChanged" AutoPostBack="true"/></asp:TableCell>
				<asp:TableCell ID="TableCell219" runat="server" Font-Size="Medium" ForeColor="Black">
					<asp:CheckBox runat="server" ID="CBTop35" OnCheckedChanged="CBTop_CheckedChanged" AutoPostBack="true" />
				</asp:TableCell>
				<asp:TableCell ID="TableCell220" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell221" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell222" runat="server" Font-Size="Medium" ForeColor="White">
					<div id="Div34" runat="server" style="overflow:auto">
						<asp:CheckBoxList ID="CBLSectores35" runat="server"  Width="800px" RepeatDirection="Horizontal"></asp:CheckBoxList>
					

					

					

					</div>
				

</asp:TableCell>
				<asp:TableCell ID="TableCell223" runat="server" Font-Size="Medium" ForeColor="White" Width="60px">
					<asp:CheckBox ID="CBTodos35" runat="server" Text="Todos" AutoPostBack="true" OnCheckedChanged="CBTodos_CheckedChanged" />
				</asp:TableCell>
			</asp:TableRow>
<%-- FIN TOP35  // 20210505 #4815--%>

<%-- INICIO TOP36 // 20210505 #4815--%>
			<asp:TableRow ID="TableRow34" runat="server" Height="30px">
				<asp:TableCell ID="TableCell224" runat="server" Font-Size="Medium" ForeColor="Black" Width="25px">
					<asp:RadioButton ID="RadioButton36" runat="server" GroupName="Selected" OnCheckedChanged="RBCheckedChanged" AutoPostBack="true"/></asp:TableCell>
				<asp:TableCell ID="TableCell225" runat="server" Font-Size="Medium" ForeColor="Black">
					<asp:CheckBox runat="server" ID="CBTop36" OnCheckedChanged="CBTop_CheckedChanged" AutoPostBack="true" />
				</asp:TableCell>
				<asp:TableCell ID="TableCell226" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell227" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell228" runat="server" Font-Size="Medium" ForeColor="White">
					<div id="Div35" runat="server" style="overflow:auto">
						<asp:CheckBoxList ID="CBLSectores36" runat="server"  Width="800px" RepeatDirection="Horizontal"></asp:CheckBoxList>
					

					

					

					</div>
				

</asp:TableCell>
				<asp:TableCell ID="TableCell229" runat="server" Font-Size="Medium" ForeColor="White" Width="60px">
					<asp:CheckBox ID="CBTodos36" runat="server" Text="Todos" AutoPostBack="true" OnCheckedChanged="CBTodos_CheckedChanged" />
				</asp:TableCell>
			</asp:TableRow>
<%-- FIN TOP36  // 20210505 #4815--%>

<%-- INICIO TOP37 // 20210505 #4815--%>
			<asp:TableRow ID="TableRow35" runat="server" Height="30px">
				<asp:TableCell ID="TableCell230" runat="server" Font-Size="Medium" ForeColor="Black" Width="25px">
					<asp:RadioButton ID="RadioButton37" runat="server" GroupName="Selected" OnCheckedChanged="RBCheckedChanged" AutoPostBack="true"/></asp:TableCell>
				<asp:TableCell ID="TableCell231" runat="server" Font-Size="Medium" ForeColor="Black">
					<asp:CheckBox runat="server" ID="CBTop37" OnCheckedChanged="CBTop_CheckedChanged" AutoPostBack="true" />
				</asp:TableCell>
				<asp:TableCell ID="TableCell232" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell233" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell234" runat="server" Font-Size="Medium" ForeColor="White">
					<div id="Div36" runat="server" style="overflow:auto">
						<asp:CheckBoxList ID="CBLSectores37" runat="server"  Width="800px" RepeatDirection="Horizontal"></asp:CheckBoxList>
					

					

					

					</div>
				

</asp:TableCell>
				<asp:TableCell ID="TableCell235" runat="server" Font-Size="Medium" ForeColor="White" Width="60px">
					<asp:CheckBox ID="CBTodos37" runat="server" Text="Todos" AutoPostBack="true" OnCheckedChanged="CBTodos_CheckedChanged" />
				</asp:TableCell>
			</asp:TableRow>
<%-- FIN TOP37  // 20210505 #4815--%>

<%-- INICIO TOP38 // 20210505 #4815--%>
			<asp:TableRow ID="TableRow36" runat="server" Height="30px">
				<asp:TableCell ID="TableCell236" runat="server" Font-Size="Medium" ForeColor="Black" Width="25px">
					<asp:RadioButton ID="RadioButton38" runat="server" GroupName="Selected" OnCheckedChanged="RBCheckedChanged" AutoPostBack="true"/></asp:TableCell>
				<asp:TableCell ID="TableCell237" runat="server" Font-Size="Medium" ForeColor="Black">
					<asp:CheckBox runat="server" ID="CBTop38" OnCheckedChanged="CBTop_CheckedChanged" AutoPostBack="true" />
				</asp:TableCell>
				<asp:TableCell ID="TableCell238" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell239" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell240" runat="server" Font-Size="Medium" ForeColor="White">
					<div id="Div37" runat="server" style="overflow:auto">
						<asp:CheckBoxList ID="CBLSectores38" runat="server"  Width="800px" RepeatDirection="Horizontal"></asp:CheckBoxList>
					

					

					

					</div>
				

</asp:TableCell>
				<asp:TableCell ID="TableCell241" runat="server" Font-Size="Medium" ForeColor="White" Width="60px">
					<asp:CheckBox ID="CBTodos38" runat="server" Text="Todos" AutoPostBack="true" OnCheckedChanged="CBTodos_CheckedChanged" />
				</asp:TableCell>
			</asp:TableRow>
<%-- FIN TOP38  // 20210505 #4815--%>
<%-- INICIO TOP39 // 20210505 #4815--%>
			<asp:TableRow ID="TableRow37" runat="server" Height="30px">
				<asp:TableCell ID="TableCell242" runat="server" Font-Size="Medium" ForeColor="Black" Width="25px">
					<asp:RadioButton ID="RadioButton39" runat="server" GroupName="Selected" OnCheckedChanged="RBCheckedChanged" AutoPostBack="true"/></asp:TableCell>
				<asp:TableCell ID="TableCell243" runat="server" Font-Size="Medium" ForeColor="Black">
					<asp:CheckBox runat="server" ID="CBTop39" OnCheckedChanged="CBTop_CheckedChanged" AutoPostBack="true" />
				</asp:TableCell>
				<asp:TableCell ID="TableCell244" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell245" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell246" runat="server" Font-Size="Medium" ForeColor="White">
					<div id="Div38" runat="server" style="overflow:auto">
						<asp:CheckBoxList ID="CBLSectores39" runat="server"  Width="800px" RepeatDirection="Horizontal"></asp:CheckBoxList>
					

					

					

					</div>
				

</asp:TableCell>
				<asp:TableCell ID="TableCell247" runat="server" Font-Size="Medium" ForeColor="White" Width="60px">
					<asp:CheckBox ID="CBTodos39" runat="server" Text="Todos" AutoPostBack="true" OnCheckedChanged="CBTodos_CheckedChanged" />
				</asp:TableCell>
			</asp:TableRow>
<%-- FIN TOP39  // 20210505 #4815--%>

<%-- INICIO TOP40 // 20210505 #4815--%>
			<asp:TableRow ID="TableRow38" runat="server" Height="30px">
				<asp:TableCell ID="TableCell248" runat="server" Font-Size="Medium" ForeColor="Black" Width="25px">
					<asp:RadioButton ID="RadioButton40" runat="server" GroupName="Selected" OnCheckedChanged="RBCheckedChanged" AutoPostBack="true"/></asp:TableCell>
				<asp:TableCell ID="TableCell249" runat="server" Font-Size="Medium" ForeColor="Black">
					<asp:CheckBox runat="server" ID="CBTop40" OnCheckedChanged="CBTop_CheckedChanged" AutoPostBack="true" />
				</asp:TableCell>
				<asp:TableCell ID="TableCell250" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell251" runat="server" Font-Size="Medium" ForeColor="White"></asp:TableCell>
				<asp:TableCell ID="TableCell252" runat="server" Font-Size="Medium" ForeColor="White">
					<div id="Div39" runat="server" style="overflow:auto">
						<asp:CheckBoxList ID="CBLSectores40" runat="server"  Width="800px" RepeatDirection="Horizontal"></asp:CheckBoxList>
					

					

					

					</div>
				

</asp:TableCell>
				<asp:TableCell ID="TableCell253" runat="server" Font-Size="Medium" ForeColor="White" Width="60px">
					<asp:CheckBox ID="CBTodos40" runat="server" Text="Todos" AutoPostBack="true" OnCheckedChanged="CBTodos_CheckedChanged" />
				</asp:TableCell>
			</asp:TableRow>
<%-- FIN TOP40  // 20210505 #4815--%>
			</asp:Table>
		</div>
		<div runat="server" 
			  style="border: thin outset #333399; background-color: #666699">
		 <asp:Table runat="server" Width="1170px">
			<asp:TableRow runat="server">
				<asp:TableCell runat="server"></asp:TableCell>
			</asp:TableRow>
			<asp:TableRow runat="server">
				<asp:TableCell runat="server" Width="200px" VerticalAlign="Top">
				    
                     <asp:LinkButton ID="BtnCargaSectorizacion" runat="server" style="height: 33px; width: 166px" Text="Cargar sectorización" OnClick="OnClick_BtnCargarSectorizacion"
				      ForeColor="LightGreen"/>

                    </asp:TableCell><asp:TableCell runat="server" Width="400px" VerticalAlign="Top">
                     <asp:LinkButton ID="BtnCargaSectorizacionSacta" runat="server" style="height: 33px; width: 200px;" Text="Solicitud sectorización Sacta" OnClick="OnClick_BtnCargarSectorizacionSacta"
				      ForeColor="LightGreen"/>

				</asp:TableCell><asp:TableCell runat="server" Width="200px" VerticalAlign="Top">
					 <asp:LinkButton ID="LBtnSectorizaciones" runat="server" style="height: 23px; width: 101px" Text="Sectorizaciones" 
					  ForeColor="LightGreen"/>		
				
</asp:TableCell><asp:TableCell runat="server" VerticalAlign="Top">
					<ajaxToolkit:CollapsiblePanelExtender ID="CollapsiblePanelExtender1" runat="server"
						 TargetControlID="PnlContent" ExpandControlID="LBtnSectorizaciones" CollapseControlID="LBtnSectorizaciones"
						 Collapsed="True" SuppressPostBack="true" ExpandDirection="Vertical">
					</ajaxToolkit:CollapsiblePanelExtender>
					
<asp:Panel ID="PnlContent" runat="server" BorderStyle="Solid" 
						CssClass="collapsePanel" Style="height:200px; width:200px;">
						<asp:ListBox ID="LBSectorizaciones" runat="server" Height="200px" Width="200px" BackColor="#FFCC99" OnSelectedIndexChanged="OnSelectedIndexChange_LBSectorizaciones" AutoPostBack="true"></asp:ListBox>
				

				</asp:Panel>
				
</asp:TableCell><asp:TableCell ID="TableCell156" runat="server" />

                <asp:TableCell ID="TableCell154" runat="server" VerticalAlign="Top">
					<ajaxToolkit:CollapsiblePanelExtender ID="CollapsiblePanelExtender2" runat="server"
						 TargetControlID="Panel2" ExpandControlID="LBtnEliminar" CollapseControlID="LBtnEliminar"
						 Collapsed="True" SuppressPostBack="true" ExpandDirection="Vertical">
					</ajaxToolkit:CollapsiblePanelExtender>
					
<asp:Panel ID="Panel2" runat="server" BorderStyle="Solid" 
						CssClass="collapsePanel" Style="height:200px; width:200px;">
						<asp:ListBox ID="LBEliminar" runat="server" Height="200px" Width="200px" BackColor="#FFCC99" OnSelectedIndexChanged="OnSelectedIndexChange_LBEliminar" AutoPostBack="true"></asp:ListBox>
				

				</asp:Panel>
</asp:TableCell><asp:TableCell ID="TableCell153" runat="server" VerticalAlign="Top">
					                 <asp:LinkButton ID="LBtnEliminar" runat="server" style="height: 23px; width: 101px" Text="Eliminar" 
					                  ForeColor="LightGreen"/>		
                				
                </asp:TableCell><asp:TableCell ID="TableCell155" runat="server" />

                <asp:TableCell runat="server" VerticalAlign="Top" HorizontalAlign="Right">
			    	<asp:LinkButton ID="LBtnGuardar" runat="server" onclick="BtnGuardar_Click" ForeColor="LightGreen">Guardar</asp:LinkButton>
				
</asp:TableCell></asp:TableRow></asp:Table></div></form></body></html>
﻿<%@ Page Language="C#" AutoEventWireup="true" CodeFile="WizardConfiguracion.aspx.cs" Inherits="Wizard_WizardConfiguracion" 
CodeFileBaseClass="PageBaseCD40.PageCD40" meta:resourcekey="PageResource1"%>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
    <title></title>
    <link rel="shortcut icon" href="~/favicon.ico">
</head>
<body>
<%--	  <script type="text/javascript">
        if (window.name != "<%=GetWindowName()%>") {
            window.name = "invalidAccess";
            window.open("../BloqueoAplicacion.aspx", "_self");
        }

			function AbreVentana(ventana) {
			window.open(ventana);
		}
	</script>
--%>    

<%--    <script type="text/javascript">
        var initHistoryLength;
        var myTimer;
        window.onload = function () {
            initHistoryLength = history.length;
            myTimer = setInterval(function () { checkHistory(); }, 1000);
        };
        function checkHistory() {
            if (history.length > initHistoryLength) {
                document.getElementById('PanelWizard').style.display = "none";ASISTENTE CONFIGURACIÓN ULISES V 5000 I
                clearInterval(myTimer);
            }
        }
    </script>--%>

    <form id="form1" runat="server">


		<asp:Panel ID="PanelWizard" runat="server" BorderColor="Black" BackColor="white" 
			Height="842px" Width="1280px" style="margin-bottom: 84px">
            		
            <div id="DivCabeceraWizard" runat="server" style="z-index: 125; left: 31px; width: 1248px; position: absolute; top: 29px;background-color:transparent;
                height: 105px; overflow:inherit; border-style:solid; border-width:1px" class="estiloDiv red" visible="true" border-style="">


            <div  id="DivLogo1" runat="server" 
                style="border-style: solid; border-color:greenyellow; z-index: 102; left: 2px; top: 2px;  width: 230px; position: absolute; height: 100px; overflow: hidden; background-color:transparent;border-style:solid; border-width:1px" class="auto-style1" >
            <asp:Image ID="Image20" runat="server" ImageUrl="~/Images/GRUPO_AMPER_CNF.bmp" Style="z-index: 109; left: 2px; width: 226px; position: absolute; top: 5px; height: 90px; 
                border-right-color: black; background-image: none; background-color: transparent;" />
             </div>   
             <div  id="DivTitulo" runat="server" 
                style="border-style: solid; border-color:greenyellow; z-index: 102; left: 240px; top: 2px;  width: 830px; position: absolute; height: 100px; overflow: hidden; background-color:transparent;border-style:solid; border-width:1px" class="auto-style1" >
                <asp:Label ID="Label1" runat="server" SkinID="LabelCabeceraCNF" 
                ForeColor="#4A7729" BackColor="Transparent" Font-Names="Arial" Font-Size="18px" Font-Bold="true"
                Font-Strikeout="False" Font-Underline="False" Style="z-index: 100; left: 200px; position: absolute; top: 17px; height: 28px; width: 600px; right: 392px;"
                meta:resourcekey="LabelUlises"></asp:Label>
                <asp:LinkButton ID="LBFinWizard" runat="server" ForeColor="#616365" Font-Names="Arial" Font-Size="14px"
			    style="height: 26px; width: 200px; position:absolute; left:40px; top:73px" Visible="true"
			    OnClick="LBFinWizard_OnCLick"  meta:resourcekey="LBFinWizardResource1"/>
                <asp:LinkButton ID="LinkLogOut" runat="server" Style="z-index: 114; left: 124px; position: absolute; top: 73px"
                    OnClick="LinkLogOut_Click" ForeColor="#616365" Font-Names="Arial" Font-Size="14px" CausesValidation="False"
                    meta:resourcekey="LinkButton1Resource1">Cerrar sesión</asp:LinkButton>                 		
            </div> 
            <div  id="DivDatos" runat="server" 
                style="border-style: solid; border-color:greenyellow; z-index: 102; left: 1078px; top: 2px;  width: 168px; position: absolute; height: 100px; overflow: hidden; background-color:transparent;border-style:solid; border-width:1px" class="auto-style1" >                                
                                       
                <asp:Label ID="Label2" runat="server" Style="z-index: 110; left: 10px; position: absolute; top: 34px" ForeColor="#616365" Font-Names="Arial" Font-Size="14px"
                    Text="Usuario:" meta:resourcekey="Label2Resource1"></asp:Label>
                <asp:Label ID="LabelUsuario" runat="server" Style="z-index: 111; left: 75px; position: absolute; top: 34px" 
                    ForeColor="#4A7729" Font-Bold="True" Font-Names="Arial" Font-Size="14px" meta:resourcekey="LabelUsuarioResource1"></asp:Label>
                <asp:Label ID="Label3" runat="server" Style="z-index: 112; left: 10px; position: absolute; top: 54px" ForeColor="#616365" Font-Names="Arial" Font-Size="14px"
                    Text="Perfil:" meta:resourcekey="Label3Resource1"></asp:Label>
                <asp:Label ID="LabelPerfil" runat="server" Style="z-index: 113; left: 61px; position: absolute; top: 54px"
                    ForeColor="#4A7729" Font-Bold="True" Font-Names="Arial" Font-Size="14px" Width="91px" meta:resourcekey="LabelPerfilResource1"></asp:Label>
            </div>
                			        
            </div>









			<iframe runat="server" id="IFrameWeb" src="../Configuracion/Nucleos.aspx?iframe=true"
				style="overflow:hidden;left:21px; height: 760px; width: 1260px; position:absolute; top: 19px; bottom: 25px;" 
				scrolling="no">
			</iframe>

           <asp:Table runat="server" ID="Table2" 
                Style="position:relative;left: 46px; top: 775px; height:55px; width: 1113px;">
            <asp:TableRow runat="server" ID="Table1Row1">
                <asp:TableCell runat="server" ID="Table1Row1Cell1">
			        <asp:Image ID="Image1" runat="server" ImageUrl="~/Images/cab1On.png" style="height: 50px; width: 50px" />
			    </asp:TableCell>
			    <asp:TableCell runat="server" ID="TableCell3">
			        <asp:Image ID="Image2" runat="server" ImageUrl="~/Images/cab2.png" style="height: 50px; width: 50px" />
                </asp:TableCell>
			    <asp:TableCell runat="server" ID="TableCell4">
			        <asp:Image ID="Image3" runat="server" ImageUrl="~/Images/cab3.png" style="height: 50px; width: 50px" />
                </asp:TableCell>
			    <asp:TableCell runat="server" ID="TableCell5">
			        <asp:Image ID="Image4" runat="server" ImageUrl="~/Images/cab4.png" style="height: 50px; width: 50px" />
                </asp:TableCell>
			    <asp:TableCell runat="server" ID="TableCell6">
			        <asp:Image ID="Image5" runat="server" ImageUrl="~/Images/cab5.png" style="height: 50px; width: 50px" />
                </asp:TableCell>
			    <asp:TableCell runat="server" ID="TableCell7">
			        <asp:Image ID="Image6" runat="server" ImageUrl="~/Images/cab6.png" style="height: 50px; width: 50px" />
                </asp:TableCell>
			    <asp:TableCell runat="server" ID="TableCell8">
			        <asp:Image ID="Image7" runat="server" ImageUrl="~/Images/cab7.png" style="height: 50px; width: 50px" />
                </asp:TableCell>
			    <asp:TableCell runat="server" ID="TableCell9">
			        <asp:Image ID="Image8" runat="server" ImageUrl="~/Images/cab8.png" style="height: 50px; width: 50px" />
                </asp:TableCell>
			    <asp:TableCell runat="server" ID="TableCell10">
			        <asp:Image ID="Image9" runat="server" ImageUrl="~/Images/cab9.png" style="height: 50px; width: 50px" />
                </asp:TableCell>
			    <asp:TableCell runat="server" ID="TableCell11">
			        <asp:Image ID="Image10" runat="server" ImageUrl="~/Images/cab10.png" style="height: 50px; width: 50px" />
                </asp:TableCell>
			    <asp:TableCell runat="server" ID="TableCell12">
			        <asp:Image ID="Image11" runat="server" ImageUrl="~/Images/cab11.png" style="height: 50px; width: 50px" />
                </asp:TableCell>
			    <asp:TableCell runat="server" ID="TableCell13">
			        <asp:Image ID="Image12" runat="server" ImageUrl="~/Images/cab12.png" style="height: 50px; width: 50px" />
                </asp:TableCell>
			    <asp:TableCell runat="server" ID="TableCell14">
			        <asp:Image ID="Image13" runat="server" ImageUrl="~/Images/cab13.png" style="height: 50px; width: 50px" />
                </asp:TableCell>
			    <asp:TableCell runat="server" ID="TableCell15">
			        <asp:Image ID="Image14" runat="server" ImageUrl="~/Images/cab14.png" style="height: 50px; width: 50px" />
                </asp:TableCell>
			    <asp:TableCell runat="server" ID="TableCell16">
			        <asp:Image ID="Image15" runat="server" ImageUrl="~/Images/cab15.png" style="height: 50px; width: 50px" />
                </asp:TableCell>
			    <asp:TableCell runat="server" ID="TableCell17">
			        <asp:Image ID="Image16" runat="server" ImageUrl="~/Images/cab16.png" style="height: 50px; width: 50px" />
                </asp:TableCell>
			    <asp:TableCell runat="server" ID="TableCell18">
			        <asp:Image ID="Image17" runat="server" ImageUrl="~/Images/cab17.png" style="height: 50px; width: 50px" />
                </asp:TableCell>
			    <asp:TableCell runat="server" ID="TableCell19">
			        <asp:Image ID="Image18" runat="server" ImageUrl="~/Images/cab18.png" style="height: 50px; width: 50px" />
                </asp:TableCell>
			    <asp:TableCell runat="server" ID="TableCell20">
			        <asp:Image ID="Image19" runat="server" ImageUrl="~/Images/cab19.png" style="height: 50px; width: 50px" />
                </asp:TableCell>
            </asp:TableRow>
           </asp:Table>

				<asp:Table runat="server" ID="Table" 
                Style="position:relative;left: 46px; top: 770px; height:39px; width: 1113px;">
				    <asp:TableRow runat="server" ID="Row1">
			            <asp:TableCell runat="server" ID="Cell1" Width="100px">
                            <asp:Button ID="BtnAnterior" runat="server" BackColor="White" 
				                BorderStyle="Outset" 
				                style="height: 26px; width: 89px" 
				                onclick="BtnAnterior_Click" Enabled="false" meta:resourcekey="BtnAnteriorResource1"/>
			            </asp:TableCell>
                        <asp:TableCell runat="server" ID="TableCell1" Width="950px">
			                <asp:Button ID="BtnSiguiente" runat="server" BackColor="White" 
				                BorderStyle="Outset" 
				                style="height: 26px; width: 89px" 
				                onclick="BtnSiguiente_Click" Enabled="true"  meta:resourcekey="BtnSiguienteResource1"/>
                            </asp:TableCell>
			            <asp:TableCell runat="server" ID="TableCell2">
<%--                            <asp:LinkButton ID="LBFinWizard" runat="server" ForeColor="#616365" Font-Names="Arial" Font-Size="14px"
			                style="height: 26px; width: 289px;" Visible="true"
			                OnClick="LBFinWizard_OnCLick"  meta:resourcekey="LBFinWizardResource1"/>					        --%>
			            </asp:TableCell>
				    </asp:TableRow>
				</asp:Table>
		</asp:Panel>

    </form>
        <div style="z-index: 301; left: 8px; width: 1264px; position: absolute; top: 890px; height: 37px; text-align:center; color: black; font-family:Arial; font-size:small" >
            <br\ />
            &#169  Grupo Amper. Copyright @ 2021-2023. Versión 3.0.0 Todos los Derechos Reservados
        </div>

</body></html>
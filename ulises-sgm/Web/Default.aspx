<%@ Page Language="C#" AutoEventWireup="true"  CodeFile="Default.aspx.cs" Inherits="_Default" 
 StylesheetTheme="TemaPaginasConfiguracion" Theme="TemaPaginasConfiguracion" meta:resourcekey="PageResource1" %>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" >
<head runat="server">
      <link rel="shortcut icon" href="~/favicon.ico"/>
</head>

<body >
    <script type="text/javascript" src="../Javascript/jquery-3.3.1.js"></script>
    <script type="text/javascript" src="../Javascript/alertify.min.js"></script>
    <script type="text/javascript" src="../Javascript/tick-alive.js"></script>

    <form id="form1" runat="server" class="estiloDiv gradpositivo white">

                <asp:Image ID="Image1" runat="server" ImageUrl="~/Images/GRUPO_AMPER_PQ.png"
			                  style="height: 230px; width:330px; position:absolute; top: 200px; left: 473px;" meta:resourcekey="Image1Resource1"/>

                <asp:LinkButton ID="LBPpalMantenimientoCluster" runat="server" 
			                  OnClick="LBPpalMantenimientoCluster_Click" 
			                  Width="258px" CssClass="reflection"
                              Style="position:absolute; top: 80px; left: 530px; height: auto; width: 200px; color: black; font-family:Arial; font-size:medium" 
			                  meta:resourcekey="LBPpalMantenimientoClusterResource1">Mantenimiento Cluster
			            </asp:LinkButton>
                <asp:LinkButton ID="LBPpalConfiguracion" runat="server" 
			                  OnClick="LBPpalConfiguracion_Click" 
			                  Width="158px" CssClass="reflection"
                             Style="position:absolute; top: 327px; left: 111px; height: auto; width: 200px; color: black; font-family:Arial; font-size:medium" 
			                  meta:resourcekey="LBPpalConfiguracionResource1">Configuración
			            </asp:LinkButton>
                <asp:LinkButton ID="LBPpalMantenimiento" runat="server"
                            OnClick="LBPpalMantenimiento_Click"
                            Width="158px" CssClass="reflection"
                            Style="position:absolute; top: 327px; left: 1004px; height: auto; width: 200px; color: black; font-family:Arial; font-size:medium" 
                            meta:resourcekey="LBPpalMantenimientoResource1">Mantenimiento
                        </asp:LinkButton>
                <asp:LinkButton ID="LBWizardConfiguracion" runat="server"
                            Enabled="true" 
                            OnClick="LBWizard_Click"
                            Width="258px" CssClass="reflection"
                            Style="position:absolute; top: 575px; left: 530px; height: auto; width: 200px; color: black; font-family:Arial; font-size:medium" 
                            meta:resourcekey="LBWizardConfiguracionResource1">Asistente de configuración
                        </asp:LinkButton>
        <div style="z-index: 301; left: 15px; width: 1260px; position: absolute; top: 875px; height: 37px; text-align:center; color: black; font-family:Arial; font-size:small" >
            <br\ />
            &#169  Grupo Amper. Copyright @ 2021-2023. Versión 3.0.0 Todos los Derechos Reservados
        </div>
    </form>

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
        setInterval("PingSessionSrv()", 15000);
    </script>
<%--Control actividad de sesión FIN    --%>

</body>
</html>

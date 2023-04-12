<%@ Page Language="C#" AutoEventWireup="true" CodeFile="MaximoSesiones.aspx.cs" Inherits="MaximoSesiones" %>

<!DOCTYPE html>

<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
<meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
    <title></title>
</head>
<body>
    <form id="form1" runat="server">
    <div>
    
    	<asp:Panel ID="Panel1" runat="server" BackColor="#B9D169" BorderColor="Black" 
			 BorderStyle="Solid" BorderWidth="5px" Font-Italic="True" 
			 Font-Names="Lucida Sans Unicode" 
			 
			 style="z-index: -2; left: 128px; top: 215px; position: absolute; height: 206px; width: 1039px" 
			 meta:resourcekey="Panel1Resource1">
			<asp:Label ID="Label1" runat="server" 
				style="position: absolute; z-index: 1; left: 214px; top: 86px; height: 89px; width: 691px;" 
							
				Text="No se puede acceder a la aplicación de configuración. Alcanzado número maximo de sesiones." 
				meta:resourcekey="Label1Resource1"></asp:Label>
			<asp:Panel ID="Panel2" runat="server" BackColor="Transparent" 
				
				style="z-index: 6; left: 11px; top: 6px; position: absolute; height: 47px; width: 1014px" 
				meta:resourcekey="Panel2Resource1">
				<asp:Label ID="Label2" runat="server" ForeColor="White" 
					style="z-index: 1; left: 373px; top: 13px; position: absolute" 
					Text="Aplicación no disponible" meta:resourcekey="Label2Resource1"></asp:Label>
			</asp:Panel>
		   <asp:LinkButton ID="LBReintentar" runat="server" OnClick="LBReintentar_Click"
				style="z-index: 1; left: 908px; top: 170px; position: absolute" 
				PostBackUrl="~/Login.aspx" meta:resourcekey="LBReintentarResource1">Reintentar</asp:LinkButton>
		 </asp:Panel>
    
    </div>3
    </form>
</body>
</html>

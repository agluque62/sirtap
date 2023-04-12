
package es.cd40;

/**
 * Please modify this class to meet your needs
 * This class is not complete
 */

import java.io.File;
import java.net.MalformedURLException;
import java.net.URL;
import javax.xml.namespace.QName;
import javax.jws.WebMethod;
import javax.jws.WebParam;
import javax.jws.WebResult;
import javax.jws.WebService;
import javax.xml.bind.annotation.XmlSeeAlso;
import javax.xml.ws.RequestWrapper;
import javax.xml.ws.ResponseWrapper;

/**
 * This class was generated by Apache CXF 3.2.7
 * 2018-12-10T15:05:38.446+01:00
 * Generated source version: 3.2.7
 *
 */
public final class InterfazSOAPConfiguracionSoap_InterfazSOAPConfiguracionSoap12_Client {

    private static final QName SERVICE_NAME = new QName("http://CD40.es/", "InterfazSOAPConfiguracion");

    private InterfazSOAPConfiguracionSoap_InterfazSOAPConfiguracionSoap12_Client() {
    }

    public static void main(String args[]) throws java.lang.Exception {
        URL wsdlURL = InterfazSOAPConfiguracion.WSDL_LOCATION;
        if (args.length > 0 && args[0] != null && !"".equals(args[0])) {
            File wsdlFile = new File(args[0]);
            try {
                if (wsdlFile.exists()) {
                    wsdlURL = wsdlFile.toURI().toURL();
                } else {
                    wsdlURL = new URL(args[0]);
                }
            } catch (MalformedURLException e) {
                e.printStackTrace();
            }
        }

        InterfazSOAPConfiguracion ss = new InterfazSOAPConfiguracion(wsdlURL, SERVICE_NAME);
        InterfazSOAPConfiguracionSoap port = ss.getInterfazSOAPConfiguracionSoap12();

        {
        System.out.println("Invoking getListaEnlacesExternos...");
        java.lang.String _getListaEnlacesExternos_idSistema = "";
        java.lang.String _getListaEnlacesExternos_idUsuario = "";
        es.cd40.ArrayOfCfgEnlaceExterno _getListaEnlacesExternos__return = port.getListaEnlacesExternos(_getListaEnlacesExternos_idSistema, _getListaEnlacesExternos_idUsuario);
        System.out.println("getListaEnlacesExternos.result=" + _getListaEnlacesExternos__return);


        }
        {
        System.out.println("Invoking getParametrosRecurso...");
        es.cd40.Recursos _getParametrosRecurso_r = null;
        es.cd40.Tablas _getParametrosRecurso__return = port.getParametrosRecurso(_getParametrosRecurso_r);
        System.out.println("getParametrosRecurso.result=" + _getParametrosRecurso__return);


        }
        {
        System.out.println("Invoking getPlanTroncales...");
        java.lang.String _getPlanTroncales_idSistema = "";
        es.cd40.ArrayOfListaTroncales _getPlanTroncales__return = port.getPlanTroncales(_getPlanTroncales_idSistema);
        System.out.println("getPlanTroncales.result=" + _getPlanTroncales__return);


        }
        {
        System.out.println("Invoking getParametrosMulticast...");
        java.lang.String _getParametrosMulticast_idSistema = "";
        es.cd40.ParametrosMulticast _getParametrosMulticast__return = port.getParametrosMulticast(_getParametrosMulticast_idSistema);
        System.out.println("getParametrosMulticast.result=" + _getParametrosMulticast__return);


        }
        {
        System.out.println("Invoking getPlanNumeracionATS...");
        java.lang.String _getPlanNumeracionATS_idSistema = "";
        es.cd40.ArrayOfNumeracionATS _getPlanNumeracionATS__return = port.getPlanNumeracionATS(_getPlanNumeracionATS_idSistema);
        System.out.println("getPlanNumeracionATS.result=" + _getPlanNumeracionATS__return);


        }
        {
        System.out.println("Invoking loginTop...");
        java.lang.String _loginTop_idSistema = "";
        java.lang.String _loginTop_idHw = "";
        es.cd40.LoginTerminalVoz _loginTop__return = port.loginTop(_loginTop_idSistema, _loginTop_idHw);
        System.out.println("loginTop.result=" + _loginTop__return);


        }
        {
        System.out.println("Invoking getParametrosGenerales...");
        java.lang.String _getParametrosGenerales_idSistema = "";
        es.cd40.ParametrosGeneralesSistema _getParametrosGenerales__return = port.getParametrosGenerales(_getParametrosGenerales_idSistema);
        System.out.println("getParametrosGenerales.result=" + _getParametrosGenerales__return);


        }
        {
        System.out.println("Invoking getCfgPasarela...");
        java.lang.String _getCfgPasarela_idSistema = "";
        java.lang.String _getCfgPasarela_idHw = "";
        es.cd40.CfgPasarela _getCfgPasarela__return = port.getCfgPasarela(_getCfgPasarela_idSistema, _getCfgPasarela_idHw);
        System.out.println("getCfgPasarela.result=" + _getCfgPasarela__return);


        }
        {
        System.out.println("Invoking getPlanAsignacionUsuarios...");
        java.lang.String _getPlanAsignacionUsuarios_idSistema = "";
        es.cd40.ArrayOfAsignacionUsuariosTV _getPlanAsignacionUsuarios__return = port.getPlanAsignacionUsuarios(_getPlanAsignacionUsuarios_idSistema);
        System.out.println("getPlanAsignacionUsuarios.result=" + _getPlanAsignacionUsuarios__return);


        }
        {
        System.out.println("Invoking getListaEnlacesInternos...");
        java.lang.String _getListaEnlacesInternos_idSistema = "";
        java.lang.String _getListaEnlacesInternos_idUsuario = "";
        es.cd40.ArrayOfCfgEnlaceInterno _getListaEnlacesInternos__return = port.getListaEnlacesInternos(_getListaEnlacesInternos_idSistema, _getListaEnlacesInternos_idUsuario);
        System.out.println("getListaEnlacesInternos.result=" + _getListaEnlacesInternos__return);


        }
        {
        System.out.println("Invoking getPlanRedes...");
        java.lang.String _getPlanRedes_idSistema = "";
        es.cd40.ArrayOfListaRedes _getPlanRedes__return = port.getPlanRedes(_getPlanRedes_idSistema);
        System.out.println("getPlanRedes.result=" + _getPlanRedes__return);


        }
        {
        System.out.println("Invoking getConfigSistema...");
        java.lang.String _getConfigSistema_idSistema = "";
        es.cd40.ConfiguracionSistema _getConfigSistema__return = port.getConfigSistema(_getConfigSistema_idSistema);
        System.out.println("getConfigSistema.result=" + _getConfigSistema__return);


        }
        {
        System.out.println("Invoking getPoolNMElements...");
        java.lang.String _getPoolNMElements_idSistema = "";
        es.cd40.ArrayOfNode _getPoolNMElements__return = port.getPoolNMElements(_getPoolNMElements_idSistema);
        System.out.println("getPoolNMElements.result=" + _getPoolNMElements__return);


        }
        {
        System.out.println("Invoking getParametrosRecursoById...");
        java.lang.String _getParametrosRecursoById_idSistema = "";
        java.lang.String _getParametrosRecursoById_idRecurso = "";
        int _getParametrosRecursoById_tipo = 0;
        es.cd40.Tablas _getParametrosRecursoById__return = port.getParametrosRecursoById(_getParametrosRecursoById_idSistema, _getParametrosRecursoById_idRecurso, _getParametrosRecursoById_tipo);
        System.out.println("getParametrosRecursoById.result=" + _getParametrosRecursoById__return);


        }
        {
        System.out.println("Invoking getCfgUsuario...");
        java.lang.String _getCfgUsuario_idSistema = "";
        java.lang.String _getCfgUsuario_idUsuario = "";
        es.cd40.CfgUsuario _getCfgUsuario__return = port.getCfgUsuario(_getCfgUsuario_idSistema, _getCfgUsuario_idUsuario);
        System.out.println("getCfgUsuario.result=" + _getCfgUsuario__return);


        }
        {
        System.out.println("Invoking getPlanDireccionamientoIP...");
        java.lang.String _getPlanDireccionamientoIP_idSistema = "";
        es.cd40.ArrayOfDireccionamientoIP _getPlanDireccionamientoIP__return = port.getPlanDireccionamientoIP(_getPlanDireccionamientoIP_idSistema);
        System.out.println("getPlanDireccionamientoIP.result=" + _getPlanDireccionamientoIP__return);


        }
        {
        System.out.println("Invoking getVersionConfiguracionXML...");
        java.lang.String _getVersionConfiguracionXML_idSistema = "";
        port.getVersionConfiguracionXML(_getVersionConfiguracionXML_idSistema);


        }
        {
        System.out.println("Invoking getEstadoCluster...");
        es.cd40.EstadoCluster _getEstadoCluster__return = port.getEstadoCluster();
        System.out.println("getEstadoCluster.result=" + _getEstadoCluster__return);


        }
        {
        System.out.println("Invoking getPoolHfElement...");
        java.lang.String _getPoolHfElement_idSistema = "";
        es.cd40.ArrayOfPoolHfElement _getPoolHfElement__return = port.getPoolHfElement(_getPoolHfElement_idSistema);
        System.out.println("getPoolHfElement.result=" + _getPoolHfElement__return);


        }
        {
        System.out.println("Invoking getPlanAsignacionRecursos...");
        java.lang.String _getPlanAsignacionRecursos_idSistema = "";
        es.cd40.ArrayOfAsignacionRecursosGW _getPlanAsignacionRecursos__return = port.getPlanAsignacionRecursos(_getPlanAsignacionRecursos_idSistema);
        System.out.println("getPlanAsignacionRecursos.result=" + _getPlanAsignacionRecursos__return);


        }
        {
        System.out.println("Invoking getVersionConfiguracion...");
        java.lang.String _getVersionConfiguracion_idSistema = "";
        java.lang.String _getVersionConfiguracion__return = port.getVersionConfiguracion(_getVersionConfiguracion_idSistema);
        System.out.println("getVersionConfiguracion.result=" + _getVersionConfiguracion__return);


        }
        {
        System.out.println("Invoking getPlanDireccionamientoSIP...");
        java.lang.String _getPlanDireccionamientoSIP_idSistema = "";
        es.cd40.ArrayOfDireccionamientoSIP _getPlanDireccionamientoSIP__return = port.getPlanDireccionamientoSIP(_getPlanDireccionamientoSIP_idSistema);
        System.out.println("getPlanDireccionamientoSIP.result=" + _getPlanDireccionamientoSIP__return);


        }
        {
        System.out.println("Invoking getScvActivo...");
        java.lang.String _getScvActivo_idSistema = "";
        java.lang.String _getScvActivo__return = port.getScvActivo(_getScvActivo_idSistema);
        System.out.println("getScvActivo.result=" + _getScvActivo__return);


        }

        System.exit(0);
    }

}

package es.cd40;

import javax.jws.WebMethod;
import javax.jws.WebParam;
import javax.jws.WebResult;
import javax.jws.WebService;
import javax.xml.bind.annotation.XmlSeeAlso;
import javax.xml.ws.RequestWrapper;
import javax.xml.ws.ResponseWrapper;

/**
 * This class was generated by Apache CXF 3.2.7
 * 2023-05-17T15:26:38.045+02:00
 * Generated source version: 3.2.7
 *
 */
@WebService(targetNamespace = "http://CD40.es/", name = "InterfazSOAPConfiguracionSoap")
@XmlSeeAlso({ObjectFactory.class})
public interface InterfazSOAPConfiguracionSoap {

    /**
     * Pasándole el identificador de usuario y el sistema al que
     * 				pertenece, devuelve la lista de enlaces externos del usuario.
     * 			
     */
    @WebMethod(operationName = "GetListaEnlacesExternos", action = "http://CD40.es/GetListaEnlacesExternos")
    @RequestWrapper(localName = "GetListaEnlacesExternos", targetNamespace = "http://CD40.es/", className = "es.cd40.GetListaEnlacesExternos")
    @ResponseWrapper(localName = "GetListaEnlacesExternosResponse", targetNamespace = "http://CD40.es/", className = "es.cd40.GetListaEnlacesExternosResponse")
    @WebResult(name = "GetListaEnlacesExternosResult", targetNamespace = "http://CD40.es/")
    public es.cd40.ArrayOfCfgEnlaceExterno getListaEnlacesExternos(
        @WebParam(name = "id_sistema", targetNamespace = "http://CD40.es/")
        java.lang.String idSistema,
        @WebParam(name = "id_usuario", targetNamespace = "http://CD40.es/")
        java.lang.String idUsuario
    );

    /**
     * Pasándole un recurso, retorna los parámetros
     * 				correspondientes.
     */
    @WebMethod(operationName = "GetParametrosRecurso", action = "http://CD40.es/GetParametrosRecurso")
    @RequestWrapper(localName = "GetParametrosRecurso", targetNamespace = "http://CD40.es/", className = "es.cd40.GetParametrosRecurso")
    @ResponseWrapper(localName = "GetParametrosRecursoResponse", targetNamespace = "http://CD40.es/", className = "es.cd40.GetParametrosRecursoResponse")
    @WebResult(name = "GetParametrosRecursoResult", targetNamespace = "http://CD40.es/")
    public es.cd40.Tablas getParametrosRecurso(
        @WebParam(name = "r", targetNamespace = "http://CD40.es/")
        es.cd40.Recursos r
    );

    /**
     * Pasándole el identificador del sistema, proporciona la
     * 				dirección multicast y el puerto para notificación de una activación
     * 				de configuración.
     */
    @WebMethod(operationName = "GetParametrosMulticast", action = "http://CD40.es/GetParametrosMulticast")
    @RequestWrapper(localName = "GetParametrosMulticast", targetNamespace = "http://CD40.es/", className = "es.cd40.GetParametrosMulticast")
    @ResponseWrapper(localName = "GetParametrosMulticastResponse", targetNamespace = "http://CD40.es/", className = "es.cd40.GetParametrosMulticastResponse")
    @WebResult(name = "GetParametrosMulticastResult", targetNamespace = "http://CD40.es/")
    public es.cd40.ParametrosMulticast getParametrosMulticast(
        @WebParam(name = "id_sistema", targetNamespace = "http://CD40.es/")
        java.lang.String idSistema
    );

    /**
     * Pasándole el identificador hardware del terminal de voz y
     * 				el sistema al que pertenece, devuelve el identificador de usuario
     * 				alojado en el mismo y el modo de arranque ('A' o 'M').
     * 			
     */
    @WebMethod(operationName = "LoginTop", action = "http://CD40.es/LoginTop")
    @RequestWrapper(localName = "LoginTop", targetNamespace = "http://CD40.es/", className = "es.cd40.LoginTop")
    @ResponseWrapper(localName = "LoginTopResponse", targetNamespace = "http://CD40.es/", className = "es.cd40.LoginTopResponse")
    @WebResult(name = "LoginTopResult", targetNamespace = "http://CD40.es/")
    public es.cd40.LoginTerminalVoz loginTop(
        @WebParam(name = "id_sistema", targetNamespace = "http://CD40.es/")
        java.lang.String idSistema,
        @WebParam(name = "id_hw", targetNamespace = "http://CD40.es/")
        java.lang.String idHw
    );

    /**
     * Pasándole el identificador de sistema, devuelve las
     * 				conferencias preprogramadas.
     */
    @WebMethod(operationName = "GetConferenciasPreprogramadas", action = "http://CD40.es/GetConferenciasPreprogramadas")
    @RequestWrapper(localName = "GetConferenciasPreprogramadas", targetNamespace = "http://CD40.es/", className = "es.cd40.GetConferenciasPreprogramadas")
    @ResponseWrapper(localName = "GetConferenciasPreprogramadasResponse", targetNamespace = "http://CD40.es/", className = "es.cd40.GetConferenciasPreprogramadasResponse")
    @WebResult(name = "GetConferenciasPreprogramadasResult", targetNamespace = "http://CD40.es/")
    public es.cd40.ConferenciasPreprogramadas getConferenciasPreprogramadas(
        @WebParam(name = "id_sistema", targetNamespace = "http://CD40.es/")
        java.lang.String idSistema
    );

    /**
     * Pasándole el identificador hardware de la pasarela y el
     * 				sistema al que pertenece, devuelve la configuración de la pasarela.
     * 			
     */
    @WebMethod(operationName = "GetCfgPasarela", action = "http://CD40.es/GetCfgPasarela")
    @RequestWrapper(localName = "GetCfgPasarela", targetNamespace = "http://CD40.es/", className = "es.cd40.GetCfgPasarela")
    @ResponseWrapper(localName = "GetCfgPasarelaResponse", targetNamespace = "http://CD40.es/", className = "es.cd40.GetCfgPasarelaResponse")
    @WebResult(name = "GetCfgPasarelaResult", targetNamespace = "http://CD40.es/")
    public es.cd40.CfgPasarela getCfgPasarela(
        @WebParam(name = "id_sistema", targetNamespace = "http://CD40.es/")
        java.lang.String idSistema,
        @WebParam(name = "id_hw", targetNamespace = "http://CD40.es/")
        java.lang.String idHw
    );

    /**
     * Pasándole el identificador del sistema devuelve el plan
     * 				de asignación de usuarios a terminales de voz.
     */
    @WebMethod(operationName = "GetPlanAsignacionUsuarios", action = "http://CD40.es/GetPlanAsignacionUsuarios")
    @RequestWrapper(localName = "GetPlanAsignacionUsuarios", targetNamespace = "http://CD40.es/", className = "es.cd40.GetPlanAsignacionUsuarios")
    @ResponseWrapper(localName = "GetPlanAsignacionUsuariosResponse", targetNamespace = "http://CD40.es/", className = "es.cd40.GetPlanAsignacionUsuariosResponse")
    @WebResult(name = "GetPlanAsignacionUsuariosResult", targetNamespace = "http://CD40.es/")
    public es.cd40.ArrayOfAsignacionUsuariosTV getPlanAsignacionUsuarios(
        @WebParam(name = "id_sistema", targetNamespace = "http://CD40.es/")
        java.lang.String idSistema
    );

    /**
     * Pasándole el identificador de usuario y el sistema al que
     * 				pertenece, devuelve la configuración del usuario.
     * 			
     */
    @WebMethod(operationName = "GetCfgUsuario", action = "http://CD40.es/GetCfgUsuario")
    @RequestWrapper(localName = "GetCfgUsuario", targetNamespace = "http://CD40.es/", className = "es.cd40.GetCfgUsuario")
    @ResponseWrapper(localName = "GetCfgUsuarioResponse", targetNamespace = "http://CD40.es/", className = "es.cd40.GetCfgUsuarioResponse")
    @WebResult(name = "GetCfgUsuarioResult", targetNamespace = "http://CD40.es/")
    public es.cd40.CfgUsuario getCfgUsuario(
        @WebParam(name = "id_sistema", targetNamespace = "http://CD40.es/")
        java.lang.String idSistema,
        @WebParam(name = "id_usuario", targetNamespace = "http://CD40.es/")
        java.lang.String idUsuario
    );

    /**
     * Pasándole el identificador de sistema, devuelve la
     * 				versión de la configuración activa registrada en la base de datos
     * 			
     */
    @WebMethod(operationName = "GetVersionConfiguracion_XML", action = "http://CD40.es/GetVersionConfiguracion_XML")
    @RequestWrapper(localName = "GetVersionConfiguracion_XML", targetNamespace = "http://CD40.es/", className = "es.cd40.GetVersionConfiguracionXML")
    @ResponseWrapper(localName = "GetVersionConfiguracion_XMLResponse", targetNamespace = "http://CD40.es/", className = "es.cd40.GetVersionConfiguracionXMLResponse")
    public void getVersionConfiguracionXML(
        @WebParam(name = "id_sistema", targetNamespace = "http://CD40.es/")
        java.lang.String idSistema
    );

    /**
     * Envia el estado de los nodos que componen el cluster
     * 			
     */
    @WebMethod(operationName = "GetEstadoCluster", action = "http://CD40.es/GetEstadoCluster")
    @RequestWrapper(localName = "GetEstadoCluster", targetNamespace = "http://CD40.es/", className = "es.cd40.GetEstadoCluster")
    @ResponseWrapper(localName = "GetEstadoClusterResponse", targetNamespace = "http://CD40.es/", className = "es.cd40.GetEstadoClusterResponse")
    @WebResult(name = "GetEstadoClusterResult", targetNamespace = "http://CD40.es/")
    public es.cd40.EstadoCluster getEstadoCluster();

    /**
     * Configuración relativa a frecuencias HF Ulises5Ki.
     * 			
     */
    @WebMethod(operationName = "GetPoolHfElement", action = "http://CD40.es/GetPoolHfElement")
    @RequestWrapper(localName = "GetPoolHfElement", targetNamespace = "http://CD40.es/", className = "es.cd40.GetPoolHfElement")
    @ResponseWrapper(localName = "GetPoolHfElementResponse", targetNamespace = "http://CD40.es/", className = "es.cd40.GetPoolHfElementResponse")
    @WebResult(name = "GetPoolHfElementResult", targetNamespace = "http://CD40.es/")
    public es.cd40.ArrayOfPoolHfElement getPoolHfElement(
        @WebParam(name = "id_sistema", targetNamespace = "http://CD40.es/")
        java.lang.String idSistema
    );

    /**
     * Pasándole el identificador de sistema, devuelve la
     * 				versión de la configuración activa registrada en la base de datos
     * 			
     */
    @WebMethod(operationName = "GetVersionConfiguracion", action = "http://CD40.es/GetVersionConfiguracion")
    @RequestWrapper(localName = "GetVersionConfiguracion", targetNamespace = "http://CD40.es/", className = "es.cd40.GetVersionConfiguracion")
    @ResponseWrapper(localName = "GetVersionConfiguracionResponse", targetNamespace = "http://CD40.es/", className = "es.cd40.GetVersionConfiguracionResponse")
    @WebResult(name = "GetVersionConfiguracionResult", targetNamespace = "http://CD40.es/")
    public java.lang.String getVersionConfiguracion(
        @WebParam(name = "id_sistema", targetNamespace = "http://CD40.es/")
        java.lang.String idSistema
    );

    /**
     * Pasándole el identificador del sistema devuelve el plan
     * 				de direccionamiento SIP.
     */
    @WebMethod(operationName = "GetPlanDireccionamientoSIP", action = "http://CD40.es/GetPlanDireccionamientoSIP")
    @RequestWrapper(localName = "GetPlanDireccionamientoSIP", targetNamespace = "http://CD40.es/", className = "es.cd40.GetPlanDireccionamientoSIP")
    @ResponseWrapper(localName = "GetPlanDireccionamientoSIPResponse", targetNamespace = "http://CD40.es/", className = "es.cd40.GetPlanDireccionamientoSIPResponse")
    @WebResult(name = "GetPlanDireccionamientoSIPResult", targetNamespace = "http://CD40.es/")
    public es.cd40.ArrayOfDireccionamientoSIP getPlanDireccionamientoSIP(
        @WebParam(name = "id_sistema", targetNamespace = "http://CD40.es/")
        java.lang.String idSistema
    );

    /**
     * Pasándole el identificador de sistema, devuelve el Scv
     * 				[1|2] activo. 0 si error
     */
    @WebMethod(operationName = "GetScvActivo", action = "http://CD40.es/GetScvActivo")
    @RequestWrapper(localName = "GetScvActivo", targetNamespace = "http://CD40.es/", className = "es.cd40.GetScvActivo")
    @ResponseWrapper(localName = "GetScvActivoResponse", targetNamespace = "http://CD40.es/", className = "es.cd40.GetScvActivoResponse")
    @WebResult(name = "GetScvActivoResult", targetNamespace = "http://CD40.es/")
    public java.lang.String getScvActivo(
        @WebParam(name = "idSistema", targetNamespace = "http://CD40.es/")
        java.lang.String idSistema
    );

    /**
     * Pasándole el identificador del sistema devuelve el plan
     * 				de troncales.
     */
    @WebMethod(operationName = "GetPlanTroncales", action = "http://CD40.es/GetPlanTroncales")
    @RequestWrapper(localName = "GetPlanTroncales", targetNamespace = "http://CD40.es/", className = "es.cd40.GetPlanTroncales")
    @ResponseWrapper(localName = "GetPlanTroncalesResponse", targetNamespace = "http://CD40.es/", className = "es.cd40.GetPlanTroncalesResponse")
    @WebResult(name = "GetPlanTroncalesResult", targetNamespace = "http://CD40.es/")
    public es.cd40.ArrayOfListaTroncales getPlanTroncales(
        @WebParam(name = "id_sistema", targetNamespace = "http://CD40.es/")
        java.lang.String idSistema
    );

    /**
     * Pasándole el identificador del sistema devuelve el plan
     * 				de numeración ATS.
     */
    @WebMethod(operationName = "GetPlanNumeracionATS", action = "http://CD40.es/GetPlanNumeracionATS")
    @RequestWrapper(localName = "GetPlanNumeracionATS", targetNamespace = "http://CD40.es/", className = "es.cd40.GetPlanNumeracionATS")
    @ResponseWrapper(localName = "GetPlanNumeracionATSResponse", targetNamespace = "http://CD40.es/", className = "es.cd40.GetPlanNumeracionATSResponse")
    @WebResult(name = "GetPlanNumeracionATSResult", targetNamespace = "http://CD40.es/")
    public es.cd40.ArrayOfNumeracionATS getPlanNumeracionATS(
        @WebParam(name = "id_sistema", targetNamespace = "http://CD40.es/")
        java.lang.String idSistema
    );

    /**
     * Pasándole el identificador del sistema devuelve los
     * 				parámetros generales del sistema.
     */
    @WebMethod(operationName = "GetParametrosGenerales", action = "http://CD40.es/GetParametrosGenerales")
    @RequestWrapper(localName = "GetParametrosGenerales", targetNamespace = "http://CD40.es/", className = "es.cd40.GetParametrosGenerales")
    @ResponseWrapper(localName = "GetParametrosGeneralesResponse", targetNamespace = "http://CD40.es/", className = "es.cd40.GetParametrosGeneralesResponse")
    @WebResult(name = "GetParametrosGeneralesResult", targetNamespace = "http://CD40.es/")
    public es.cd40.ParametrosGeneralesSistema getParametrosGenerales(
        @WebParam(name = "id_sistema", targetNamespace = "http://CD40.es/")
        java.lang.String idSistema
    );

    /**
     * Pasándole el identificador de usuario y el sistema al que
     * 				pertenece, devuelve la lista de enlaces internos del usuario.
     * 			
     */
    @WebMethod(operationName = "GetListaEnlacesInternos", action = "http://CD40.es/GetListaEnlacesInternos")
    @RequestWrapper(localName = "GetListaEnlacesInternos", targetNamespace = "http://CD40.es/", className = "es.cd40.GetListaEnlacesInternos")
    @ResponseWrapper(localName = "GetListaEnlacesInternosResponse", targetNamespace = "http://CD40.es/", className = "es.cd40.GetListaEnlacesInternosResponse")
    @WebResult(name = "GetListaEnlacesInternosResult", targetNamespace = "http://CD40.es/")
    public es.cd40.ArrayOfCfgEnlaceInterno getListaEnlacesInternos(
        @WebParam(name = "id_sistema", targetNamespace = "http://CD40.es/")
        java.lang.String idSistema,
        @WebParam(name = "id_usuario", targetNamespace = "http://CD40.es/")
        java.lang.String idUsuario
    );

    /**
     * Pasándole el identificador del sistema devuelve el plan
     * 				de redes.
     */
    @WebMethod(operationName = "GetPlanRedes", action = "http://CD40.es/GetPlanRedes")
    @RequestWrapper(localName = "GetPlanRedes", targetNamespace = "http://CD40.es/", className = "es.cd40.GetPlanRedes")
    @ResponseWrapper(localName = "GetPlanRedesResponse", targetNamespace = "http://CD40.es/", className = "es.cd40.GetPlanRedesResponse")
    @WebResult(name = "GetPlanRedesResult", targetNamespace = "http://CD40.es/")
    public es.cd40.ArrayOfListaRedes getPlanRedes(
        @WebParam(name = "id_sistema", targetNamespace = "http://CD40.es/")
        java.lang.String idSistema
    );

    /**
     * Pasándole el identificador del sistema devuelve el
     * 				conjunto de parámetros que conforman la configuración común de los
     * 				distintos elementos del SCV.
     */
    @WebMethod(operationName = "GetConfigSistema", action = "http://CD40.es/GetConfigSistema")
    @RequestWrapper(localName = "GetConfigSistema", targetNamespace = "http://CD40.es/", className = "es.cd40.GetConfigSistema")
    @ResponseWrapper(localName = "GetConfigSistemaResponse", targetNamespace = "http://CD40.es/", className = "es.cd40.GetConfigSistemaResponse")
    @WebResult(name = "GetConfigSistemaResult", targetNamespace = "http://CD40.es/")
    public es.cd40.ConfiguracionSistema getConfigSistema(
        @WebParam(name = "id_sistema", targetNamespace = "http://CD40.es/")
        java.lang.String idSistema
    );

    /**
     * Configuración relativa a los equipos N+M (tipo=0) o Audio
     * 				EE (tipo=1).
     */
    @WebMethod(operationName = "GetPoolNMElements", action = "http://CD40.es/GetPoolNMElements")
    @RequestWrapper(localName = "GetPoolNMElements", targetNamespace = "http://CD40.es/", className = "es.cd40.GetPoolNMElements")
    @ResponseWrapper(localName = "GetPoolNMElementsResponse", targetNamespace = "http://CD40.es/", className = "es.cd40.GetPoolNMElementsResponse")
    @WebResult(name = "GetPoolNMElementsResult", targetNamespace = "http://CD40.es/")
    public es.cd40.ArrayOfNode getPoolNMElements(
        @WebParam(name = "id_sistema", targetNamespace = "http://CD40.es/")
        java.lang.String idSistema,
        @WebParam(name = "tipo", targetNamespace = "http://CD40.es/")
        java.lang.String tipo
    );

    /**
     * Pasándole un identificador de sistema, un identificador
     * 				de recurso y el tipo de interfaz (TI_Radio=0, TI_LCEN=1, TI_BC=2,
     * 				TI_BL=3, TI_AB=4, TI_ATS_R2=5, TI_ATS_N5=6, TI_ATS_QSIG=7,
     * 				TI_ISDN_2BD=8, TI_ISDN_30BD=9, TI_I_O=10, TI_DATOS=11), retorna los
     * 				parámetros correspondientes al recurso.
     */
    @WebMethod(operationName = "GetParametrosRecursoById", action = "http://CD40.es/GetParametrosRecursoById")
    @RequestWrapper(localName = "GetParametrosRecursoById", targetNamespace = "http://CD40.es/", className = "es.cd40.GetParametrosRecursoById")
    @ResponseWrapper(localName = "GetParametrosRecursoByIdResponse", targetNamespace = "http://CD40.es/", className = "es.cd40.GetParametrosRecursoByIdResponse")
    @WebResult(name = "GetParametrosRecursoByIdResult", targetNamespace = "http://CD40.es/")
    public es.cd40.Tablas getParametrosRecursoById(
        @WebParam(name = "idSistema", targetNamespace = "http://CD40.es/")
        java.lang.String idSistema,
        @WebParam(name = "idRecurso", targetNamespace = "http://CD40.es/")
        java.lang.String idRecurso,
        @WebParam(name = "tipo", targetNamespace = "http://CD40.es/")
        int tipo
    );

    /**
     * Pasándole el identificador del sistema devuelve el plan
     * 				de direccionamiento IP.
     */
    @WebMethod(operationName = "GetPlanDireccionamientoIP", action = "http://CD40.es/GetPlanDireccionamientoIP")
    @RequestWrapper(localName = "GetPlanDireccionamientoIP", targetNamespace = "http://CD40.es/", className = "es.cd40.GetPlanDireccionamientoIP")
    @ResponseWrapper(localName = "GetPlanDireccionamientoIPResponse", targetNamespace = "http://CD40.es/", className = "es.cd40.GetPlanDireccionamientoIPResponse")
    @WebResult(name = "GetPlanDireccionamientoIPResult", targetNamespace = "http://CD40.es/")
    public es.cd40.ArrayOfDireccionamientoIP getPlanDireccionamientoIP(
        @WebParam(name = "id_sistema", targetNamespace = "http://CD40.es/")
        java.lang.String idSistema
    );

    /**
     * Pasándole el identificador del sistema devuelve el plan
     * 				de asignación de recursos a pasarelas.
     */
    @WebMethod(operationName = "GetPlanAsignacionRecursos", action = "http://CD40.es/GetPlanAsignacionRecursos")
    @RequestWrapper(localName = "GetPlanAsignacionRecursos", targetNamespace = "http://CD40.es/", className = "es.cd40.GetPlanAsignacionRecursos")
    @ResponseWrapper(localName = "GetPlanAsignacionRecursosResponse", targetNamespace = "http://CD40.es/", className = "es.cd40.GetPlanAsignacionRecursosResponse")
    @WebResult(name = "GetPlanAsignacionRecursosResult", targetNamespace = "http://CD40.es/")
    public es.cd40.ArrayOfAsignacionRecursosGW getPlanAsignacionRecursos(
        @WebParam(name = "id_sistema", targetNamespace = "http://CD40.es/")
        java.lang.String idSistema
    );
}

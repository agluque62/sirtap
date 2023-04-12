
package es.cd40;

import javax.xml.bind.JAXBElement;
import javax.xml.bind.annotation.XmlElementDecl;
import javax.xml.bind.annotation.XmlRegistry;
import javax.xml.namespace.QName;


/**
 * This object contains factory methods for each 
 * Java content interface and Java element interface 
 * generated in the es.cd40 package. 
 * <p>An ObjectFactory allows you to programatically 
 * construct new instances of the Java representation 
 * for XML content. The Java representation of XML 
 * content can consist of schema derived interfaces 
 * and classes representing the binding of schema 
 * type definitions, element declarations and model 
 * groups.  Factory methods for each of these are 
 * provided in this class.
 * 
 */
@XmlRegistry
public class ObjectFactory {

    private final static QName _String_QNAME = new QName("http://CD40.es/", "string");
    private final static QName _EstadoCluster_QNAME = new QName("http://CD40.es/", "EstadoCluster");
    private final static QName _ArrayOfCfgEnlaceExterno_QNAME = new QName("http://CD40.es/", "ArrayOfCfgEnlaceExterno");
    private final static QName _ArrayOfCfgEnlaceInterno_QNAME = new QName("http://CD40.es/", "ArrayOfCfgEnlaceInterno");
    private final static QName _LoginTerminalVoz_QNAME = new QName("http://CD40.es/", "LoginTerminalVoz");
    private final static QName _CfgUsuario_QNAME = new QName("http://CD40.es/", "CfgUsuario");
    private final static QName _CfgPasarela_QNAME = new QName("http://CD40.es/", "CfgPasarela");
    private final static QName _Tablas_QNAME = new QName("http://CD40.es/", "Tablas");
    private final static QName _ConfiguracionSistema_QNAME = new QName("http://CD40.es/", "ConfiguracionSistema");
    private final static QName _ParametrosGeneralesSistema_QNAME = new QName("http://CD40.es/", "ParametrosGeneralesSistema");
    private final static QName _ArrayOfNumeracionATS_QNAME = new QName("http://CD40.es/", "ArrayOfNumeracionATS");
    private final static QName _ArrayOfDireccionamientoIP_QNAME = new QName("http://CD40.es/", "ArrayOfDireccionamientoIP");
    private final static QName _ArrayOfListaTroncales_QNAME = new QName("http://CD40.es/", "ArrayOfListaTroncales");
    private final static QName _ArrayOfListaRedes_QNAME = new QName("http://CD40.es/", "ArrayOfListaRedes");
    private final static QName _ArrayOfAsignacionUsuariosTV_QNAME = new QName("http://CD40.es/", "ArrayOfAsignacionUsuariosTV");
    private final static QName _ArrayOfAsignacionRecursosGW_QNAME = new QName("http://CD40.es/", "ArrayOfAsignacionRecursosGW");
    private final static QName _ArrayOfDireccionamientoSIP_QNAME = new QName("http://CD40.es/", "ArrayOfDireccionamientoSIP");
    private final static QName _ParametrosMulticast_QNAME = new QName("http://CD40.es/", "ParametrosMulticast");
    private final static QName _ArrayOfPoolHfElement_QNAME = new QName("http://CD40.es/", "ArrayOfPoolHfElement");
    private final static QName _ArrayOfNode_QNAME = new QName("http://CD40.es/", "ArrayOfNode");

    /**
     * Create a new ObjectFactory that can be used to create new instances of schema derived classes for package: es.cd40
     * 
     */
    public ObjectFactory() {
    }

    /**
     * Create an instance of {@link GetVersionConfiguracionXML }
     * 
     */
    public GetVersionConfiguracionXML createGetVersionConfiguracionXML() {
        return new GetVersionConfiguracionXML();
    }

    /**
     * Create an instance of {@link GetVersionConfiguracionXMLResponse }
     * 
     */
    public GetVersionConfiguracionXMLResponse createGetVersionConfiguracionXMLResponse() {
        return new GetVersionConfiguracionXMLResponse();
    }

    /**
     * Create an instance of {@link GetVersionConfiguracion }
     * 
     */
    public GetVersionConfiguracion createGetVersionConfiguracion() {
        return new GetVersionConfiguracion();
    }

    /**
     * Create an instance of {@link GetVersionConfiguracionResponse }
     * 
     */
    public GetVersionConfiguracionResponse createGetVersionConfiguracionResponse() {
        return new GetVersionConfiguracionResponse();
    }

    /**
     * Create an instance of {@link GetScvActivo }
     * 
     */
    public GetScvActivo createGetScvActivo() {
        return new GetScvActivo();
    }

    /**
     * Create an instance of {@link GetScvActivoResponse }
     * 
     */
    public GetScvActivoResponse createGetScvActivoResponse() {
        return new GetScvActivoResponse();
    }

    /**
     * Create an instance of {@link GetEstadoCluster }
     * 
     */
    public GetEstadoCluster createGetEstadoCluster() {
        return new GetEstadoCluster();
    }

    /**
     * Create an instance of {@link GetEstadoClusterResponse }
     * 
     */
    public GetEstadoClusterResponse createGetEstadoClusterResponse() {
        return new GetEstadoClusterResponse();
    }

    /**
     * Create an instance of {@link EstadoCluster }
     * 
     */
    public EstadoCluster createEstadoCluster() {
        return new EstadoCluster();
    }

    /**
     * Create an instance of {@link GetListaEnlacesExternos }
     * 
     */
    public GetListaEnlacesExternos createGetListaEnlacesExternos() {
        return new GetListaEnlacesExternos();
    }

    /**
     * Create an instance of {@link GetListaEnlacesExternosResponse }
     * 
     */
    public GetListaEnlacesExternosResponse createGetListaEnlacesExternosResponse() {
        return new GetListaEnlacesExternosResponse();
    }

    /**
     * Create an instance of {@link ArrayOfCfgEnlaceExterno }
     * 
     */
    public ArrayOfCfgEnlaceExterno createArrayOfCfgEnlaceExterno() {
        return new ArrayOfCfgEnlaceExterno();
    }

    /**
     * Create an instance of {@link GetListaEnlacesInternos }
     * 
     */
    public GetListaEnlacesInternos createGetListaEnlacesInternos() {
        return new GetListaEnlacesInternos();
    }

    /**
     * Create an instance of {@link GetListaEnlacesInternosResponse }
     * 
     */
    public GetListaEnlacesInternosResponse createGetListaEnlacesInternosResponse() {
        return new GetListaEnlacesInternosResponse();
    }

    /**
     * Create an instance of {@link ArrayOfCfgEnlaceInterno }
     * 
     */
    public ArrayOfCfgEnlaceInterno createArrayOfCfgEnlaceInterno() {
        return new ArrayOfCfgEnlaceInterno();
    }

    /**
     * Create an instance of {@link LoginTop }
     * 
     */
    public LoginTop createLoginTop() {
        return new LoginTop();
    }

    /**
     * Create an instance of {@link LoginTopResponse }
     * 
     */
    public LoginTopResponse createLoginTopResponse() {
        return new LoginTopResponse();
    }

    /**
     * Create an instance of {@link LoginTerminalVoz }
     * 
     */
    public LoginTerminalVoz createLoginTerminalVoz() {
        return new LoginTerminalVoz();
    }

    /**
     * Create an instance of {@link GetCfgUsuario }
     * 
     */
    public GetCfgUsuario createGetCfgUsuario() {
        return new GetCfgUsuario();
    }

    /**
     * Create an instance of {@link GetCfgUsuarioResponse }
     * 
     */
    public GetCfgUsuarioResponse createGetCfgUsuarioResponse() {
        return new GetCfgUsuarioResponse();
    }

    /**
     * Create an instance of {@link CfgUsuario }
     * 
     */
    public CfgUsuario createCfgUsuario() {
        return new CfgUsuario();
    }

    /**
     * Create an instance of {@link GetCfgPasarela }
     * 
     */
    public GetCfgPasarela createGetCfgPasarela() {
        return new GetCfgPasarela();
    }

    /**
     * Create an instance of {@link GetCfgPasarelaResponse }
     * 
     */
    public GetCfgPasarelaResponse createGetCfgPasarelaResponse() {
        return new GetCfgPasarelaResponse();
    }

    /**
     * Create an instance of {@link CfgPasarela }
     * 
     */
    public CfgPasarela createCfgPasarela() {
        return new CfgPasarela();
    }

    /**
     * Create an instance of {@link GetParametrosRecursoById }
     * 
     */
    public GetParametrosRecursoById createGetParametrosRecursoById() {
        return new GetParametrosRecursoById();
    }

    /**
     * Create an instance of {@link GetParametrosRecursoByIdResponse }
     * 
     */
    public GetParametrosRecursoByIdResponse createGetParametrosRecursoByIdResponse() {
        return new GetParametrosRecursoByIdResponse();
    }

    /**
     * Create an instance of {@link GetParametrosRecurso }
     * 
     */
    public GetParametrosRecurso createGetParametrosRecurso() {
        return new GetParametrosRecurso();
    }

    /**
     * Create an instance of {@link Recursos }
     * 
     */
    public Recursos createRecursos() {
        return new Recursos();
    }

    /**
     * Create an instance of {@link GetParametrosRecursoResponse }
     * 
     */
    public GetParametrosRecursoResponse createGetParametrosRecursoResponse() {
        return new GetParametrosRecursoResponse();
    }

    /**
     * Create an instance of {@link GetConfigSistema }
     * 
     */
    public GetConfigSistema createGetConfigSistema() {
        return new GetConfigSistema();
    }

    /**
     * Create an instance of {@link GetConfigSistemaResponse }
     * 
     */
    public GetConfigSistemaResponse createGetConfigSistemaResponse() {
        return new GetConfigSistemaResponse();
    }

    /**
     * Create an instance of {@link ConfiguracionSistema }
     * 
     */
    public ConfiguracionSistema createConfiguracionSistema() {
        return new ConfiguracionSistema();
    }

    /**
     * Create an instance of {@link GetParametrosGenerales }
     * 
     */
    public GetParametrosGenerales createGetParametrosGenerales() {
        return new GetParametrosGenerales();
    }

    /**
     * Create an instance of {@link GetParametrosGeneralesResponse }
     * 
     */
    public GetParametrosGeneralesResponse createGetParametrosGeneralesResponse() {
        return new GetParametrosGeneralesResponse();
    }

    /**
     * Create an instance of {@link ParametrosGeneralesSistema }
     * 
     */
    public ParametrosGeneralesSistema createParametrosGeneralesSistema() {
        return new ParametrosGeneralesSistema();
    }

    /**
     * Create an instance of {@link GetPlanNumeracionATS }
     * 
     */
    public GetPlanNumeracionATS createGetPlanNumeracionATS() {
        return new GetPlanNumeracionATS();
    }

    /**
     * Create an instance of {@link GetPlanNumeracionATSResponse }
     * 
     */
    public GetPlanNumeracionATSResponse createGetPlanNumeracionATSResponse() {
        return new GetPlanNumeracionATSResponse();
    }

    /**
     * Create an instance of {@link ArrayOfNumeracionATS }
     * 
     */
    public ArrayOfNumeracionATS createArrayOfNumeracionATS() {
        return new ArrayOfNumeracionATS();
    }

    /**
     * Create an instance of {@link GetPlanDireccionamientoIP }
     * 
     */
    public GetPlanDireccionamientoIP createGetPlanDireccionamientoIP() {
        return new GetPlanDireccionamientoIP();
    }

    /**
     * Create an instance of {@link GetPlanDireccionamientoIPResponse }
     * 
     */
    public GetPlanDireccionamientoIPResponse createGetPlanDireccionamientoIPResponse() {
        return new GetPlanDireccionamientoIPResponse();
    }

    /**
     * Create an instance of {@link ArrayOfDireccionamientoIP }
     * 
     */
    public ArrayOfDireccionamientoIP createArrayOfDireccionamientoIP() {
        return new ArrayOfDireccionamientoIP();
    }

    /**
     * Create an instance of {@link GetPlanTroncales }
     * 
     */
    public GetPlanTroncales createGetPlanTroncales() {
        return new GetPlanTroncales();
    }

    /**
     * Create an instance of {@link GetPlanTroncalesResponse }
     * 
     */
    public GetPlanTroncalesResponse createGetPlanTroncalesResponse() {
        return new GetPlanTroncalesResponse();
    }

    /**
     * Create an instance of {@link ArrayOfListaTroncales }
     * 
     */
    public ArrayOfListaTroncales createArrayOfListaTroncales() {
        return new ArrayOfListaTroncales();
    }

    /**
     * Create an instance of {@link GetPlanRedes }
     * 
     */
    public GetPlanRedes createGetPlanRedes() {
        return new GetPlanRedes();
    }

    /**
     * Create an instance of {@link GetPlanRedesResponse }
     * 
     */
    public GetPlanRedesResponse createGetPlanRedesResponse() {
        return new GetPlanRedesResponse();
    }

    /**
     * Create an instance of {@link ArrayOfListaRedes }
     * 
     */
    public ArrayOfListaRedes createArrayOfListaRedes() {
        return new ArrayOfListaRedes();
    }

    /**
     * Create an instance of {@link GetPlanAsignacionUsuarios }
     * 
     */
    public GetPlanAsignacionUsuarios createGetPlanAsignacionUsuarios() {
        return new GetPlanAsignacionUsuarios();
    }

    /**
     * Create an instance of {@link GetPlanAsignacionUsuariosResponse }
     * 
     */
    public GetPlanAsignacionUsuariosResponse createGetPlanAsignacionUsuariosResponse() {
        return new GetPlanAsignacionUsuariosResponse();
    }

    /**
     * Create an instance of {@link ArrayOfAsignacionUsuariosTV }
     * 
     */
    public ArrayOfAsignacionUsuariosTV createArrayOfAsignacionUsuariosTV() {
        return new ArrayOfAsignacionUsuariosTV();
    }

    /**
     * Create an instance of {@link GetPlanAsignacionRecursos }
     * 
     */
    public GetPlanAsignacionRecursos createGetPlanAsignacionRecursos() {
        return new GetPlanAsignacionRecursos();
    }

    /**
     * Create an instance of {@link GetPlanAsignacionRecursosResponse }
     * 
     */
    public GetPlanAsignacionRecursosResponse createGetPlanAsignacionRecursosResponse() {
        return new GetPlanAsignacionRecursosResponse();
    }

    /**
     * Create an instance of {@link ArrayOfAsignacionRecursosGW }
     * 
     */
    public ArrayOfAsignacionRecursosGW createArrayOfAsignacionRecursosGW() {
        return new ArrayOfAsignacionRecursosGW();
    }

    /**
     * Create an instance of {@link GetPlanDireccionamientoSIP }
     * 
     */
    public GetPlanDireccionamientoSIP createGetPlanDireccionamientoSIP() {
        return new GetPlanDireccionamientoSIP();
    }

    /**
     * Create an instance of {@link GetPlanDireccionamientoSIPResponse }
     * 
     */
    public GetPlanDireccionamientoSIPResponse createGetPlanDireccionamientoSIPResponse() {
        return new GetPlanDireccionamientoSIPResponse();
    }

    /**
     * Create an instance of {@link ArrayOfDireccionamientoSIP }
     * 
     */
    public ArrayOfDireccionamientoSIP createArrayOfDireccionamientoSIP() {
        return new ArrayOfDireccionamientoSIP();
    }

    /**
     * Create an instance of {@link GetParametrosMulticast }
     * 
     */
    public GetParametrosMulticast createGetParametrosMulticast() {
        return new GetParametrosMulticast();
    }

    /**
     * Create an instance of {@link GetParametrosMulticastResponse }
     * 
     */
    public GetParametrosMulticastResponse createGetParametrosMulticastResponse() {
        return new GetParametrosMulticastResponse();
    }

    /**
     * Create an instance of {@link ParametrosMulticast }
     * 
     */
    public ParametrosMulticast createParametrosMulticast() {
        return new ParametrosMulticast();
    }

    /**
     * Create an instance of {@link GetPoolHfElement }
     * 
     */
    public GetPoolHfElement createGetPoolHfElement() {
        return new GetPoolHfElement();
    }

    /**
     * Create an instance of {@link GetPoolHfElementResponse }
     * 
     */
    public GetPoolHfElementResponse createGetPoolHfElementResponse() {
        return new GetPoolHfElementResponse();
    }

    /**
     * Create an instance of {@link ArrayOfPoolHfElement }
     * 
     */
    public ArrayOfPoolHfElement createArrayOfPoolHfElement() {
        return new ArrayOfPoolHfElement();
    }

    /**
     * Create an instance of {@link GetPoolNMElements }
     * 
     */
    public GetPoolNMElements createGetPoolNMElements() {
        return new GetPoolNMElements();
    }

    /**
     * Create an instance of {@link GetPoolNMElementsResponse }
     * 
     */
    public GetPoolNMElementsResponse createGetPoolNMElementsResponse() {
        return new GetPoolNMElementsResponse();
    }

    /**
     * Create an instance of {@link ArrayOfNode }
     * 
     */
    public ArrayOfNode createArrayOfNode() {
        return new ArrayOfNode();
    }

    /**
     * Create an instance of {@link EstadoNode }
     * 
     */
    public EstadoNode createEstadoNode() {
        return new EstadoNode();
    }

    /**
     * Create an instance of {@link CfgEnlaceExterno }
     * 
     */
    public CfgEnlaceExterno createCfgEnlaceExterno() {
        return new CfgEnlaceExterno();
    }

    /**
     * Create an instance of {@link ArrayOfUnsignedInt }
     * 
     */
    public ArrayOfUnsignedInt createArrayOfUnsignedInt() {
        return new ArrayOfUnsignedInt();
    }

    /**
     * Create an instance of {@link ArrayOfCfgRecursoEnlaceExterno }
     * 
     */
    public ArrayOfCfgRecursoEnlaceExterno createArrayOfCfgRecursoEnlaceExterno() {
        return new ArrayOfCfgRecursoEnlaceExterno();
    }

    /**
     * Create an instance of {@link CfgRecursoEnlaceExterno }
     * 
     */
    public CfgRecursoEnlaceExterno createCfgRecursoEnlaceExterno() {
        return new CfgRecursoEnlaceExterno();
    }

    /**
     * Create an instance of {@link ArrayOfInt }
     * 
     */
    public ArrayOfInt createArrayOfInt() {
        return new ArrayOfInt();
    }

    /**
     * Create an instance of {@link ArrayOfString }
     * 
     */
    public ArrayOfString createArrayOfString() {
        return new ArrayOfString();
    }

    /**
     * Create an instance of {@link CfgEnlaceInterno }
     * 
     */
    public CfgEnlaceInterno createCfgEnlaceInterno() {
        return new CfgEnlaceInterno();
    }

    /**
     * Create an instance of {@link ArrayOfCfgRecursoEnlaceInternoConInterface }
     * 
     */
    public ArrayOfCfgRecursoEnlaceInternoConInterface createArrayOfCfgRecursoEnlaceInternoConInterface() {
        return new ArrayOfCfgRecursoEnlaceInternoConInterface();
    }

    /**
     * Create an instance of {@link CfgRecursoEnlaceInternoConInterface }
     * 
     */
    public CfgRecursoEnlaceInternoConInterface createCfgRecursoEnlaceInternoConInterface() {
        return new CfgRecursoEnlaceInternoConInterface();
    }

    /**
     * Create an instance of {@link CfgRecursoEnlaceInterno }
     * 
     */
    public CfgRecursoEnlaceInterno createCfgRecursoEnlaceInterno() {
        return new CfgRecursoEnlaceInterno();
    }

    /**
     * Create an instance of {@link ArrayOfNumerosAbonado }
     * 
     */
    public ArrayOfNumerosAbonado createArrayOfNumerosAbonado() {
        return new ArrayOfNumerosAbonado();
    }

    /**
     * Create an instance of {@link NumerosAbonado }
     * 
     */
    public NumerosAbonado createNumerosAbonado() {
        return new NumerosAbonado();
    }

    /**
     * Create an instance of {@link SectoresSCV }
     * 
     */
    public SectoresSCV createSectoresSCV() {
        return new SectoresSCV();
    }

    /**
     * Create an instance of {@link SectoresAgrupacion }
     * 
     */
    public SectoresAgrupacion createSectoresAgrupacion() {
        return new SectoresAgrupacion();
    }

    /**
     * Create an instance of {@link Agrupaciones }
     * 
     */
    public Agrupaciones createAgrupaciones() {
        return new Agrupaciones();
    }

    /**
     * Create an instance of {@link Internos }
     * 
     */
    public Internos createInternos() {
        return new Internos();
    }

    /**
     * Create an instance of {@link HFRangoFrecuencias }
     * 
     */
    public HFRangoFrecuencias createHFRangoFrecuencias() {
        return new HFRangoFrecuencias();
    }

    /**
     * Create an instance of {@link HFParams }
     * 
     */
    public HFParams createHFParams() {
        return new HFParams();
    }

    /**
     * Create an instance of {@link ParametrosSectorSCV }
     * 
     */
    public ParametrosSectorSCV createParametrosSectorSCV() {
        return new ParametrosSectorSCV();
    }

    /**
     * Create an instance of {@link ParametrosSectorSCVKeepAlive }
     * 
     */
    public ParametrosSectorSCVKeepAlive createParametrosSectorSCVKeepAlive() {
        return new ParametrosSectorSCVKeepAlive();
    }

    /**
     * Create an instance of {@link ParametrosSectorRecording }
     * 
     */
    public ParametrosSectorRecording createParametrosSectorRecording() {
        return new ParametrosSectorRecording();
    }

    /**
     * Create an instance of {@link ParametrosSector }
     * 
     */
    public ParametrosSector createParametrosSector() {
        return new ParametrosSector();
    }

    /**
     * Create an instance of {@link TeclasSectorSCV }
     * 
     */
    public TeclasSectorSCV createTeclasSectorSCV() {
        return new TeclasSectorSCV();
    }

    /**
     * Create an instance of {@link TeclasSector }
     * 
     */
    public TeclasSector createTeclasSector() {
        return new TeclasSector();
    }

    /**
     * Create an instance of {@link TroncalesRuta }
     * 
     */
    public TroncalesRuta createTroncalesRuta() {
        return new TroncalesRuta();
    }

    /**
     * Create an instance of {@link SectoresSector }
     * 
     */
    public SectoresSector createSectoresSector() {
        return new SectoresSector();
    }

    /**
     * Create an instance of {@link Altavoces }
     * 
     */
    public Altavoces createAltavoces() {
        return new Altavoces();
    }

    /**
     * Create an instance of {@link NivelesSCV }
     * 
     */
    public NivelesSCV createNivelesSCV() {
        return new NivelesSCV();
    }

    /**
     * Create an instance of {@link Niveles }
     * 
     */
    public Niveles createNiveles() {
        return new Niveles();
    }

    /**
     * Create an instance of {@link Radio }
     * 
     */
    public Radio createRadio() {
        return new Radio();
    }

    /**
     * Create an instance of {@link EstadosRecursos }
     * 
     */
    public EstadosRecursos createEstadosRecursos() {
        return new EstadosRecursos();
    }

    /**
     * Create an instance of {@link DestinosRadioSector }
     * 
     */
    public DestinosRadioSector createDestinosRadioSector() {
        return new DestinosRadioSector();
    }

    /**
     * Create an instance of {@link DestinosInternosSector }
     * 
     */
    public DestinosInternosSector createDestinosInternosSector() {
        return new DestinosInternosSector();
    }

    /**
     * Create an instance of {@link GruposTelefonia }
     * 
     */
    public GruposTelefonia createGruposTelefonia() {
        return new GruposTelefonia();
    }

    /**
     * Create an instance of {@link DestinosExternosSector }
     * 
     */
    public DestinosExternosSector createDestinosExternosSector() {
        return new DestinosExternosSector();
    }

    /**
     * Create an instance of {@link Externos }
     * 
     */
    public Externos createExternos() {
        return new Externos();
    }

    /**
     * Create an instance of {@link Destinos }
     * 
     */
    public Destinos createDestinos() {
        return new Destinos();
    }

    /**
     * Create an instance of {@link DestinosRadio }
     * 
     */
    public DestinosRadio createDestinosRadio() {
        return new DestinosRadio();
    }

    /**
     * Create an instance of {@link ArrayOfMetodosBssDelRecurso }
     * 
     */
    public ArrayOfMetodosBssDelRecurso createArrayOfMetodosBssDelRecurso() {
        return new ArrayOfMetodosBssDelRecurso();
    }

    /**
     * Create an instance of {@link MetodosBssDelRecurso }
     * 
     */
    public MetodosBssDelRecurso createMetodosBssDelRecurso() {
        return new MetodosBssDelRecurso();
    }

    /**
     * Create an instance of {@link DestinosTelefonia }
     * 
     */
    public DestinosTelefonia createDestinosTelefonia() {
        return new DestinosTelefonia();
    }

    /**
     * Create an instance of {@link DestinosInternos }
     * 
     */
    public DestinosInternos createDestinosInternos() {
        return new DestinosInternos();
    }

    /**
     * Create an instance of {@link DestinosExternos }
     * 
     */
    public DestinosExternos createDestinosExternos() {
        return new DestinosExternos();
    }

    /**
     * Create an instance of {@link UsuariosAbonados }
     * 
     */
    public UsuariosAbonados createUsuariosAbonados() {
        return new UsuariosAbonados();
    }

    /**
     * Create an instance of {@link RecursosSCV }
     * 
     */
    public RecursosSCV createRecursosSCV() {
        return new RecursosSCV();
    }

    /**
     * Create an instance of {@link ParametrosLCENSCV }
     * 
     */
    public ParametrosLCENSCV createParametrosLCENSCV() {
        return new ParametrosLCENSCV();
    }

    /**
     * Create an instance of {@link ParametrosRecursoLCEN }
     * 
     */
    public ParametrosRecursoLCEN createParametrosRecursoLCEN() {
        return new ParametrosRecursoLCEN();
    }

    /**
     * Create an instance of {@link RecursosLCEN }
     * 
     */
    public RecursosLCEN createRecursosLCEN() {
        return new RecursosLCEN();
    }

    /**
     * Create an instance of {@link ParametrosTFSCV }
     * 
     */
    public ParametrosTFSCV createParametrosTFSCV() {
        return new ParametrosTFSCV();
    }

    /**
     * Create an instance of {@link ParametrosRecursoTelefonia }
     * 
     */
    public ParametrosRecursoTelefonia createParametrosRecursoTelefonia() {
        return new ParametrosRecursoTelefonia();
    }

    /**
     * Create an instance of {@link ParametrosEM }
     * 
     */
    public ParametrosEM createParametrosEM() {
        return new ParametrosEM();
    }

    /**
     * Create an instance of {@link ParametrosGrabacionTf }
     * 
     */
    public ParametrosGrabacionTf createParametrosGrabacionTf() {
        return new ParametrosGrabacionTf();
    }

    /**
     * Create an instance of {@link RecursosTF }
     * 
     */
    public RecursosTF createRecursosTF() {
        return new RecursosTF();
    }

    /**
     * Create an instance of {@link ParametrosRDSCV }
     * 
     */
    public ParametrosRDSCV createParametrosRDSCV() {
        return new ParametrosRDSCV();
    }

    /**
     * Create an instance of {@link ArrayOfRecursosExternos }
     * 
     */
    public ArrayOfRecursosExternos createArrayOfRecursosExternos() {
        return new ArrayOfRecursosExternos();
    }

    /**
     * Create an instance of {@link RecursosExternos }
     * 
     */
    public RecursosExternos createRecursosExternos() {
        return new RecursosExternos();
    }

    /**
     * Create an instance of {@link ParametrosRecursoRadio }
     * 
     */
    public ParametrosRecursoRadio createParametrosRecursoRadio() {
        return new ParametrosRecursoRadio();
    }

    /**
     * Create an instance of {@link ParametrosRecursosRadioKeepAlive }
     * 
     */
    public ParametrosRecursosRadioKeepAlive createParametrosRecursosRadioKeepAlive() {
        return new ParametrosRecursosRadioKeepAlive();
    }

    /**
     * Create an instance of {@link ParametrosRecursosRadioKASiccip }
     * 
     */
    public ParametrosRecursosRadioKASiccip createParametrosRecursosRadioKASiccip() {
        return new ParametrosRecursosRadioKASiccip();
    }

    /**
     * Create an instance of {@link RecursosRadio }
     * 
     */
    public RecursosRadio createRecursosRadio() {
        return new RecursosRadio();
    }

    /**
     * Create an instance of {@link RecursosRadioForGateway }
     * 
     */
    public RecursosRadioForGateway createRecursosRadioForGateway() {
        return new RecursosRadioForGateway();
    }

    /**
     * Create an instance of {@link Rutas }
     * 
     */
    public Rutas createRutas() {
        return new Rutas();
    }

    /**
     * Create an instance of {@link RangosSCV }
     * 
     */
    public RangosSCV createRangosSCV() {
        return new RangosSCV();
    }

    /**
     * Create an instance of {@link Rangos }
     * 
     */
    public Rangos createRangos() {
        return new Rangos();
    }

    /**
     * Create an instance of {@link Encaminamientos }
     * 
     */
    public Encaminamientos createEncaminamientos() {
        return new Encaminamientos();
    }

    /**
     * Create an instance of {@link SectoresSectorizacion }
     * 
     */
    public SectoresSectorizacion createSectoresSectorizacion() {
        return new SectoresSectorizacion();
    }

    /**
     * Create an instance of {@link Troncales }
     * 
     */
    public Troncales createTroncales() {
        return new Troncales();
    }

    /**
     * Create an instance of {@link PermisosRedesSCV }
     * 
     */
    public PermisosRedesSCV createPermisosRedesSCV() {
        return new PermisosRedesSCV();
    }

    /**
     * Create an instance of {@link PermisosRedes }
     * 
     */
    public PermisosRedes createPermisosRedes() {
        return new PermisosRedes();
    }

    /**
     * Create an instance of {@link Redes }
     * 
     */
    public Redes createRedes() {
        return new Redes();
    }

    /**
     * Create an instance of {@link Sectorizaciones }
     * 
     */
    public Sectorizaciones createSectorizaciones() {
        return new Sectorizaciones();
    }

    /**
     * Create an instance of {@link Prefijos }
     * 
     */
    public Prefijos createPrefijos() {
        return new Prefijos();
    }

    /**
     * Create an instance of {@link Nucleos }
     * 
     */
    public Nucleos createNucleos() {
        return new Nucleos();
    }

    /**
     * Create an instance of {@link Emplazamientos }
     * 
     */
    public Emplazamientos createEmplazamientos() {
        return new Emplazamientos();
    }

    /**
     * Create an instance of {@link Operadores }
     * 
     */
    public Operadores createOperadores() {
        return new Operadores();
    }

    /**
     * Create an instance of {@link Sistema }
     * 
     */
    public Sistema createSistema() {
        return new Sistema();
    }

    /**
     * Create an instance of {@link Top }
     * 
     */
    public Top createTop() {
        return new Top();
    }

    /**
     * Create an instance of {@link TifX }
     * 
     */
    public TifX createTifX() {
        return new TifX();
    }

    /**
     * Create an instance of {@link Sectores }
     * 
     */
    public Sectores createSectores() {
        return new Sectores();
    }

    /**
     * Create an instance of {@link ArrayOfPermisosRedesSCV }
     * 
     */
    public ArrayOfPermisosRedesSCV createArrayOfPermisosRedesSCV() {
        return new ArrayOfPermisosRedesSCV();
    }

    /**
     * Create an instance of {@link ArrayOfRecursosSCV }
     * 
     */
    public ArrayOfRecursosSCV createArrayOfRecursosSCV() {
        return new ArrayOfRecursosSCV();
    }

    /**
     * Create an instance of {@link NumeracionATS }
     * 
     */
    public NumeracionATS createNumeracionATS() {
        return new NumeracionATS();
    }

    /**
     * Create an instance of {@link ArrayOfRangosSCV }
     * 
     */
    public ArrayOfRangosSCV createArrayOfRangosSCV() {
        return new ArrayOfRangosSCV();
    }

    /**
     * Create an instance of {@link ArrayOfPlanRutas }
     * 
     */
    public ArrayOfPlanRutas createArrayOfPlanRutas() {
        return new ArrayOfPlanRutas();
    }

    /**
     * Create an instance of {@link PlanRutas }
     * 
     */
    public PlanRutas createPlanRutas() {
        return new PlanRutas();
    }

    /**
     * Create an instance of {@link DireccionamientoIP }
     * 
     */
    public DireccionamientoIP createDireccionamientoIP() {
        return new DireccionamientoIP();
    }

    /**
     * Create an instance of {@link ListaTroncales }
     * 
     */
    public ListaTroncales createListaTroncales() {
        return new ListaTroncales();
    }

    /**
     * Create an instance of {@link ArrayOfPlanRecursos }
     * 
     */
    public ArrayOfPlanRecursos createArrayOfPlanRecursos() {
        return new ArrayOfPlanRecursos();
    }

    /**
     * Create an instance of {@link PlanRecursos }
     * 
     */
    public PlanRecursos createPlanRecursos() {
        return new PlanRecursos();
    }

    /**
     * Create an instance of {@link ListaRedes }
     * 
     */
    public ListaRedes createListaRedes() {
        return new ListaRedes();
    }

    /**
     * Create an instance of {@link AsignacionUsuariosTV }
     * 
     */
    public AsignacionUsuariosTV createAsignacionUsuariosTV() {
        return new AsignacionUsuariosTV();
    }

    /**
     * Create an instance of {@link AsignacionRecursosGW }
     * 
     */
    public AsignacionRecursosGW createAsignacionRecursosGW() {
        return new AsignacionRecursosGW();
    }

    /**
     * Create an instance of {@link DireccionamientoSIP }
     * 
     */
    public DireccionamientoSIP createDireccionamientoSIP() {
        return new DireccionamientoSIP();
    }

    /**
     * Create an instance of {@link ArrayOfStrNumeroAbonado }
     * 
     */
    public ArrayOfStrNumeroAbonado createArrayOfStrNumeroAbonado() {
        return new ArrayOfStrNumeroAbonado();
    }

    /**
     * Create an instance of {@link StrNumeroAbonado }
     * 
     */
    public StrNumeroAbonado createStrNumeroAbonado() {
        return new StrNumeroAbonado();
    }

    /**
     * Create an instance of {@link PoolHfElement }
     * 
     */
    public PoolHfElement createPoolHfElement() {
        return new PoolHfElement();
    }

    /**
     * Create an instance of {@link ArrayOfHfRangoFrecuencias }
     * 
     */
    public ArrayOfHfRangoFrecuencias createArrayOfHfRangoFrecuencias() {
        return new ArrayOfHfRangoFrecuencias();
    }

    /**
     * Create an instance of {@link HfRangoFrecuenciass }
     * 
     */
    public HfRangoFrecuenciass createHfRangoFrecuenciass() {
        return new HfRangoFrecuenciass();
    }

    /**
     * Create an instance of {@link Node }
     * 
     */
    public Node createNode() {
        return new Node();
    }

    /**
     * Create an instance of {@link JAXBElement }{@code <}{@link String }{@code >}}
     * 
     */
    @XmlElementDecl(namespace = "http://CD40.es/", name = "string")
    public JAXBElement<String> createString(String value) {
        return new JAXBElement<String>(_String_QNAME, String.class, null, value);
    }

    /**
     * Create an instance of {@link JAXBElement }{@code <}{@link EstadoCluster }{@code >}}
     * 
     */
    @XmlElementDecl(namespace = "http://CD40.es/", name = "EstadoCluster")
    public JAXBElement<EstadoCluster> createEstadoCluster(EstadoCluster value) {
        return new JAXBElement<EstadoCluster>(_EstadoCluster_QNAME, EstadoCluster.class, null, value);
    }

    /**
     * Create an instance of {@link JAXBElement }{@code <}{@link ArrayOfCfgEnlaceExterno }{@code >}}
     * 
     */
    @XmlElementDecl(namespace = "http://CD40.es/", name = "ArrayOfCfgEnlaceExterno")
    public JAXBElement<ArrayOfCfgEnlaceExterno> createArrayOfCfgEnlaceExterno(ArrayOfCfgEnlaceExterno value) {
        return new JAXBElement<ArrayOfCfgEnlaceExterno>(_ArrayOfCfgEnlaceExterno_QNAME, ArrayOfCfgEnlaceExterno.class, null, value);
    }

    /**
     * Create an instance of {@link JAXBElement }{@code <}{@link ArrayOfCfgEnlaceInterno }{@code >}}
     * 
     */
    @XmlElementDecl(namespace = "http://CD40.es/", name = "ArrayOfCfgEnlaceInterno")
    public JAXBElement<ArrayOfCfgEnlaceInterno> createArrayOfCfgEnlaceInterno(ArrayOfCfgEnlaceInterno value) {
        return new JAXBElement<ArrayOfCfgEnlaceInterno>(_ArrayOfCfgEnlaceInterno_QNAME, ArrayOfCfgEnlaceInterno.class, null, value);
    }

    /**
     * Create an instance of {@link JAXBElement }{@code <}{@link LoginTerminalVoz }{@code >}}
     * 
     */
    @XmlElementDecl(namespace = "http://CD40.es/", name = "LoginTerminalVoz")
    public JAXBElement<LoginTerminalVoz> createLoginTerminalVoz(LoginTerminalVoz value) {
        return new JAXBElement<LoginTerminalVoz>(_LoginTerminalVoz_QNAME, LoginTerminalVoz.class, null, value);
    }

    /**
     * Create an instance of {@link JAXBElement }{@code <}{@link CfgUsuario }{@code >}}
     * 
     */
    @XmlElementDecl(namespace = "http://CD40.es/", name = "CfgUsuario")
    public JAXBElement<CfgUsuario> createCfgUsuario(CfgUsuario value) {
        return new JAXBElement<CfgUsuario>(_CfgUsuario_QNAME, CfgUsuario.class, null, value);
    }

    /**
     * Create an instance of {@link JAXBElement }{@code <}{@link CfgPasarela }{@code >}}
     * 
     */
    @XmlElementDecl(namespace = "http://CD40.es/", name = "CfgPasarela")
    public JAXBElement<CfgPasarela> createCfgPasarela(CfgPasarela value) {
        return new JAXBElement<CfgPasarela>(_CfgPasarela_QNAME, CfgPasarela.class, null, value);
    }

    /**
     * Create an instance of {@link JAXBElement }{@code <}{@link Tablas }{@code >}}
     * 
     */
    @XmlElementDecl(namespace = "http://CD40.es/", name = "Tablas")
    public JAXBElement<Tablas> createTablas(Tablas value) {
        return new JAXBElement<Tablas>(_Tablas_QNAME, Tablas.class, null, value);
    }

    /**
     * Create an instance of {@link JAXBElement }{@code <}{@link ConfiguracionSistema }{@code >}}
     * 
     */
    @XmlElementDecl(namespace = "http://CD40.es/", name = "ConfiguracionSistema")
    public JAXBElement<ConfiguracionSistema> createConfiguracionSistema(ConfiguracionSistema value) {
        return new JAXBElement<ConfiguracionSistema>(_ConfiguracionSistema_QNAME, ConfiguracionSistema.class, null, value);
    }

    /**
     * Create an instance of {@link JAXBElement }{@code <}{@link ParametrosGeneralesSistema }{@code >}}
     * 
     */
    @XmlElementDecl(namespace = "http://CD40.es/", name = "ParametrosGeneralesSistema")
    public JAXBElement<ParametrosGeneralesSistema> createParametrosGeneralesSistema(ParametrosGeneralesSistema value) {
        return new JAXBElement<ParametrosGeneralesSistema>(_ParametrosGeneralesSistema_QNAME, ParametrosGeneralesSistema.class, null, value);
    }

    /**
     * Create an instance of {@link JAXBElement }{@code <}{@link ArrayOfNumeracionATS }{@code >}}
     * 
     */
    @XmlElementDecl(namespace = "http://CD40.es/", name = "ArrayOfNumeracionATS")
    public JAXBElement<ArrayOfNumeracionATS> createArrayOfNumeracionATS(ArrayOfNumeracionATS value) {
        return new JAXBElement<ArrayOfNumeracionATS>(_ArrayOfNumeracionATS_QNAME, ArrayOfNumeracionATS.class, null, value);
    }

    /**
     * Create an instance of {@link JAXBElement }{@code <}{@link ArrayOfDireccionamientoIP }{@code >}}
     * 
     */
    @XmlElementDecl(namespace = "http://CD40.es/", name = "ArrayOfDireccionamientoIP")
    public JAXBElement<ArrayOfDireccionamientoIP> createArrayOfDireccionamientoIP(ArrayOfDireccionamientoIP value) {
        return new JAXBElement<ArrayOfDireccionamientoIP>(_ArrayOfDireccionamientoIP_QNAME, ArrayOfDireccionamientoIP.class, null, value);
    }

    /**
     * Create an instance of {@link JAXBElement }{@code <}{@link ArrayOfListaTroncales }{@code >}}
     * 
     */
    @XmlElementDecl(namespace = "http://CD40.es/", name = "ArrayOfListaTroncales")
    public JAXBElement<ArrayOfListaTroncales> createArrayOfListaTroncales(ArrayOfListaTroncales value) {
        return new JAXBElement<ArrayOfListaTroncales>(_ArrayOfListaTroncales_QNAME, ArrayOfListaTroncales.class, null, value);
    }

    /**
     * Create an instance of {@link JAXBElement }{@code <}{@link ArrayOfListaRedes }{@code >}}
     * 
     */
    @XmlElementDecl(namespace = "http://CD40.es/", name = "ArrayOfListaRedes")
    public JAXBElement<ArrayOfListaRedes> createArrayOfListaRedes(ArrayOfListaRedes value) {
        return new JAXBElement<ArrayOfListaRedes>(_ArrayOfListaRedes_QNAME, ArrayOfListaRedes.class, null, value);
    }

    /**
     * Create an instance of {@link JAXBElement }{@code <}{@link ArrayOfAsignacionUsuariosTV }{@code >}}
     * 
     */
    @XmlElementDecl(namespace = "http://CD40.es/", name = "ArrayOfAsignacionUsuariosTV")
    public JAXBElement<ArrayOfAsignacionUsuariosTV> createArrayOfAsignacionUsuariosTV(ArrayOfAsignacionUsuariosTV value) {
        return new JAXBElement<ArrayOfAsignacionUsuariosTV>(_ArrayOfAsignacionUsuariosTV_QNAME, ArrayOfAsignacionUsuariosTV.class, null, value);
    }

    /**
     * Create an instance of {@link JAXBElement }{@code <}{@link ArrayOfAsignacionRecursosGW }{@code >}}
     * 
     */
    @XmlElementDecl(namespace = "http://CD40.es/", name = "ArrayOfAsignacionRecursosGW")
    public JAXBElement<ArrayOfAsignacionRecursosGW> createArrayOfAsignacionRecursosGW(ArrayOfAsignacionRecursosGW value) {
        return new JAXBElement<ArrayOfAsignacionRecursosGW>(_ArrayOfAsignacionRecursosGW_QNAME, ArrayOfAsignacionRecursosGW.class, null, value);
    }

    /**
     * Create an instance of {@link JAXBElement }{@code <}{@link ArrayOfDireccionamientoSIP }{@code >}}
     * 
     */
    @XmlElementDecl(namespace = "http://CD40.es/", name = "ArrayOfDireccionamientoSIP")
    public JAXBElement<ArrayOfDireccionamientoSIP> createArrayOfDireccionamientoSIP(ArrayOfDireccionamientoSIP value) {
        return new JAXBElement<ArrayOfDireccionamientoSIP>(_ArrayOfDireccionamientoSIP_QNAME, ArrayOfDireccionamientoSIP.class, null, value);
    }

    /**
     * Create an instance of {@link JAXBElement }{@code <}{@link ParametrosMulticast }{@code >}}
     * 
     */
    @XmlElementDecl(namespace = "http://CD40.es/", name = "ParametrosMulticast")
    public JAXBElement<ParametrosMulticast> createParametrosMulticast(ParametrosMulticast value) {
        return new JAXBElement<ParametrosMulticast>(_ParametrosMulticast_QNAME, ParametrosMulticast.class, null, value);
    }

    /**
     * Create an instance of {@link JAXBElement }{@code <}{@link ArrayOfPoolHfElement }{@code >}}
     * 
     */
    @XmlElementDecl(namespace = "http://CD40.es/", name = "ArrayOfPoolHfElement")
    public JAXBElement<ArrayOfPoolHfElement> createArrayOfPoolHfElement(ArrayOfPoolHfElement value) {
        return new JAXBElement<ArrayOfPoolHfElement>(_ArrayOfPoolHfElement_QNAME, ArrayOfPoolHfElement.class, null, value);
    }

    /**
     * Create an instance of {@link JAXBElement }{@code <}{@link ArrayOfNode }{@code >}}
     * 
     */
    @XmlElementDecl(namespace = "http://CD40.es/", name = "ArrayOfNode")
    public JAXBElement<ArrayOfNode> createArrayOfNode(ArrayOfNode value) {
        return new JAXBElement<ArrayOfNode>(_ArrayOfNode_QNAME, ArrayOfNode.class, null, value);
    }

}

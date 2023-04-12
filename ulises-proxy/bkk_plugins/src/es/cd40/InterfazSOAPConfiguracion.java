package es.cd40;

import java.net.MalformedURLException;
import java.net.URL;
import javax.xml.namespace.QName;
import javax.xml.ws.WebEndpoint;
import javax.xml.ws.WebServiceClient;
import javax.xml.ws.WebServiceFeature;
import javax.xml.ws.Service;

/**
 * This class was generated by Apache CXF 3.2.7
 * 2018-12-10T15:05:38.568+01:00
 * Generated source version: 3.2.7
 *
 */
@WebServiceClient(name = "InterfazSOAPConfiguracion",
                  wsdlLocation = "file:InterfazSOAPConfiguracion.wsdl",
                  targetNamespace = "http://CD40.es/")
public class InterfazSOAPConfiguracion extends Service {

    public final static URL WSDL_LOCATION;

    public final static QName SERVICE = new QName("http://CD40.es/", "InterfazSOAPConfiguracion");
    public final static QName InterfazSOAPConfiguracionSoap12 = new QName("http://CD40.es/", "InterfazSOAPConfiguracionSoap12");
    public final static QName InterfazSOAPConfiguracionSoap = new QName("http://CD40.es/", "InterfazSOAPConfiguracionSoap");
    public final static QName InterfazSOAPConfiguracionHttpPost = new QName("http://CD40.es/", "InterfazSOAPConfiguracionHttpPost");
    static {
        URL url = null;
        try {
            url = new URL("file:InterfazSOAPConfiguracion.wsdl");
        } catch (MalformedURLException e) {
            java.util.logging.Logger.getLogger(InterfazSOAPConfiguracion.class.getName())
                .log(java.util.logging.Level.INFO,
                     "Can not initialize the default wsdl from {0}", "file:InterfazSOAPConfiguracion.wsdl");
        }
        WSDL_LOCATION = url;
    }

    public InterfazSOAPConfiguracion(URL wsdlLocation) {
        super(wsdlLocation, SERVICE);
    }

    public InterfazSOAPConfiguracion(URL wsdlLocation, QName serviceName) {
        super(wsdlLocation, serviceName);
    }

    public InterfazSOAPConfiguracion() {
        super(WSDL_LOCATION, SERVICE);
    }

    public InterfazSOAPConfiguracion(WebServiceFeature ... features) {
        super(WSDL_LOCATION, SERVICE, features);
    }

    public InterfazSOAPConfiguracion(URL wsdlLocation, WebServiceFeature ... features) {
        super(wsdlLocation, SERVICE, features);
    }

    public InterfazSOAPConfiguracion(URL wsdlLocation, QName serviceName, WebServiceFeature ... features) {
        super(wsdlLocation, serviceName, features);
    }




    /**
     *
     * @return
     *     returns InterfazSOAPConfiguracionSoap
     */
    @WebEndpoint(name = "InterfazSOAPConfiguracionSoap12")
    public InterfazSOAPConfiguracionSoap getInterfazSOAPConfiguracionSoap12() {
        return super.getPort(InterfazSOAPConfiguracionSoap12, InterfazSOAPConfiguracionSoap.class);
    }

    /**
     *
     * @param features
     *     A list of {@link javax.xml.ws.WebServiceFeature} to configure on the proxy.  Supported features not in the <code>features</code> parameter will have their default values.
     * @return
     *     returns InterfazSOAPConfiguracionSoap
     */
    @WebEndpoint(name = "InterfazSOAPConfiguracionSoap12")
    public InterfazSOAPConfiguracionSoap getInterfazSOAPConfiguracionSoap12(WebServiceFeature... features) {
        return super.getPort(InterfazSOAPConfiguracionSoap12, InterfazSOAPConfiguracionSoap.class, features);
    }


    /**
     *
     * @return
     *     returns InterfazSOAPConfiguracionSoap
     */
    @WebEndpoint(name = "InterfazSOAPConfiguracionSoap")
    public InterfazSOAPConfiguracionSoap getInterfazSOAPConfiguracionSoap() {
        return super.getPort(InterfazSOAPConfiguracionSoap, InterfazSOAPConfiguracionSoap.class);
    }

    /**
     *
     * @param features
     *     A list of {@link javax.xml.ws.WebServiceFeature} to configure on the proxy.  Supported features not in the <code>features</code> parameter will have their default values.
     * @return
     *     returns InterfazSOAPConfiguracionSoap
     */
    @WebEndpoint(name = "InterfazSOAPConfiguracionSoap")
    public InterfazSOAPConfiguracionSoap getInterfazSOAPConfiguracionSoap(WebServiceFeature... features) {
        return super.getPort(InterfazSOAPConfiguracionSoap, InterfazSOAPConfiguracionSoap.class, features);
    }


    /**
     *
     * @return
     *     returns InterfazSOAPConfiguracionHttpPost
     */
    @WebEndpoint(name = "InterfazSOAPConfiguracionHttpPost")
    public InterfazSOAPConfiguracionHttpPost getInterfazSOAPConfiguracionHttpPost() {
        return super.getPort(InterfazSOAPConfiguracionHttpPost, InterfazSOAPConfiguracionHttpPost.class);
    }

    /**
     *
     * @param features
     *     A list of {@link javax.xml.ws.WebServiceFeature} to configure on the proxy.  Supported features not in the <code>features</code> parameter will have their default values.
     * @return
     *     returns InterfazSOAPConfiguracionHttpPost
     */
    @WebEndpoint(name = "InterfazSOAPConfiguracionHttpPost")
    public InterfazSOAPConfiguracionHttpPost getInterfazSOAPConfiguracionHttpPost(WebServiceFeature... features) {
        return super.getPort(InterfazSOAPConfiguracionHttpPost, InterfazSOAPConfiguracionHttpPost.class, features);
    }

}

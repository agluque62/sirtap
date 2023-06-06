
package es.cd40;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para AsignacionUsuariosTV complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="AsignacionUsuariosTV"&gt;
 *   &lt;complexContent&gt;
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="IdUsuario" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="IdHost" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="IpGrabador1" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="IpGrabador2" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="RtspPort" type="{http://www.w3.org/2001/XMLSchema}int"/&gt;
 *         &lt;element name="TipoGrabacionAnalogica" type="{http://www.w3.org/2001/XMLSchema}int"/&gt;
 *         &lt;element name="EnableGrabacionEd137" type="{http://www.w3.org/2001/XMLSchema}boolean"/&gt;
 *         &lt;element name="EnableGrabacionAnalogica" type="{http://www.w3.org/2001/XMLSchema}boolean"/&gt;
 *         &lt;element name="RtspPort1" type="{http://www.w3.org/2001/XMLSchema}int"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/restriction&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "AsignacionUsuariosTV", propOrder = {
    "idUsuario",
    "idHost",
    "ipGrabador1",
    "ipGrabador2",
    "rtspPort",
    "tipoGrabacionAnalogica",
    "enableGrabacionEd137",
    "enableGrabacionAnalogica",
    "rtspPort1"
})
public class AsignacionUsuariosTV {

    @XmlElement(name = "IdUsuario")
    protected String idUsuario;
    @XmlElement(name = "IdHost")
    protected String idHost;
    @XmlElement(name = "IpGrabador1")
    protected String ipGrabador1;
    @XmlElement(name = "IpGrabador2")
    protected String ipGrabador2;
    @XmlElement(name = "RtspPort")
    protected int rtspPort;
    @XmlElement(name = "TipoGrabacionAnalogica")
    protected int tipoGrabacionAnalogica;
    @XmlElement(name = "EnableGrabacionEd137")
    protected boolean enableGrabacionEd137;
    @XmlElement(name = "EnableGrabacionAnalogica")
    protected boolean enableGrabacionAnalogica;
    @XmlElement(name = "RtspPort1")
    protected int rtspPort1;

    /**
     * Obtiene el valor de la propiedad idUsuario.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getIdUsuario() {
        return idUsuario;
    }

    /**
     * Define el valor de la propiedad idUsuario.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setIdUsuario(String value) {
        this.idUsuario = value;
    }

    /**
     * Obtiene el valor de la propiedad idHost.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getIdHost() {
        return idHost;
    }

    /**
     * Define el valor de la propiedad idHost.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setIdHost(String value) {
        this.idHost = value;
    }

    /**
     * Obtiene el valor de la propiedad ipGrabador1.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getIpGrabador1() {
        return ipGrabador1;
    }

    /**
     * Define el valor de la propiedad ipGrabador1.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setIpGrabador1(String value) {
        this.ipGrabador1 = value;
    }

    /**
     * Obtiene el valor de la propiedad ipGrabador2.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getIpGrabador2() {
        return ipGrabador2;
    }

    /**
     * Define el valor de la propiedad ipGrabador2.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setIpGrabador2(String value) {
        this.ipGrabador2 = value;
    }

    /**
     * Obtiene el valor de la propiedad rtspPort.
     * 
     */
    public int getRtspPort() {
        return rtspPort;
    }

    /**
     * Define el valor de la propiedad rtspPort.
     * 
     */
    public void setRtspPort(int value) {
        this.rtspPort = value;
    }

    /**
     * Obtiene el valor de la propiedad tipoGrabacionAnalogica.
     * 
     */
    public int getTipoGrabacionAnalogica() {
        return tipoGrabacionAnalogica;
    }

    /**
     * Define el valor de la propiedad tipoGrabacionAnalogica.
     * 
     */
    public void setTipoGrabacionAnalogica(int value) {
        this.tipoGrabacionAnalogica = value;
    }

    /**
     * Obtiene el valor de la propiedad enableGrabacionEd137.
     * 
     */
    public boolean isEnableGrabacionEd137() {
        return enableGrabacionEd137;
    }

    /**
     * Define el valor de la propiedad enableGrabacionEd137.
     * 
     */
    public void setEnableGrabacionEd137(boolean value) {
        this.enableGrabacionEd137 = value;
    }

    /**
     * Obtiene el valor de la propiedad enableGrabacionAnalogica.
     * 
     */
    public boolean isEnableGrabacionAnalogica() {
        return enableGrabacionAnalogica;
    }

    /**
     * Define el valor de la propiedad enableGrabacionAnalogica.
     * 
     */
    public void setEnableGrabacionAnalogica(boolean value) {
        this.enableGrabacionAnalogica = value;
    }

    /**
     * Obtiene el valor de la propiedad rtspPort1.
     * 
     */
    public int getRtspPort1() {
        return rtspPort1;
    }

    /**
     * Define el valor de la propiedad rtspPort1.
     * 
     */
    public void setRtspPort1(int value) {
        this.rtspPort1 = value;
    }

}

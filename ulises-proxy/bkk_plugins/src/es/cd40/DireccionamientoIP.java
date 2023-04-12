
package es.cd40;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlSchemaType;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para DireccionamientoIP complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="DireccionamientoIP"&gt;
 *   &lt;complexContent&gt;
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="IdHost" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="IpRed1" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="IpRed2" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="IpRed3" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="TipoHost" type="{http://CD40.es/}Tipo_Elemento_HW"/&gt;
 *         &lt;element name="Interno" type="{http://www.w3.org/2001/XMLSchema}boolean"/&gt;
 *         &lt;element name="Min" type="{http://www.w3.org/2001/XMLSchema}long"/&gt;
 *         &lt;element name="Max" type="{http://www.w3.org/2001/XMLSchema}long"/&gt;
 *         &lt;element name="EsCentralIP" type="{http://www.w3.org/2001/XMLSchema}boolean"/&gt;
 *         &lt;element name="SrvPresenciaIpRed1" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="SrvPresenciaIpRed2" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="SrvPresenciaIpRed3" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/restriction&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "DireccionamientoIP", propOrder = {
    "idHost",
    "ipRed1",
    "ipRed2",
    "ipRed3",
    "tipoHost",
    "interno",
    "min",
    "max",
    "esCentralIP",
    "srvPresenciaIpRed1",
    "srvPresenciaIpRed2",
    "srvPresenciaIpRed3"
})
public class DireccionamientoIP {

    @XmlElement(name = "IdHost")
    protected String idHost;
    @XmlElement(name = "IpRed1")
    protected String ipRed1;
    @XmlElement(name = "IpRed2")
    protected String ipRed2;
    @XmlElement(name = "IpRed3")
    protected String ipRed3;
    @XmlElement(name = "TipoHost", required = true)
    @XmlSchemaType(name = "string")
    protected TipoElementoHW tipoHost;
    @XmlElement(name = "Interno")
    protected boolean interno;
    @XmlElement(name = "Min")
    protected long min;
    @XmlElement(name = "Max")
    protected long max;
    @XmlElement(name = "EsCentralIP")
    protected boolean esCentralIP;
    @XmlElement(name = "SrvPresenciaIpRed1")
    protected String srvPresenciaIpRed1;
    @XmlElement(name = "SrvPresenciaIpRed2")
    protected String srvPresenciaIpRed2;
    @XmlElement(name = "SrvPresenciaIpRed3")
    protected String srvPresenciaIpRed3;

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
     * Obtiene el valor de la propiedad ipRed1.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getIpRed1() {
        return ipRed1;
    }

    /**
     * Define el valor de la propiedad ipRed1.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setIpRed1(String value) {
        this.ipRed1 = value;
    }

    /**
     * Obtiene el valor de la propiedad ipRed2.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getIpRed2() {
        return ipRed2;
    }

    /**
     * Define el valor de la propiedad ipRed2.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setIpRed2(String value) {
        this.ipRed2 = value;
    }

    /**
     * Obtiene el valor de la propiedad ipRed3.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getIpRed3() {
        return ipRed3;
    }

    /**
     * Define el valor de la propiedad ipRed3.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setIpRed3(String value) {
        this.ipRed3 = value;
    }

    /**
     * Obtiene el valor de la propiedad tipoHost.
     * 
     * @return
     *     possible object is
     *     {@link TipoElementoHW }
     *     
     */
    public TipoElementoHW getTipoHost() {
        return tipoHost;
    }

    /**
     * Define el valor de la propiedad tipoHost.
     * 
     * @param value
     *     allowed object is
     *     {@link TipoElementoHW }
     *     
     */
    public void setTipoHost(TipoElementoHW value) {
        this.tipoHost = value;
    }

    /**
     * Obtiene el valor de la propiedad interno.
     * 
     */
    public boolean isInterno() {
        return interno;
    }

    /**
     * Define el valor de la propiedad interno.
     * 
     */
    public void setInterno(boolean value) {
        this.interno = value;
    }

    /**
     * Obtiene el valor de la propiedad min.
     * 
     */
    public long getMin() {
        return min;
    }

    /**
     * Define el valor de la propiedad min.
     * 
     */
    public void setMin(long value) {
        this.min = value;
    }

    /**
     * Obtiene el valor de la propiedad max.
     * 
     */
    public long getMax() {
        return max;
    }

    /**
     * Define el valor de la propiedad max.
     * 
     */
    public void setMax(long value) {
        this.max = value;
    }

    /**
     * Obtiene el valor de la propiedad esCentralIP.
     * 
     */
    public boolean isEsCentralIP() {
        return esCentralIP;
    }

    /**
     * Define el valor de la propiedad esCentralIP.
     * 
     */
    public void setEsCentralIP(boolean value) {
        this.esCentralIP = value;
    }

    /**
     * Obtiene el valor de la propiedad srvPresenciaIpRed1.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getSrvPresenciaIpRed1() {
        return srvPresenciaIpRed1;
    }

    /**
     * Define el valor de la propiedad srvPresenciaIpRed1.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setSrvPresenciaIpRed1(String value) {
        this.srvPresenciaIpRed1 = value;
    }

    /**
     * Obtiene el valor de la propiedad srvPresenciaIpRed2.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getSrvPresenciaIpRed2() {
        return srvPresenciaIpRed2;
    }

    /**
     * Define el valor de la propiedad srvPresenciaIpRed2.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setSrvPresenciaIpRed2(String value) {
        this.srvPresenciaIpRed2 = value;
    }

    /**
     * Obtiene el valor de la propiedad srvPresenciaIpRed3.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getSrvPresenciaIpRed3() {
        return srvPresenciaIpRed3;
    }

    /**
     * Define el valor de la propiedad srvPresenciaIpRed3.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setSrvPresenciaIpRed3(String value) {
        this.srvPresenciaIpRed3 = value;
    }

}

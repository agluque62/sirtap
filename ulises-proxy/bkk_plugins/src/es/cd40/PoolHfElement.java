
package es.cd40;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlSeeAlso;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para PoolHfElement complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="PoolHfElement"&gt;
 *   &lt;complexContent&gt;
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="Id" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="SipUri" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="IpGestor" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="Oid" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="Frecs" type="{http://CD40.es/}ArrayOfHfRangoFrecuencias" minOccurs="0"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/restriction&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "PoolHfElement", propOrder = {
    "id",
    "sipUri",
    "ipGestor",
    "oid",
    "frecs"
})
@XmlSeeAlso({
    Node.class
})
public class PoolHfElement {

    @XmlElement(name = "Id")
    protected String id;
    @XmlElement(name = "SipUri")
    protected String sipUri;
    @XmlElement(name = "IpGestor")
    protected String ipGestor;
    @XmlElement(name = "Oid")
    protected String oid;
    @XmlElement(name = "Frecs")
    protected ArrayOfHfRangoFrecuencias frecs;

    /**
     * Obtiene el valor de la propiedad id.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getId() {
        return id;
    }

    /**
     * Define el valor de la propiedad id.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setId(String value) {
        this.id = value;
    }

    /**
     * Obtiene el valor de la propiedad sipUri.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getSipUri() {
        return sipUri;
    }

    /**
     * Define el valor de la propiedad sipUri.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setSipUri(String value) {
        this.sipUri = value;
    }

    /**
     * Obtiene el valor de la propiedad ipGestor.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getIpGestor() {
        return ipGestor;
    }

    /**
     * Define el valor de la propiedad ipGestor.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setIpGestor(String value) {
        this.ipGestor = value;
    }

    /**
     * Obtiene el valor de la propiedad oid.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getOid() {
        return oid;
    }

    /**
     * Define el valor de la propiedad oid.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setOid(String value) {
        this.oid = value;
    }

    /**
     * Obtiene el valor de la propiedad frecs.
     * 
     * @return
     *     possible object is
     *     {@link ArrayOfHfRangoFrecuencias }
     *     
     */
    public ArrayOfHfRangoFrecuencias getFrecs() {
        return frecs;
    }

    /**
     * Define el valor de la propiedad frecs.
     * 
     * @param value
     *     allowed object is
     *     {@link ArrayOfHfRangoFrecuencias }
     *     
     */
    public void setFrecs(ArrayOfHfRangoFrecuencias value) {
        this.frecs = value;
    }

}

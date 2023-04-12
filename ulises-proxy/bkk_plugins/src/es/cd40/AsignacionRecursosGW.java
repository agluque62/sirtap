
package es.cd40;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlSchemaType;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para AsignacionRecursosGW complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="AsignacionRecursosGW"&gt;
 *   &lt;complexContent&gt;
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="IdRecurso" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="IdHost" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="SipPort" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/restriction&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "AsignacionRecursosGW", propOrder = {
    "idRecurso",
    "idHost",
    "sipPort"
})
public class AsignacionRecursosGW {

    @XmlElement(name = "IdRecurso")
    protected String idRecurso;
    @XmlElement(name = "IdHost")
    protected String idHost;
    @XmlElement(name = "SipPort")
    @XmlSchemaType(name = "unsignedInt")
    protected long sipPort;

    /**
     * Obtiene el valor de la propiedad idRecurso.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getIdRecurso() {
        return idRecurso;
    }

    /**
     * Define el valor de la propiedad idRecurso.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setIdRecurso(String value) {
        this.idRecurso = value;
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
     * Obtiene el valor de la propiedad sipPort.
     * 
     */
    public long getSipPort() {
        return sipPort;
    }

    /**
     * Define el valor de la propiedad sipPort.
     * 
     */
    public void setSipPort(long value) {
        this.sipPort = value;
    }

}


package es.cd40;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para Encaminamientos complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="Encaminamientos"&gt;
 *   &lt;complexContent&gt;
 *     &lt;extension base="{http://CD40.es/}Tablas"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="Central" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="IdSistema" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="CentralPropia" type="{http://www.w3.org/2001/XMLSchema}boolean"/&gt;
 *         &lt;element name="Throwswitching" type="{http://www.w3.org/2001/XMLSchema}boolean"/&gt;
 *         &lt;element name="NumTest" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="CentralIp" type="{http://www.w3.org/2001/XMLSchema}boolean"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/extension&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "Encaminamientos", propOrder = {
    "central",
    "idSistema",
    "centralPropia",
    "throwswitching",
    "numTest",
    "centralIp"
})
public class Encaminamientos
    extends Tablas
{

    @XmlElement(name = "Central")
    protected String central;
    @XmlElement(name = "IdSistema")
    protected String idSistema;
    @XmlElement(name = "CentralPropia")
    protected boolean centralPropia;
    @XmlElement(name = "Throwswitching")
    protected boolean throwswitching;
    @XmlElement(name = "NumTest")
    protected String numTest;
    @XmlElement(name = "CentralIp")
    protected boolean centralIp;

    /**
     * Obtiene el valor de la propiedad central.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getCentral() {
        return central;
    }

    /**
     * Define el valor de la propiedad central.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setCentral(String value) {
        this.central = value;
    }

    /**
     * Obtiene el valor de la propiedad idSistema.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getIdSistema() {
        return idSistema;
    }

    /**
     * Define el valor de la propiedad idSistema.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setIdSistema(String value) {
        this.idSistema = value;
    }

    /**
     * Obtiene el valor de la propiedad centralPropia.
     * 
     */
    public boolean isCentralPropia() {
        return centralPropia;
    }

    /**
     * Define el valor de la propiedad centralPropia.
     * 
     */
    public void setCentralPropia(boolean value) {
        this.centralPropia = value;
    }

    /**
     * Obtiene el valor de la propiedad throwswitching.
     * 
     */
    public boolean isThrowswitching() {
        return throwswitching;
    }

    /**
     * Define el valor de la propiedad throwswitching.
     * 
     */
    public void setThrowswitching(boolean value) {
        this.throwswitching = value;
    }

    /**
     * Obtiene el valor de la propiedad numTest.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getNumTest() {
        return numTest;
    }

    /**
     * Define el valor de la propiedad numTest.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setNumTest(String value) {
        this.numTest = value;
    }

    /**
     * Obtiene el valor de la propiedad centralIp.
     * 
     */
    public boolean isCentralIp() {
        return centralIp;
    }

    /**
     * Define el valor de la propiedad centralIp.
     * 
     */
    public void setCentralIp(boolean value) {
        this.centralIp = value;
    }

}

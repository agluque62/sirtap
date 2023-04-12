
package es.cd40;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para Troncales complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="Troncales"&gt;
 *   &lt;complexContent&gt;
 *     &lt;extension base="{http://CD40.es/}Tablas"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="IdTroncal" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="IdSistema" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="NumTest" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/extension&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "Troncales", propOrder = {
    "idTroncal",
    "idSistema",
    "numTest"
})
public class Troncales
    extends Tablas
{

    @XmlElement(name = "IdTroncal")
    protected String idTroncal;
    @XmlElement(name = "IdSistema")
    protected String idSistema;
    @XmlElement(name = "NumTest")
    protected String numTest;

    /**
     * Obtiene el valor de la propiedad idTroncal.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getIdTroncal() {
        return idTroncal;
    }

    /**
     * Define el valor de la propiedad idTroncal.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setIdTroncal(String value) {
        this.idTroncal = value;
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

}


package es.cd40;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlRootElement;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para anonymous complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType&gt;
 *   &lt;complexContent&gt;
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="GetParametrosRecursoByIdResult" type="{http://CD40.es/}Tablas" minOccurs="0"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/restriction&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "", propOrder = {
    "getParametrosRecursoByIdResult"
})
@XmlRootElement(name = "GetParametrosRecursoByIdResponse")
public class GetParametrosRecursoByIdResponse {

    @XmlElement(name = "GetParametrosRecursoByIdResult")
    protected Tablas getParametrosRecursoByIdResult;

    /**
     * Obtiene el valor de la propiedad getParametrosRecursoByIdResult.
     * 
     * @return
     *     possible object is
     *     {@link Tablas }
     *     
     */
    public Tablas getGetParametrosRecursoByIdResult() {
        return getParametrosRecursoByIdResult;
    }

    /**
     * Define el valor de la propiedad getParametrosRecursoByIdResult.
     * 
     * @param value
     *     allowed object is
     *     {@link Tablas }
     *     
     */
    public void setGetParametrosRecursoByIdResult(Tablas value) {
        this.getParametrosRecursoByIdResult = value;
    }

}


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
 *         &lt;element name="GetParametrosRecursoResult" type="{http://CD40.es/}Tablas" minOccurs="0"/&gt;
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
    "getParametrosRecursoResult"
})
@XmlRootElement(name = "GetParametrosRecursoResponse")
public class GetParametrosRecursoResponse {

    @XmlElement(name = "GetParametrosRecursoResult")
    protected Tablas getParametrosRecursoResult;

    /**
     * Obtiene el valor de la propiedad getParametrosRecursoResult.
     * 
     * @return
     *     possible object is
     *     {@link Tablas }
     *     
     */
    public Tablas getGetParametrosRecursoResult() {
        return getParametrosRecursoResult;
    }

    /**
     * Define el valor de la propiedad getParametrosRecursoResult.
     * 
     * @param value
     *     allowed object is
     *     {@link Tablas }
     *     
     */
    public void setGetParametrosRecursoResult(Tablas value) {
        this.getParametrosRecursoResult = value;
    }

}

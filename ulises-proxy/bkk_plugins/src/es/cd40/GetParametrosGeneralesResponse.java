
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
 *         &lt;element name="GetParametrosGeneralesResult" type="{http://CD40.es/}ParametrosGeneralesSistema" minOccurs="0"/&gt;
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
    "getParametrosGeneralesResult"
})
@XmlRootElement(name = "GetParametrosGeneralesResponse")
public class GetParametrosGeneralesResponse {

    @XmlElement(name = "GetParametrosGeneralesResult")
    protected ParametrosGeneralesSistema getParametrosGeneralesResult;

    /**
     * Obtiene el valor de la propiedad getParametrosGeneralesResult.
     * 
     * @return
     *     possible object is
     *     {@link ParametrosGeneralesSistema }
     *     
     */
    public ParametrosGeneralesSistema getGetParametrosGeneralesResult() {
        return getParametrosGeneralesResult;
    }

    /**
     * Define el valor de la propiedad getParametrosGeneralesResult.
     * 
     * @param value
     *     allowed object is
     *     {@link ParametrosGeneralesSistema }
     *     
     */
    public void setGetParametrosGeneralesResult(ParametrosGeneralesSistema value) {
        this.getParametrosGeneralesResult = value;
    }

}

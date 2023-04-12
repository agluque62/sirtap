
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
 *         &lt;element name="GetPlanNumeracionATSResult" type="{http://CD40.es/}ArrayOfNumeracionATS" minOccurs="0"/&gt;
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
    "getPlanNumeracionATSResult"
})
@XmlRootElement(name = "GetPlanNumeracionATSResponse")
public class GetPlanNumeracionATSResponse {

    @XmlElement(name = "GetPlanNumeracionATSResult")
    protected ArrayOfNumeracionATS getPlanNumeracionATSResult;

    /**
     * Obtiene el valor de la propiedad getPlanNumeracionATSResult.
     * 
     * @return
     *     possible object is
     *     {@link ArrayOfNumeracionATS }
     *     
     */
    public ArrayOfNumeracionATS getGetPlanNumeracionATSResult() {
        return getPlanNumeracionATSResult;
    }

    /**
     * Define el valor de la propiedad getPlanNumeracionATSResult.
     * 
     * @param value
     *     allowed object is
     *     {@link ArrayOfNumeracionATS }
     *     
     */
    public void setGetPlanNumeracionATSResult(ArrayOfNumeracionATS value) {
        this.getPlanNumeracionATSResult = value;
    }

}

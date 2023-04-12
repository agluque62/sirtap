
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
 *         &lt;element name="GetPlanTroncalesResult" type="{http://CD40.es/}ArrayOfListaTroncales" minOccurs="0"/&gt;
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
    "getPlanTroncalesResult"
})
@XmlRootElement(name = "GetPlanTroncalesResponse")
public class GetPlanTroncalesResponse {

    @XmlElement(name = "GetPlanTroncalesResult")
    protected ArrayOfListaTroncales getPlanTroncalesResult;

    /**
     * Obtiene el valor de la propiedad getPlanTroncalesResult.
     * 
     * @return
     *     possible object is
     *     {@link ArrayOfListaTroncales }
     *     
     */
    public ArrayOfListaTroncales getGetPlanTroncalesResult() {
        return getPlanTroncalesResult;
    }

    /**
     * Define el valor de la propiedad getPlanTroncalesResult.
     * 
     * @param value
     *     allowed object is
     *     {@link ArrayOfListaTroncales }
     *     
     */
    public void setGetPlanTroncalesResult(ArrayOfListaTroncales value) {
        this.getPlanTroncalesResult = value;
    }

}

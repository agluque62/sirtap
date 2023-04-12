
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
 *         &lt;element name="GetPlanDireccionamientoSIPResult" type="{http://CD40.es/}ArrayOfDireccionamientoSIP" minOccurs="0"/&gt;
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
    "getPlanDireccionamientoSIPResult"
})
@XmlRootElement(name = "GetPlanDireccionamientoSIPResponse")
public class GetPlanDireccionamientoSIPResponse {

    @XmlElement(name = "GetPlanDireccionamientoSIPResult")
    protected ArrayOfDireccionamientoSIP getPlanDireccionamientoSIPResult;

    /**
     * Obtiene el valor de la propiedad getPlanDireccionamientoSIPResult.
     * 
     * @return
     *     possible object is
     *     {@link ArrayOfDireccionamientoSIP }
     *     
     */
    public ArrayOfDireccionamientoSIP getGetPlanDireccionamientoSIPResult() {
        return getPlanDireccionamientoSIPResult;
    }

    /**
     * Define el valor de la propiedad getPlanDireccionamientoSIPResult.
     * 
     * @param value
     *     allowed object is
     *     {@link ArrayOfDireccionamientoSIP }
     *     
     */
    public void setGetPlanDireccionamientoSIPResult(ArrayOfDireccionamientoSIP value) {
        this.getPlanDireccionamientoSIPResult = value;
    }

}

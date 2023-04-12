
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
 *         &lt;element name="GetPlanDireccionamientoIPResult" type="{http://CD40.es/}ArrayOfDireccionamientoIP" minOccurs="0"/&gt;
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
    "getPlanDireccionamientoIPResult"
})
@XmlRootElement(name = "GetPlanDireccionamientoIPResponse")
public class GetPlanDireccionamientoIPResponse {

    @XmlElement(name = "GetPlanDireccionamientoIPResult")
    protected ArrayOfDireccionamientoIP getPlanDireccionamientoIPResult;

    /**
     * Obtiene el valor de la propiedad getPlanDireccionamientoIPResult.
     * 
     * @return
     *     possible object is
     *     {@link ArrayOfDireccionamientoIP }
     *     
     */
    public ArrayOfDireccionamientoIP getGetPlanDireccionamientoIPResult() {
        return getPlanDireccionamientoIPResult;
    }

    /**
     * Define el valor de la propiedad getPlanDireccionamientoIPResult.
     * 
     * @param value
     *     allowed object is
     *     {@link ArrayOfDireccionamientoIP }
     *     
     */
    public void setGetPlanDireccionamientoIPResult(ArrayOfDireccionamientoIP value) {
        this.getPlanDireccionamientoIPResult = value;
    }

}

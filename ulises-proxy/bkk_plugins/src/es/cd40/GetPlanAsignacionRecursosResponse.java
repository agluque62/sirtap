
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
 *         &lt;element name="GetPlanAsignacionRecursosResult" type="{http://CD40.es/}ArrayOfAsignacionRecursosGW" minOccurs="0"/&gt;
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
    "getPlanAsignacionRecursosResult"
})
@XmlRootElement(name = "GetPlanAsignacionRecursosResponse")
public class GetPlanAsignacionRecursosResponse {

    @XmlElement(name = "GetPlanAsignacionRecursosResult")
    protected ArrayOfAsignacionRecursosGW getPlanAsignacionRecursosResult;

    /**
     * Obtiene el valor de la propiedad getPlanAsignacionRecursosResult.
     * 
     * @return
     *     possible object is
     *     {@link ArrayOfAsignacionRecursosGW }
     *     
     */
    public ArrayOfAsignacionRecursosGW getGetPlanAsignacionRecursosResult() {
        return getPlanAsignacionRecursosResult;
    }

    /**
     * Define el valor de la propiedad getPlanAsignacionRecursosResult.
     * 
     * @param value
     *     allowed object is
     *     {@link ArrayOfAsignacionRecursosGW }
     *     
     */
    public void setGetPlanAsignacionRecursosResult(ArrayOfAsignacionRecursosGW value) {
        this.getPlanAsignacionRecursosResult = value;
    }

}

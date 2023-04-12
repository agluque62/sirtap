
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
 *         &lt;element name="GetParametrosMulticastResult" type="{http://CD40.es/}ParametrosMulticast" minOccurs="0"/&gt;
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
    "getParametrosMulticastResult"
})
@XmlRootElement(name = "GetParametrosMulticastResponse")
public class GetParametrosMulticastResponse {

    @XmlElement(name = "GetParametrosMulticastResult")
    protected ParametrosMulticast getParametrosMulticastResult;

    /**
     * Obtiene el valor de la propiedad getParametrosMulticastResult.
     * 
     * @return
     *     possible object is
     *     {@link ParametrosMulticast }
     *     
     */
    public ParametrosMulticast getGetParametrosMulticastResult() {
        return getParametrosMulticastResult;
    }

    /**
     * Define el valor de la propiedad getParametrosMulticastResult.
     * 
     * @param value
     *     allowed object is
     *     {@link ParametrosMulticast }
     *     
     */
    public void setGetParametrosMulticastResult(ParametrosMulticast value) {
        this.getParametrosMulticastResult = value;
    }

}

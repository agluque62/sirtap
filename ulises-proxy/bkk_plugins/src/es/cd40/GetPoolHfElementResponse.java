
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
 *         &lt;element name="GetPoolHfElementResult" type="{http://CD40.es/}ArrayOfPoolHfElement" minOccurs="0"/&gt;
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
    "getPoolHfElementResult"
})
@XmlRootElement(name = "GetPoolHfElementResponse")
public class GetPoolHfElementResponse {

    @XmlElement(name = "GetPoolHfElementResult")
    protected ArrayOfPoolHfElement getPoolHfElementResult;

    /**
     * Obtiene el valor de la propiedad getPoolHfElementResult.
     * 
     * @return
     *     possible object is
     *     {@link ArrayOfPoolHfElement }
     *     
     */
    public ArrayOfPoolHfElement getGetPoolHfElementResult() {
        return getPoolHfElementResult;
    }

    /**
     * Define el valor de la propiedad getPoolHfElementResult.
     * 
     * @param value
     *     allowed object is
     *     {@link ArrayOfPoolHfElement }
     *     
     */
    public void setGetPoolHfElementResult(ArrayOfPoolHfElement value) {
        this.getPoolHfElementResult = value;
    }

}

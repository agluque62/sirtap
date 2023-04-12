
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
 *         &lt;element name="GetScvActivoResult" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
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
    "getScvActivoResult"
})
@XmlRootElement(name = "GetScvActivoResponse")
public class GetScvActivoResponse {

    @XmlElement(name = "GetScvActivoResult")
    protected String getScvActivoResult;

    /**
     * Obtiene el valor de la propiedad getScvActivoResult.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getGetScvActivoResult() {
        return getScvActivoResult;
    }

    /**
     * Define el valor de la propiedad getScvActivoResult.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setGetScvActivoResult(String value) {
        this.getScvActivoResult = value;
    }

}

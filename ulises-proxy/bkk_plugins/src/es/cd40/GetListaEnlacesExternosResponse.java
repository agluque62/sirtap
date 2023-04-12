
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
 *         &lt;element name="GetListaEnlacesExternosResult" type="{http://CD40.es/}ArrayOfCfgEnlaceExterno" minOccurs="0"/&gt;
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
    "getListaEnlacesExternosResult"
})
@XmlRootElement(name = "GetListaEnlacesExternosResponse")
public class GetListaEnlacesExternosResponse {

    @XmlElement(name = "GetListaEnlacesExternosResult")
    protected ArrayOfCfgEnlaceExterno getListaEnlacesExternosResult;

    /**
     * Obtiene el valor de la propiedad getListaEnlacesExternosResult.
     * 
     * @return
     *     possible object is
     *     {@link ArrayOfCfgEnlaceExterno }
     *     
     */
    public ArrayOfCfgEnlaceExterno getGetListaEnlacesExternosResult() {
        return getListaEnlacesExternosResult;
    }

    /**
     * Define el valor de la propiedad getListaEnlacesExternosResult.
     * 
     * @param value
     *     allowed object is
     *     {@link ArrayOfCfgEnlaceExterno }
     *     
     */
    public void setGetListaEnlacesExternosResult(ArrayOfCfgEnlaceExterno value) {
        this.getListaEnlacesExternosResult = value;
    }

}

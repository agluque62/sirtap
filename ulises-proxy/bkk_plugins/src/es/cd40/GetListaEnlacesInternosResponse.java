
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
 *         &lt;element name="GetListaEnlacesInternosResult" type="{http://CD40.es/}ArrayOfCfgEnlaceInterno" minOccurs="0"/&gt;
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
    "getListaEnlacesInternosResult"
})
@XmlRootElement(name = "GetListaEnlacesInternosResponse")
public class GetListaEnlacesInternosResponse {

    @XmlElement(name = "GetListaEnlacesInternosResult")
    protected ArrayOfCfgEnlaceInterno getListaEnlacesInternosResult;

    /**
     * Obtiene el valor de la propiedad getListaEnlacesInternosResult.
     * 
     * @return
     *     possible object is
     *     {@link ArrayOfCfgEnlaceInterno }
     *     
     */
    public ArrayOfCfgEnlaceInterno getGetListaEnlacesInternosResult() {
        return getListaEnlacesInternosResult;
    }

    /**
     * Define el valor de la propiedad getListaEnlacesInternosResult.
     * 
     * @param value
     *     allowed object is
     *     {@link ArrayOfCfgEnlaceInterno }
     *     
     */
    public void setGetListaEnlacesInternosResult(ArrayOfCfgEnlaceInterno value) {
        this.getListaEnlacesInternosResult = value;
    }

}

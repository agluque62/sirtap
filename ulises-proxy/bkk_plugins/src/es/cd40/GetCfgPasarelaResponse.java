
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
 *         &lt;element name="GetCfgPasarelaResult" type="{http://CD40.es/}CfgPasarela" minOccurs="0"/&gt;
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
    "getCfgPasarelaResult"
})
@XmlRootElement(name = "GetCfgPasarelaResponse")
public class GetCfgPasarelaResponse {

    @XmlElement(name = "GetCfgPasarelaResult")
    protected CfgPasarela getCfgPasarelaResult;

    /**
     * Obtiene el valor de la propiedad getCfgPasarelaResult.
     * 
     * @return
     *     possible object is
     *     {@link CfgPasarela }
     *     
     */
    public CfgPasarela getGetCfgPasarelaResult() {
        return getCfgPasarelaResult;
    }

    /**
     * Define el valor de la propiedad getCfgPasarelaResult.
     * 
     * @param value
     *     allowed object is
     *     {@link CfgPasarela }
     *     
     */
    public void setGetCfgPasarelaResult(CfgPasarela value) {
        this.getCfgPasarelaResult = value;
    }

}

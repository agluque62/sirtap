
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
 *         &lt;element name="GetCfgUsuarioResult" type="{http://CD40.es/}CfgUsuario" minOccurs="0"/&gt;
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
    "getCfgUsuarioResult"
})
@XmlRootElement(name = "GetCfgUsuarioResponse")
public class GetCfgUsuarioResponse {

    @XmlElement(name = "GetCfgUsuarioResult")
    protected CfgUsuario getCfgUsuarioResult;

    /**
     * Obtiene el valor de la propiedad getCfgUsuarioResult.
     * 
     * @return
     *     possible object is
     *     {@link CfgUsuario }
     *     
     */
    public CfgUsuario getGetCfgUsuarioResult() {
        return getCfgUsuarioResult;
    }

    /**
     * Define el valor de la propiedad getCfgUsuarioResult.
     * 
     * @param value
     *     allowed object is
     *     {@link CfgUsuario }
     *     
     */
    public void setGetCfgUsuarioResult(CfgUsuario value) {
        this.getCfgUsuarioResult = value;
    }

}

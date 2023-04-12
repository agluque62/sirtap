
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
 *         &lt;element name="GetConfigSistemaResult" type="{http://CD40.es/}ConfiguracionSistema" minOccurs="0"/&gt;
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
    "getConfigSistemaResult"
})
@XmlRootElement(name = "GetConfigSistemaResponse")
public class GetConfigSistemaResponse {

    @XmlElement(name = "GetConfigSistemaResult")
    protected ConfiguracionSistema getConfigSistemaResult;

    /**
     * Obtiene el valor de la propiedad getConfigSistemaResult.
     * 
     * @return
     *     possible object is
     *     {@link ConfiguracionSistema }
     *     
     */
    public ConfiguracionSistema getGetConfigSistemaResult() {
        return getConfigSistemaResult;
    }

    /**
     * Define el valor de la propiedad getConfigSistemaResult.
     * 
     * @param value
     *     allowed object is
     *     {@link ConfiguracionSistema }
     *     
     */
    public void setGetConfigSistemaResult(ConfiguracionSistema value) {
        this.getConfigSistemaResult = value;
    }

}

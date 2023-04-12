
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
 *         &lt;element name="LoginTopResult" type="{http://CD40.es/}LoginTerminalVoz" minOccurs="0"/&gt;
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
    "loginTopResult"
})
@XmlRootElement(name = "LoginTopResponse")
public class LoginTopResponse {

    @XmlElement(name = "LoginTopResult")
    protected LoginTerminalVoz loginTopResult;

    /**
     * Obtiene el valor de la propiedad loginTopResult.
     * 
     * @return
     *     possible object is
     *     {@link LoginTerminalVoz }
     *     
     */
    public LoginTerminalVoz getLoginTopResult() {
        return loginTopResult;
    }

    /**
     * Define el valor de la propiedad loginTopResult.
     * 
     * @param value
     *     allowed object is
     *     {@link LoginTerminalVoz }
     *     
     */
    public void setLoginTopResult(LoginTerminalVoz value) {
        this.loginTopResult = value;
    }

}


package es.cd40;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para LoginTerminalVoz complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="LoginTerminalVoz"&gt;
 *   &lt;complexContent&gt;
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="ModoLogin" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="IdUsuario" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/restriction&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "LoginTerminalVoz", propOrder = {
    "modoLogin",
    "idUsuario"
})
public class LoginTerminalVoz {

    @XmlElement(name = "ModoLogin")
    protected String modoLogin;
    @XmlElement(name = "IdUsuario")
    protected String idUsuario;

    /**
     * Obtiene el valor de la propiedad modoLogin.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getModoLogin() {
        return modoLogin;
    }

    /**
     * Define el valor de la propiedad modoLogin.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setModoLogin(String value) {
        this.modoLogin = value;
    }

    /**
     * Obtiene el valor de la propiedad idUsuario.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getIdUsuario() {
        return idUsuario;
    }

    /**
     * Define el valor de la propiedad idUsuario.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setIdUsuario(String value) {
        this.idUsuario = value;
    }

}

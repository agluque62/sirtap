
package es.cd40;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlSeeAlso;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para PermisosRedesSCV complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="PermisosRedesSCV"&gt;
 *   &lt;complexContent&gt;
 *     &lt;extension base="{http://CD40.es/}Tablas"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="IdRed" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="Llamar" type="{http://www.w3.org/2001/XMLSchema}boolean"/&gt;
 *         &lt;element name="Recibir" type="{http://www.w3.org/2001/XMLSchema}boolean"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/extension&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "PermisosRedesSCV", propOrder = {
    "idRed",
    "llamar",
    "recibir"
})
@XmlSeeAlso({
    PermisosRedes.class
})
public class PermisosRedesSCV
    extends Tablas
{

    @XmlElement(name = "IdRed")
    protected String idRed;
    @XmlElement(name = "Llamar")
    protected boolean llamar;
    @XmlElement(name = "Recibir")
    protected boolean recibir;

    /**
     * Obtiene el valor de la propiedad idRed.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getIdRed() {
        return idRed;
    }

    /**
     * Define el valor de la propiedad idRed.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setIdRed(String value) {
        this.idRed = value;
    }

    /**
     * Obtiene el valor de la propiedad llamar.
     * 
     */
    public boolean isLlamar() {
        return llamar;
    }

    /**
     * Define el valor de la propiedad llamar.
     * 
     */
    public void setLlamar(boolean value) {
        this.llamar = value;
    }

    /**
     * Obtiene el valor de la propiedad recibir.
     * 
     */
    public boolean isRecibir() {
        return recibir;
    }

    /**
     * Define el valor de la propiedad recibir.
     * 
     */
    public void setRecibir(boolean value) {
        this.recibir = value;
    }

}

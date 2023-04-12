
package es.cd40;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlSchemaType;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para RecursosExternos complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="RecursosExternos"&gt;
 *   &lt;complexContent&gt;
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="Nombre" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="Tipo" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="ModoConfirmacionPtt" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="TipoPtt" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/restriction&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "RecursosExternos", propOrder = {
    "nombre",
    "tipo",
    "modoConfirmacionPtt",
    "tipoPtt"
})
public class RecursosExternos {

    @XmlElement(name = "Nombre")
    protected String nombre;
    @XmlElement(name = "Tipo")
    protected String tipo;
    @XmlElement(name = "ModoConfirmacionPtt")
    @XmlSchemaType(name = "unsignedInt")
    protected long modoConfirmacionPtt;
    @XmlElement(name = "TipoPtt")
    @XmlSchemaType(name = "unsignedInt")
    protected long tipoPtt;

    /**
     * Obtiene el valor de la propiedad nombre.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getNombre() {
        return nombre;
    }

    /**
     * Define el valor de la propiedad nombre.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setNombre(String value) {
        this.nombre = value;
    }

    /**
     * Obtiene el valor de la propiedad tipo.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getTipo() {
        return tipo;
    }

    /**
     * Define el valor de la propiedad tipo.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setTipo(String value) {
        this.tipo = value;
    }

    /**
     * Obtiene el valor de la propiedad modoConfirmacionPtt.
     * 
     */
    public long getModoConfirmacionPtt() {
        return modoConfirmacionPtt;
    }

    /**
     * Define el valor de la propiedad modoConfirmacionPtt.
     * 
     */
    public void setModoConfirmacionPtt(long value) {
        this.modoConfirmacionPtt = value;
    }

    /**
     * Obtiene el valor de la propiedad tipoPtt.
     * 
     */
    public long getTipoPtt() {
        return tipoPtt;
    }

    /**
     * Define el valor de la propiedad tipoPtt.
     * 
     */
    public void setTipoPtt(long value) {
        this.tipoPtt = value;
    }

}

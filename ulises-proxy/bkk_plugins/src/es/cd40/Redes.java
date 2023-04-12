
package es.cd40;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlSchemaType;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para Redes complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="Redes"&gt;
 *   &lt;complexContent&gt;
 *     &lt;extension base="{http://CD40.es/}Tablas"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="IdRed" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="IdSistema" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="Prefijo" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/extension&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "Redes", propOrder = {
    "idRed",
    "idSistema",
    "prefijo"
})
public class Redes
    extends Tablas
{

    @XmlElement(name = "IdRed")
    protected String idRed;
    @XmlElement(name = "IdSistema")
    protected String idSistema;
    @XmlElement(name = "Prefijo")
    @XmlSchemaType(name = "unsignedInt")
    protected long prefijo;

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
     * Obtiene el valor de la propiedad idSistema.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getIdSistema() {
        return idSistema;
    }

    /**
     * Define el valor de la propiedad idSistema.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setIdSistema(String value) {
        this.idSistema = value;
    }

    /**
     * Obtiene el valor de la propiedad prefijo.
     * 
     */
    public long getPrefijo() {
        return prefijo;
    }

    /**
     * Define el valor de la propiedad prefijo.
     * 
     */
    public void setPrefijo(long value) {
        this.prefijo = value;
    }

}


package es.cd40;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para Rutas complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="Rutas"&gt;
 *   &lt;complexContent&gt;
 *     &lt;extension base="{http://CD40.es/}Tablas"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="IdSistema" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="Central" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="IdRuta" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="Tipo" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="Orden" type="{http://www.w3.org/2001/XMLSchema}int"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/extension&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "Rutas", propOrder = {
    "idSistema",
    "central",
    "idRuta",
    "tipo",
    "orden"
})
public class Rutas
    extends Tablas
{

    @XmlElement(name = "IdSistema")
    protected String idSistema;
    @XmlElement(name = "Central")
    protected String central;
    @XmlElement(name = "IdRuta")
    protected String idRuta;
    @XmlElement(name = "Tipo")
    protected String tipo;
    @XmlElement(name = "Orden")
    protected int orden;

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
     * Obtiene el valor de la propiedad central.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getCentral() {
        return central;
    }

    /**
     * Define el valor de la propiedad central.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setCentral(String value) {
        this.central = value;
    }

    /**
     * Obtiene el valor de la propiedad idRuta.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getIdRuta() {
        return idRuta;
    }

    /**
     * Define el valor de la propiedad idRuta.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setIdRuta(String value) {
        this.idRuta = value;
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
     * Obtiene el valor de la propiedad orden.
     * 
     */
    public int getOrden() {
        return orden;
    }

    /**
     * Define el valor de la propiedad orden.
     * 
     */
    public void setOrden(int value) {
        this.orden = value;
    }

}

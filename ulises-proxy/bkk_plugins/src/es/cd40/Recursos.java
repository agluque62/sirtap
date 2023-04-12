
package es.cd40;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlSchemaType;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para Recursos complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="Recursos"&gt;
 *   &lt;complexContent&gt;
 *     &lt;extension base="{http://CD40.es/}RecursosSCV"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="IdSistema" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="TipoRecurso" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="IdEquipo" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="IdTifX" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="PuertoSip" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/extension&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "Recursos", propOrder = {
    "idSistema",
    "tipoRecurso",
    "idEquipo",
    "idTifX",
    "puertoSip"
})
public class Recursos
    extends RecursosSCV
{

    @XmlElement(name = "IdSistema")
    protected String idSistema;
    @XmlElement(name = "TipoRecurso")
    @XmlSchemaType(name = "unsignedInt")
    protected long tipoRecurso;
    @XmlElement(name = "IdEquipo")
    protected String idEquipo;
    @XmlElement(name = "IdTifX")
    protected String idTifX;
    @XmlElement(name = "PuertoSip")
    @XmlSchemaType(name = "unsignedInt")
    protected long puertoSip;

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
     * Obtiene el valor de la propiedad tipoRecurso.
     * 
     */
    public long getTipoRecurso() {
        return tipoRecurso;
    }

    /**
     * Define el valor de la propiedad tipoRecurso.
     * 
     */
    public void setTipoRecurso(long value) {
        this.tipoRecurso = value;
    }

    /**
     * Obtiene el valor de la propiedad idEquipo.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getIdEquipo() {
        return idEquipo;
    }

    /**
     * Define el valor de la propiedad idEquipo.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setIdEquipo(String value) {
        this.idEquipo = value;
    }

    /**
     * Obtiene el valor de la propiedad idTifX.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getIdTifX() {
        return idTifX;
    }

    /**
     * Define el valor de la propiedad idTifX.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setIdTifX(String value) {
        this.idTifX = value;
    }

    /**
     * Obtiene el valor de la propiedad puertoSip.
     * 
     */
    public long getPuertoSip() {
        return puertoSip;
    }

    /**
     * Define el valor de la propiedad puertoSip.
     * 
     */
    public void setPuertoSip(long value) {
        this.puertoSip = value;
    }

}

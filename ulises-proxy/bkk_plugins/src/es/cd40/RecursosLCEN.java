
package es.cd40;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlSchemaType;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para RecursosLCEN complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="RecursosLCEN"&gt;
 *   &lt;complexContent&gt;
 *     &lt;extension base="{http://CD40.es/}ParametrosRecursoLCEN"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="IdRecurso" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="IdSistema" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="IdPrefijo" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="TipoDestino" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="IdDestino" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="isuperv_options" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="itm_superv_options" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/extension&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "RecursosLCEN", propOrder = {
    "idRecurso",
    "idSistema",
    "idPrefijo",
    "tipoDestino",
    "idDestino",
    "isupervOptions",
    "itmSupervOptions"
})
public class RecursosLCEN
    extends ParametrosRecursoLCEN
{

    @XmlElement(name = "IdRecurso")
    protected String idRecurso;
    @XmlElement(name = "IdSistema")
    protected String idSistema;
    @XmlElement(name = "IdPrefijo")
    @XmlSchemaType(name = "unsignedInt")
    protected long idPrefijo;
    @XmlElement(name = "TipoDestino")
    @XmlSchemaType(name = "unsignedInt")
    protected long tipoDestino;
    @XmlElement(name = "IdDestino")
    protected String idDestino;
    @XmlElement(name = "isuperv_options")
    @XmlSchemaType(name = "unsignedInt")
    protected long isupervOptions;
    @XmlElement(name = "itm_superv_options")
    @XmlSchemaType(name = "unsignedInt")
    protected long itmSupervOptions;

    /**
     * Obtiene el valor de la propiedad idRecurso.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getIdRecurso() {
        return idRecurso;
    }

    /**
     * Define el valor de la propiedad idRecurso.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setIdRecurso(String value) {
        this.idRecurso = value;
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
     * Obtiene el valor de la propiedad idPrefijo.
     * 
     */
    public long getIdPrefijo() {
        return idPrefijo;
    }

    /**
     * Define el valor de la propiedad idPrefijo.
     * 
     */
    public void setIdPrefijo(long value) {
        this.idPrefijo = value;
    }

    /**
     * Obtiene el valor de la propiedad tipoDestino.
     * 
     */
    public long getTipoDestino() {
        return tipoDestino;
    }

    /**
     * Define el valor de la propiedad tipoDestino.
     * 
     */
    public void setTipoDestino(long value) {
        this.tipoDestino = value;
    }

    /**
     * Obtiene el valor de la propiedad idDestino.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getIdDestino() {
        return idDestino;
    }

    /**
     * Define el valor de la propiedad idDestino.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setIdDestino(String value) {
        this.idDestino = value;
    }

    /**
     * Obtiene el valor de la propiedad isupervOptions.
     * 
     */
    public long getIsupervOptions() {
        return isupervOptions;
    }

    /**
     * Define el valor de la propiedad isupervOptions.
     * 
     */
    public void setIsupervOptions(long value) {
        this.isupervOptions = value;
    }

    /**
     * Obtiene el valor de la propiedad itmSupervOptions.
     * 
     */
    public long getItmSupervOptions() {
        return itmSupervOptions;
    }

    /**
     * Define el valor de la propiedad itmSupervOptions.
     * 
     */
    public void setItmSupervOptions(long value) {
        this.itmSupervOptions = value;
    }

}

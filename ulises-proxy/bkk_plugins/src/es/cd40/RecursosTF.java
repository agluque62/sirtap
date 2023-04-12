
package es.cd40;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlSchemaType;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para RecursosTF complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="RecursosTF"&gt;
 *   &lt;complexContent&gt;
 *     &lt;extension base="{http://CD40.es/}ParametrosGrabacionTf"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="IdSistema" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="IdRecurso" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="IdPrefijo" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="TipoDestino" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="IdDestino" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="IdTroncal" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="IdRed" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/extension&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "RecursosTF", propOrder = {
    "idSistema",
    "idRecurso",
    "idPrefijo",
    "tipoDestino",
    "idDestino",
    "idTroncal",
    "idRed"
})
public class RecursosTF
    extends ParametrosGrabacionTf
{

    @XmlElement(name = "IdSistema")
    protected String idSistema;
    @XmlElement(name = "IdRecurso")
    protected String idRecurso;
    @XmlElement(name = "IdPrefijo")
    @XmlSchemaType(name = "unsignedInt")
    protected long idPrefijo;
    @XmlElement(name = "TipoDestino")
    @XmlSchemaType(name = "unsignedInt")
    protected long tipoDestino;
    @XmlElement(name = "IdDestino")
    protected String idDestino;
    @XmlElement(name = "IdTroncal")
    protected String idTroncal;
    @XmlElement(name = "IdRed")
    protected String idRed;

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
     * Obtiene el valor de la propiedad idTroncal.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getIdTroncal() {
        return idTroncal;
    }

    /**
     * Define el valor de la propiedad idTroncal.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setIdTroncal(String value) {
        this.idTroncal = value;
    }

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

}


package es.cd40;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlSchemaType;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para EstadosRecursos complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="EstadosRecursos"&gt;
 *   &lt;complexContent&gt;
 *     &lt;extension base="{http://CD40.es/}Tablas"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="IdSistema" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="IdSector" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="IdNucleo" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="PosHMI" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="IdRecurso" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="TipoRecurso" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="IdDestino" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="TipoDestino" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="Estado" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="DescDestino" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/extension&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "EstadosRecursos", propOrder = {
    "idSistema",
    "idSector",
    "idNucleo",
    "posHMI",
    "idRecurso",
    "tipoRecurso",
    "idDestino",
    "tipoDestino",
    "estado",
    "descDestino"
})
public class EstadosRecursos
    extends Tablas
{

    @XmlElement(name = "IdSistema")
    protected String idSistema;
    @XmlElement(name = "IdSector")
    protected String idSector;
    @XmlElement(name = "IdNucleo")
    protected String idNucleo;
    @XmlElement(name = "PosHMI")
    @XmlSchemaType(name = "unsignedInt")
    protected long posHMI;
    @XmlElement(name = "IdRecurso")
    protected String idRecurso;
    @XmlElement(name = "TipoRecurso")
    @XmlSchemaType(name = "unsignedInt")
    protected long tipoRecurso;
    @XmlElement(name = "IdDestino")
    protected String idDestino;
    @XmlElement(name = "TipoDestino")
    @XmlSchemaType(name = "unsignedInt")
    protected long tipoDestino;
    @XmlElement(name = "Estado")
    protected String estado;
    @XmlElement(name = "DescDestino")
    protected String descDestino;

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
     * Obtiene el valor de la propiedad idSector.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getIdSector() {
        return idSector;
    }

    /**
     * Define el valor de la propiedad idSector.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setIdSector(String value) {
        this.idSector = value;
    }

    /**
     * Obtiene el valor de la propiedad idNucleo.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getIdNucleo() {
        return idNucleo;
    }

    /**
     * Define el valor de la propiedad idNucleo.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setIdNucleo(String value) {
        this.idNucleo = value;
    }

    /**
     * Obtiene el valor de la propiedad posHMI.
     * 
     */
    public long getPosHMI() {
        return posHMI;
    }

    /**
     * Define el valor de la propiedad posHMI.
     * 
     */
    public void setPosHMI(long value) {
        this.posHMI = value;
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
     * Obtiene el valor de la propiedad estado.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getEstado() {
        return estado;
    }

    /**
     * Define el valor de la propiedad estado.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setEstado(String value) {
        this.estado = value;
    }

    /**
     * Obtiene el valor de la propiedad descDestino.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getDescDestino() {
        return descDestino;
    }

    /**
     * Define el valor de la propiedad descDestino.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setDescDestino(String value) {
        this.descDestino = value;
    }

}

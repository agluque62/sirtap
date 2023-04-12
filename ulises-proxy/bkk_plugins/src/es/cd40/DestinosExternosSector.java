
package es.cd40;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlSchemaType;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para DestinosExternosSector complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="DestinosExternosSector"&gt;
 *   &lt;complexContent&gt;
 *     &lt;extension base="{http://CD40.es/}Tablas"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="IdSistema" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="IdDestino" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="TipoDestino" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="IdNucleo" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="IdSector" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="IdPrefijo" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="PosHMI" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="Prioridad" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="OrigenR2" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="PrioridadSIP" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="TipoAcceso" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="Literal" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="IdPrefijoDestinoLCEN" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="IdDestinoLCEN" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/extension&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "DestinosExternosSector", propOrder = {
    "idSistema",
    "idDestino",
    "tipoDestino",
    "idNucleo",
    "idSector",
    "idPrefijo",
    "posHMI",
    "prioridad",
    "origenR2",
    "prioridadSIP",
    "tipoAcceso",
    "literal",
    "idPrefijoDestinoLCEN",
    "idDestinoLCEN"
})
public class DestinosExternosSector
    extends Tablas
{

    @XmlElement(name = "IdSistema")
    protected String idSistema;
    @XmlElement(name = "IdDestino")
    protected String idDestino;
    @XmlElement(name = "TipoDestino")
    @XmlSchemaType(name = "unsignedInt")
    protected long tipoDestino;
    @XmlElement(name = "IdNucleo")
    protected String idNucleo;
    @XmlElement(name = "IdSector")
    protected String idSector;
    @XmlElement(name = "IdPrefijo")
    @XmlSchemaType(name = "unsignedInt")
    protected long idPrefijo;
    @XmlElement(name = "PosHMI")
    @XmlSchemaType(name = "unsignedInt")
    protected long posHMI;
    @XmlElement(name = "Prioridad")
    @XmlSchemaType(name = "unsignedInt")
    protected long prioridad;
    @XmlElement(name = "OrigenR2")
    protected String origenR2;
    @XmlElement(name = "PrioridadSIP")
    @XmlSchemaType(name = "unsignedInt")
    protected long prioridadSIP;
    @XmlElement(name = "TipoAcceso")
    protected String tipoAcceso;
    @XmlElement(name = "Literal")
    protected String literal;
    @XmlElement(name = "IdPrefijoDestinoLCEN", required = true, type = Long.class, nillable = true)
    @XmlSchemaType(name = "unsignedInt")
    protected Long idPrefijoDestinoLCEN;
    @XmlElement(name = "IdDestinoLCEN")
    protected String idDestinoLCEN;

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
     * Obtiene el valor de la propiedad prioridad.
     * 
     */
    public long getPrioridad() {
        return prioridad;
    }

    /**
     * Define el valor de la propiedad prioridad.
     * 
     */
    public void setPrioridad(long value) {
        this.prioridad = value;
    }

    /**
     * Obtiene el valor de la propiedad origenR2.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getOrigenR2() {
        return origenR2;
    }

    /**
     * Define el valor de la propiedad origenR2.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setOrigenR2(String value) {
        this.origenR2 = value;
    }

    /**
     * Obtiene el valor de la propiedad prioridadSIP.
     * 
     */
    public long getPrioridadSIP() {
        return prioridadSIP;
    }

    /**
     * Define el valor de la propiedad prioridadSIP.
     * 
     */
    public void setPrioridadSIP(long value) {
        this.prioridadSIP = value;
    }

    /**
     * Obtiene el valor de la propiedad tipoAcceso.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getTipoAcceso() {
        return tipoAcceso;
    }

    /**
     * Define el valor de la propiedad tipoAcceso.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setTipoAcceso(String value) {
        this.tipoAcceso = value;
    }

    /**
     * Obtiene el valor de la propiedad literal.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getLiteral() {
        return literal;
    }

    /**
     * Define el valor de la propiedad literal.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setLiteral(String value) {
        this.literal = value;
    }

    /**
     * Obtiene el valor de la propiedad idPrefijoDestinoLCEN.
     * 
     * @return
     *     possible object is
     *     {@link Long }
     *     
     */
    public Long getIdPrefijoDestinoLCEN() {
        return idPrefijoDestinoLCEN;
    }

    /**
     * Define el valor de la propiedad idPrefijoDestinoLCEN.
     * 
     * @param value
     *     allowed object is
     *     {@link Long }
     *     
     */
    public void setIdPrefijoDestinoLCEN(Long value) {
        this.idPrefijoDestinoLCEN = value;
    }

    /**
     * Obtiene el valor de la propiedad idDestinoLCEN.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getIdDestinoLCEN() {
        return idDestinoLCEN;
    }

    /**
     * Define el valor de la propiedad idDestinoLCEN.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setIdDestinoLCEN(String value) {
        this.idDestinoLCEN = value;
    }

}

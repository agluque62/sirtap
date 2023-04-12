
package es.cd40;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlSchemaType;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para Radio complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="Radio"&gt;
 *   &lt;complexContent&gt;
 *     &lt;extension base="{http://CD40.es/}Tablas"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="IdSistema" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="IdSectorizacion" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="IdNucleo" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="IdSector" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="PosHMI" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="IdDestino" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="Prioridad" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="PrioridadSIP" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="ModoOperacion" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="Cascos" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="Literal" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="SupervisionPortadora" type="{http://www.w3.org/2001/XMLSchema}boolean"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/extension&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "Radio", propOrder = {
    "idSistema",
    "idSectorizacion",
    "idNucleo",
    "idSector",
    "posHMI",
    "idDestino",
    "prioridad",
    "prioridadSIP",
    "modoOperacion",
    "cascos",
    "literal",
    "supervisionPortadora"
})
public class Radio
    extends Tablas
{

    @XmlElement(name = "IdSistema")
    protected String idSistema;
    @XmlElement(name = "IdSectorizacion")
    protected String idSectorizacion;
    @XmlElement(name = "IdNucleo")
    protected String idNucleo;
    @XmlElement(name = "IdSector")
    protected String idSector;
    @XmlElement(name = "PosHMI")
    @XmlSchemaType(name = "unsignedInt")
    protected long posHMI;
    @XmlElement(name = "IdDestino")
    protected String idDestino;
    @XmlElement(name = "Prioridad")
    @XmlSchemaType(name = "unsignedInt")
    protected long prioridad;
    @XmlElement(name = "PrioridadSIP")
    @XmlSchemaType(name = "unsignedInt")
    protected long prioridadSIP;
    @XmlElement(name = "ModoOperacion")
    protected String modoOperacion;
    @XmlElement(name = "Cascos")
    protected String cascos;
    @XmlElement(name = "Literal")
    protected String literal;
    @XmlElement(name = "SupervisionPortadora")
    protected boolean supervisionPortadora;

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
     * Obtiene el valor de la propiedad idSectorizacion.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getIdSectorizacion() {
        return idSectorizacion;
    }

    /**
     * Define el valor de la propiedad idSectorizacion.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setIdSectorizacion(String value) {
        this.idSectorizacion = value;
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
     * Obtiene el valor de la propiedad modoOperacion.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getModoOperacion() {
        return modoOperacion;
    }

    /**
     * Define el valor de la propiedad modoOperacion.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setModoOperacion(String value) {
        this.modoOperacion = value;
    }

    /**
     * Obtiene el valor de la propiedad cascos.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getCascos() {
        return cascos;
    }

    /**
     * Define el valor de la propiedad cascos.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setCascos(String value) {
        this.cascos = value;
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
     * Obtiene el valor de la propiedad supervisionPortadora.
     * 
     */
    public boolean isSupervisionPortadora() {
        return supervisionPortadora;
    }

    /**
     * Define el valor de la propiedad supervisionPortadora.
     * 
     */
    public void setSupervisionPortadora(boolean value) {
        this.supervisionPortadora = value;
    }

}

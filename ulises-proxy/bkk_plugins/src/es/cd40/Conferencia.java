
package es.cd40;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para Conferencia complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="Conferencia"&gt;
 *   &lt;complexContent&gt;
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="IdSalaBkk" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="TipoConferencia" type="{http://www.w3.org/2001/XMLSchema}int"/&gt;
 *         &lt;element name="Alias" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="PosHMI" type="{http://www.w3.org/2001/XMLSchema}int"/&gt;
 *         &lt;element name="ParticipantesConferencia" type="{http://CD40.es/}ArrayOfParticipantes" minOccurs="0"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/restriction&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "Conferencia", propOrder = {
    "idSalaBkk",
    "tipoConferencia",
    "alias",
    "posHMI",
    "participantesConferencia"
})
public class Conferencia {

    @XmlElement(name = "IdSalaBkk")
    protected String idSalaBkk;
    @XmlElement(name = "TipoConferencia")
    protected int tipoConferencia;
    @XmlElement(name = "Alias")
    protected String alias;
    @XmlElement(name = "PosHMI")
    protected int posHMI;
    @XmlElement(name = "ParticipantesConferencia")
    protected ArrayOfParticipantes participantesConferencia;

    /**
     * Obtiene el valor de la propiedad idSalaBkk.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getIdSalaBkk() {
        return idSalaBkk;
    }

    /**
     * Define el valor de la propiedad idSalaBkk.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setIdSalaBkk(String value) {
        this.idSalaBkk = value;
    }

    /**
     * Obtiene el valor de la propiedad tipoConferencia.
     * 
     */
    public int getTipoConferencia() {
        return tipoConferencia;
    }

    /**
     * Define el valor de la propiedad tipoConferencia.
     * 
     */
    public void setTipoConferencia(int value) {
        this.tipoConferencia = value;
    }

    /**
     * Obtiene el valor de la propiedad alias.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getAlias() {
        return alias;
    }

    /**
     * Define el valor de la propiedad alias.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setAlias(String value) {
        this.alias = value;
    }

    /**
     * Obtiene el valor de la propiedad posHMI.
     * 
     */
    public int getPosHMI() {
        return posHMI;
    }

    /**
     * Define el valor de la propiedad posHMI.
     * 
     */
    public void setPosHMI(int value) {
        this.posHMI = value;
    }

    /**
     * Obtiene el valor de la propiedad participantesConferencia.
     * 
     * @return
     *     possible object is
     *     {@link ArrayOfParticipantes }
     *     
     */
    public ArrayOfParticipantes getParticipantesConferencia() {
        return participantesConferencia;
    }

    /**
     * Define el valor de la propiedad participantesConferencia.
     * 
     * @param value
     *     allowed object is
     *     {@link ArrayOfParticipantes }
     *     
     */
    public void setParticipantesConferencia(ArrayOfParticipantes value) {
        this.participantesConferencia = value;
    }

}

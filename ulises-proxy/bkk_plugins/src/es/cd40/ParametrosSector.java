
package es.cd40;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlSchemaType;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para ParametrosSector complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="ParametrosSector"&gt;
 *   &lt;complexContent&gt;
 *     &lt;extension base="{http://CD40.es/}ParametrosSectorRecording"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="IdSector" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="IdNucleo" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="IdSistema" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="NumDestinosInternosPag" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="NumPagDestinosInt" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/extension&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "ParametrosSector", propOrder = {
    "idSector",
    "idNucleo",
    "idSistema",
    "numDestinosInternosPag",
    "numPagDestinosInt"
})
public class ParametrosSector
    extends ParametrosSectorRecording
{

    @XmlElement(name = "IdSector")
    protected String idSector;
    @XmlElement(name = "IdNucleo")
    protected String idNucleo;
    @XmlElement(name = "IdSistema")
    protected String idSistema;
    @XmlElement(name = "NumDestinosInternosPag")
    @XmlSchemaType(name = "unsignedInt")
    protected long numDestinosInternosPag;
    @XmlElement(name = "NumPagDestinosInt")
    @XmlSchemaType(name = "unsignedInt")
    protected long numPagDestinosInt;

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
     * Obtiene el valor de la propiedad numDestinosInternosPag.
     * 
     */
    public long getNumDestinosInternosPag() {
        return numDestinosInternosPag;
    }

    /**
     * Define el valor de la propiedad numDestinosInternosPag.
     * 
     */
    public void setNumDestinosInternosPag(long value) {
        this.numDestinosInternosPag = value;
    }

    /**
     * Obtiene el valor de la propiedad numPagDestinosInt.
     * 
     */
    public long getNumPagDestinosInt() {
        return numPagDestinosInt;
    }

    /**
     * Define el valor de la propiedad numPagDestinosInt.
     * 
     */
    public void setNumPagDestinosInt(long value) {
        this.numPagDestinosInt = value;
    }

}


package es.cd40;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlSchemaType;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para Sectores complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="Sectores"&gt;
 *   &lt;complexContent&gt;
 *     &lt;extension base="{http://CD40.es/}SectoresSCV"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="IdSistema" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="IdSector" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="IdNucleo" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="IdNucleoParejaUCS" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="IdSistemaParejaUCS" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="SectorSimple" type="{http://www.w3.org/2001/XMLSchema}boolean"/&gt;
 *         &lt;element name="Tipo" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="NumSacta" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/extension&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "Sectores", propOrder = {
    "idSistema",
    "idSector",
    "idNucleo",
    "idNucleoParejaUCS",
    "idSistemaParejaUCS",
    "sectorSimple",
    "tipo",
    "numSacta"
})
public class Sectores
    extends SectoresSCV
{

    @XmlElement(name = "IdSistema")
    protected String idSistema;
    @XmlElement(name = "IdSector")
    protected String idSector;
    @XmlElement(name = "IdNucleo")
    protected String idNucleo;
    @XmlElement(name = "IdNucleoParejaUCS")
    protected String idNucleoParejaUCS;
    @XmlElement(name = "IdSistemaParejaUCS")
    protected String idSistemaParejaUCS;
    @XmlElement(name = "SectorSimple")
    protected boolean sectorSimple;
    @XmlElement(name = "Tipo")
    protected String tipo;
    @XmlElement(name = "NumSacta")
    @XmlSchemaType(name = "unsignedInt")
    protected long numSacta;

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
     * Obtiene el valor de la propiedad idNucleoParejaUCS.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getIdNucleoParejaUCS() {
        return idNucleoParejaUCS;
    }

    /**
     * Define el valor de la propiedad idNucleoParejaUCS.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setIdNucleoParejaUCS(String value) {
        this.idNucleoParejaUCS = value;
    }

    /**
     * Obtiene el valor de la propiedad idSistemaParejaUCS.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getIdSistemaParejaUCS() {
        return idSistemaParejaUCS;
    }

    /**
     * Define el valor de la propiedad idSistemaParejaUCS.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setIdSistemaParejaUCS(String value) {
        this.idSistemaParejaUCS = value;
    }

    /**
     * Obtiene el valor de la propiedad sectorSimple.
     * 
     */
    public boolean isSectorSimple() {
        return sectorSimple;
    }

    /**
     * Define el valor de la propiedad sectorSimple.
     * 
     */
    public void setSectorSimple(boolean value) {
        this.sectorSimple = value;
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
     * Obtiene el valor de la propiedad numSacta.
     * 
     */
    public long getNumSacta() {
        return numSacta;
    }

    /**
     * Define el valor de la propiedad numSacta.
     * 
     */
    public void setNumSacta(long value) {
        this.numSacta = value;
    }

}

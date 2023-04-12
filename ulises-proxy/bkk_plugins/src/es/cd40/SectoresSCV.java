
package es.cd40;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlSchemaType;
import javax.xml.bind.annotation.XmlSeeAlso;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para SectoresSCV complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="SectoresSCV"&gt;
 *   &lt;complexContent&gt;
 *     &lt;extension base="{http://CD40.es/}Tablas"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="IdParejaUCS" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="TipoPosicion" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="PrioridadR2" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="TipoHMI" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/extension&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "SectoresSCV", propOrder = {
    "idParejaUCS",
    "tipoPosicion",
    "prioridadR2",
    "tipoHMI"
})
@XmlSeeAlso({
    Sectores.class
})
public class SectoresSCV
    extends Tablas
{

    @XmlElement(name = "IdParejaUCS")
    protected String idParejaUCS;
    @XmlElement(name = "TipoPosicion")
    protected String tipoPosicion;
    @XmlElement(name = "PrioridadR2")
    @XmlSchemaType(name = "unsignedInt")
    protected long prioridadR2;
    @XmlElement(name = "TipoHMI")
    @XmlSchemaType(name = "unsignedInt")
    protected long tipoHMI;

    /**
     * Obtiene el valor de la propiedad idParejaUCS.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getIdParejaUCS() {
        return idParejaUCS;
    }

    /**
     * Define el valor de la propiedad idParejaUCS.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setIdParejaUCS(String value) {
        this.idParejaUCS = value;
    }

    /**
     * Obtiene el valor de la propiedad tipoPosicion.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getTipoPosicion() {
        return tipoPosicion;
    }

    /**
     * Define el valor de la propiedad tipoPosicion.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setTipoPosicion(String value) {
        this.tipoPosicion = value;
    }

    /**
     * Obtiene el valor de la propiedad prioridadR2.
     * 
     */
    public long getPrioridadR2() {
        return prioridadR2;
    }

    /**
     * Define el valor de la propiedad prioridadR2.
     * 
     */
    public void setPrioridadR2(long value) {
        this.prioridadR2 = value;
    }

    /**
     * Obtiene el valor de la propiedad tipoHMI.
     * 
     */
    public long getTipoHMI() {
        return tipoHMI;
    }

    /**
     * Define el valor de la propiedad tipoHMI.
     * 
     */
    public void setTipoHMI(long value) {
        this.tipoHMI = value;
    }

}

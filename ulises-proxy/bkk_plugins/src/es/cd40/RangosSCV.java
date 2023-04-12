
package es.cd40;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlSchemaType;
import javax.xml.bind.annotation.XmlSeeAlso;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para RangosSCV complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="RangosSCV"&gt;
 *   &lt;complexContent&gt;
 *     &lt;extension base="{http://CD40.es/}Tablas"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="IdPrefijo" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="IdAbonado" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="Inicial" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="Final" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/extension&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "RangosSCV", propOrder = {
    "idPrefijo",
    "idAbonado",
    "inicial",
    "_final"
})

@XmlSeeAlso({
    Rangos.class
})
public class RangosSCV
    extends Tablas
{

    @XmlElement(name = "IdPrefijo")
    @XmlSchemaType(name = "unsignedInt")
    protected long idPrefijo;
    @XmlElement(name = "IdAbonado")
    protected String idAbonado;
    @XmlElement(name = "Inicial")
    protected String inicial;
    @XmlElement(name = "Final")
    protected String _final;

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
     * Obtiene el valor de la propiedad idAbonado.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getIdAbonado() {
        return idAbonado;
    }

    /**
     * Define el valor de la propiedad idAbonado.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setIdAbonado(String value) {
        this.idAbonado = value;
    }

    /**
     * Obtiene el valor de la propiedad inicial.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getInicial() {
        return inicial;
    }

    /**
     * Define el valor de la propiedad inicial.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setInicial(String value) {
        this.inicial = value;
    }

    /**
     * Obtiene el valor de la propiedad final.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getFinal() {
        return _final;
    }

    /**
     * Define el valor de la propiedad final.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setFinal(String value) {
        this._final = value;
    }

}


package es.cd40;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para PlanRutas complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="PlanRutas"&gt;
 *   &lt;complexContent&gt;
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="TipoRuta" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="ListaTroncales" type="{http://CD40.es/}ArrayOfString" minOccurs="0"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/restriction&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "PlanRutas", propOrder = {
    "tipoRuta",
    "listaTroncales"
})
public class PlanRutas {

    @XmlElement(name = "TipoRuta")
    protected String tipoRuta;
    @XmlElement(name = "ListaTroncales")
    protected ArrayOfString listaTroncales;

    /**
     * Obtiene el valor de la propiedad tipoRuta.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getTipoRuta() {
        return tipoRuta;
    }

    /**
     * Define el valor de la propiedad tipoRuta.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setTipoRuta(String value) {
        this.tipoRuta = value;
    }

    /**
     * Obtiene el valor de la propiedad listaTroncales.
     * 
     * @return
     *     possible object is
     *     {@link ArrayOfString }
     *     
     */
    public ArrayOfString getListaTroncales() {
        return listaTroncales;
    }

    /**
     * Define el valor de la propiedad listaTroncales.
     * 
     * @param value
     *     allowed object is
     *     {@link ArrayOfString }
     *     
     */
    public void setListaTroncales(ArrayOfString value) {
        this.listaTroncales = value;
    }

}

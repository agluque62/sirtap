
package es.cd40;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para Agrupaciones complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="Agrupaciones"&gt;
 *   &lt;complexContent&gt;
 *     &lt;extension base="{http://CD40.es/}Tablas"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="IdAgrupacion" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="IdSistema" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="AgrupacionEspecial" type="{http://www.w3.org/2001/XMLSchema}boolean"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/extension&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "Agrupaciones", propOrder = {
    "idAgrupacion",
    "idSistema",
    "agrupacionEspecial"
})
public class Agrupaciones
    extends Tablas
{

    @XmlElement(name = "IdAgrupacion")
    protected String idAgrupacion;
    @XmlElement(name = "IdSistema")
    protected String idSistema;
    @XmlElement(name = "AgrupacionEspecial")
    protected boolean agrupacionEspecial;

    /**
     * Obtiene el valor de la propiedad idAgrupacion.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getIdAgrupacion() {
        return idAgrupacion;
    }

    /**
     * Define el valor de la propiedad idAgrupacion.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setIdAgrupacion(String value) {
        this.idAgrupacion = value;
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
     * Obtiene el valor de la propiedad agrupacionEspecial.
     * 
     */
    public boolean isAgrupacionEspecial() {
        return agrupacionEspecial;
    }

    /**
     * Define el valor de la propiedad agrupacionEspecial.
     * 
     */
    public void setAgrupacionEspecial(boolean value) {
        this.agrupacionEspecial = value;
    }

}

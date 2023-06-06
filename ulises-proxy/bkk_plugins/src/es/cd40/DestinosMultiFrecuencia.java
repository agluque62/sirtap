
package es.cd40;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para DestinosMultiFrecuencia complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="DestinosMultiFrecuencia"&gt;
 *   &lt;complexContent&gt;
 *     &lt;extension base="{http://CD40.es/}Tablas"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="IdSistema" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="IdDestino" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="FrecuenciaDefecto" type="{http://www.w3.org/2001/XMLSchema}boolean"/&gt;
 *         &lt;element name="Frecuencia" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/extension&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "DestinosMultiFrecuencia", propOrder = {
    "idSistema",
    "idDestino",
    "frecuenciaDefecto",
    "frecuencia"
})
public class DestinosMultiFrecuencia
    extends Tablas
{

    @XmlElement(name = "IdSistema")
    protected String idSistema;
    @XmlElement(name = "IdDestino")
    protected String idDestino;
    @XmlElement(name = "FrecuenciaDefecto")
    protected boolean frecuenciaDefecto;
    @XmlElement(name = "Frecuencia")
    protected String frecuencia;

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
     * Obtiene el valor de la propiedad frecuenciaDefecto.
     * 
     */
    public boolean isFrecuenciaDefecto() {
        return frecuenciaDefecto;
    }

    /**
     * Define el valor de la propiedad frecuenciaDefecto.
     * 
     */
    public void setFrecuenciaDefecto(boolean value) {
        this.frecuenciaDefecto = value;
    }

    /**
     * Obtiene el valor de la propiedad frecuencia.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getFrecuencia() {
        return frecuencia;
    }

    /**
     * Define el valor de la propiedad frecuencia.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setFrecuencia(String value) {
        this.frecuencia = value;
    }

}


package es.cd40;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para SectoresSectorizacion complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="SectoresSectorizacion"&gt;
 *   &lt;complexContent&gt;
 *     &lt;extension base="{http://CD40.es/}Tablas"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="IdSistema" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="IdSectorizacion" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="IdNucleo" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="IdSector" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="IdTop" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/extension&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "SectoresSectorizacion", propOrder = {
    "idSistema",
    "idSectorizacion",
    "idNucleo",
    "idSector",
    "idTop"
})
public class SectoresSectorizacion
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
    @XmlElement(name = "IdTop")
    protected String idTop;

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
     * Obtiene el valor de la propiedad idTop.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getIdTop() {
        return idTop;
    }

    /**
     * Define el valor de la propiedad idTop.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setIdTop(String value) {
        this.idTop = value;
    }

}

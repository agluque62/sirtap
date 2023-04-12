
package es.cd40;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlSchemaType;
import javax.xml.bind.annotation.XmlType;
import javax.xml.datatype.XMLGregorianCalendar;


/**
 * <p>Clase Java para Sectorizaciones complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="Sectorizaciones"&gt;
 *   &lt;complexContent&gt;
 *     &lt;extension base="{http://CD40.es/}Tablas"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="IdSectorizacion" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="IdSistema" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="Activa" type="{http://www.w3.org/2001/XMLSchema}boolean"/&gt;
 *         &lt;element name="FechaActivacion" type="{http://www.w3.org/2001/XMLSchema}dateTime"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/extension&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "Sectorizaciones", propOrder = {
    "idSectorizacion",
    "idSistema",
    "activa",
    "fechaActivacion"
})
public class Sectorizaciones
    extends Tablas
{

    @XmlElement(name = "IdSectorizacion")
    protected String idSectorizacion;
    @XmlElement(name = "IdSistema")
    protected String idSistema;
    @XmlElement(name = "Activa")
    protected boolean activa;
    @XmlElement(name = "FechaActivacion", required = true)
    @XmlSchemaType(name = "dateTime")
    protected XMLGregorianCalendar fechaActivacion;

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
     * Obtiene el valor de la propiedad activa.
     * 
     */
    public boolean isActiva() {
        return activa;
    }

    /**
     * Define el valor de la propiedad activa.
     * 
     */
    public void setActiva(boolean value) {
        this.activa = value;
    }

    /**
     * Obtiene el valor de la propiedad fechaActivacion.
     * 
     * @return
     *     possible object is
     *     {@link XMLGregorianCalendar }
     *     
     */
    public XMLGregorianCalendar getFechaActivacion() {
        return fechaActivacion;
    }

    /**
     * Define el valor de la propiedad fechaActivacion.
     * 
     * @param value
     *     allowed object is
     *     {@link XMLGregorianCalendar }
     *     
     */
    public void setFechaActivacion(XMLGregorianCalendar value) {
        this.fechaActivacion = value;
    }

}


package es.cd40;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlSchemaType;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para ParametrosMulticast complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="ParametrosMulticast"&gt;
 *   &lt;complexContent&gt;
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="GrupoMulticastConfiguracion" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="PuertoMulticastConfiguracion" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/restriction&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "ParametrosMulticast", propOrder = {
    "grupoMulticastConfiguracion",
    "puertoMulticastConfiguracion"
})
public class ParametrosMulticast {

    @XmlElement(name = "GrupoMulticastConfiguracion")
    protected String grupoMulticastConfiguracion;
    @XmlElement(name = "PuertoMulticastConfiguracion")
    @XmlSchemaType(name = "unsignedInt")
    protected long puertoMulticastConfiguracion;

    /**
     * Obtiene el valor de la propiedad grupoMulticastConfiguracion.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getGrupoMulticastConfiguracion() {
        return grupoMulticastConfiguracion;
    }

    /**
     * Define el valor de la propiedad grupoMulticastConfiguracion.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setGrupoMulticastConfiguracion(String value) {
        this.grupoMulticastConfiguracion = value;
    }

    /**
     * Obtiene el valor de la propiedad puertoMulticastConfiguracion.
     * 
     */
    public long getPuertoMulticastConfiguracion() {
        return puertoMulticastConfiguracion;
    }

    /**
     * Define el valor de la propiedad puertoMulticastConfiguracion.
     * 
     */
    public void setPuertoMulticastConfiguracion(long value) {
        this.puertoMulticastConfiguracion = value;
    }

}

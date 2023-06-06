
package es.cd40;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para ConferenciasPreprogramadas complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="ConferenciasPreprogramadas"&gt;
 *   &lt;complexContent&gt;
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="ConferenciaProgramada" type="{http://CD40.es/}ArrayOfConferencia" minOccurs="0"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/restriction&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "ConferenciasPreprogramadas", propOrder = {
    "conferenciaProgramada"
})
public class ConferenciasPreprogramadas {

    @XmlElement(name = "ConferenciaProgramada")
    protected ArrayOfConferencia conferenciaProgramada;

    /**
     * Obtiene el valor de la propiedad conferenciaProgramada.
     * 
     * @return
     *     possible object is
     *     {@link ArrayOfConferencia }
     *     
     */
    public ArrayOfConferencia getConferenciaProgramada() {
        return conferenciaProgramada;
    }

    /**
     * Define el valor de la propiedad conferenciaProgramada.
     * 
     * @param value
     *     allowed object is
     *     {@link ArrayOfConferencia }
     *     
     */
    public void setConferenciaProgramada(ArrayOfConferencia value) {
        this.conferenciaProgramada = value;
    }

}

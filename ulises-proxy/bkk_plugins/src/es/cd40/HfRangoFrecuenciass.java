
package es.cd40;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlSchemaType;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para HfRangoFrecuenciass complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="HfRangoFrecuenciass"&gt;
 *   &lt;complexContent&gt;
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="FMin" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="FMax" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/restriction&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "HfRangoFrecuenciass", propOrder = {
    "fMin",
    "fMax"
})
public class HfRangoFrecuenciass {

    @XmlElement(name = "FMin")
    @XmlSchemaType(name = "unsignedInt")
    protected long fMin;
    @XmlElement(name = "FMax")
    @XmlSchemaType(name = "unsignedInt")
    protected long fMax;

    /**
     * Obtiene el valor de la propiedad fMin.
     * 
     */
    public long getFMin() {
        return fMin;
    }

    /**
     * Define el valor de la propiedad fMin.
     * 
     */
    public void setFMin(long value) {
        this.fMin = value;
    }

    /**
     * Obtiene el valor de la propiedad fMax.
     * 
     */
    public long getFMax() {
        return fMax;
    }

    /**
     * Define el valor de la propiedad fMax.
     * 
     */
    public void setFMax(long value) {
        this.fMax = value;
    }

}

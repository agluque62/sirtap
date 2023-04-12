
package es.cd40;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlRootElement;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para anonymous complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType&gt;
 *   &lt;complexContent&gt;
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="r" type="{http://CD40.es/}Recursos" minOccurs="0"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/restriction&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "", propOrder = {
    "r"
})
@XmlRootElement(name = "GetParametrosRecurso")
public class GetParametrosRecurso {

    protected Recursos r;

    /**
     * Obtiene el valor de la propiedad r.
     * 
     * @return
     *     possible object is
     *     {@link Recursos }
     *     
     */
    public Recursos getR() {
        return r;
    }

    /**
     * Define el valor de la propiedad r.
     * 
     * @param value
     *     allowed object is
     *     {@link Recursos }
     *     
     */
    public void setR(Recursos value) {
        this.r = value;
    }

}

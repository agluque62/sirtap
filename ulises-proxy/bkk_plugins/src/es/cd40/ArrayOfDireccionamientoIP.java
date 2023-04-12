
package es.cd40;

import java.util.ArrayList;
import java.util.List;
import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para ArrayOfDireccionamientoIP complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="ArrayOfDireccionamientoIP"&gt;
 *   &lt;complexContent&gt;
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="DireccionamientoIP" type="{http://CD40.es/}DireccionamientoIP" maxOccurs="unbounded" minOccurs="0"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/restriction&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "ArrayOfDireccionamientoIP", propOrder = {
    "direccionamientoIP"
})
public class ArrayOfDireccionamientoIP {

    @XmlElement(name = "DireccionamientoIP", nillable = true)
    protected List<DireccionamientoIP> direccionamientoIP;

    /**
     * Gets the value of the direccionamientoIP property.
     * 
     * <p>
     * This accessor method returns a reference to the live list,
     * not a snapshot. Therefore any modification you make to the
     * returned list will be present inside the JAXB object.
     * This is why there is not a <CODE>set</CODE> method for the direccionamientoIP property.
     * 
     * <p>
     * For example, to add a new item, do as follows:
     * <pre>
     *    getDireccionamientoIP().add(newItem);
     * </pre>
     * 
     * 
     * <p>
     * Objects of the following type(s) are allowed in the list
     * {@link DireccionamientoIP }
     * 
     * 
     */
    public List<DireccionamientoIP> getDireccionamientoIP() {
        if (direccionamientoIP == null) {
            direccionamientoIP = new ArrayList<DireccionamientoIP>();
        }
        return this.direccionamientoIP;
    }

}


package es.cd40;

import java.util.ArrayList;
import java.util.List;
import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para ArrayOfAsignacionRecursosGW complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="ArrayOfAsignacionRecursosGW"&gt;
 *   &lt;complexContent&gt;
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="AsignacionRecursosGW" type="{http://CD40.es/}AsignacionRecursosGW" maxOccurs="unbounded" minOccurs="0"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/restriction&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "ArrayOfAsignacionRecursosGW", propOrder = {
    "asignacionRecursosGW"
})
public class ArrayOfAsignacionRecursosGW {

    @XmlElement(name = "AsignacionRecursosGW", nillable = true)
    protected List<AsignacionRecursosGW> asignacionRecursosGW;

    /**
     * Gets the value of the asignacionRecursosGW property.
     * 
     * <p>
     * This accessor method returns a reference to the live list,
     * not a snapshot. Therefore any modification you make to the
     * returned list will be present inside the JAXB object.
     * This is why there is not a <CODE>set</CODE> method for the asignacionRecursosGW property.
     * 
     * <p>
     * For example, to add a new item, do as follows:
     * <pre>
     *    getAsignacionRecursosGW().add(newItem);
     * </pre>
     * 
     * 
     * <p>
     * Objects of the following type(s) are allowed in the list
     * {@link AsignacionRecursosGW }
     * 
     * 
     */
    public List<AsignacionRecursosGW> getAsignacionRecursosGW() {
        if (asignacionRecursosGW == null) {
            asignacionRecursosGW = new ArrayList<AsignacionRecursosGW>();
        }
        return this.asignacionRecursosGW;
    }

}

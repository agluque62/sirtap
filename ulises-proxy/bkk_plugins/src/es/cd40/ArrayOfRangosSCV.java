
package es.cd40;

import java.util.ArrayList;
import java.util.List;
import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para ArrayOfRangosSCV complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="ArrayOfRangosSCV"&gt;
 *   &lt;complexContent&gt;
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="RangosSCV" type="{http://CD40.es/}RangosSCV" maxOccurs="unbounded" minOccurs="0"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/restriction&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "ArrayOfRangosSCV", propOrder = {
    "rangosSCV"
})
public class ArrayOfRangosSCV {

    @XmlElement(name = "RangosSCV", nillable = true)
    protected List<RangosSCV> rangosSCV;

    /**
     * Gets the value of the rangosSCV property.
     * 
     * <p>
     * This accessor method returns a reference to the live list,
     * not a snapshot. Therefore any modification you make to the
     * returned list will be present inside the JAXB object.
     * This is why there is not a <CODE>set</CODE> method for the rangosSCV property.
     * 
     * <p>
     * For example, to add a new item, do as follows:
     * <pre>
     *    getRangosSCV().add(newItem);
     * </pre>
     * 
     * 
     * <p>
     * Objects of the following type(s) are allowed in the list
     * {@link RangosSCV }
     * 
     * 
     */
    public List<RangosSCV> getRangosSCV() {
        if (rangosSCV == null) {
            rangosSCV = new ArrayList<RangosSCV>();
        }
        return this.rangosSCV;
    }

}


package es.cd40;

import java.util.ArrayList;
import java.util.List;
import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para ArrayOfNumeracionATS complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="ArrayOfNumeracionATS"&gt;
 *   &lt;complexContent&gt;
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="NumeracionATS" type="{http://CD40.es/}NumeracionATS" maxOccurs="unbounded" minOccurs="0"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/restriction&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "ArrayOfNumeracionATS", propOrder = {
    "numeracionATS"
})
public class ArrayOfNumeracionATS {

    @XmlElement(name = "NumeracionATS", nillable = true)
    protected List<NumeracionATS> numeracionATS;

    /**
     * Gets the value of the numeracionATS property.
     * 
     * <p>
     * This accessor method returns a reference to the live list,
     * not a snapshot. Therefore any modification you make to the
     * returned list will be present inside the JAXB object.
     * This is why there is not a <CODE>set</CODE> method for the numeracionATS property.
     * 
     * <p>
     * For example, to add a new item, do as follows:
     * <pre>
     *    getNumeracionATS().add(newItem);
     * </pre>
     * 
     * 
     * <p>
     * Objects of the following type(s) are allowed in the list
     * {@link NumeracionATS }
     * 
     * 
     */
    public List<NumeracionATS> getNumeracionATS() {
        if (numeracionATS == null) {
            numeracionATS = new ArrayList<NumeracionATS>();
        }
        return this.numeracionATS;
    }

}

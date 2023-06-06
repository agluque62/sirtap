
package es.cd40;

import java.util.ArrayList;
import java.util.List;
import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para ArrayOfHFRangoFrecuenciasBis complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="ArrayOfHFRangoFrecuenciasBis"&gt;
 *   &lt;complexContent&gt;
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="HFRangoFrecuenciasBis" type="{http://CD40.es/}HFRangoFrecuenciasBis" maxOccurs="unbounded" minOccurs="0"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/restriction&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "ArrayOfHFRangoFrecuenciasBis", propOrder = {
    "hfRangoFrecuenciasBis"
})
public class ArrayOfHFRangoFrecuenciasBis {

    @XmlElement(name = "HFRangoFrecuenciasBis", nillable = true)
    protected List<HFRangoFrecuenciasBis> hfRangoFrecuenciasBis;

    /**
     * Gets the value of the hfRangoFrecuenciasBis property.
     * 
     * <p>
     * This accessor method returns a reference to the live list,
     * not a snapshot. Therefore any modification you make to the
     * returned list will be present inside the JAXB object.
     * This is why there is not a <CODE>set</CODE> method for the hfRangoFrecuenciasBis property.
     * 
     * <p>
     * For example, to add a new item, do as follows:
     * <pre>
     *    getHFRangoFrecuenciasBis().add(newItem);
     * </pre>
     * 
     * 
     * <p>
     * Objects of the following type(s) are allowed in the list
     * {@link HFRangoFrecuenciasBis }
     * 
     * 
     */
    public List<HFRangoFrecuenciasBis> getHFRangoFrecuenciasBis() {
        if (hfRangoFrecuenciasBis == null) {
            hfRangoFrecuenciasBis = new ArrayList<HFRangoFrecuenciasBis>();
        }
        return this.hfRangoFrecuenciasBis;
    }

}

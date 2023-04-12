
package es.cd40;

import java.util.ArrayList;
import java.util.List;
import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para ArrayOfMetodosBssDelRecurso complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="ArrayOfMetodosBssDelRecurso"&gt;
 *   &lt;complexContent&gt;
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="MetodosBssDelRecurso" type="{http://CD40.es/}MetodosBssDelRecurso" maxOccurs="unbounded" minOccurs="0"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/restriction&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "ArrayOfMetodosBssDelRecurso", propOrder = {
    "metodosBssDelRecurso"
})
public class ArrayOfMetodosBssDelRecurso {

    @XmlElement(name = "MetodosBssDelRecurso")
    protected List<MetodosBssDelRecurso> metodosBssDelRecurso;

    /**
     * Gets the value of the metodosBssDelRecurso property.
     * 
     * <p>
     * This accessor method returns a reference to the live list,
     * not a snapshot. Therefore any modification you make to the
     * returned list will be present inside the JAXB object.
     * This is why there is not a <CODE>set</CODE> method for the metodosBssDelRecurso property.
     * 
     * <p>
     * For example, to add a new item, do as follows:
     * <pre>
     *    getMetodosBssDelRecurso().add(newItem);
     * </pre>
     * 
     * 
     * <p>
     * Objects of the following type(s) are allowed in the list
     * {@link MetodosBssDelRecurso }
     * 
     * 
     */
    public List<MetodosBssDelRecurso> getMetodosBssDelRecurso() {
        if (metodosBssDelRecurso == null) {
            metodosBssDelRecurso = new ArrayList<MetodosBssDelRecurso>();
        }
        return this.metodosBssDelRecurso;
    }

}


package es.cd40;

import java.util.ArrayList;
import java.util.List;
import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para ArrayOfPlanRecursos complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="ArrayOfPlanRecursos"&gt;
 *   &lt;complexContent&gt;
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="PlanRecursos" type="{http://CD40.es/}PlanRecursos" maxOccurs="unbounded" minOccurs="0"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/restriction&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "ArrayOfPlanRecursos", propOrder = {
    "planRecursos"
})
public class ArrayOfPlanRecursos {

    @XmlElement(name = "PlanRecursos", nillable = true)
    protected List<PlanRecursos> planRecursos;

    /**
     * Gets the value of the planRecursos property.
     * 
     * <p>
     * This accessor method returns a reference to the live list,
     * not a snapshot. Therefore any modification you make to the
     * returned list will be present inside the JAXB object.
     * This is why there is not a <CODE>set</CODE> method for the planRecursos property.
     * 
     * <p>
     * For example, to add a new item, do as follows:
     * <pre>
     *    getPlanRecursos().add(newItem);
     * </pre>
     * 
     * 
     * <p>
     * Objects of the following type(s) are allowed in the list
     * {@link PlanRecursos }
     * 
     * 
     */
    public List<PlanRecursos> getPlanRecursos() {
        if (planRecursos == null) {
            planRecursos = new ArrayList<PlanRecursos>();
        }
        return this.planRecursos;
    }

}

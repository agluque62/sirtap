
package es.cd40;

import java.util.ArrayList;
import java.util.List;
import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para ArrayOfCfgRecursoEnlaceInternoConInterface complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="ArrayOfCfgRecursoEnlaceInternoConInterface"&gt;
 *   &lt;complexContent&gt;
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="CfgRecursoEnlaceInternoConInterface" type="{http://CD40.es/}CfgRecursoEnlaceInternoConInterface" maxOccurs="unbounded" minOccurs="0"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/restriction&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "ArrayOfCfgRecursoEnlaceInternoConInterface", propOrder = {
    "cfgRecursoEnlaceInternoConInterface"
})
public class ArrayOfCfgRecursoEnlaceInternoConInterface {

    @XmlElement(name = "CfgRecursoEnlaceInternoConInterface", nillable = true)
    protected List<CfgRecursoEnlaceInternoConInterface> cfgRecursoEnlaceInternoConInterface;

    /**
     * Gets the value of the cfgRecursoEnlaceInternoConInterface property.
     * 
     * <p>
     * This accessor method returns a reference to the live list,
     * not a snapshot. Therefore any modification you make to the
     * returned list will be present inside the JAXB object.
     * This is why there is not a <CODE>set</CODE> method for the cfgRecursoEnlaceInternoConInterface property.
     * 
     * <p>
     * For example, to add a new item, do as follows:
     * <pre>
     *    getCfgRecursoEnlaceInternoConInterface().add(newItem);
     * </pre>
     * 
     * 
     * <p>
     * Objects of the following type(s) are allowed in the list
     * {@link CfgRecursoEnlaceInternoConInterface }
     * 
     * 
     */
    public List<CfgRecursoEnlaceInternoConInterface> getCfgRecursoEnlaceInternoConInterface() {
        if (cfgRecursoEnlaceInternoConInterface == null) {
            cfgRecursoEnlaceInternoConInterface = new ArrayList<CfgRecursoEnlaceInternoConInterface>();
        }
        return this.cfgRecursoEnlaceInternoConInterface;
    }

}

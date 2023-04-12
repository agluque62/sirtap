
package es.cd40;

import java.util.ArrayList;
import java.util.List;
import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para ArrayOfPoolHfElement complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="ArrayOfPoolHfElement"&gt;
 *   &lt;complexContent&gt;
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="PoolHfElement" type="{http://CD40.es/}PoolHfElement" maxOccurs="unbounded" minOccurs="0"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/restriction&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "ArrayOfPoolHfElement", propOrder = {
    "poolHfElement"
})
public class ArrayOfPoolHfElement {

    @XmlElement(name = "PoolHfElement", nillable = true)
    protected List<PoolHfElement> poolHfElement;

    /**
     * Gets the value of the poolHfElement property.
     * 
     * <p>
     * This accessor method returns a reference to the live list,
     * not a snapshot. Therefore any modification you make to the
     * returned list will be present inside the JAXB object.
     * This is why there is not a <CODE>set</CODE> method for the poolHfElement property.
     * 
     * <p>
     * For example, to add a new item, do as follows:
     * <pre>
     *    getPoolHfElement().add(newItem);
     * </pre>
     * 
     * 
     * <p>
     * Objects of the following type(s) are allowed in the list
     * {@link PoolHfElement }
     * 
     * 
     */
    public List<PoolHfElement> getPoolHfElement() {
        if (poolHfElement == null) {
            poolHfElement = new ArrayList<PoolHfElement>();
        }
        return this.poolHfElement;
    }

}

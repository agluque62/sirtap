
package es.cd40;

import java.util.ArrayList;
import java.util.List;
import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para ArrayOfCfgEnlaceInterno complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="ArrayOfCfgEnlaceInterno"&gt;
 *   &lt;complexContent&gt;
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="CfgEnlaceInterno" type="{http://CD40.es/}CfgEnlaceInterno" maxOccurs="unbounded" minOccurs="0"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/restriction&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "ArrayOfCfgEnlaceInterno", propOrder = {
    "cfgEnlaceInterno"
})
public class ArrayOfCfgEnlaceInterno {

    @XmlElement(name = "CfgEnlaceInterno", nillable = true)
    protected List<CfgEnlaceInterno> cfgEnlaceInterno;

    /**
     * Gets the value of the cfgEnlaceInterno property.
     * 
     * <p>
     * This accessor method returns a reference to the live list,
     * not a snapshot. Therefore any modification you make to the
     * returned list will be present inside the JAXB object.
     * This is why there is not a <CODE>set</CODE> method for the cfgEnlaceInterno property.
     * 
     * <p>
     * For example, to add a new item, do as follows:
     * <pre>
     *    getCfgEnlaceInterno().add(newItem);
     * </pre>
     * 
     * 
     * <p>
     * Objects of the following type(s) are allowed in the list
     * {@link CfgEnlaceInterno }
     * 
     * 
     */
    public List<CfgEnlaceInterno> getCfgEnlaceInterno() {
        if (cfgEnlaceInterno == null) {
            cfgEnlaceInterno = new ArrayList<CfgEnlaceInterno>();
        }
        return this.cfgEnlaceInterno;
    }

}

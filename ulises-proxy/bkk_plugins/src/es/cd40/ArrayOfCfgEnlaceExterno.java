
package es.cd40;

import java.util.ArrayList;
import java.util.List;
import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para ArrayOfCfgEnlaceExterno complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="ArrayOfCfgEnlaceExterno"&gt;
 *   &lt;complexContent&gt;
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="CfgEnlaceExterno" type="{http://CD40.es/}CfgEnlaceExterno" maxOccurs="unbounded" minOccurs="0"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/restriction&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "ArrayOfCfgEnlaceExterno", propOrder = {
    "cfgEnlaceExterno"
})
public class ArrayOfCfgEnlaceExterno {

    @XmlElement(name = "CfgEnlaceExterno", nillable = true)
    protected List<CfgEnlaceExterno> cfgEnlaceExterno;

    /**
     * Gets the value of the cfgEnlaceExterno property.
     * 
     * <p>
     * This accessor method returns a reference to the live list,
     * not a snapshot. Therefore any modification you make to the
     * returned list will be present inside the JAXB object.
     * This is why there is not a <CODE>set</CODE> method for the cfgEnlaceExterno property.
     * 
     * <p>
     * For example, to add a new item, do as follows:
     * <pre>
     *    getCfgEnlaceExterno().add(newItem);
     * </pre>
     * 
     * 
     * <p>
     * Objects of the following type(s) are allowed in the list
     * {@link CfgEnlaceExterno }
     * 
     * 
     */
    public List<CfgEnlaceExterno> getCfgEnlaceExterno() {
        if (cfgEnlaceExterno == null) {
            cfgEnlaceExterno = new ArrayList<CfgEnlaceExterno>();
        }
        return this.cfgEnlaceExterno;
    }

}

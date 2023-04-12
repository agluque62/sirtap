
package es.cd40;

import java.util.ArrayList;
import java.util.List;
import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para ArrayOfStrNumeroAbonado complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="ArrayOfStrNumeroAbonado"&gt;
 *   &lt;complexContent&gt;
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="StrNumeroAbonado" type="{http://CD40.es/}StrNumeroAbonado" maxOccurs="unbounded" minOccurs="0"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/restriction&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "ArrayOfStrNumeroAbonado", propOrder = {
    "strNumeroAbonado"
})
public class ArrayOfStrNumeroAbonado {

    @XmlElement(name = "StrNumeroAbonado")
    protected List<StrNumeroAbonado> strNumeroAbonado;

    /**
     * Gets the value of the strNumeroAbonado property.
     * 
     * <p>
     * This accessor method returns a reference to the live list,
     * not a snapshot. Therefore any modification you make to the
     * returned list will be present inside the JAXB object.
     * This is why there is not a <CODE>set</CODE> method for the strNumeroAbonado property.
     * 
     * <p>
     * For example, to add a new item, do as follows:
     * <pre>
     *    getStrNumeroAbonado().add(newItem);
     * </pre>
     * 
     * 
     * <p>
     * Objects of the following type(s) are allowed in the list
     * {@link StrNumeroAbonado }
     * 
     * 
     */
    public List<StrNumeroAbonado> getStrNumeroAbonado() {
        if (strNumeroAbonado == null) {
            strNumeroAbonado = new ArrayList<StrNumeroAbonado>();
        }
        return this.strNumeroAbonado;
    }

}

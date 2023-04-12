
package es.cd40;

import java.util.ArrayList;
import java.util.List;
import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para ArrayOfNumerosAbonado complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="ArrayOfNumerosAbonado"&gt;
 *   &lt;complexContent&gt;
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="NumerosAbonado" type="{http://CD40.es/}NumerosAbonado" maxOccurs="unbounded" minOccurs="0"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/restriction&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "ArrayOfNumerosAbonado", propOrder = {
    "numerosAbonado"
})
public class ArrayOfNumerosAbonado {

    @XmlElement(name = "NumerosAbonado", nillable = true)
    protected List<NumerosAbonado> numerosAbonado;

    /**
     * Gets the value of the numerosAbonado property.
     * 
     * <p>
     * This accessor method returns a reference to the live list,
     * not a snapshot. Therefore any modification you make to the
     * returned list will be present inside the JAXB object.
     * This is why there is not a <CODE>set</CODE> method for the numerosAbonado property.
     * 
     * <p>
     * For example, to add a new item, do as follows:
     * <pre>
     *    getNumerosAbonado().add(newItem);
     * </pre>
     * 
     * 
     * <p>
     * Objects of the following type(s) are allowed in the list
     * {@link NumerosAbonado }
     * 
     * 
     */
    public List<NumerosAbonado> getNumerosAbonado() {
        if (numerosAbonado == null) {
            numerosAbonado = new ArrayList<NumerosAbonado>();
        }
        return this.numerosAbonado;
    }

}

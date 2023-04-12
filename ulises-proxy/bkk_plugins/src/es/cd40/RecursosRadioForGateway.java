
package es.cd40;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para RecursosRadioForGateway complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="RecursosRadioForGateway"&gt;
 *   &lt;complexContent&gt;
 *     &lt;extension base="{http://CD40.es/}RecursosRadio"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="ValuesTablaBss" type="{http://CD40.es/}ArrayOfInt" minOccurs="0"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/extension&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "RecursosRadioForGateway", propOrder = {
    "valuesTablaBss"
})
public class RecursosRadioForGateway
    extends RecursosRadio
{

    @XmlElement(name = "ValuesTablaBss")
    protected ArrayOfInt valuesTablaBss;

    /**
     * Obtiene el valor de la propiedad valuesTablaBss.
     * 
     * @return
     *     possible object is
     *     {@link ArrayOfInt }
     *     
     */
    public ArrayOfInt getValuesTablaBss() {
        return valuesTablaBss;
    }

    /**
     * Define el valor de la propiedad valuesTablaBss.
     * 
     * @param value
     *     allowed object is
     *     {@link ArrayOfInt }
     *     
     */
    public void setValuesTablaBss(ArrayOfInt value) {
        this.valuesTablaBss = value;
    }

}

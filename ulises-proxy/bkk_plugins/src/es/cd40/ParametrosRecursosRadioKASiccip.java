
package es.cd40;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlSeeAlso;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para ParametrosRecursosRadioKASiccip complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="ParametrosRecursosRadioKASiccip"&gt;
 *   &lt;complexContent&gt;
 *     &lt;extension base="{http://CD40.es/}ParametrosRecursosRadioKeepAlive"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="UriRecursoDestino" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/extension&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "ParametrosRecursosRadioKASiccip", propOrder = {
    "uriRecursoDestino"
})
@XmlSeeAlso({
    RecursosRadio.class
})
public class ParametrosRecursosRadioKASiccip
    extends ParametrosRecursosRadioKeepAlive
{

    @XmlElement(name = "UriRecursoDestino")
    protected String uriRecursoDestino;

    /**
     * Obtiene el valor de la propiedad uriRecursoDestino.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getUriRecursoDestino() {
        return uriRecursoDestino;
    }

    /**
     * Define el valor de la propiedad uriRecursoDestino.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setUriRecursoDestino(String value) {
        this.uriRecursoDestino = value;
    }

}


package es.cd40;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlRootElement;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para anonymous complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType&gt;
 *   &lt;complexContent&gt;
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="GetConferenciasPreprogramadasResult" type="{http://CD40.es/}ConferenciasPreprogramadas" minOccurs="0"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/restriction&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "", propOrder = {
    "getConferenciasPreprogramadasResult"
})
@XmlRootElement(name = "GetConferenciasPreprogramadasResponse")
public class GetConferenciasPreprogramadasResponse {

    @XmlElement(name = "GetConferenciasPreprogramadasResult")
    protected ConferenciasPreprogramadas getConferenciasPreprogramadasResult;

    /**
     * Obtiene el valor de la propiedad getConferenciasPreprogramadasResult.
     * 
     * @return
     *     possible object is
     *     {@link ConferenciasPreprogramadas }
     *     
     */
    public ConferenciasPreprogramadas getGetConferenciasPreprogramadasResult() {
        return getConferenciasPreprogramadasResult;
    }

    /**
     * Define el valor de la propiedad getConferenciasPreprogramadasResult.
     * 
     * @param value
     *     allowed object is
     *     {@link ConferenciasPreprogramadas }
     *     
     */
    public void setGetConferenciasPreprogramadasResult(ConferenciasPreprogramadas value) {
        this.getConferenciasPreprogramadasResult = value;
    }

}

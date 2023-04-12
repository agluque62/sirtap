
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
 *         &lt;element name="GetPlanRedesResult" type="{http://CD40.es/}ArrayOfListaRedes" minOccurs="0"/&gt;
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
    "getPlanRedesResult"
})
@XmlRootElement(name = "GetPlanRedesResponse")
public class GetPlanRedesResponse {

    @XmlElement(name = "GetPlanRedesResult")
    protected ArrayOfListaRedes getPlanRedesResult;

    /**
     * Obtiene el valor de la propiedad getPlanRedesResult.
     * 
     * @return
     *     possible object is
     *     {@link ArrayOfListaRedes }
     *     
     */
    public ArrayOfListaRedes getGetPlanRedesResult() {
        return getPlanRedesResult;
    }

    /**
     * Define el valor de la propiedad getPlanRedesResult.
     * 
     * @param value
     *     allowed object is
     *     {@link ArrayOfListaRedes }
     *     
     */
    public void setGetPlanRedesResult(ArrayOfListaRedes value) {
        this.getPlanRedesResult = value;
    }

}

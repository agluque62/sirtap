
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
 *         &lt;element name="GetEstadoClusterResult" type="{http://CD40.es/}EstadoCluster"/&gt;
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
    "getEstadoClusterResult"
})
@XmlRootElement(name = "GetEstadoClusterResponse")
public class GetEstadoClusterResponse {

    @XmlElement(name = "GetEstadoClusterResult", required = true)
    protected EstadoCluster getEstadoClusterResult;

    /**
     * Obtiene el valor de la propiedad getEstadoClusterResult.
     * 
     * @return
     *     possible object is
     *     {@link EstadoCluster }
     *     
     */
    public EstadoCluster getGetEstadoClusterResult() {
        return getEstadoClusterResult;
    }

    /**
     * Define el valor de la propiedad getEstadoClusterResult.
     * 
     * @param value
     *     allowed object is
     *     {@link EstadoCluster }
     *     
     */
    public void setGetEstadoClusterResult(EstadoCluster value) {
        this.getEstadoClusterResult = value;
    }

}

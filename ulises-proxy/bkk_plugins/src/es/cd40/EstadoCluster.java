
package es.cd40;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para EstadoCluster complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="EstadoCluster"&gt;
 *   &lt;complexContent&gt;
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="EstadoNode1" type="{http://CD40.es/}EstadoNode"/&gt;
 *         &lt;element name="EstadoNode2" type="{http://CD40.es/}EstadoNode"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/restriction&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "EstadoCluster", propOrder = {
    "estadoNode1",
    "estadoNode2"
})
public class EstadoCluster {

    @XmlElement(name = "EstadoNode1", required = true)
    protected EstadoNode estadoNode1;
    @XmlElement(name = "EstadoNode2", required = true)
    protected EstadoNode estadoNode2;

    /**
     * Obtiene el valor de la propiedad estadoNode1.
     * 
     * @return
     *     possible object is
     *     {@link EstadoNode }
     *     
     */
    public EstadoNode getEstadoNode1() {
        return estadoNode1;
    }

    /**
     * Define el valor de la propiedad estadoNode1.
     * 
     * @param value
     *     allowed object is
     *     {@link EstadoNode }
     *     
     */
    public void setEstadoNode1(EstadoNode value) {
        this.estadoNode1 = value;
    }

    /**
     * Obtiene el valor de la propiedad estadoNode2.
     * 
     * @return
     *     possible object is
     *     {@link EstadoNode }
     *     
     */
    public EstadoNode getEstadoNode2() {
        return estadoNode2;
    }

    /**
     * Define el valor de la propiedad estadoNode2.
     * 
     * @param value
     *     allowed object is
     *     {@link EstadoNode }
     *     
     */
    public void setEstadoNode2(EstadoNode value) {
        this.estadoNode2 = value;
    }

}

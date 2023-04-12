
package es.cd40;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para DireccionamientoSIP complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="DireccionamientoSIP"&gt;
 *   &lt;complexContent&gt;
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="IdUsuario" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="NumerosAbonadoQueAtiende" type="{http://CD40.es/}ArrayOfStrNumeroAbonado" minOccurs="0"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/restriction&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "DireccionamientoSIP", propOrder = {
    "idUsuario",
    "numerosAbonadoQueAtiende"
})
public class DireccionamientoSIP {

    @XmlElement(name = "IdUsuario")
    protected String idUsuario;
    @XmlElement(name = "NumerosAbonadoQueAtiende")
    protected ArrayOfStrNumeroAbonado numerosAbonadoQueAtiende;

    /**
     * Obtiene el valor de la propiedad idUsuario.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getIdUsuario() {
        return idUsuario;
    }

    /**
     * Define el valor de la propiedad idUsuario.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setIdUsuario(String value) {
        this.idUsuario = value;
    }

    /**
     * Obtiene el valor de la propiedad numerosAbonadoQueAtiende.
     * 
     * @return
     *     possible object is
     *     {@link ArrayOfStrNumeroAbonado }
     *     
     */
    public ArrayOfStrNumeroAbonado getNumerosAbonadoQueAtiende() {
        return numerosAbonadoQueAtiende;
    }

    /**
     * Define el valor de la propiedad numerosAbonadoQueAtiende.
     * 
     * @param value
     *     allowed object is
     *     {@link ArrayOfStrNumeroAbonado }
     *     
     */
    public void setNumerosAbonadoQueAtiende(ArrayOfStrNumeroAbonado value) {
        this.numerosAbonadoQueAtiende = value;
    }

}

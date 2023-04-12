
package es.cd40;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para MetodosBssDelRecurso complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="MetodosBssDelRecurso"&gt;
 *   &lt;complexContent&gt;
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="idMetodo" type="{http://www.w3.org/2001/XMLSchema}int"/&gt;
 *         &lt;element name="nombreMetodo" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/restriction&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "MetodosBssDelRecurso", propOrder = {
    "idMetodo",
    "nombreMetodo"
})
public class MetodosBssDelRecurso {

    protected int idMetodo;
    protected String nombreMetodo;

    /**
     * Obtiene el valor de la propiedad idMetodo.
     * 
     */
    public int getIdMetodo() {
        return idMetodo;
    }

    /**
     * Define el valor de la propiedad idMetodo.
     * 
     */
    public void setIdMetodo(int value) {
        this.idMetodo = value;
    }

    /**
     * Obtiene el valor de la propiedad nombreMetodo.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getNombreMetodo() {
        return nombreMetodo;
    }

    /**
     * Define el valor de la propiedad nombreMetodo.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setNombreMetodo(String value) {
        this.nombreMetodo = value;
    }

}


package es.cd40;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlSchemaType;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para CfgRecursoEnlaceInternoConInterface complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="CfgRecursoEnlaceInternoConInterface"&gt;
 *   &lt;complexContent&gt;
 *     &lt;extension base="{http://CD40.es/}CfgRecursoEnlaceInterno"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="Interface" type="{http://CD40.es/}TipoInterface"/&gt;
 *         &lt;element name="NombreMostrar" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/extension&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "CfgRecursoEnlaceInternoConInterface", propOrder = {
    "_interface",
    "nombreMostrar"
})
public class CfgRecursoEnlaceInternoConInterface
    extends CfgRecursoEnlaceInterno
{

    @XmlElement(name = "Interface", required = true)
    @XmlSchemaType(name = "string")
    protected TipoInterface _interface;
    @XmlElement(name = "NombreMostrar")
    protected String nombreMostrar;

    /**
     * Obtiene el valor de la propiedad interface.
     * 
     * @return
     *     possible object is
     *     {@link TipoInterface }
     *     
     */
    public TipoInterface getInterface() {
        return _interface;
    }

    /**
     * Define el valor de la propiedad interface.
     * 
     * @param value
     *     allowed object is
     *     {@link TipoInterface }
     *     
     */
    public void setInterface(TipoInterface value) {
        this._interface = value;
    }

    /**
     * Obtiene el valor de la propiedad nombreMostrar.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getNombreMostrar() {
        return nombreMostrar;
    }

    /**
     * Define el valor de la propiedad nombreMostrar.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setNombreMostrar(String value) {
        this.nombreMostrar = value;
    }

}

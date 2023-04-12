
package es.cd40;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para DestinosExternos complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="DestinosExternos"&gt;
 *   &lt;complexContent&gt;
 *     &lt;extension base="{http://CD40.es/}DestinosTelefonia"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="IdAbonado" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/extension&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "DestinosExternos", propOrder = {
    "idAbonado"
})
public class DestinosExternos
    extends DestinosTelefonia
{

    @XmlElement(name = "IdAbonado")
    protected String idAbonado;

    /**
     * Obtiene el valor de la propiedad idAbonado.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getIdAbonado() {
        return idAbonado;
    }

    /**
     * Define el valor de la propiedad idAbonado.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setIdAbonado(String value) {
        this.idAbonado = value;
    }

}


package es.cd40;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlSchemaType;
import javax.xml.bind.annotation.XmlSeeAlso;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para ParametrosEM complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="ParametrosEM"&gt;
 *   &lt;complexContent&gt;
 *     &lt;extension base="{http://CD40.es/}ParametrosRecursoTelefonia"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="TipoEM" type="{http://CD40.es/}Tipo_EM"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/extension&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "ParametrosEM", propOrder = {
    "tipoEM"
})
@XmlSeeAlso({
    ParametrosGrabacionTf.class
})
public class ParametrosEM
    extends ParametrosRecursoTelefonia
{

    @XmlElement(name = "TipoEM", required = true)
    @XmlSchemaType(name = "string")
    protected TipoEM tipoEM;

    /**
     * Obtiene el valor de la propiedad tipoEM.
     * 
     * @return
     *     possible object is
     *     {@link TipoEM }
     *     
     */
    public TipoEM getTipoEM() {
        return tipoEM;
    }

    /**
     * Define el valor de la propiedad tipoEM.
     * 
     * @param value
     *     allowed object is
     *     {@link TipoEM }
     *     
     */
    public void setTipoEM(TipoEM value) {
        this.tipoEM = value;
    }

}

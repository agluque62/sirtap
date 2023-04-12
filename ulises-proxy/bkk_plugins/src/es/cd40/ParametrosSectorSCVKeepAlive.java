
package es.cd40;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlSchemaType;
import javax.xml.bind.annotation.XmlSeeAlso;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para ParametrosSectorSCVKeepAlive complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="ParametrosSectorSCVKeepAlive"&gt;
 *   &lt;complexContent&gt;
 *     &lt;extension base="{http://CD40.es/}ParametrosSectorSCV"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="KeepAlivePeriod" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="KeepAliveMultiplier" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/extension&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "ParametrosSectorSCVKeepAlive", propOrder = {
    "keepAlivePeriod",
    "keepAliveMultiplier"
})
@XmlSeeAlso({
    ParametrosSectorRecording.class
})
public class ParametrosSectorSCVKeepAlive
    extends ParametrosSectorSCV
{

    @XmlElement(name = "KeepAlivePeriod")
    @XmlSchemaType(name = "unsignedInt")
    protected long keepAlivePeriod;
    @XmlElement(name = "KeepAliveMultiplier")
    @XmlSchemaType(name = "unsignedInt")
    protected long keepAliveMultiplier;

    /**
     * Obtiene el valor de la propiedad keepAlivePeriod.
     * 
     */
    public long getKeepAlivePeriod() {
        return keepAlivePeriod;
    }

    /**
     * Define el valor de la propiedad keepAlivePeriod.
     * 
     */
    public void setKeepAlivePeriod(long value) {
        this.keepAlivePeriod = value;
    }

    /**
     * Obtiene el valor de la propiedad keepAliveMultiplier.
     * 
     */
    public long getKeepAliveMultiplier() {
        return keepAliveMultiplier;
    }

    /**
     * Define el valor de la propiedad keepAliveMultiplier.
     * 
     */
    public void setKeepAliveMultiplier(long value) {
        this.keepAliveMultiplier = value;
    }

}

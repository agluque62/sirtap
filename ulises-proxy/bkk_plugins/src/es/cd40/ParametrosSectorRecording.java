
package es.cd40;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlSeeAlso;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para ParametrosSectorRecording complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="ParametrosSectorRecording"&gt;
 *   &lt;complexContent&gt;
 *     &lt;extension base="{http://CD40.es/}ParametrosSectorSCVKeepAlive"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="GrabacionEd137" type="{http://www.w3.org/2001/XMLSchema}boolean"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/extension&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "ParametrosSectorRecording", propOrder = {
    "grabacionEd137"
})
@XmlSeeAlso({
    ParametrosSector.class
})
public class ParametrosSectorRecording
    extends ParametrosSectorSCVKeepAlive
{

    @XmlElement(name = "GrabacionEd137")
    protected boolean grabacionEd137;

    /**
     * Obtiene el valor de la propiedad grabacionEd137.
     * 
     */
    public boolean isGrabacionEd137() {
        return grabacionEd137;
    }

    /**
     * Define el valor de la propiedad grabacionEd137.
     * 
     */
    public void setGrabacionEd137(boolean value) {
        this.grabacionEd137 = value;
    }

}

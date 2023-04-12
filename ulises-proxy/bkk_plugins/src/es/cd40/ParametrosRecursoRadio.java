
package es.cd40;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlSchemaType;
import javax.xml.bind.annotation.XmlSeeAlso;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para ParametrosRecursoRadio complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="ParametrosRecursoRadio"&gt;
 *   &lt;complexContent&gt;
 *     &lt;extension base="{http://CD40.es/}ParametrosRDSCV"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="GananciaAGCTX" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="GananciaAGCTXdBm" type="{http://www.w3.org/2001/XMLSchema}float"/&gt;
 *         &lt;element name="GananciaAGCRX" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="GananciaAGCRXdBm" type="{http://www.w3.org/2001/XMLSchema}float"/&gt;
 *         &lt;element name="SupresionSilencio" type="{http://www.w3.org/2001/XMLSchema}boolean"/&gt;
 *         &lt;element name="TamRTP" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="Codec" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
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
@XmlType(name = "ParametrosRecursoRadio", propOrder = {
    "gananciaAGCTX",
    "gananciaAGCTXdBm",
    "gananciaAGCRX",
    "gananciaAGCRXdBm",
    "supresionSilencio",
    "tamRTP",
    "codec",
    "grabacionEd137"
})
@XmlSeeAlso({
    ParametrosRecursosRadioKeepAlive.class
})
public class ParametrosRecursoRadio
    extends ParametrosRDSCV
{

    @XmlElement(name = "GananciaAGCTX")
    @XmlSchemaType(name = "unsignedInt")
    protected long gananciaAGCTX;
    @XmlElement(name = "GananciaAGCTXdBm")
    protected float gananciaAGCTXdBm;
    @XmlElement(name = "GananciaAGCRX")
    @XmlSchemaType(name = "unsignedInt")
    protected long gananciaAGCRX;
    @XmlElement(name = "GananciaAGCRXdBm")
    protected float gananciaAGCRXdBm;
    @XmlElement(name = "SupresionSilencio")
    protected boolean supresionSilencio;
    @XmlElement(name = "TamRTP")
    @XmlSchemaType(name = "unsignedInt")
    protected long tamRTP;
    @XmlElement(name = "Codec")
    @XmlSchemaType(name = "unsignedInt")
    protected long codec;
    @XmlElement(name = "GrabacionEd137")
    protected boolean grabacionEd137;

    /**
     * Obtiene el valor de la propiedad gananciaAGCTX.
     * 
     */
    public long getGananciaAGCTX() {
        return gananciaAGCTX;
    }

    /**
     * Define el valor de la propiedad gananciaAGCTX.
     * 
     */
    public void setGananciaAGCTX(long value) {
        this.gananciaAGCTX = value;
    }

    /**
     * Obtiene el valor de la propiedad gananciaAGCTXdBm.
     * 
     */
    public float getGananciaAGCTXdBm() {
        return gananciaAGCTXdBm;
    }

    /**
     * Define el valor de la propiedad gananciaAGCTXdBm.
     * 
     */
    public void setGananciaAGCTXdBm(float value) {
        this.gananciaAGCTXdBm = value;
    }

    /**
     * Obtiene el valor de la propiedad gananciaAGCRX.
     * 
     */
    public long getGananciaAGCRX() {
        return gananciaAGCRX;
    }

    /**
     * Define el valor de la propiedad gananciaAGCRX.
     * 
     */
    public void setGananciaAGCRX(long value) {
        this.gananciaAGCRX = value;
    }

    /**
     * Obtiene el valor de la propiedad gananciaAGCRXdBm.
     * 
     */
    public float getGananciaAGCRXdBm() {
        return gananciaAGCRXdBm;
    }

    /**
     * Define el valor de la propiedad gananciaAGCRXdBm.
     * 
     */
    public void setGananciaAGCRXdBm(float value) {
        this.gananciaAGCRXdBm = value;
    }

    /**
     * Obtiene el valor de la propiedad supresionSilencio.
     * 
     */
    public boolean isSupresionSilencio() {
        return supresionSilencio;
    }

    /**
     * Define el valor de la propiedad supresionSilencio.
     * 
     */
    public void setSupresionSilencio(boolean value) {
        this.supresionSilencio = value;
    }

    /**
     * Obtiene el valor de la propiedad tamRTP.
     * 
     */
    public long getTamRTP() {
        return tamRTP;
    }

    /**
     * Define el valor de la propiedad tamRTP.
     * 
     */
    public void setTamRTP(long value) {
        this.tamRTP = value;
    }

    /**
     * Obtiene el valor de la propiedad codec.
     * 
     */
    public long getCodec() {
        return codec;
    }

    /**
     * Define el valor de la propiedad codec.
     * 
     */
    public void setCodec(long value) {
        this.codec = value;
    }

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

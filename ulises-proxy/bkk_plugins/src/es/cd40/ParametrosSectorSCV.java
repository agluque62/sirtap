
package es.cd40;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlSchemaType;
import javax.xml.bind.annotation.XmlSeeAlso;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para ParametrosSectorSCV complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="ParametrosSectorSCV"&gt;
 *   &lt;complexContent&gt;
 *     &lt;extension base="{http://CD40.es/}Tablas"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="NumLlamadasEntrantesIda" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="NumLlamadasEnIda" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="NumFrecPagina" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="NumPagFrec" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="NumEnlacesInternosPag" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="NumPagEnlacesInt" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="NumEnlacesAI" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="Intrusion" type="{http://www.w3.org/2001/XMLSchema}boolean"/&gt;
 *         &lt;element name="Intruido" type="{http://www.w3.org/2001/XMLSchema}boolean"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/extension&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "ParametrosSectorSCV", propOrder = {
    "numLlamadasEntrantesIda",
    "numLlamadasEnIda",
    "numFrecPagina",
    "numPagFrec",
    "numEnlacesInternosPag",
    "numPagEnlacesInt",
    "numEnlacesAI",
    "intrusion",
    "intruido"
})
@XmlSeeAlso({
    ParametrosSectorSCVKeepAlive.class
})
public class ParametrosSectorSCV
    extends Tablas
{

    @XmlElement(name = "NumLlamadasEntrantesIda")
    @XmlSchemaType(name = "unsignedInt")
    protected long numLlamadasEntrantesIda;
    @XmlElement(name = "NumLlamadasEnIda")
    @XmlSchemaType(name = "unsignedInt")
    protected long numLlamadasEnIda;
    @XmlElement(name = "NumFrecPagina")
    @XmlSchemaType(name = "unsignedInt")
    protected long numFrecPagina;
    @XmlElement(name = "NumPagFrec")
    @XmlSchemaType(name = "unsignedInt")
    protected long numPagFrec;
    @XmlElement(name = "NumEnlacesInternosPag")
    @XmlSchemaType(name = "unsignedInt")
    protected long numEnlacesInternosPag;
    @XmlElement(name = "NumPagEnlacesInt")
    @XmlSchemaType(name = "unsignedInt")
    protected long numPagEnlacesInt;
    @XmlElement(name = "NumEnlacesAI")
    @XmlSchemaType(name = "unsignedInt")
    protected long numEnlacesAI;
    @XmlElement(name = "Intrusion")
    protected boolean intrusion;
    @XmlElement(name = "Intruido")
    protected boolean intruido;

    /**
     * Obtiene el valor de la propiedad numLlamadasEntrantesIda.
     * 
     */
    public long getNumLlamadasEntrantesIda() {
        return numLlamadasEntrantesIda;
    }

    /**
     * Define el valor de la propiedad numLlamadasEntrantesIda.
     * 
     */
    public void setNumLlamadasEntrantesIda(long value) {
        this.numLlamadasEntrantesIda = value;
    }

    /**
     * Obtiene el valor de la propiedad numLlamadasEnIda.
     * 
     */
    public long getNumLlamadasEnIda() {
        return numLlamadasEnIda;
    }

    /**
     * Define el valor de la propiedad numLlamadasEnIda.
     * 
     */
    public void setNumLlamadasEnIda(long value) {
        this.numLlamadasEnIda = value;
    }

    /**
     * Obtiene el valor de la propiedad numFrecPagina.
     * 
     */
    public long getNumFrecPagina() {
        return numFrecPagina;
    }

    /**
     * Define el valor de la propiedad numFrecPagina.
     * 
     */
    public void setNumFrecPagina(long value) {
        this.numFrecPagina = value;
    }

    /**
     * Obtiene el valor de la propiedad numPagFrec.
     * 
     */
    public long getNumPagFrec() {
        return numPagFrec;
    }

    /**
     * Define el valor de la propiedad numPagFrec.
     * 
     */
    public void setNumPagFrec(long value) {
        this.numPagFrec = value;
    }

    /**
     * Obtiene el valor de la propiedad numEnlacesInternosPag.
     * 
     */
    public long getNumEnlacesInternosPag() {
        return numEnlacesInternosPag;
    }

    /**
     * Define el valor de la propiedad numEnlacesInternosPag.
     * 
     */
    public void setNumEnlacesInternosPag(long value) {
        this.numEnlacesInternosPag = value;
    }

    /**
     * Obtiene el valor de la propiedad numPagEnlacesInt.
     * 
     */
    public long getNumPagEnlacesInt() {
        return numPagEnlacesInt;
    }

    /**
     * Define el valor de la propiedad numPagEnlacesInt.
     * 
     */
    public void setNumPagEnlacesInt(long value) {
        this.numPagEnlacesInt = value;
    }

    /**
     * Obtiene el valor de la propiedad numEnlacesAI.
     * 
     */
    public long getNumEnlacesAI() {
        return numEnlacesAI;
    }

    /**
     * Define el valor de la propiedad numEnlacesAI.
     * 
     */
    public void setNumEnlacesAI(long value) {
        this.numEnlacesAI = value;
    }

    /**
     * Obtiene el valor de la propiedad intrusion.
     * 
     */
    public boolean isIntrusion() {
        return intrusion;
    }

    /**
     * Define el valor de la propiedad intrusion.
     * 
     */
    public void setIntrusion(boolean value) {
        this.intrusion = value;
    }

    /**
     * Obtiene el valor de la propiedad intruido.
     * 
     */
    public boolean isIntruido() {
        return intruido;
    }

    /**
     * Define el valor de la propiedad intruido.
     * 
     */
    public void setIntruido(boolean value) {
        this.intruido = value;
    }

}

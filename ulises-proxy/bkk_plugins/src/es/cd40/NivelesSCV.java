
package es.cd40;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlSchemaType;
import javax.xml.bind.annotation.XmlSeeAlso;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para NivelesSCV complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="NivelesSCV"&gt;
 *   &lt;complexContent&gt;
 *     &lt;extension base="{http://CD40.es/}Tablas"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="CICL" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="CIPL" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="CPICL" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="CPIPL" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/extension&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "NivelesSCV", propOrder = {
    "cicl",
    "cipl",
    "cpicl",
    "cpipl"
})
@XmlSeeAlso({
    Niveles.class
})
public class NivelesSCV
    extends Tablas
{

    @XmlElement(name = "CICL")
    @XmlSchemaType(name = "unsignedInt")
    protected long cicl;
    @XmlElement(name = "CIPL")
    @XmlSchemaType(name = "unsignedInt")
    protected long cipl;
    @XmlElement(name = "CPICL")
    @XmlSchemaType(name = "unsignedInt")
    protected long cpicl;
    @XmlElement(name = "CPIPL")
    @XmlSchemaType(name = "unsignedInt")
    protected long cpipl;

    /**
     * Obtiene el valor de la propiedad cicl.
     * 
     */
    public long getCICL() {
        return cicl;
    }

    /**
     * Define el valor de la propiedad cicl.
     * 
     */
    public void setCICL(long value) {
        this.cicl = value;
    }

    /**
     * Obtiene el valor de la propiedad cipl.
     * 
     */
    public long getCIPL() {
        return cipl;
    }

    /**
     * Define el valor de la propiedad cipl.
     * 
     */
    public void setCIPL(long value) {
        this.cipl = value;
    }

    /**
     * Obtiene el valor de la propiedad cpicl.
     * 
     */
    public long getCPICL() {
        return cpicl;
    }

    /**
     * Define el valor de la propiedad cpicl.
     * 
     */
    public void setCPICL(long value) {
        this.cpicl = value;
    }

    /**
     * Obtiene el valor de la propiedad cpipl.
     * 
     */
    public long getCPIPL() {
        return cpipl;
    }

    /**
     * Define el valor de la propiedad cpipl.
     * 
     */
    public void setCPIPL(long value) {
        this.cpipl = value;
    }

}

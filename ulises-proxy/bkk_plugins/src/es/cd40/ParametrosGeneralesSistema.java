
package es.cd40;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlSchemaType;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para ParametrosGeneralesSistema complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="ParametrosGeneralesSistema"&gt;
 *   &lt;complexContent&gt;
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="TiempoMaximoPTT" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="TiempoSinJack1" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="TiempoSinJack2" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="TamLiteralEnlExt" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="TamLiteralEnlDA" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="TamLiteralEnlIA" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="TamLiteralEnlAG" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="TamLiteralEmplazamiento" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/restriction&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "ParametrosGeneralesSistema", propOrder = {
    "tiempoMaximoPTT",
    "tiempoSinJack1",
    "tiempoSinJack2",
    "tamLiteralEnlExt",
    "tamLiteralEnlDA",
    "tamLiteralEnlIA",
    "tamLiteralEnlAG",
    "tamLiteralEmplazamiento"
})
public class ParametrosGeneralesSistema {

    @XmlElement(name = "TiempoMaximoPTT")
    @XmlSchemaType(name = "unsignedInt")
    protected long tiempoMaximoPTT;
    @XmlElement(name = "TiempoSinJack1")
    @XmlSchemaType(name = "unsignedInt")
    protected long tiempoSinJack1;
    @XmlElement(name = "TiempoSinJack2")
    @XmlSchemaType(name = "unsignedInt")
    protected long tiempoSinJack2;
    @XmlElement(name = "TamLiteralEnlExt")
    @XmlSchemaType(name = "unsignedInt")
    protected long tamLiteralEnlExt;
    @XmlElement(name = "TamLiteralEnlDA")
    @XmlSchemaType(name = "unsignedInt")
    protected long tamLiteralEnlDA;
    @XmlElement(name = "TamLiteralEnlIA")
    @XmlSchemaType(name = "unsignedInt")
    protected long tamLiteralEnlIA;
    @XmlElement(name = "TamLiteralEnlAG")
    @XmlSchemaType(name = "unsignedInt")
    protected long tamLiteralEnlAG;
    @XmlElement(name = "TamLiteralEmplazamiento")
    @XmlSchemaType(name = "unsignedInt")
    protected long tamLiteralEmplazamiento;

    /**
     * Obtiene el valor de la propiedad tiempoMaximoPTT.
     * 
     */
    public long getTiempoMaximoPTT() {
        return tiempoMaximoPTT;
    }

    /**
     * Define el valor de la propiedad tiempoMaximoPTT.
     * 
     */
    public void setTiempoMaximoPTT(long value) {
        this.tiempoMaximoPTT = value;
    }

    /**
     * Obtiene el valor de la propiedad tiempoSinJack1.
     * 
     */
    public long getTiempoSinJack1() {
        return tiempoSinJack1;
    }

    /**
     * Define el valor de la propiedad tiempoSinJack1.
     * 
     */
    public void setTiempoSinJack1(long value) {
        this.tiempoSinJack1 = value;
    }

    /**
     * Obtiene el valor de la propiedad tiempoSinJack2.
     * 
     */
    public long getTiempoSinJack2() {
        return tiempoSinJack2;
    }

    /**
     * Define el valor de la propiedad tiempoSinJack2.
     * 
     */
    public void setTiempoSinJack2(long value) {
        this.tiempoSinJack2 = value;
    }

    /**
     * Obtiene el valor de la propiedad tamLiteralEnlExt.
     * 
     */
    public long getTamLiteralEnlExt() {
        return tamLiteralEnlExt;
    }

    /**
     * Define el valor de la propiedad tamLiteralEnlExt.
     * 
     */
    public void setTamLiteralEnlExt(long value) {
        this.tamLiteralEnlExt = value;
    }

    /**
     * Obtiene el valor de la propiedad tamLiteralEnlDA.
     * 
     */
    public long getTamLiteralEnlDA() {
        return tamLiteralEnlDA;
    }

    /**
     * Define el valor de la propiedad tamLiteralEnlDA.
     * 
     */
    public void setTamLiteralEnlDA(long value) {
        this.tamLiteralEnlDA = value;
    }

    /**
     * Obtiene el valor de la propiedad tamLiteralEnlIA.
     * 
     */
    public long getTamLiteralEnlIA() {
        return tamLiteralEnlIA;
    }

    /**
     * Define el valor de la propiedad tamLiteralEnlIA.
     * 
     */
    public void setTamLiteralEnlIA(long value) {
        this.tamLiteralEnlIA = value;
    }

    /**
     * Obtiene el valor de la propiedad tamLiteralEnlAG.
     * 
     */
    public long getTamLiteralEnlAG() {
        return tamLiteralEnlAG;
    }

    /**
     * Define el valor de la propiedad tamLiteralEnlAG.
     * 
     */
    public void setTamLiteralEnlAG(long value) {
        this.tamLiteralEnlAG = value;
    }

    /**
     * Obtiene el valor de la propiedad tamLiteralEmplazamiento.
     * 
     */
    public long getTamLiteralEmplazamiento() {
        return tamLiteralEmplazamiento;
    }

    /**
     * Define el valor de la propiedad tamLiteralEmplazamiento.
     * 
     */
    public void setTamLiteralEmplazamiento(long value) {
        this.tamLiteralEmplazamiento = value;
    }

}

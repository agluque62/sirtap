
package es.cd40;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlSchemaType;
import javax.xml.bind.annotation.XmlSeeAlso;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para RecursosSCV complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="RecursosSCV"&gt;
 *   &lt;complexContent&gt;
 *     &lt;extension base="{http://CD40.es/}Tablas"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="IdRecurso" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="Tipo" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="Interface" type="{http://CD40.es/}TipoInterface"/&gt;
 *         &lt;element name="SlotPasarela" type="{http://www.w3.org/2001/XMLSchema}int"/&gt;
 *         &lt;element name="NumDispositivoSlot" type="{http://www.w3.org/2001/XMLSchema}int"/&gt;
 *         &lt;element name="ServidorSIP" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="Diffserv" type="{http://www.w3.org/2001/XMLSchema}boolean"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/extension&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "RecursosSCV", propOrder = {
    "idRecurso",
    "tipo",
    "_interface",
    "slotPasarela",
    "numDispositivoSlot",
    "servidorSIP",
    "diffserv"
})
@XmlSeeAlso({
    Recursos.class
})
public class RecursosSCV
    extends Tablas
{

    @XmlElement(name = "IdRecurso")
    protected String idRecurso;
    @XmlElement(name = "Tipo")
    @XmlSchemaType(name = "unsignedInt")
    protected long tipo;
    @XmlElement(name = "Interface", required = true)
    @XmlSchemaType(name = "string")
    protected TipoInterface _interface;
    @XmlElement(name = "SlotPasarela")
    protected int slotPasarela;
    @XmlElement(name = "NumDispositivoSlot")
    protected int numDispositivoSlot;
    @XmlElement(name = "ServidorSIP")
    protected String servidorSIP;
    @XmlElement(name = "Diffserv")
    protected boolean diffserv;

    /**
     * Obtiene el valor de la propiedad idRecurso.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getIdRecurso() {
        return idRecurso;
    }

    /**
     * Define el valor de la propiedad idRecurso.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setIdRecurso(String value) {
        this.idRecurso = value;
    }

    /**
     * Obtiene el valor de la propiedad tipo.
     * 
     */
    public long getTipo() {
        return tipo;
    }

    /**
     * Define el valor de la propiedad tipo.
     * 
     */
    public void setTipo(long value) {
        this.tipo = value;
    }

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
     * Obtiene el valor de la propiedad slotPasarela.
     * 
     */
    public int getSlotPasarela() {
        return slotPasarela;
    }

    /**
     * Define el valor de la propiedad slotPasarela.
     * 
     */
    public void setSlotPasarela(int value) {
        this.slotPasarela = value;
    }

    /**
     * Obtiene el valor de la propiedad numDispositivoSlot.
     * 
     */
    public int getNumDispositivoSlot() {
        return numDispositivoSlot;
    }

    /**
     * Define el valor de la propiedad numDispositivoSlot.
     * 
     */
    public void setNumDispositivoSlot(int value) {
        this.numDispositivoSlot = value;
    }

    /**
     * Obtiene el valor de la propiedad servidorSIP.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getServidorSIP() {
        return servidorSIP;
    }

    /**
     * Define el valor de la propiedad servidorSIP.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setServidorSIP(String value) {
        this.servidorSIP = value;
    }

    /**
     * Obtiene el valor de la propiedad diffserv.
     * 
     */
    public boolean isDiffserv() {
        return diffserv;
    }

    /**
     * Define el valor de la propiedad diffserv.
     * 
     */
    public void setDiffserv(boolean value) {
        this.diffserv = value;
    }

}

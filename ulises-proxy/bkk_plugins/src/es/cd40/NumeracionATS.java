
package es.cd40;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para NumeracionATS complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="NumeracionATS"&gt;
 *   &lt;complexContent&gt;
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="Central" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="CentralPropia" type="{http://www.w3.org/2001/XMLSchema}boolean"/&gt;
 *         &lt;element name="Throwswitching" type="{http://www.w3.org/2001/XMLSchema}boolean"/&gt;
 *         &lt;element name="RangosOperador" type="{http://CD40.es/}ArrayOfRangosSCV" minOccurs="0"/&gt;
 *         &lt;element name="RangosPrivilegiados" type="{http://CD40.es/}ArrayOfRangosSCV" minOccurs="0"/&gt;
 *         &lt;element name="ListaRutas" type="{http://CD40.es/}ArrayOfPlanRutas" minOccurs="0"/&gt;
 *         &lt;element name="NumTest" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/restriction&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "NumeracionATS", propOrder = {
    "central",
    "centralPropia",
    "throwswitching",
    "rangosOperador",
    "rangosPrivilegiados",
    "listaRutas",
    "numTest"
})
public class NumeracionATS {

    @XmlElement(name = "Central")
    protected String central;
    @XmlElement(name = "CentralPropia")
    protected boolean centralPropia;
    @XmlElement(name = "Throwswitching")
    protected boolean throwswitching;
    @XmlElement(name = "RangosOperador")
    protected ArrayOfRangosSCV rangosOperador;
    @XmlElement(name = "RangosPrivilegiados")
    protected ArrayOfRangosSCV rangosPrivilegiados;
    @XmlElement(name = "ListaRutas")
    protected ArrayOfPlanRutas listaRutas;
    @XmlElement(name = "NumTest")
    protected String numTest;

    /**
     * Obtiene el valor de la propiedad central.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getCentral() {
        return central;
    }

    /**
     * Define el valor de la propiedad central.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setCentral(String value) {
        this.central = value;
    }

    /**
     * Obtiene el valor de la propiedad centralPropia.
     * 
     */
    public boolean isCentralPropia() {
        return centralPropia;
    }

    /**
     * Define el valor de la propiedad centralPropia.
     * 
     */
    public void setCentralPropia(boolean value) {
        this.centralPropia = value;
    }

    /**
     * Obtiene el valor de la propiedad throwswitching.
     * 
     */
    public boolean isThrowswitching() {
        return throwswitching;
    }

    /**
     * Define el valor de la propiedad throwswitching.
     * 
     */
    public void setThrowswitching(boolean value) {
        this.throwswitching = value;
    }

    /**
     * Obtiene el valor de la propiedad rangosOperador.
     * 
     * @return
     *     possible object is
     *     {@link ArrayOfRangosSCV }
     *     
     */
    public ArrayOfRangosSCV getRangosOperador() {
        return rangosOperador;
    }

    /**
     * Define el valor de la propiedad rangosOperador.
     * 
     * @param value
     *     allowed object is
     *     {@link ArrayOfRangosSCV }
     *     
     */
    public void setRangosOperador(ArrayOfRangosSCV value) {
        this.rangosOperador = value;
    }

    /**
     * Obtiene el valor de la propiedad rangosPrivilegiados.
     * 
     * @return
     *     possible object is
     *     {@link ArrayOfRangosSCV }
     *     
     */
    public ArrayOfRangosSCV getRangosPrivilegiados() {
        return rangosPrivilegiados;
    }

    /**
     * Define el valor de la propiedad rangosPrivilegiados.
     * 
     * @param value
     *     allowed object is
     *     {@link ArrayOfRangosSCV }
     *     
     */
    public void setRangosPrivilegiados(ArrayOfRangosSCV value) {
        this.rangosPrivilegiados = value;
    }

    /**
     * Obtiene el valor de la propiedad listaRutas.
     * 
     * @return
     *     possible object is
     *     {@link ArrayOfPlanRutas }
     *     
     */
    public ArrayOfPlanRutas getListaRutas() {
        return listaRutas;
    }

    /**
     * Define el valor de la propiedad listaRutas.
     * 
     * @param value
     *     allowed object is
     *     {@link ArrayOfPlanRutas }
     *     
     */
    public void setListaRutas(ArrayOfPlanRutas value) {
        this.listaRutas = value;
    }

    /**
     * Obtiene el valor de la propiedad numTest.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getNumTest() {
        return numTest;
    }

    /**
     * Define el valor de la propiedad numTest.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setNumTest(String value) {
        this.numTest = value;
    }

}

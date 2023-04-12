
package es.cd40;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlSchemaType;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para Top complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="Top"&gt;
 *   &lt;complexContent&gt;
 *     &lt;extension base="{http://CD40.es/}Tablas"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="IdSistema" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="IdTop" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="ModoArranque" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="PosicionSacta" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="IpRed1" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="IpRed2" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="ConexionJacksEjecutivo" type="{http://www.w3.org/2001/XMLSchema}boolean"/&gt;
 *         &lt;element name="ConexionJacksAyudante" type="{http://www.w3.org/2001/XMLSchema}boolean"/&gt;
 *         &lt;element name="NumAltavoces" type="{http://www.w3.org/2001/XMLSchema}int"/&gt;
 *         &lt;element name="Grabador1" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="Grabador2" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/extension&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "Top", propOrder = {
    "idSistema",
    "idTop",
    "modoArranque",
    "posicionSacta",
    "ipRed1",
    "ipRed2",
    "conexionJacksEjecutivo",
    "conexionJacksAyudante",
    "numAltavoces",
    "grabador1",
    "grabador2"
})
public class Top
    extends Tablas
{

    @XmlElement(name = "IdSistema")
    protected String idSistema;
    @XmlElement(name = "IdTop")
    protected String idTop;
    @XmlElement(name = "ModoArranque")
    protected String modoArranque;
    @XmlElement(name = "PosicionSacta")
    @XmlSchemaType(name = "unsignedInt")
    protected long posicionSacta;
    @XmlElement(name = "IpRed1")
    protected String ipRed1;
    @XmlElement(name = "IpRed2")
    protected String ipRed2;
    @XmlElement(name = "ConexionJacksEjecutivo")
    protected boolean conexionJacksEjecutivo;
    @XmlElement(name = "ConexionJacksAyudante")
    protected boolean conexionJacksAyudante;
    @XmlElement(name = "NumAltavoces")
    protected int numAltavoces;
    @XmlElement(name = "Grabador1")
    protected String grabador1;
    @XmlElement(name = "Grabador2")
    protected String grabador2;

    /**
     * Obtiene el valor de la propiedad idSistema.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getIdSistema() {
        return idSistema;
    }

    /**
     * Define el valor de la propiedad idSistema.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setIdSistema(String value) {
        this.idSistema = value;
    }

    /**
     * Obtiene el valor de la propiedad idTop.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getIdTop() {
        return idTop;
    }

    /**
     * Define el valor de la propiedad idTop.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setIdTop(String value) {
        this.idTop = value;
    }

    /**
     * Obtiene el valor de la propiedad modoArranque.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getModoArranque() {
        return modoArranque;
    }

    /**
     * Define el valor de la propiedad modoArranque.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setModoArranque(String value) {
        this.modoArranque = value;
    }

    /**
     * Obtiene el valor de la propiedad posicionSacta.
     * 
     */
    public long getPosicionSacta() {
        return posicionSacta;
    }

    /**
     * Define el valor de la propiedad posicionSacta.
     * 
     */
    public void setPosicionSacta(long value) {
        this.posicionSacta = value;
    }

    /**
     * Obtiene el valor de la propiedad ipRed1.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getIpRed1() {
        return ipRed1;
    }

    /**
     * Define el valor de la propiedad ipRed1.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setIpRed1(String value) {
        this.ipRed1 = value;
    }

    /**
     * Obtiene el valor de la propiedad ipRed2.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getIpRed2() {
        return ipRed2;
    }

    /**
     * Define el valor de la propiedad ipRed2.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setIpRed2(String value) {
        this.ipRed2 = value;
    }

    /**
     * Obtiene el valor de la propiedad conexionJacksEjecutivo.
     * 
     */
    public boolean isConexionJacksEjecutivo() {
        return conexionJacksEjecutivo;
    }

    /**
     * Define el valor de la propiedad conexionJacksEjecutivo.
     * 
     */
    public void setConexionJacksEjecutivo(boolean value) {
        this.conexionJacksEjecutivo = value;
    }

    /**
     * Obtiene el valor de la propiedad conexionJacksAyudante.
     * 
     */
    public boolean isConexionJacksAyudante() {
        return conexionJacksAyudante;
    }

    /**
     * Define el valor de la propiedad conexionJacksAyudante.
     * 
     */
    public void setConexionJacksAyudante(boolean value) {
        this.conexionJacksAyudante = value;
    }

    /**
     * Obtiene el valor de la propiedad numAltavoces.
     * 
     */
    public int getNumAltavoces() {
        return numAltavoces;
    }

    /**
     * Define el valor de la propiedad numAltavoces.
     * 
     */
    public void setNumAltavoces(int value) {
        this.numAltavoces = value;
    }

    /**
     * Obtiene el valor de la propiedad grabador1.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getGrabador1() {
        return grabador1;
    }

    /**
     * Define el valor de la propiedad grabador1.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setGrabador1(String value) {
        this.grabador1 = value;
    }

    /**
     * Obtiene el valor de la propiedad grabador2.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getGrabador2() {
        return grabador2;
    }

    /**
     * Define el valor de la propiedad grabador2.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setGrabador2(String value) {
        this.grabador2 = value;
    }

}

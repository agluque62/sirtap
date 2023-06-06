
package es.cd40;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlSchemaType;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para TifX complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="TifX"&gt;
 *   &lt;complexContent&gt;
 *     &lt;extension base="{http://CD40.es/}Tablas"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="IdSistema" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="IdTifx" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="ModoArranque" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="ModoSincronizacion" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="Master" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="SNMPPortLocal" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="SNMPPortRemoto" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="SNMPTraps" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="SIPPortLocal" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="TimeSupervision" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="IpRed1" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="IpRed2" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="Grabador1" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="Grabador2" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="iSupervLanGW" type="{http://www.w3.org/2001/XMLSchema}unsignedByte"/&gt;
 *         &lt;element name="itmmaxSupervLanGW" type="{http://www.w3.org/2001/XMLSchema}unsignedByte"/&gt;
 *         &lt;element name="GrabacionED137" type="{http://www.w3.org/2001/XMLSchema}int"/&gt;
 *         &lt;element name="RtspGrabador1" type="{http://www.w3.org/2001/XMLSchema}int"/&gt;
 *         &lt;element name="RtspGrabador2" type="{http://www.w3.org/2001/XMLSchema}int"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/extension&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "TifX", propOrder = {
    "idSistema",
    "idTifx",
    "modoArranque",
    "modoSincronizacion",
    "master",
    "snmpPortLocal",
    "snmpPortRemoto",
    "snmpTraps",
    "sipPortLocal",
    "timeSupervision",
    "ipRed1",
    "ipRed2",
    "grabador1",
    "grabador2",
    "iSupervLanGW",
    "itmmaxSupervLanGW",
    "grabacionED137",
    "rtspGrabador1",
    "rtspGrabador2"
})
public class TifX
    extends Tablas
{

    @XmlElement(name = "IdSistema")
    protected String idSistema;
    @XmlElement(name = "IdTifx")
    protected String idTifx;
    @XmlElement(name = "ModoArranque")
    protected String modoArranque;
    @XmlElement(name = "ModoSincronizacion")
    @XmlSchemaType(name = "unsignedInt")
    protected long modoSincronizacion;
    @XmlElement(name = "Master")
    protected String master;
    @XmlElement(name = "SNMPPortLocal")
    @XmlSchemaType(name = "unsignedInt")
    protected long snmpPortLocal;
    @XmlElement(name = "SNMPPortRemoto")
    @XmlSchemaType(name = "unsignedInt")
    protected long snmpPortRemoto;
    @XmlElement(name = "SNMPTraps")
    @XmlSchemaType(name = "unsignedInt")
    protected long snmpTraps;
    @XmlElement(name = "SIPPortLocal")
    @XmlSchemaType(name = "unsignedInt")
    protected long sipPortLocal;
    @XmlElement(name = "TimeSupervision")
    @XmlSchemaType(name = "unsignedInt")
    protected long timeSupervision;
    @XmlElement(name = "IpRed1")
    protected String ipRed1;
    @XmlElement(name = "IpRed2")
    protected String ipRed2;
    @XmlElement(name = "Grabador1")
    protected String grabador1;
    @XmlElement(name = "Grabador2")
    protected String grabador2;
    @XmlSchemaType(name = "unsignedByte")
    protected short iSupervLanGW;
    @XmlSchemaType(name = "unsignedByte")
    protected short itmmaxSupervLanGW;
    @XmlElement(name = "GrabacionED137")
    protected int grabacionED137;
    @XmlElement(name = "RtspGrabador1")
    protected int rtspGrabador1;
    @XmlElement(name = "RtspGrabador2")
    protected int rtspGrabador2;

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
     * Obtiene el valor de la propiedad idTifx.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getIdTifx() {
        return idTifx;
    }

    /**
     * Define el valor de la propiedad idTifx.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setIdTifx(String value) {
        this.idTifx = value;
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
     * Obtiene el valor de la propiedad modoSincronizacion.
     * 
     */
    public long getModoSincronizacion() {
        return modoSincronizacion;
    }

    /**
     * Define el valor de la propiedad modoSincronizacion.
     * 
     */
    public void setModoSincronizacion(long value) {
        this.modoSincronizacion = value;
    }

    /**
     * Obtiene el valor de la propiedad master.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getMaster() {
        return master;
    }

    /**
     * Define el valor de la propiedad master.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setMaster(String value) {
        this.master = value;
    }

    /**
     * Obtiene el valor de la propiedad snmpPortLocal.
     * 
     */
    public long getSNMPPortLocal() {
        return snmpPortLocal;
    }

    /**
     * Define el valor de la propiedad snmpPortLocal.
     * 
     */
    public void setSNMPPortLocal(long value) {
        this.snmpPortLocal = value;
    }

    /**
     * Obtiene el valor de la propiedad snmpPortRemoto.
     * 
     */
    public long getSNMPPortRemoto() {
        return snmpPortRemoto;
    }

    /**
     * Define el valor de la propiedad snmpPortRemoto.
     * 
     */
    public void setSNMPPortRemoto(long value) {
        this.snmpPortRemoto = value;
    }

    /**
     * Obtiene el valor de la propiedad snmpTraps.
     * 
     */
    public long getSNMPTraps() {
        return snmpTraps;
    }

    /**
     * Define el valor de la propiedad snmpTraps.
     * 
     */
    public void setSNMPTraps(long value) {
        this.snmpTraps = value;
    }

    /**
     * Obtiene el valor de la propiedad sipPortLocal.
     * 
     */
    public long getSIPPortLocal() {
        return sipPortLocal;
    }

    /**
     * Define el valor de la propiedad sipPortLocal.
     * 
     */
    public void setSIPPortLocal(long value) {
        this.sipPortLocal = value;
    }

    /**
     * Obtiene el valor de la propiedad timeSupervision.
     * 
     */
    public long getTimeSupervision() {
        return timeSupervision;
    }

    /**
     * Define el valor de la propiedad timeSupervision.
     * 
     */
    public void setTimeSupervision(long value) {
        this.timeSupervision = value;
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

    /**
     * Obtiene el valor de la propiedad iSupervLanGW.
     * 
     */
    public short getISupervLanGW() {
        return iSupervLanGW;
    }

    /**
     * Define el valor de la propiedad iSupervLanGW.
     * 
     */
    public void setISupervLanGW(short value) {
        this.iSupervLanGW = value;
    }

    /**
     * Obtiene el valor de la propiedad itmmaxSupervLanGW.
     * 
     */
    public short getItmmaxSupervLanGW() {
        return itmmaxSupervLanGW;
    }

    /**
     * Define el valor de la propiedad itmmaxSupervLanGW.
     * 
     */
    public void setItmmaxSupervLanGW(short value) {
        this.itmmaxSupervLanGW = value;
    }

    /**
     * Obtiene el valor de la propiedad grabacionED137.
     * 
     */
    public int getGrabacionED137() {
        return grabacionED137;
    }

    /**
     * Define el valor de la propiedad grabacionED137.
     * 
     */
    public void setGrabacionED137(int value) {
        this.grabacionED137 = value;
    }

    /**
     * Obtiene el valor de la propiedad rtspGrabador1.
     * 
     */
    public int getRtspGrabador1() {
        return rtspGrabador1;
    }

    /**
     * Define el valor de la propiedad rtspGrabador1.
     * 
     */
    public void setRtspGrabador1(int value) {
        this.rtspGrabador1 = value;
    }

    /**
     * Obtiene el valor de la propiedad rtspGrabador2.
     * 
     */
    public int getRtspGrabador2() {
        return rtspGrabador2;
    }

    /**
     * Define el valor de la propiedad rtspGrabador2.
     * 
     */
    public void setRtspGrabador2(int value) {
        this.rtspGrabador2 = value;
    }

}

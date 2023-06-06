
package es.cd40;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlSchemaType;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para CfgPasarela complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="CfgPasarela"&gt;
 *   &lt;complexContent&gt;
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="Nombre" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="NumRecursos" type="{http://www.w3.org/2001/XMLSchema}int"/&gt;
 *         &lt;element name="ModoSincronizacion" type="{http://www.w3.org/2001/XMLSchema}int"/&gt;
 *         &lt;element name="MasterSincronizacion" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="PuertoLocalSNMP" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="PuertoRemotoSNMP" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="PuertoRemotoTrapsSNMP" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="PuertoLocalSIP" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="PeriodoSupervisionSIP" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="IpGrabador1" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="IpGrabador2" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="iSupervLanGW" type="{http://www.w3.org/2001/XMLSchema}unsignedByte"/&gt;
 *         &lt;element name="itmmaxSupervLanGW" type="{http://www.w3.org/2001/XMLSchema}unsignedByte"/&gt;
 *         &lt;element name="EnableGrabacionEd137" type="{http://www.w3.org/2001/XMLSchema}boolean"/&gt;
 *         &lt;element name="RtspPort" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="RtspPort1" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="ListaRecursos" type="{http://CD40.es/}ArrayOfRecursosSCV" minOccurs="0"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/restriction&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "CfgPasarela", propOrder = {
    "nombre",
    "numRecursos",
    "modoSincronizacion",
    "masterSincronizacion",
    "puertoLocalSNMP",
    "puertoRemotoSNMP",
    "puertoRemotoTrapsSNMP",
    "puertoLocalSIP",
    "periodoSupervisionSIP",
    "ipGrabador1",
    "ipGrabador2",
    "iSupervLanGW",
    "itmmaxSupervLanGW",
    "enableGrabacionEd137",
    "rtspPort",
    "rtspPort1",
    "listaRecursos"
})
public class CfgPasarela {

    @XmlElement(name = "Nombre")
    protected String nombre;
    @XmlElement(name = "NumRecursos")
    protected int numRecursos;
    @XmlElement(name = "ModoSincronizacion")
    protected int modoSincronizacion;
    @XmlElement(name = "MasterSincronizacion")
    protected String masterSincronizacion;
    @XmlElement(name = "PuertoLocalSNMP")
    @XmlSchemaType(name = "unsignedInt")
    protected long puertoLocalSNMP;
    @XmlElement(name = "PuertoRemotoSNMP")
    @XmlSchemaType(name = "unsignedInt")
    protected long puertoRemotoSNMP;
    @XmlElement(name = "PuertoRemotoTrapsSNMP")
    @XmlSchemaType(name = "unsignedInt")
    protected long puertoRemotoTrapsSNMP;
    @XmlElement(name = "PuertoLocalSIP")
    @XmlSchemaType(name = "unsignedInt")
    protected long puertoLocalSIP;
    @XmlElement(name = "PeriodoSupervisionSIP")
    @XmlSchemaType(name = "unsignedInt")
    protected long periodoSupervisionSIP;
    @XmlElement(name = "IpGrabador1")
    protected String ipGrabador1;
    @XmlElement(name = "IpGrabador2")
    protected String ipGrabador2;
    @XmlSchemaType(name = "unsignedByte")
    protected short iSupervLanGW;
    @XmlSchemaType(name = "unsignedByte")
    protected short itmmaxSupervLanGW;
    @XmlElement(name = "EnableGrabacionEd137")
    protected boolean enableGrabacionEd137;
    @XmlElement(name = "RtspPort")
    @XmlSchemaType(name = "unsignedInt")
    protected long rtspPort;
    @XmlElement(name = "RtspPort1")
    @XmlSchemaType(name = "unsignedInt")
    protected long rtspPort1;
    @XmlElement(name = "ListaRecursos")
    protected ArrayOfRecursosSCV listaRecursos;

    /**
     * Obtiene el valor de la propiedad nombre.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getNombre() {
        return nombre;
    }

    /**
     * Define el valor de la propiedad nombre.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setNombre(String value) {
        this.nombre = value;
    }

    /**
     * Obtiene el valor de la propiedad numRecursos.
     * 
     */
    public int getNumRecursos() {
        return numRecursos;
    }

    /**
     * Define el valor de la propiedad numRecursos.
     * 
     */
    public void setNumRecursos(int value) {
        this.numRecursos = value;
    }

    /**
     * Obtiene el valor de la propiedad modoSincronizacion.
     * 
     */
    public int getModoSincronizacion() {
        return modoSincronizacion;
    }

    /**
     * Define el valor de la propiedad modoSincronizacion.
     * 
     */
    public void setModoSincronizacion(int value) {
        this.modoSincronizacion = value;
    }

    /**
     * Obtiene el valor de la propiedad masterSincronizacion.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getMasterSincronizacion() {
        return masterSincronizacion;
    }

    /**
     * Define el valor de la propiedad masterSincronizacion.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setMasterSincronizacion(String value) {
        this.masterSincronizacion = value;
    }

    /**
     * Obtiene el valor de la propiedad puertoLocalSNMP.
     * 
     */
    public long getPuertoLocalSNMP() {
        return puertoLocalSNMP;
    }

    /**
     * Define el valor de la propiedad puertoLocalSNMP.
     * 
     */
    public void setPuertoLocalSNMP(long value) {
        this.puertoLocalSNMP = value;
    }

    /**
     * Obtiene el valor de la propiedad puertoRemotoSNMP.
     * 
     */
    public long getPuertoRemotoSNMP() {
        return puertoRemotoSNMP;
    }

    /**
     * Define el valor de la propiedad puertoRemotoSNMP.
     * 
     */
    public void setPuertoRemotoSNMP(long value) {
        this.puertoRemotoSNMP = value;
    }

    /**
     * Obtiene el valor de la propiedad puertoRemotoTrapsSNMP.
     * 
     */
    public long getPuertoRemotoTrapsSNMP() {
        return puertoRemotoTrapsSNMP;
    }

    /**
     * Define el valor de la propiedad puertoRemotoTrapsSNMP.
     * 
     */
    public void setPuertoRemotoTrapsSNMP(long value) {
        this.puertoRemotoTrapsSNMP = value;
    }

    /**
     * Obtiene el valor de la propiedad puertoLocalSIP.
     * 
     */
    public long getPuertoLocalSIP() {
        return puertoLocalSIP;
    }

    /**
     * Define el valor de la propiedad puertoLocalSIP.
     * 
     */
    public void setPuertoLocalSIP(long value) {
        this.puertoLocalSIP = value;
    }

    /**
     * Obtiene el valor de la propiedad periodoSupervisionSIP.
     * 
     */
    public long getPeriodoSupervisionSIP() {
        return periodoSupervisionSIP;
    }

    /**
     * Define el valor de la propiedad periodoSupervisionSIP.
     * 
     */
    public void setPeriodoSupervisionSIP(long value) {
        this.periodoSupervisionSIP = value;
    }

    /**
     * Obtiene el valor de la propiedad ipGrabador1.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getIpGrabador1() {
        return ipGrabador1;
    }

    /**
     * Define el valor de la propiedad ipGrabador1.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setIpGrabador1(String value) {
        this.ipGrabador1 = value;
    }

    /**
     * Obtiene el valor de la propiedad ipGrabador2.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getIpGrabador2() {
        return ipGrabador2;
    }

    /**
     * Define el valor de la propiedad ipGrabador2.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setIpGrabador2(String value) {
        this.ipGrabador2 = value;
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
     * Obtiene el valor de la propiedad enableGrabacionEd137.
     * 
     */
    public boolean isEnableGrabacionEd137() {
        return enableGrabacionEd137;
    }

    /**
     * Define el valor de la propiedad enableGrabacionEd137.
     * 
     */
    public void setEnableGrabacionEd137(boolean value) {
        this.enableGrabacionEd137 = value;
    }

    /**
     * Obtiene el valor de la propiedad rtspPort.
     * 
     */
    public long getRtspPort() {
        return rtspPort;
    }

    /**
     * Define el valor de la propiedad rtspPort.
     * 
     */
    public void setRtspPort(long value) {
        this.rtspPort = value;
    }

    /**
     * Obtiene el valor de la propiedad rtspPort1.
     * 
     */
    public long getRtspPort1() {
        return rtspPort1;
    }

    /**
     * Define el valor de la propiedad rtspPort1.
     * 
     */
    public void setRtspPort1(long value) {
        this.rtspPort1 = value;
    }

    /**
     * Obtiene el valor de la propiedad listaRecursos.
     * 
     * @return
     *     possible object is
     *     {@link ArrayOfRecursosSCV }
     *     
     */
    public ArrayOfRecursosSCV getListaRecursos() {
        return listaRecursos;
    }

    /**
     * Define el valor de la propiedad listaRecursos.
     * 
     * @param value
     *     allowed object is
     *     {@link ArrayOfRecursosSCV }
     *     
     */
    public void setListaRecursos(ArrayOfRecursosSCV value) {
        this.listaRecursos = value;
    }

}


package es.cd40;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlSchemaType;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para DestinosRadio complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="DestinosRadio"&gt;
 *   &lt;complexContent&gt;
 *     &lt;extension base="{http://CD40.es/}Destinos"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="Fk_Metodosbss" type="{http://www.w3.org/2001/XMLSchema}int"/&gt;
 *         &lt;element name="Zonas_IdZonas" type="{http://www.w3.org/2001/XMLSchema}int"/&gt;
 *         &lt;element name="TipoFrec" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="ExclusividadTXRX" type="{http://www.w3.org/2001/XMLSchema}boolean"/&gt;
 *         &lt;element name="Frecuencia" type="{http://www.w3.org/2001/XMLSchema}int"/&gt;
 *         &lt;element name="MetodoCalculoClimax" type="{http://www.w3.org/2001/XMLSchema}int"/&gt;
 *         &lt;element name="VentanaSeleccionBss" type="{http://www.w3.org/2001/XMLSchema}int"/&gt;
 *         &lt;element name="SincronizaGrupoClimax" type="{http://www.w3.org/2001/XMLSchema}boolean"/&gt;
 *         &lt;element name="AudioPrimerSqBss" type="{http://www.w3.org/2001/XMLSchema}boolean"/&gt;
 *         &lt;element name="MetodosBss" type="{http://CD40.es/}ArrayOfMetodosBssDelRecurso" minOccurs="0"/&gt;
 *         &lt;element name="FrecuenciaNoDesasignable" type="{http://www.w3.org/2001/XMLSchema}boolean"/&gt;
 *         &lt;element name="VentanaReposoZonaTxDefecto" type="{http://www.w3.org/2001/XMLSchema}int"/&gt;
 *         &lt;element name="PrioridadSesionSip" type="{http://www.w3.org/2001/XMLSchema}int"/&gt;
 *         &lt;element name="MetodosBssOfrecidos" type="{http://www.w3.org/2001/XMLSchema}int"/&gt;
 *         &lt;element name="CldSupervisionTime" type="{http://www.w3.org/2001/XMLSchema}int"/&gt;
 *         &lt;element name="NombreZonaTxDefecto" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="CnfModoDestino" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="CnfTipoFrecuencia" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="ModoTransmision" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="EmplazamientoDefecto" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="TiempoVueltaADefecto" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="PorcentajeRSSI" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="ConRedundancia" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="DescDestino" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="PasivoRetransmision" type="{http://www.w3.org/2001/XMLSchema}boolean"/&gt;
 *         &lt;element name="MultiFrecuencia" type="{http://www.w3.org/2001/XMLSchema}boolean"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/extension&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "DestinosRadio", propOrder = {
    "fkMetodosbss",
    "zonasIdZonas",
    "tipoFrec",
    "exclusividadTXRX",
    "frecuencia",
    "metodoCalculoClimax",
    "ventanaSeleccionBss",
    "sincronizaGrupoClimax",
    "audioPrimerSqBss",
    "metodosBss",
    "frecuenciaNoDesasignable",
    "ventanaReposoZonaTxDefecto",
    "prioridadSesionSip",
    "metodosBssOfrecidos",
    "cldSupervisionTime",
    "nombreZonaTxDefecto",
    "cnfModoDestino",
    "cnfTipoFrecuencia",
    "modoTransmision",
    "emplazamientoDefecto",
    "tiempoVueltaADefecto",
    "porcentajeRSSI",
    "conRedundancia",
    "descDestino",
    "pasivoRetransmision",
    "multiFrecuencia"
})
public class DestinosRadio
    extends Destinos
{

    @XmlElement(name = "Fk_Metodosbss")
    protected int fkMetodosbss;
    @XmlElement(name = "Zonas_IdZonas")
    protected int zonasIdZonas;
    @XmlElement(name = "TipoFrec")
    @XmlSchemaType(name = "unsignedInt")
    protected long tipoFrec;
    @XmlElement(name = "ExclusividadTXRX")
    protected boolean exclusividadTXRX;
    @XmlElement(name = "Frecuencia")
    protected int frecuencia;
    @XmlElement(name = "MetodoCalculoClimax")
    protected int metodoCalculoClimax;
    @XmlElement(name = "VentanaSeleccionBss")
    protected int ventanaSeleccionBss;
    @XmlElement(name = "SincronizaGrupoClimax")
    protected boolean sincronizaGrupoClimax;
    @XmlElement(name = "AudioPrimerSqBss")
    protected boolean audioPrimerSqBss;
    @XmlElement(name = "MetodosBss")
    protected ArrayOfMetodosBssDelRecurso metodosBss;
    @XmlElement(name = "FrecuenciaNoDesasignable")
    protected boolean frecuenciaNoDesasignable;
    @XmlElement(name = "VentanaReposoZonaTxDefecto")
    protected int ventanaReposoZonaTxDefecto;
    @XmlElement(name = "PrioridadSesionSip")
    protected int prioridadSesionSip;
    @XmlElement(name = "MetodosBssOfrecidos")
    protected int metodosBssOfrecidos;
    @XmlElement(name = "CldSupervisionTime")
    protected int cldSupervisionTime;
    @XmlElement(name = "NombreZonaTxDefecto")
    protected String nombreZonaTxDefecto;
    @XmlElement(name = "CnfModoDestino")
    protected String cnfModoDestino;
    @XmlElement(name = "CnfTipoFrecuencia")
    protected String cnfTipoFrecuencia;
    @XmlElement(name = "ModoTransmision")
    protected String modoTransmision;
    @XmlElement(name = "EmplazamientoDefecto")
    protected String emplazamientoDefecto;
    @XmlElement(name = "TiempoVueltaADefecto")
    protected String tiempoVueltaADefecto;
    @XmlElement(name = "PorcentajeRSSI")
    protected String porcentajeRSSI;
    @XmlElement(name = "ConRedundancia")
    protected String conRedundancia;
    @XmlElement(name = "DescDestino")
    protected String descDestino;
    @XmlElement(name = "PasivoRetransmision")
    protected boolean pasivoRetransmision;
    @XmlElement(name = "MultiFrecuencia")
    protected boolean multiFrecuencia;

    /**
     * Obtiene el valor de la propiedad fkMetodosbss.
     * 
     */
    public int getFkMetodosbss() {
        return fkMetodosbss;
    }

    /**
     * Define el valor de la propiedad fkMetodosbss.
     * 
     */
    public void setFkMetodosbss(int value) {
        this.fkMetodosbss = value;
    }

    /**
     * Obtiene el valor de la propiedad zonasIdZonas.
     * 
     */
    public int getZonasIdZonas() {
        return zonasIdZonas;
    }

    /**
     * Define el valor de la propiedad zonasIdZonas.
     * 
     */
    public void setZonasIdZonas(int value) {
        this.zonasIdZonas = value;
    }

    /**
     * Obtiene el valor de la propiedad tipoFrec.
     * 
     */
    public long getTipoFrec() {
        return tipoFrec;
    }

    /**
     * Define el valor de la propiedad tipoFrec.
     * 
     */
    public void setTipoFrec(long value) {
        this.tipoFrec = value;
    }

    /**
     * Obtiene el valor de la propiedad exclusividadTXRX.
     * 
     */
    public boolean isExclusividadTXRX() {
        return exclusividadTXRX;
    }

    /**
     * Define el valor de la propiedad exclusividadTXRX.
     * 
     */
    public void setExclusividadTXRX(boolean value) {
        this.exclusividadTXRX = value;
    }

    /**
     * Obtiene el valor de la propiedad frecuencia.
     * 
     */
    public int getFrecuencia() {
        return frecuencia;
    }

    /**
     * Define el valor de la propiedad frecuencia.
     * 
     */
    public void setFrecuencia(int value) {
        this.frecuencia = value;
    }

    /**
     * Obtiene el valor de la propiedad metodoCalculoClimax.
     * 
     */
    public int getMetodoCalculoClimax() {
        return metodoCalculoClimax;
    }

    /**
     * Define el valor de la propiedad metodoCalculoClimax.
     * 
     */
    public void setMetodoCalculoClimax(int value) {
        this.metodoCalculoClimax = value;
    }

    /**
     * Obtiene el valor de la propiedad ventanaSeleccionBss.
     * 
     */
    public int getVentanaSeleccionBss() {
        return ventanaSeleccionBss;
    }

    /**
     * Define el valor de la propiedad ventanaSeleccionBss.
     * 
     */
    public void setVentanaSeleccionBss(int value) {
        this.ventanaSeleccionBss = value;
    }

    /**
     * Obtiene el valor de la propiedad sincronizaGrupoClimax.
     * 
     */
    public boolean isSincronizaGrupoClimax() {
        return sincronizaGrupoClimax;
    }

    /**
     * Define el valor de la propiedad sincronizaGrupoClimax.
     * 
     */
    public void setSincronizaGrupoClimax(boolean value) {
        this.sincronizaGrupoClimax = value;
    }

    /**
     * Obtiene el valor de la propiedad audioPrimerSqBss.
     * 
     */
    public boolean isAudioPrimerSqBss() {
        return audioPrimerSqBss;
    }

    /**
     * Define el valor de la propiedad audioPrimerSqBss.
     * 
     */
    public void setAudioPrimerSqBss(boolean value) {
        this.audioPrimerSqBss = value;
    }

    /**
     * Obtiene el valor de la propiedad metodosBss.
     * 
     * @return
     *     possible object is
     *     {@link ArrayOfMetodosBssDelRecurso }
     *     
     */
    public ArrayOfMetodosBssDelRecurso getMetodosBss() {
        return metodosBss;
    }

    /**
     * Define el valor de la propiedad metodosBss.
     * 
     * @param value
     *     allowed object is
     *     {@link ArrayOfMetodosBssDelRecurso }
     *     
     */
    public void setMetodosBss(ArrayOfMetodosBssDelRecurso value) {
        this.metodosBss = value;
    }

    /**
     * Obtiene el valor de la propiedad frecuenciaNoDesasignable.
     * 
     */
    public boolean isFrecuenciaNoDesasignable() {
        return frecuenciaNoDesasignable;
    }

    /**
     * Define el valor de la propiedad frecuenciaNoDesasignable.
     * 
     */
    public void setFrecuenciaNoDesasignable(boolean value) {
        this.frecuenciaNoDesasignable = value;
    }

    /**
     * Obtiene el valor de la propiedad ventanaReposoZonaTxDefecto.
     * 
     */
    public int getVentanaReposoZonaTxDefecto() {
        return ventanaReposoZonaTxDefecto;
    }

    /**
     * Define el valor de la propiedad ventanaReposoZonaTxDefecto.
     * 
     */
    public void setVentanaReposoZonaTxDefecto(int value) {
        this.ventanaReposoZonaTxDefecto = value;
    }

    /**
     * Obtiene el valor de la propiedad prioridadSesionSip.
     * 
     */
    public int getPrioridadSesionSip() {
        return prioridadSesionSip;
    }

    /**
     * Define el valor de la propiedad prioridadSesionSip.
     * 
     */
    public void setPrioridadSesionSip(int value) {
        this.prioridadSesionSip = value;
    }

    /**
     * Obtiene el valor de la propiedad metodosBssOfrecidos.
     * 
     */
    public int getMetodosBssOfrecidos() {
        return metodosBssOfrecidos;
    }

    /**
     * Define el valor de la propiedad metodosBssOfrecidos.
     * 
     */
    public void setMetodosBssOfrecidos(int value) {
        this.metodosBssOfrecidos = value;
    }

    /**
     * Obtiene el valor de la propiedad cldSupervisionTime.
     * 
     */
    public int getCldSupervisionTime() {
        return cldSupervisionTime;
    }

    /**
     * Define el valor de la propiedad cldSupervisionTime.
     * 
     */
    public void setCldSupervisionTime(int value) {
        this.cldSupervisionTime = value;
    }

    /**
     * Obtiene el valor de la propiedad nombreZonaTxDefecto.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getNombreZonaTxDefecto() {
        return nombreZonaTxDefecto;
    }

    /**
     * Define el valor de la propiedad nombreZonaTxDefecto.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setNombreZonaTxDefecto(String value) {
        this.nombreZonaTxDefecto = value;
    }

    /**
     * Obtiene el valor de la propiedad cnfModoDestino.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getCnfModoDestino() {
        return cnfModoDestino;
    }

    /**
     * Define el valor de la propiedad cnfModoDestino.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setCnfModoDestino(String value) {
        this.cnfModoDestino = value;
    }

    /**
     * Obtiene el valor de la propiedad cnfTipoFrecuencia.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getCnfTipoFrecuencia() {
        return cnfTipoFrecuencia;
    }

    /**
     * Define el valor de la propiedad cnfTipoFrecuencia.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setCnfTipoFrecuencia(String value) {
        this.cnfTipoFrecuencia = value;
    }

    /**
     * Obtiene el valor de la propiedad modoTransmision.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getModoTransmision() {
        return modoTransmision;
    }

    /**
     * Define el valor de la propiedad modoTransmision.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setModoTransmision(String value) {
        this.modoTransmision = value;
    }

    /**
     * Obtiene el valor de la propiedad emplazamientoDefecto.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getEmplazamientoDefecto() {
        return emplazamientoDefecto;
    }

    /**
     * Define el valor de la propiedad emplazamientoDefecto.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setEmplazamientoDefecto(String value) {
        this.emplazamientoDefecto = value;
    }

    /**
     * Obtiene el valor de la propiedad tiempoVueltaADefecto.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getTiempoVueltaADefecto() {
        return tiempoVueltaADefecto;
    }

    /**
     * Define el valor de la propiedad tiempoVueltaADefecto.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setTiempoVueltaADefecto(String value) {
        this.tiempoVueltaADefecto = value;
    }

    /**
     * Obtiene el valor de la propiedad porcentajeRSSI.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getPorcentajeRSSI() {
        return porcentajeRSSI;
    }

    /**
     * Define el valor de la propiedad porcentajeRSSI.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setPorcentajeRSSI(String value) {
        this.porcentajeRSSI = value;
    }

    /**
     * Obtiene el valor de la propiedad conRedundancia.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getConRedundancia() {
        return conRedundancia;
    }

    /**
     * Define el valor de la propiedad conRedundancia.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setConRedundancia(String value) {
        this.conRedundancia = value;
    }

    /**
     * Obtiene el valor de la propiedad descDestino.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getDescDestino() {
        return descDestino;
    }

    /**
     * Define el valor de la propiedad descDestino.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setDescDestino(String value) {
        this.descDestino = value;
    }

    /**
     * Obtiene el valor de la propiedad pasivoRetransmision.
     * 
     */
    public boolean isPasivoRetransmision() {
        return pasivoRetransmision;
    }

    /**
     * Define el valor de la propiedad pasivoRetransmision.
     * 
     */
    public void setPasivoRetransmision(boolean value) {
        this.pasivoRetransmision = value;
    }

    /**
     * Obtiene el valor de la propiedad multiFrecuencia.
     * 
     */
    public boolean isMultiFrecuencia() {
        return multiFrecuencia;
    }

    /**
     * Define el valor de la propiedad multiFrecuencia.
     * 
     */
    public void setMultiFrecuencia(boolean value) {
        this.multiFrecuencia = value;
    }

}

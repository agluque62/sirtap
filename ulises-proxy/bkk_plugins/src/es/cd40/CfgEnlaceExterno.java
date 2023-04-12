
package es.cd40;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlSchemaType;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para CfgEnlaceExterno complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="CfgEnlaceExterno"&gt;
 *   &lt;complexContent&gt;
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="Literal" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="AliasEnlace" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="ListaPosicionesEnHmi" type="{http://CD40.es/}ArrayOfUnsignedInt" minOccurs="0"/&gt;
 *         &lt;element name="TipoFrecuencia" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="ExclusividadTxRx" type="{http://www.w3.org/2001/XMLSchema}boolean"/&gt;
 *         &lt;element name="ListaRecursos" type="{http://CD40.es/}ArrayOfCfgRecursoEnlaceExterno" minOccurs="0"/&gt;
 *         &lt;element name="EstadoAsignacion" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="DestinoAudio" type="{http://CD40.es/}ArrayOfString" minOccurs="0"/&gt;
 *         &lt;element name="Prioridad" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="SupervisionPortadora" type="{http://www.w3.org/2001/XMLSchema}boolean"/&gt;
 *         &lt;element name="FrecuenciaSintonizada" type="{http://www.w3.org/2001/XMLSchema}int"/&gt;
 *         &lt;element name="MetodoCalculoClimax" type="{http://www.w3.org/2001/XMLSchema}int"/&gt;
 *         &lt;element name="VentanaSeleccionBss" type="{http://www.w3.org/2001/XMLSchema}int"/&gt;
 *         &lt;element name="SincronizaGrupoClimax" type="{http://www.w3.org/2001/XMLSchema}boolean"/&gt;
 *         &lt;element name="AudioPrimerSqBss" type="{http://www.w3.org/2001/XMLSchema}boolean"/&gt;
 *         &lt;element name="FrecuenciaNoDesasignable" type="{http://www.w3.org/2001/XMLSchema}boolean"/&gt;
 *         &lt;element name="VentanaReposoZonaTxDefecto" type="{http://www.w3.org/2001/XMLSchema}int"/&gt;
 *         &lt;element name="NombreZonaTxDefecto" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="PrioridadSesionSIP" type="{http://www.w3.org/2001/XMLSchema}int"/&gt;
 *         &lt;element name="CldSupervisionTime" type="{http://www.w3.org/2001/XMLSchema}int"/&gt;
 *         &lt;element name="MetodosBssOfrecidos" type="{http://www.w3.org/2001/XMLSchema}int"/&gt;
 *         &lt;element name="ModoTransmision" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/restriction&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "CfgEnlaceExterno", propOrder = {
    "literal",
    "aliasEnlace",
    "listaPosicionesEnHmi",
    "tipoFrecuencia",
    "exclusividadTxRx",
    "listaRecursos",
    "estadoAsignacion",
    "destinoAudio",
    "prioridad",
    "supervisionPortadora",
    "frecuenciaSintonizada",
    "metodoCalculoClimax",
    "ventanaSeleccionBss",
    "sincronizaGrupoClimax",
    "audioPrimerSqBss",
    "frecuenciaNoDesasignable",
    "ventanaReposoZonaTxDefecto",
    "nombreZonaTxDefecto",
    "prioridadSesionSIP",
    "cldSupervisionTime",
    "metodosBssOfrecidos",
    "modoTransmision"
})
public class CfgEnlaceExterno {

    @XmlElement(name = "Literal")
    protected String literal;
    @XmlElement(name = "AliasEnlace")
    protected String aliasEnlace;
    @XmlElement(name = "ListaPosicionesEnHmi")
    protected ArrayOfUnsignedInt listaPosicionesEnHmi;
    @XmlElement(name = "TipoFrecuencia")
    @XmlSchemaType(name = "unsignedInt")
    protected long tipoFrecuencia;
    @XmlElement(name = "ExclusividadTxRx")
    protected boolean exclusividadTxRx;
    @XmlElement(name = "ListaRecursos")
    protected ArrayOfCfgRecursoEnlaceExterno listaRecursos;
    @XmlElement(name = "EstadoAsignacion")
    protected String estadoAsignacion;
    @XmlElement(name = "DestinoAudio")
    protected ArrayOfString destinoAudio;
    @XmlElement(name = "Prioridad")
    @XmlSchemaType(name = "unsignedInt")
    protected long prioridad;
    @XmlElement(name = "SupervisionPortadora")
    protected boolean supervisionPortadora;
    @XmlElement(name = "FrecuenciaSintonizada")
    protected int frecuenciaSintonizada;
    @XmlElement(name = "MetodoCalculoClimax")
    protected int metodoCalculoClimax;
    @XmlElement(name = "VentanaSeleccionBss")
    protected int ventanaSeleccionBss;
    @XmlElement(name = "SincronizaGrupoClimax")
    protected boolean sincronizaGrupoClimax;
    @XmlElement(name = "AudioPrimerSqBss")
    protected boolean audioPrimerSqBss;
    @XmlElement(name = "FrecuenciaNoDesasignable")
    protected boolean frecuenciaNoDesasignable;
    @XmlElement(name = "VentanaReposoZonaTxDefecto")
    protected int ventanaReposoZonaTxDefecto;
    @XmlElement(name = "NombreZonaTxDefecto")
    protected String nombreZonaTxDefecto;
    @XmlElement(name = "PrioridadSesionSIP")
    protected int prioridadSesionSIP;
    @XmlElement(name = "CldSupervisionTime")
    protected int cldSupervisionTime;
    @XmlElement(name = "MetodosBssOfrecidos")
    protected int metodosBssOfrecidos;
    @XmlElement(name = "ModoTransmision")
    protected String modoTransmision;

    /**
     * Obtiene el valor de la propiedad literal.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getLiteral() {
        return literal;
    }

    /**
     * Define el valor de la propiedad literal.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setLiteral(String value) {
        this.literal = value;
    }

    /**
     * Obtiene el valor de la propiedad aliasEnlace.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getAliasEnlace() {
        return aliasEnlace;
    }

    /**
     * Define el valor de la propiedad aliasEnlace.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setAliasEnlace(String value) {
        this.aliasEnlace = value;
    }

    /**
     * Obtiene el valor de la propiedad listaPosicionesEnHmi.
     * 
     * @return
     *     possible object is
     *     {@link ArrayOfUnsignedInt }
     *     
     */
    public ArrayOfUnsignedInt getListaPosicionesEnHmi() {
        return listaPosicionesEnHmi;
    }

    /**
     * Define el valor de la propiedad listaPosicionesEnHmi.
     * 
     * @param value
     *     allowed object is
     *     {@link ArrayOfUnsignedInt }
     *     
     */
    public void setListaPosicionesEnHmi(ArrayOfUnsignedInt value) {
        this.listaPosicionesEnHmi = value;
    }

    /**
     * Obtiene el valor de la propiedad tipoFrecuencia.
     * 
     */
    public long getTipoFrecuencia() {
        return tipoFrecuencia;
    }

    /**
     * Define el valor de la propiedad tipoFrecuencia.
     * 
     */
    public void setTipoFrecuencia(long value) {
        this.tipoFrecuencia = value;
    }

    /**
     * Obtiene el valor de la propiedad exclusividadTxRx.
     * 
     */
    public boolean isExclusividadTxRx() {
        return exclusividadTxRx;
    }

    /**
     * Define el valor de la propiedad exclusividadTxRx.
     * 
     */
    public void setExclusividadTxRx(boolean value) {
        this.exclusividadTxRx = value;
    }

    /**
     * Obtiene el valor de la propiedad listaRecursos.
     * 
     * @return
     *     possible object is
     *     {@link ArrayOfCfgRecursoEnlaceExterno }
     *     
     */
    public ArrayOfCfgRecursoEnlaceExterno getListaRecursos() {
        return listaRecursos;
    }

    /**
     * Define el valor de la propiedad listaRecursos.
     * 
     * @param value
     *     allowed object is
     *     {@link ArrayOfCfgRecursoEnlaceExterno }
     *     
     */
    public void setListaRecursos(ArrayOfCfgRecursoEnlaceExterno value) {
        this.listaRecursos = value;
    }

    /**
     * Obtiene el valor de la propiedad estadoAsignacion.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getEstadoAsignacion() {
        return estadoAsignacion;
    }

    /**
     * Define el valor de la propiedad estadoAsignacion.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setEstadoAsignacion(String value) {
        this.estadoAsignacion = value;
    }

    /**
     * Obtiene el valor de la propiedad destinoAudio.
     * 
     * @return
     *     possible object is
     *     {@link ArrayOfString }
     *     
     */
    public ArrayOfString getDestinoAudio() {
        return destinoAudio;
    }

    /**
     * Define el valor de la propiedad destinoAudio.
     * 
     * @param value
     *     allowed object is
     *     {@link ArrayOfString }
     *     
     */
    public void setDestinoAudio(ArrayOfString value) {
        this.destinoAudio = value;
    }

    /**
     * Obtiene el valor de la propiedad prioridad.
     * 
     */
    public long getPrioridad() {
        return prioridad;
    }

    /**
     * Define el valor de la propiedad prioridad.
     * 
     */
    public void setPrioridad(long value) {
        this.prioridad = value;
    }

    /**
     * Obtiene el valor de la propiedad supervisionPortadora.
     * 
     */
    public boolean isSupervisionPortadora() {
        return supervisionPortadora;
    }

    /**
     * Define el valor de la propiedad supervisionPortadora.
     * 
     */
    public void setSupervisionPortadora(boolean value) {
        this.supervisionPortadora = value;
    }

    /**
     * Obtiene el valor de la propiedad frecuenciaSintonizada.
     * 
     */
    public int getFrecuenciaSintonizada() {
        return frecuenciaSintonizada;
    }

    /**
     * Define el valor de la propiedad frecuenciaSintonizada.
     * 
     */
    public void setFrecuenciaSintonizada(int value) {
        this.frecuenciaSintonizada = value;
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
     * Obtiene el valor de la propiedad prioridadSesionSIP.
     * 
     */
    public int getPrioridadSesionSIP() {
        return prioridadSesionSIP;
    }

    /**
     * Define el valor de la propiedad prioridadSesionSIP.
     * 
     */
    public void setPrioridadSesionSIP(int value) {
        this.prioridadSesionSIP = value;
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

}

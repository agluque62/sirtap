
package es.cd40;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlSchemaType;
import javax.xml.bind.annotation.XmlSeeAlso;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para RecursosRadio complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="RecursosRadio"&gt;
 *   &lt;complexContent&gt;
 *     &lt;extension base="{http://CD40.es/}ParametrosRecursosRadioKASiccip"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="IdRecurso" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="IdSistema" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="TipoDestino" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="IdDestino" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="IdEmplazamiento" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="NameTablaBss" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="Tipo" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="MetodoBSS" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="Zonas_IdZonas" type="{http://www.w3.org/2001/XMLSchema}int"/&gt;
 *         &lt;element name="Radio_param_idradio_param" type="{http://www.w3.org/2001/XMLSchema}int"/&gt;
 *         &lt;element name="IdTablaBss" type="{http://www.w3.org/2001/XMLSchema}int"/&gt;
 *         &lt;element name="NombreZona" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="GrsDelay" type="{http://www.w3.org/2001/XMLSchema}int"/&gt;
 *         &lt;element name="OffSetFrequency" type="{http://www.w3.org/2001/XMLSchema}int"/&gt;
 *         &lt;element name="EnableEventPttSq" type="{http://www.w3.org/2001/XMLSchema}boolean"/&gt;
 *         &lt;element name="Metodos_bss_idmetodos_bss" type="{http://www.w3.org/2001/XMLSchema}int"/&gt;
 *         &lt;element name="idTIFX" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="DestRad_tipoFrec" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/extension&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "RecursosRadio", propOrder = {
    "idRecurso",
    "idSistema",
    "tipoDestino",
    "idDestino",
    "idEmplazamiento",
    "nameTablaBss",
    "tipo",
    "metodoBSS",
    "zonasIdZonas",
    "radioParamIdradioParam",
    "idTablaBss",
    "nombreZona",
    "grsDelay",
    "offSetFrequency",
    "enableEventPttSq",
    "metodosBssIdmetodosBss",
    "idTIFX",
    "destRadTipoFrec"
})
@XmlSeeAlso({
    RecursosRadioForGateway.class
})
public class RecursosRadio
    extends ParametrosRecursosRadioKASiccip
{

    @XmlElement(name = "IdRecurso")
    protected String idRecurso;
    @XmlElement(name = "IdSistema")
    protected String idSistema;
    @XmlElement(name = "TipoDestino")
    @XmlSchemaType(name = "unsignedInt")
    protected long tipoDestino;
    @XmlElement(name = "IdDestino")
    protected String idDestino;
    @XmlElement(name = "IdEmplazamiento")
    protected String idEmplazamiento;
    @XmlElement(name = "NameTablaBss")
    protected String nameTablaBss;
    @XmlElement(name = "Tipo")
    @XmlSchemaType(name = "unsignedInt")
    protected long tipo;
    @XmlElement(name = "MetodoBSS")
    protected String metodoBSS;
    @XmlElement(name = "Zonas_IdZonas")
    protected int zonasIdZonas;
    @XmlElement(name = "Radio_param_idradio_param")
    protected int radioParamIdradioParam;
    @XmlElement(name = "IdTablaBss")
    protected int idTablaBss;
    @XmlElement(name = "NombreZona")
    protected String nombreZona;
    @XmlElement(name = "GrsDelay")
    protected int grsDelay;
    @XmlElement(name = "OffSetFrequency")
    protected int offSetFrequency;
    @XmlElement(name = "EnableEventPttSq")
    protected boolean enableEventPttSq;
    @XmlElement(name = "Metodos_bss_idmetodos_bss")
    protected int metodosBssIdmetodosBss;
    protected String idTIFX;
    @XmlElement(name = "DestRad_tipoFrec")
    @XmlSchemaType(name = "unsignedInt")
    protected long destRadTipoFrec;

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
     * Obtiene el valor de la propiedad tipoDestino.
     * 
     */
    public long getTipoDestino() {
        return tipoDestino;
    }

    /**
     * Define el valor de la propiedad tipoDestino.
     * 
     */
    public void setTipoDestino(long value) {
        this.tipoDestino = value;
    }

    /**
     * Obtiene el valor de la propiedad idDestino.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getIdDestino() {
        return idDestino;
    }

    /**
     * Define el valor de la propiedad idDestino.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setIdDestino(String value) {
        this.idDestino = value;
    }

    /**
     * Obtiene el valor de la propiedad idEmplazamiento.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getIdEmplazamiento() {
        return idEmplazamiento;
    }

    /**
     * Define el valor de la propiedad idEmplazamiento.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setIdEmplazamiento(String value) {
        this.idEmplazamiento = value;
    }

    /**
     * Obtiene el valor de la propiedad nameTablaBss.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getNameTablaBss() {
        return nameTablaBss;
    }

    /**
     * Define el valor de la propiedad nameTablaBss.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setNameTablaBss(String value) {
        this.nameTablaBss = value;
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
     * Obtiene el valor de la propiedad metodoBSS.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getMetodoBSS() {
        return metodoBSS;
    }

    /**
     * Define el valor de la propiedad metodoBSS.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setMetodoBSS(String value) {
        this.metodoBSS = value;
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
     * Obtiene el valor de la propiedad radioParamIdradioParam.
     * 
     */
    public int getRadioParamIdradioParam() {
        return radioParamIdradioParam;
    }

    /**
     * Define el valor de la propiedad radioParamIdradioParam.
     * 
     */
    public void setRadioParamIdradioParam(int value) {
        this.radioParamIdradioParam = value;
    }

    /**
     * Obtiene el valor de la propiedad idTablaBss.
     * 
     */
    public int getIdTablaBss() {
        return idTablaBss;
    }

    /**
     * Define el valor de la propiedad idTablaBss.
     * 
     */
    public void setIdTablaBss(int value) {
        this.idTablaBss = value;
    }

    /**
     * Obtiene el valor de la propiedad nombreZona.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getNombreZona() {
        return nombreZona;
    }

    /**
     * Define el valor de la propiedad nombreZona.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setNombreZona(String value) {
        this.nombreZona = value;
    }

    /**
     * Obtiene el valor de la propiedad grsDelay.
     * 
     */
    public int getGrsDelay() {
        return grsDelay;
    }

    /**
     * Define el valor de la propiedad grsDelay.
     * 
     */
    public void setGrsDelay(int value) {
        this.grsDelay = value;
    }

    /**
     * Obtiene el valor de la propiedad offSetFrequency.
     * 
     */
    public int getOffSetFrequency() {
        return offSetFrequency;
    }

    /**
     * Define el valor de la propiedad offSetFrequency.
     * 
     */
    public void setOffSetFrequency(int value) {
        this.offSetFrequency = value;
    }

    /**
     * Obtiene el valor de la propiedad enableEventPttSq.
     * 
     */
    public boolean isEnableEventPttSq() {
        return enableEventPttSq;
    }

    /**
     * Define el valor de la propiedad enableEventPttSq.
     * 
     */
    public void setEnableEventPttSq(boolean value) {
        this.enableEventPttSq = value;
    }

    /**
     * Obtiene el valor de la propiedad metodosBssIdmetodosBss.
     * 
     */
    public int getMetodosBssIdmetodosBss() {
        return metodosBssIdmetodosBss;
    }

    /**
     * Define el valor de la propiedad metodosBssIdmetodosBss.
     * 
     */
    public void setMetodosBssIdmetodosBss(int value) {
        this.metodosBssIdmetodosBss = value;
    }

    /**
     * Obtiene el valor de la propiedad idTIFX.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getIdTIFX() {
        return idTIFX;
    }

    /**
     * Define el valor de la propiedad idTIFX.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setIdTIFX(String value) {
        this.idTIFX = value;
    }

    /**
     * Obtiene el valor de la propiedad destRadTipoFrec.
     * 
     */
    public long getDestRadTipoFrec() {
        return destRadTipoFrec;
    }

    /**
     * Define el valor de la propiedad destRadTipoFrec.
     * 
     */
    public void setDestRadTipoFrec(long value) {
        this.destRadTipoFrec = value;
    }

}


package es.cd40;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlSchemaType;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para Node complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="Node"&gt;
 *   &lt;complexContent&gt;
 *     &lt;extension base="{http://CD40.es/}PoolHfElement"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="EsReceptor" type="{http://www.w3.org/2001/XMLSchema}boolean"/&gt;
 *         &lt;element name="EsTransmisor" type="{http://www.w3.org/2001/XMLSchema}boolean"/&gt;
 *         &lt;element name="TipoDeFrecuencia" type="{http://CD40.es/}Tipo_Frecuencia"/&gt;
 *         &lt;element name="FrecuenciaPrincipal" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="TipoDeCanal" type="{http://CD40.es/}Tipo_Canal"/&gt;
 *         &lt;element name="FormaDeTrabajo" type="{http://CD40.es/}Tipo_Formato_Trabajo"/&gt;
 *         &lt;element name="Prioridad" type="{http://www.w3.org/2001/XMLSchema}int"/&gt;
 *         &lt;element name="Puerto" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="Offset" type="{http://CD40.es/}GearCarrierOffStatus"/&gt;
 *         &lt;element name="Canalizacion" type="{http://CD40.es/}GearChannelSpacings"/&gt;
 *         &lt;element name="Modulacion" type="{http://CD40.es/}GearModulations"/&gt;
 *         &lt;element name="NivelDePotencia" type="{http://CD40.es/}GearPowerLevels"/&gt;
 *         &lt;element name="FormatoFrecuenciaPrincipal" type="{http://CD40.es/}Tipo_Formato_Frecuencia"/&gt;
 *         &lt;element name="ModeloEquipo" type="{http://www.w3.org/2001/XMLSchema}int"/&gt;
 *         &lt;element name="IdEmplazamiento" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/extension&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "Node", propOrder = {
    "esReceptor",
    "esTransmisor",
    "tipoDeFrecuencia",
    "frecuenciaPrincipal",
    "tipoDeCanal",
    "formaDeTrabajo",
    "prioridad",
    "puerto",
    "offset",
    "canalizacion",
    "modulacion",
    "nivelDePotencia",
    "formatoFrecuenciaPrincipal",
    "modeloEquipo",
    "idEmplazamiento"
})
public class Node
    extends PoolHfElement
{

    @XmlElement(name = "EsReceptor")
    protected boolean esReceptor;
    @XmlElement(name = "EsTransmisor")
    protected boolean esTransmisor;
    @XmlElement(name = "TipoDeFrecuencia", required = true)
    @XmlSchemaType(name = "string")
    protected TipoFrecuencia tipoDeFrecuencia;
    @XmlElement(name = "FrecuenciaPrincipal")
    protected String frecuenciaPrincipal;
    @XmlElement(name = "TipoDeCanal", required = true)
    @XmlSchemaType(name = "string")
    protected TipoCanal tipoDeCanal;
    @XmlElement(name = "FormaDeTrabajo", required = true)
    @XmlSchemaType(name = "string")
    protected TipoFormatoTrabajo formaDeTrabajo;
    @XmlElement(name = "Prioridad")
    protected int prioridad;
    @XmlElement(name = "Puerto")
    @XmlSchemaType(name = "unsignedInt")
    protected long puerto;
    @XmlElement(name = "Offset", required = true)
    @XmlSchemaType(name = "string")
    protected GearCarrierOffStatus offset;
    @XmlElement(name = "Canalizacion", required = true)
    @XmlSchemaType(name = "string")
    protected GearChannelSpacings canalizacion;
    @XmlElement(name = "Modulacion", required = true)
    @XmlSchemaType(name = "string")
    protected GearModulations modulacion;
    @XmlElement(name = "NivelDePotencia", required = true)
    @XmlSchemaType(name = "string")
    protected GearPowerLevels nivelDePotencia;
    @XmlElement(name = "FormatoFrecuenciaPrincipal", required = true)
    @XmlSchemaType(name = "string")
    protected TipoFormatoFrecuencia formatoFrecuenciaPrincipal;
    @XmlElement(name = "ModeloEquipo")
    protected int modeloEquipo;
    @XmlElement(name = "IdEmplazamiento")
    protected String idEmplazamiento;

    /**
     * Obtiene el valor de la propiedad esReceptor.
     * 
     */
    public boolean isEsReceptor() {
        return esReceptor;
    }

    /**
     * Define el valor de la propiedad esReceptor.
     * 
     */
    public void setEsReceptor(boolean value) {
        this.esReceptor = value;
    }

    /**
     * Obtiene el valor de la propiedad esTransmisor.
     * 
     */
    public boolean isEsTransmisor() {
        return esTransmisor;
    }

    /**
     * Define el valor de la propiedad esTransmisor.
     * 
     */
    public void setEsTransmisor(boolean value) {
        this.esTransmisor = value;
    }

    /**
     * Obtiene el valor de la propiedad tipoDeFrecuencia.
     * 
     * @return
     *     possible object is
     *     {@link TipoFrecuencia }
     *     
     */
    public TipoFrecuencia getTipoDeFrecuencia() {
        return tipoDeFrecuencia;
    }

    /**
     * Define el valor de la propiedad tipoDeFrecuencia.
     * 
     * @param value
     *     allowed object is
     *     {@link TipoFrecuencia }
     *     
     */
    public void setTipoDeFrecuencia(TipoFrecuencia value) {
        this.tipoDeFrecuencia = value;
    }

    /**
     * Obtiene el valor de la propiedad frecuenciaPrincipal.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getFrecuenciaPrincipal() {
        return frecuenciaPrincipal;
    }

    /**
     * Define el valor de la propiedad frecuenciaPrincipal.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setFrecuenciaPrincipal(String value) {
        this.frecuenciaPrincipal = value;
    }

    /**
     * Obtiene el valor de la propiedad tipoDeCanal.
     * 
     * @return
     *     possible object is
     *     {@link TipoCanal }
     *     
     */
    public TipoCanal getTipoDeCanal() {
        return tipoDeCanal;
    }

    /**
     * Define el valor de la propiedad tipoDeCanal.
     * 
     * @param value
     *     allowed object is
     *     {@link TipoCanal }
     *     
     */
    public void setTipoDeCanal(TipoCanal value) {
        this.tipoDeCanal = value;
    }

    /**
     * Obtiene el valor de la propiedad formaDeTrabajo.
     * 
     * @return
     *     possible object is
     *     {@link TipoFormatoTrabajo }
     *     
     */
    public TipoFormatoTrabajo getFormaDeTrabajo() {
        return formaDeTrabajo;
    }

    /**
     * Define el valor de la propiedad formaDeTrabajo.
     * 
     * @param value
     *     allowed object is
     *     {@link TipoFormatoTrabajo }
     *     
     */
    public void setFormaDeTrabajo(TipoFormatoTrabajo value) {
        this.formaDeTrabajo = value;
    }

    /**
     * Obtiene el valor de la propiedad prioridad.
     * 
     */
    public int getPrioridad() {
        return prioridad;
    }

    /**
     * Define el valor de la propiedad prioridad.
     * 
     */
    public void setPrioridad(int value) {
        this.prioridad = value;
    }

    /**
     * Obtiene el valor de la propiedad puerto.
     * 
     */
    public long getPuerto() {
        return puerto;
    }

    /**
     * Define el valor de la propiedad puerto.
     * 
     */
    public void setPuerto(long value) {
        this.puerto = value;
    }

    /**
     * Obtiene el valor de la propiedad offset.
     * 
     * @return
     *     possible object is
     *     {@link GearCarrierOffStatus }
     *     
     */
    public GearCarrierOffStatus getOffset() {
        return offset;
    }

    /**
     * Define el valor de la propiedad offset.
     * 
     * @param value
     *     allowed object is
     *     {@link GearCarrierOffStatus }
     *     
     */
    public void setOffset(GearCarrierOffStatus value) {
        this.offset = value;
    }

    /**
     * Obtiene el valor de la propiedad canalizacion.
     * 
     * @return
     *     possible object is
     *     {@link GearChannelSpacings }
     *     
     */
    public GearChannelSpacings getCanalizacion() {
        return canalizacion;
    }

    /**
     * Define el valor de la propiedad canalizacion.
     * 
     * @param value
     *     allowed object is
     *     {@link GearChannelSpacings }
     *     
     */
    public void setCanalizacion(GearChannelSpacings value) {
        this.canalizacion = value;
    }

    /**
     * Obtiene el valor de la propiedad modulacion.
     * 
     * @return
     *     possible object is
     *     {@link GearModulations }
     *     
     */
    public GearModulations getModulacion() {
        return modulacion;
    }

    /**
     * Define el valor de la propiedad modulacion.
     * 
     * @param value
     *     allowed object is
     *     {@link GearModulations }
     *     
     */
    public void setModulacion(GearModulations value) {
        this.modulacion = value;
    }

    /**
     * Obtiene el valor de la propiedad nivelDePotencia.
     * 
     * @return
     *     possible object is
     *     {@link GearPowerLevels }
     *     
     */
    public GearPowerLevels getNivelDePotencia() {
        return nivelDePotencia;
    }

    /**
     * Define el valor de la propiedad nivelDePotencia.
     * 
     * @param value
     *     allowed object is
     *     {@link GearPowerLevels }
     *     
     */
    public void setNivelDePotencia(GearPowerLevels value) {
        this.nivelDePotencia = value;
    }

    /**
     * Obtiene el valor de la propiedad formatoFrecuenciaPrincipal.
     * 
     * @return
     *     possible object is
     *     {@link TipoFormatoFrecuencia }
     *     
     */
    public TipoFormatoFrecuencia getFormatoFrecuenciaPrincipal() {
        return formatoFrecuenciaPrincipal;
    }

    /**
     * Define el valor de la propiedad formatoFrecuenciaPrincipal.
     * 
     * @param value
     *     allowed object is
     *     {@link TipoFormatoFrecuencia }
     *     
     */
    public void setFormatoFrecuenciaPrincipal(TipoFormatoFrecuencia value) {
        this.formatoFrecuenciaPrincipal = value;
    }

    /**
     * Obtiene el valor de la propiedad modeloEquipo.
     * 
     */
    public int getModeloEquipo() {
        return modeloEquipo;
    }

    /**
     * Define el valor de la propiedad modeloEquipo.
     * 
     */
    public void setModeloEquipo(int value) {
        this.modeloEquipo = value;
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

}

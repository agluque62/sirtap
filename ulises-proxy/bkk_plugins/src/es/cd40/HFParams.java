
package es.cd40;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlSchemaType;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para HFParams complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="HFParams"&gt;
 *   &lt;complexContent&gt;
 *     &lt;extension base="{http://CD40.es/}Tablas"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="IdSistema" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="IdRecurso" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="SipUri" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="IpGestor" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="Oid" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="Frecuencia" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="IdEmplazamiento" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="Puerto" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="Offset" type="{http://www.w3.org/2001/XMLSchema}int"/&gt;
 *         &lt;element name="Canalizacion" type="{http://www.w3.org/2001/XMLSchema}int"/&gt;
 *         &lt;element name="Modulacion" type="{http://www.w3.org/2001/XMLSchema}int"/&gt;
 *         &lt;element name="Potencia" type="{http://www.w3.org/2001/XMLSchema}int"/&gt;
 *         &lt;element name="TipoEquipo" type="{http://www.w3.org/2001/XMLSchema}int"/&gt;
 *         &lt;element name="TipoCanal" type="{http://www.w3.org/2001/XMLSchema}int"/&gt;
 *         &lt;element name="TipoFrecuencia" type="{http://www.w3.org/2001/XMLSchema}int"/&gt;
 *         &lt;element name="TipoModo" type="{http://www.w3.org/2001/XMLSchema}int"/&gt;
 *         &lt;element name="PrioridadEquipo" type="{http://www.w3.org/2001/XMLSchema}int"/&gt;
 *         &lt;element name="FormatoFrecuenciaPrincipal" type="{http://www.w3.org/2001/XMLSchema}int"/&gt;
 *         &lt;element name="ModeloEquipo" type="{http://www.w3.org/2001/XMLSchema}int"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/extension&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "HFParams", propOrder = {
    "idSistema",
    "idRecurso",
    "sipUri",
    "ipGestor",
    "oid",
    "frecuencia",
    "idEmplazamiento",
    "puerto",
    "offset",
    "canalizacion",
    "modulacion",
    "potencia",
    "tipoEquipo",
    "tipoCanal",
    "tipoFrecuencia",
    "tipoModo",
    "prioridadEquipo",
    "formatoFrecuenciaPrincipal",
    "modeloEquipo"
})
public class HFParams
    extends Tablas
{

    @XmlElement(name = "IdSistema")
    protected String idSistema;
    @XmlElement(name = "IdRecurso")
    protected String idRecurso;
    @XmlElement(name = "SipUri")
    protected String sipUri;
    @XmlElement(name = "IpGestor")
    protected String ipGestor;
    @XmlElement(name = "Oid")
    protected String oid;
    @XmlElement(name = "Frecuencia")
    protected String frecuencia;
    @XmlElement(name = "IdEmplazamiento")
    protected String idEmplazamiento;
    @XmlElement(name = "Puerto")
    @XmlSchemaType(name = "unsignedInt")
    protected long puerto;
    @XmlElement(name = "Offset")
    protected int offset;
    @XmlElement(name = "Canalizacion")
    protected int canalizacion;
    @XmlElement(name = "Modulacion")
    protected int modulacion;
    @XmlElement(name = "Potencia")
    protected int potencia;
    @XmlElement(name = "TipoEquipo")
    protected int tipoEquipo;
    @XmlElement(name = "TipoCanal")
    protected int tipoCanal;
    @XmlElement(name = "TipoFrecuencia")
    protected int tipoFrecuencia;
    @XmlElement(name = "TipoModo")
    protected int tipoModo;
    @XmlElement(name = "PrioridadEquipo")
    protected int prioridadEquipo;
    @XmlElement(name = "FormatoFrecuenciaPrincipal")
    protected int formatoFrecuenciaPrincipal;
    @XmlElement(name = "ModeloEquipo")
    protected int modeloEquipo;

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
     * Obtiene el valor de la propiedad sipUri.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getSipUri() {
        return sipUri;
    }

    /**
     * Define el valor de la propiedad sipUri.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setSipUri(String value) {
        this.sipUri = value;
    }

    /**
     * Obtiene el valor de la propiedad ipGestor.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getIpGestor() {
        return ipGestor;
    }

    /**
     * Define el valor de la propiedad ipGestor.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setIpGestor(String value) {
        this.ipGestor = value;
    }

    /**
     * Obtiene el valor de la propiedad oid.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getOid() {
        return oid;
    }

    /**
     * Define el valor de la propiedad oid.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setOid(String value) {
        this.oid = value;
    }

    /**
     * Obtiene el valor de la propiedad frecuencia.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getFrecuencia() {
        return frecuencia;
    }

    /**
     * Define el valor de la propiedad frecuencia.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setFrecuencia(String value) {
        this.frecuencia = value;
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
     */
    public int getOffset() {
        return offset;
    }

    /**
     * Define el valor de la propiedad offset.
     * 
     */
    public void setOffset(int value) {
        this.offset = value;
    }

    /**
     * Obtiene el valor de la propiedad canalizacion.
     * 
     */
    public int getCanalizacion() {
        return canalizacion;
    }

    /**
     * Define el valor de la propiedad canalizacion.
     * 
     */
    public void setCanalizacion(int value) {
        this.canalizacion = value;
    }

    /**
     * Obtiene el valor de la propiedad modulacion.
     * 
     */
    public int getModulacion() {
        return modulacion;
    }

    /**
     * Define el valor de la propiedad modulacion.
     * 
     */
    public void setModulacion(int value) {
        this.modulacion = value;
    }

    /**
     * Obtiene el valor de la propiedad potencia.
     * 
     */
    public int getPotencia() {
        return potencia;
    }

    /**
     * Define el valor de la propiedad potencia.
     * 
     */
    public void setPotencia(int value) {
        this.potencia = value;
    }

    /**
     * Obtiene el valor de la propiedad tipoEquipo.
     * 
     */
    public int getTipoEquipo() {
        return tipoEquipo;
    }

    /**
     * Define el valor de la propiedad tipoEquipo.
     * 
     */
    public void setTipoEquipo(int value) {
        this.tipoEquipo = value;
    }

    /**
     * Obtiene el valor de la propiedad tipoCanal.
     * 
     */
    public int getTipoCanal() {
        return tipoCanal;
    }

    /**
     * Define el valor de la propiedad tipoCanal.
     * 
     */
    public void setTipoCanal(int value) {
        this.tipoCanal = value;
    }

    /**
     * Obtiene el valor de la propiedad tipoFrecuencia.
     * 
     */
    public int getTipoFrecuencia() {
        return tipoFrecuencia;
    }

    /**
     * Define el valor de la propiedad tipoFrecuencia.
     * 
     */
    public void setTipoFrecuencia(int value) {
        this.tipoFrecuencia = value;
    }

    /**
     * Obtiene el valor de la propiedad tipoModo.
     * 
     */
    public int getTipoModo() {
        return tipoModo;
    }

    /**
     * Define el valor de la propiedad tipoModo.
     * 
     */
    public void setTipoModo(int value) {
        this.tipoModo = value;
    }

    /**
     * Obtiene el valor de la propiedad prioridadEquipo.
     * 
     */
    public int getPrioridadEquipo() {
        return prioridadEquipo;
    }

    /**
     * Define el valor de la propiedad prioridadEquipo.
     * 
     */
    public void setPrioridadEquipo(int value) {
        this.prioridadEquipo = value;
    }

    /**
     * Obtiene el valor de la propiedad formatoFrecuenciaPrincipal.
     * 
     */
    public int getFormatoFrecuenciaPrincipal() {
        return formatoFrecuenciaPrincipal;
    }

    /**
     * Define el valor de la propiedad formatoFrecuenciaPrincipal.
     * 
     */
    public void setFormatoFrecuenciaPrincipal(int value) {
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

}

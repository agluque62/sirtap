
package es.cd40;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlSchemaType;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para CfgRecursoEnlaceExterno complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="CfgRecursoEnlaceExterno"&gt;
 *   &lt;complexContent&gt;
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="IdRecurso" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="Tipo" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="Estado" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="ModoConfPTT" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="NumFlujosAudio" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="IdEmplazamiento" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="NombreZona" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="MetodoBss" type="{http://www.w3.org/2001/XMLSchema}int"/&gt;
 *         &lt;element name="IdMetodoBss" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="NameTablaBss" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="ValuesTablaBss" type="{http://CD40.es/}ArrayOfInt" minOccurs="0"/&gt;
 *         &lt;element name="GrsDelay" type="{http://www.w3.org/2001/XMLSchema}int"/&gt;
 *         &lt;element name="OffSetFrequency" type="{http://www.w3.org/2001/XMLSchema}int"/&gt;
 *         &lt;element name="EnableEventPttSq" type="{http://www.w3.org/2001/XMLSchema}boolean"/&gt;
 *         &lt;element name="RedundanciaIdPareja" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="RedundanciaRol" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="Telemando" type="{http://www.w3.org/2001/XMLSchema}int"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/restriction&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "CfgRecursoEnlaceExterno", propOrder = {
    "idRecurso",
    "tipo",
    "estado",
    "modoConfPTT",
    "numFlujosAudio",
    "idEmplazamiento",
    "nombreZona",
    "metodoBss",
    "idMetodoBss",
    "nameTablaBss",
    "valuesTablaBss",
    "grsDelay",
    "offSetFrequency",
    "enableEventPttSq",
    "redundanciaIdPareja",
    "redundanciaRol",
    "telemando"
})
public class CfgRecursoEnlaceExterno {

    @XmlElement(name = "IdRecurso")
    protected String idRecurso;
    @XmlElement(name = "Tipo")
    @XmlSchemaType(name = "unsignedInt")
    protected long tipo;
    @XmlElement(name = "Estado")
    protected String estado;
    @XmlElement(name = "ModoConfPTT")
    @XmlSchemaType(name = "unsignedInt")
    protected long modoConfPTT;
    @XmlElement(name = "NumFlujosAudio")
    @XmlSchemaType(name = "unsignedInt")
    protected long numFlujosAudio;
    @XmlElement(name = "IdEmplazamiento")
    protected String idEmplazamiento;
    @XmlElement(name = "NombreZona")
    protected String nombreZona;
    @XmlElement(name = "MetodoBss")
    protected int metodoBss;
    @XmlElement(name = "IdMetodoBss")
    protected String idMetodoBss;
    @XmlElement(name = "NameTablaBss")
    protected String nameTablaBss;
    @XmlElement(name = "ValuesTablaBss")
    protected ArrayOfInt valuesTablaBss;
    @XmlElement(name = "GrsDelay")
    protected int grsDelay;
    @XmlElement(name = "OffSetFrequency")
    protected int offSetFrequency;
    @XmlElement(name = "EnableEventPttSq")
    protected boolean enableEventPttSq;
    @XmlElement(name = "RedundanciaIdPareja")
    protected String redundanciaIdPareja;
    @XmlElement(name = "RedundanciaRol")
    protected String redundanciaRol;
    @XmlElement(name = "Telemando")
    protected int telemando;

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
     * Obtiene el valor de la propiedad estado.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getEstado() {
        return estado;
    }

    /**
     * Define el valor de la propiedad estado.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setEstado(String value) {
        this.estado = value;
    }

    /**
     * Obtiene el valor de la propiedad modoConfPTT.
     * 
     */
    public long getModoConfPTT() {
        return modoConfPTT;
    }

    /**
     * Define el valor de la propiedad modoConfPTT.
     * 
     */
    public void setModoConfPTT(long value) {
        this.modoConfPTT = value;
    }

    /**
     * Obtiene el valor de la propiedad numFlujosAudio.
     * 
     */
    public long getNumFlujosAudio() {
        return numFlujosAudio;
    }

    /**
     * Define el valor de la propiedad numFlujosAudio.
     * 
     */
    public void setNumFlujosAudio(long value) {
        this.numFlujosAudio = value;
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
     * Obtiene el valor de la propiedad metodoBss.
     * 
     */
    public int getMetodoBss() {
        return metodoBss;
    }

    /**
     * Define el valor de la propiedad metodoBss.
     * 
     */
    public void setMetodoBss(int value) {
        this.metodoBss = value;
    }

    /**
     * Obtiene el valor de la propiedad idMetodoBss.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getIdMetodoBss() {
        return idMetodoBss;
    }

    /**
     * Define el valor de la propiedad idMetodoBss.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setIdMetodoBss(String value) {
        this.idMetodoBss = value;
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
     * Obtiene el valor de la propiedad valuesTablaBss.
     * 
     * @return
     *     possible object is
     *     {@link ArrayOfInt }
     *     
     */
    public ArrayOfInt getValuesTablaBss() {
        return valuesTablaBss;
    }

    /**
     * Define el valor de la propiedad valuesTablaBss.
     * 
     * @param value
     *     allowed object is
     *     {@link ArrayOfInt }
     *     
     */
    public void setValuesTablaBss(ArrayOfInt value) {
        this.valuesTablaBss = value;
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
     * Obtiene el valor de la propiedad redundanciaIdPareja.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getRedundanciaIdPareja() {
        return redundanciaIdPareja;
    }

    /**
     * Define el valor de la propiedad redundanciaIdPareja.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setRedundanciaIdPareja(String value) {
        this.redundanciaIdPareja = value;
    }

    /**
     * Obtiene el valor de la propiedad redundanciaRol.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getRedundanciaRol() {
        return redundanciaRol;
    }

    /**
     * Define el valor de la propiedad redundanciaRol.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setRedundanciaRol(String value) {
        this.redundanciaRol = value;
    }

    /**
     * Obtiene el valor de la propiedad telemando.
     * 
     */
    public int getTelemando() {
        return telemando;
    }

    /**
     * Define el valor de la propiedad telemando.
     * 
     */
    public void setTelemando(int value) {
        this.telemando = value;
    }

}


package es.cd40;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlSchemaType;
import javax.xml.bind.annotation.XmlSeeAlso;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para ParametrosRecursoTelefonia complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="ParametrosRecursoTelefonia"&gt;
 *   &lt;complexContent&gt;
 *     &lt;extension base="{http://CD40.es/}ParametrosTFSCV"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="GananciaAGCTX" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="GananciaAGCTXdBm" type="{http://www.w3.org/2001/XMLSchema}float"/&gt;
 *         &lt;element name="GananciaAGCRX" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="GananciaAGCRXdBm" type="{http://www.w3.org/2001/XMLSchema}float"/&gt;
 *         &lt;element name="SupresionSilencio" type="{http://www.w3.org/2001/XMLSchema}boolean"/&gt;
 *         &lt;element name="TamRTP" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="Codec" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="iPrTmLlamEntrante" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="iPrDetDtmf" type="{http://www.w3.org/2001/XMLSchema}unsignedByte"/&gt;
 *         &lt;element name="TPrReleaseBL" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="iPrPeriodoSpvRing" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="iPrFiltroSpvRing" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="iPrDetCallerId" type="{http://www.w3.org/2001/XMLSchema}unsignedByte"/&gt;
 *         &lt;element name="iPrTmCallerId" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="iPrDetInversionPol" type="{http://www.w3.org/2001/XMLSchema}unsignedByte"/&gt;
 *         &lt;element name="iPrTmDetFinLlamada" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="tipoInterface" type="{http://CD40.es/}TipoInterface"/&gt;
 *         &lt;element name="UmbralAGCTXdBm" type="{http://www.w3.org/2001/XMLSchema}float"/&gt;
 *         &lt;element name="UmbralAGCRXdBm" type="{http://www.w3.org/2001/XMLSchema}float"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/extension&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "ParametrosRecursoTelefonia", propOrder = {
    "gananciaAGCTX",
    "gananciaAGCTXdBm",
    "gananciaAGCRX",
    "gananciaAGCRXdBm",
    "supresionSilencio",
    "tamRTP",
    "codec",
    "iPrTmLlamEntrante",
    "iPrDetDtmf",
    "tPrReleaseBL",
    "iPrPeriodoSpvRing",
    "iPrFiltroSpvRing",
    "iPrDetCallerId",
    "iPrTmCallerId",
    "iPrDetInversionPol",
    "iPrTmDetFinLlamada",
    "tipoInterface",
    "umbralAGCTXdBm",
    "umbralAGCRXdBm"
})
@XmlSeeAlso({
    ParametrosEM.class
})
public class ParametrosRecursoTelefonia
    extends ParametrosTFSCV
{

    @XmlElement(name = "GananciaAGCTX")
    @XmlSchemaType(name = "unsignedInt")
    protected long gananciaAGCTX;
    @XmlElement(name = "GananciaAGCTXdBm")
    protected float gananciaAGCTXdBm;
    @XmlElement(name = "GananciaAGCRX")
    @XmlSchemaType(name = "unsignedInt")
    protected long gananciaAGCRX;
    @XmlElement(name = "GananciaAGCRXdBm")
    protected float gananciaAGCRXdBm;
    @XmlElement(name = "SupresionSilencio")
    protected boolean supresionSilencio;
    @XmlElement(name = "TamRTP")
    @XmlSchemaType(name = "unsignedInt")
    protected long tamRTP;
    @XmlElement(name = "Codec")
    @XmlSchemaType(name = "unsignedInt")
    protected long codec;
    @XmlSchemaType(name = "unsignedInt")
    protected long iPrTmLlamEntrante;
    @XmlSchemaType(name = "unsignedByte")
    protected short iPrDetDtmf;
    @XmlElement(name = "TPrReleaseBL")
    @XmlSchemaType(name = "unsignedInt")
    protected long tPrReleaseBL;
    @XmlSchemaType(name = "unsignedInt")
    protected long iPrPeriodoSpvRing;
    @XmlSchemaType(name = "unsignedInt")
    protected long iPrFiltroSpvRing;
    @XmlSchemaType(name = "unsignedByte")
    protected short iPrDetCallerId;
    @XmlSchemaType(name = "unsignedInt")
    protected long iPrTmCallerId;
    @XmlSchemaType(name = "unsignedByte")
    protected short iPrDetInversionPol;
    @XmlSchemaType(name = "unsignedInt")
    protected long iPrTmDetFinLlamada;
    @XmlElement(required = true)
    @XmlSchemaType(name = "string")
    protected TipoInterface tipoInterface;
    @XmlElement(name = "UmbralAGCTXdBm")
    protected float umbralAGCTXdBm;
    @XmlElement(name = "UmbralAGCRXdBm")
    protected float umbralAGCRXdBm;

    /**
     * Obtiene el valor de la propiedad gananciaAGCTX.
     * 
     */
    public long getGananciaAGCTX() {
        return gananciaAGCTX;
    }

    /**
     * Define el valor de la propiedad gananciaAGCTX.
     * 
     */
    public void setGananciaAGCTX(long value) {
        this.gananciaAGCTX = value;
    }

    /**
     * Obtiene el valor de la propiedad gananciaAGCTXdBm.
     * 
     */
    public float getGananciaAGCTXdBm() {
        return gananciaAGCTXdBm;
    }

    /**
     * Define el valor de la propiedad gananciaAGCTXdBm.
     * 
     */
    public void setGananciaAGCTXdBm(float value) {
        this.gananciaAGCTXdBm = value;
    }

    /**
     * Obtiene el valor de la propiedad gananciaAGCRX.
     * 
     */
    public long getGananciaAGCRX() {
        return gananciaAGCRX;
    }

    /**
     * Define el valor de la propiedad gananciaAGCRX.
     * 
     */
    public void setGananciaAGCRX(long value) {
        this.gananciaAGCRX = value;
    }

    /**
     * Obtiene el valor de la propiedad gananciaAGCRXdBm.
     * 
     */
    public float getGananciaAGCRXdBm() {
        return gananciaAGCRXdBm;
    }

    /**
     * Define el valor de la propiedad gananciaAGCRXdBm.
     * 
     */
    public void setGananciaAGCRXdBm(float value) {
        this.gananciaAGCRXdBm = value;
    }

    /**
     * Obtiene el valor de la propiedad supresionSilencio.
     * 
     */
    public boolean isSupresionSilencio() {
        return supresionSilencio;
    }

    /**
     * Define el valor de la propiedad supresionSilencio.
     * 
     */
    public void setSupresionSilencio(boolean value) {
        this.supresionSilencio = value;
    }

    /**
     * Obtiene el valor de la propiedad tamRTP.
     * 
     */
    public long getTamRTP() {
        return tamRTP;
    }

    /**
     * Define el valor de la propiedad tamRTP.
     * 
     */
    public void setTamRTP(long value) {
        this.tamRTP = value;
    }

    /**
     * Obtiene el valor de la propiedad codec.
     * 
     */
    public long getCodec() {
        return codec;
    }

    /**
     * Define el valor de la propiedad codec.
     * 
     */
    public void setCodec(long value) {
        this.codec = value;
    }

    /**
     * Obtiene el valor de la propiedad iPrTmLlamEntrante.
     * 
     */
    public long getIPrTmLlamEntrante() {
        return iPrTmLlamEntrante;
    }

    /**
     * Define el valor de la propiedad iPrTmLlamEntrante.
     * 
     */
    public void setIPrTmLlamEntrante(long value) {
        this.iPrTmLlamEntrante = value;
    }

    /**
     * Obtiene el valor de la propiedad iPrDetDtmf.
     * 
     */
    public short getIPrDetDtmf() {
        return iPrDetDtmf;
    }

    /**
     * Define el valor de la propiedad iPrDetDtmf.
     * 
     */
    public void setIPrDetDtmf(short value) {
        this.iPrDetDtmf = value;
    }

    /**
     * Obtiene el valor de la propiedad tPrReleaseBL.
     * 
     */
    public long getTPrReleaseBL() {
        return tPrReleaseBL;
    }

    /**
     * Define el valor de la propiedad tPrReleaseBL.
     * 
     */
    public void setTPrReleaseBL(long value) {
        this.tPrReleaseBL = value;
    }

    /**
     * Obtiene el valor de la propiedad iPrPeriodoSpvRing.
     * 
     */
    public long getIPrPeriodoSpvRing() {
        return iPrPeriodoSpvRing;
    }

    /**
     * Define el valor de la propiedad iPrPeriodoSpvRing.
     * 
     */
    public void setIPrPeriodoSpvRing(long value) {
        this.iPrPeriodoSpvRing = value;
    }

    /**
     * Obtiene el valor de la propiedad iPrFiltroSpvRing.
     * 
     */
    public long getIPrFiltroSpvRing() {
        return iPrFiltroSpvRing;
    }

    /**
     * Define el valor de la propiedad iPrFiltroSpvRing.
     * 
     */
    public void setIPrFiltroSpvRing(long value) {
        this.iPrFiltroSpvRing = value;
    }

    /**
     * Obtiene el valor de la propiedad iPrDetCallerId.
     * 
     */
    public short getIPrDetCallerId() {
        return iPrDetCallerId;
    }

    /**
     * Define el valor de la propiedad iPrDetCallerId.
     * 
     */
    public void setIPrDetCallerId(short value) {
        this.iPrDetCallerId = value;
    }

    /**
     * Obtiene el valor de la propiedad iPrTmCallerId.
     * 
     */
    public long getIPrTmCallerId() {
        return iPrTmCallerId;
    }

    /**
     * Define el valor de la propiedad iPrTmCallerId.
     * 
     */
    public void setIPrTmCallerId(long value) {
        this.iPrTmCallerId = value;
    }

    /**
     * Obtiene el valor de la propiedad iPrDetInversionPol.
     * 
     */
    public short getIPrDetInversionPol() {
        return iPrDetInversionPol;
    }

    /**
     * Define el valor de la propiedad iPrDetInversionPol.
     * 
     */
    public void setIPrDetInversionPol(short value) {
        this.iPrDetInversionPol = value;
    }

    /**
     * Obtiene el valor de la propiedad iPrTmDetFinLlamada.
     * 
     */
    public long getIPrTmDetFinLlamada() {
        return iPrTmDetFinLlamada;
    }

    /**
     * Define el valor de la propiedad iPrTmDetFinLlamada.
     * 
     */
    public void setIPrTmDetFinLlamada(long value) {
        this.iPrTmDetFinLlamada = value;
    }

    /**
     * Obtiene el valor de la propiedad tipoInterface.
     * 
     * @return
     *     possible object is
     *     {@link TipoInterface }
     *     
     */
    public TipoInterface getTipoInterface() {
        return tipoInterface;
    }

    /**
     * Define el valor de la propiedad tipoInterface.
     * 
     * @param value
     *     allowed object is
     *     {@link TipoInterface }
     *     
     */
    public void setTipoInterface(TipoInterface value) {
        this.tipoInterface = value;
    }

    /**
     * Obtiene el valor de la propiedad umbralAGCTXdBm.
     * 
     */
    public float getUmbralAGCTXdBm() {
        return umbralAGCTXdBm;
    }

    /**
     * Define el valor de la propiedad umbralAGCTXdBm.
     * 
     */
    public void setUmbralAGCTXdBm(float value) {
        this.umbralAGCTXdBm = value;
    }

    /**
     * Obtiene el valor de la propiedad umbralAGCRXdBm.
     * 
     */
    public float getUmbralAGCRXdBm() {
        return umbralAGCRXdBm;
    }

    /**
     * Define el valor de la propiedad umbralAGCRXdBm.
     * 
     */
    public void setUmbralAGCRXdBm(float value) {
        this.umbralAGCRXdBm = value;
    }

}

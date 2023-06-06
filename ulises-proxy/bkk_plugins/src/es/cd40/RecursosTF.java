
package es.cd40;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlSchemaType;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para RecursosTF complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="RecursosTF"&gt;
 *   &lt;complexContent&gt;
 *     &lt;extension base="{http://CD40.es/}ParametrosGrabacionTf"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="IdSistema" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="IdRecurso" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="IdPrefijo" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="TipoDestino" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="IdDestino" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="IdTroncal" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="IdRed" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="TReleaseBL" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="iDetCallerId" type="{http://www.w3.org/2001/XMLSchema}unsignedByte"/&gt;
 *         &lt;element name="iTmCallerId" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="iDetInversionPol" type="{http://www.w3.org/2001/XMLSchema}unsignedByte"/&gt;
 *         &lt;element name="iTmLlamEntrante" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="iTmDetFinLlamada" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="iPeriodoSpvRing" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="iFiltroSpvRing" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="iDetDtmf" type="{http://www.w3.org/2001/XMLSchema}unsignedByte"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/extension&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "RecursosTF", propOrder = {
    "idSistema",
    "idRecurso",
    "idPrefijo",
    "tipoDestino",
    "idDestino",
    "idTroncal",
    "idRed",
    "tReleaseBL",
    "iDetCallerId",
    "iTmCallerId",
    "iDetInversionPol",
    "iTmLlamEntrante",
    "iTmDetFinLlamada",
    "iPeriodoSpvRing",
    "iFiltroSpvRing",
    "iDetDtmf"
})
public class RecursosTF
    extends ParametrosGrabacionTf
{

    @XmlElement(name = "IdSistema")
    protected String idSistema;
    @XmlElement(name = "IdRecurso")
    protected String idRecurso;
    @XmlElement(name = "IdPrefijo")
    @XmlSchemaType(name = "unsignedInt")
    protected long idPrefijo;
    @XmlElement(name = "TipoDestino")
    @XmlSchemaType(name = "unsignedInt")
    protected long tipoDestino;
    @XmlElement(name = "IdDestino")
    protected String idDestino;
    @XmlElement(name = "IdTroncal")
    protected String idTroncal;
    @XmlElement(name = "IdRed")
    protected String idRed;
    @XmlElement(name = "TReleaseBL")
    @XmlSchemaType(name = "unsignedInt")
    protected long tReleaseBL;
    @XmlSchemaType(name = "unsignedByte")
    protected short iDetCallerId;
    @XmlSchemaType(name = "unsignedInt")
    protected long iTmCallerId;
    @XmlSchemaType(name = "unsignedByte")
    protected short iDetInversionPol;
    @XmlSchemaType(name = "unsignedInt")
    protected long iTmLlamEntrante;
    @XmlSchemaType(name = "unsignedInt")
    protected long iTmDetFinLlamada;
    @XmlSchemaType(name = "unsignedInt")
    protected long iPeriodoSpvRing;
    @XmlSchemaType(name = "unsignedInt")
    protected long iFiltroSpvRing;
    @XmlSchemaType(name = "unsignedByte")
    protected short iDetDtmf;

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
     * Obtiene el valor de la propiedad idPrefijo.
     * 
     */
    public long getIdPrefijo() {
        return idPrefijo;
    }

    /**
     * Define el valor de la propiedad idPrefijo.
     * 
     */
    public void setIdPrefijo(long value) {
        this.idPrefijo = value;
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
     * Obtiene el valor de la propiedad idTroncal.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getIdTroncal() {
        return idTroncal;
    }

    /**
     * Define el valor de la propiedad idTroncal.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setIdTroncal(String value) {
        this.idTroncal = value;
    }

    /**
     * Obtiene el valor de la propiedad idRed.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getIdRed() {
        return idRed;
    }

    /**
     * Define el valor de la propiedad idRed.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setIdRed(String value) {
        this.idRed = value;
    }

    /**
     * Obtiene el valor de la propiedad tReleaseBL.
     * 
     */
    public long getTReleaseBL() {
        return tReleaseBL;
    }

    /**
     * Define el valor de la propiedad tReleaseBL.
     * 
     */
    public void setTReleaseBL(long value) {
        this.tReleaseBL = value;
    }

    /**
     * Obtiene el valor de la propiedad iDetCallerId.
     * 
     */
    public short getIDetCallerId() {
        return iDetCallerId;
    }

    /**
     * Define el valor de la propiedad iDetCallerId.
     * 
     */
    public void setIDetCallerId(short value) {
        this.iDetCallerId = value;
    }

    /**
     * Obtiene el valor de la propiedad iTmCallerId.
     * 
     */
    public long getITmCallerId() {
        return iTmCallerId;
    }

    /**
     * Define el valor de la propiedad iTmCallerId.
     * 
     */
    public void setITmCallerId(long value) {
        this.iTmCallerId = value;
    }

    /**
     * Obtiene el valor de la propiedad iDetInversionPol.
     * 
     */
    public short getIDetInversionPol() {
        return iDetInversionPol;
    }

    /**
     * Define el valor de la propiedad iDetInversionPol.
     * 
     */
    public void setIDetInversionPol(short value) {
        this.iDetInversionPol = value;
    }

    /**
     * Obtiene el valor de la propiedad iTmLlamEntrante.
     * 
     */
    public long getITmLlamEntrante() {
        return iTmLlamEntrante;
    }

    /**
     * Define el valor de la propiedad iTmLlamEntrante.
     * 
     */
    public void setITmLlamEntrante(long value) {
        this.iTmLlamEntrante = value;
    }

    /**
     * Obtiene el valor de la propiedad iTmDetFinLlamada.
     * 
     */
    public long getITmDetFinLlamada() {
        return iTmDetFinLlamada;
    }

    /**
     * Define el valor de la propiedad iTmDetFinLlamada.
     * 
     */
    public void setITmDetFinLlamada(long value) {
        this.iTmDetFinLlamada = value;
    }

    /**
     * Obtiene el valor de la propiedad iPeriodoSpvRing.
     * 
     */
    public long getIPeriodoSpvRing() {
        return iPeriodoSpvRing;
    }

    /**
     * Define el valor de la propiedad iPeriodoSpvRing.
     * 
     */
    public void setIPeriodoSpvRing(long value) {
        this.iPeriodoSpvRing = value;
    }

    /**
     * Obtiene el valor de la propiedad iFiltroSpvRing.
     * 
     */
    public long getIFiltroSpvRing() {
        return iFiltroSpvRing;
    }

    /**
     * Define el valor de la propiedad iFiltroSpvRing.
     * 
     */
    public void setIFiltroSpvRing(long value) {
        this.iFiltroSpvRing = value;
    }

    /**
     * Obtiene el valor de la propiedad iDetDtmf.
     * 
     */
    public short getIDetDtmf() {
        return iDetDtmf;
    }

    /**
     * Define el valor de la propiedad iDetDtmf.
     * 
     */
    public void setIDetDtmf(short value) {
        this.iDetDtmf = value;
    }

}

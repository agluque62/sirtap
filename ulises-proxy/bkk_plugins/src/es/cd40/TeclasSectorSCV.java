
package es.cd40;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlSeeAlso;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para TeclasSectorSCV complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="TeclasSectorSCV"&gt;
 *   &lt;complexContent&gt;
 *     &lt;extension base="{http://CD40.es/}Tablas"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="TransConConsultaPrev" type="{http://www.w3.org/2001/XMLSchema}boolean"/&gt;
 *         &lt;element name="TransDirecta" type="{http://www.w3.org/2001/XMLSchema}boolean"/&gt;
 *         &lt;element name="Conferencia" type="{http://www.w3.org/2001/XMLSchema}boolean"/&gt;
 *         &lt;element name="Escucha" type="{http://www.w3.org/2001/XMLSchema}boolean"/&gt;
 *         &lt;element name="Retener" type="{http://www.w3.org/2001/XMLSchema}boolean"/&gt;
 *         &lt;element name="Captura" type="{http://www.w3.org/2001/XMLSchema}boolean"/&gt;
 *         &lt;element name="Redireccion" type="{http://www.w3.org/2001/XMLSchema}boolean"/&gt;
 *         &lt;element name="RepeticionUltLlamada" type="{http://www.w3.org/2001/XMLSchema}boolean"/&gt;
 *         &lt;element name="RellamadaAut" type="{http://www.w3.org/2001/XMLSchema}boolean"/&gt;
 *         &lt;element name="TeclaPrioridad" type="{http://www.w3.org/2001/XMLSchema}boolean"/&gt;
 *         &lt;element name="Tecla55mas1" type="{http://www.w3.org/2001/XMLSchema}boolean"/&gt;
 *         &lt;element name="Monitoring" type="{http://www.w3.org/2001/XMLSchema}boolean"/&gt;
 *         &lt;element name="CoordinadorTF" type="{http://www.w3.org/2001/XMLSchema}boolean"/&gt;
 *         &lt;element name="CoordinadorRD" type="{http://www.w3.org/2001/XMLSchema}boolean"/&gt;
 *         &lt;element name="IntegracionRDTF" type="{http://www.w3.org/2001/XMLSchema}boolean"/&gt;
 *         &lt;element name="LlamadaSelectiva" type="{http://www.w3.org/2001/XMLSchema}boolean"/&gt;
 *         &lt;element name="GrupoBSS" type="{http://www.w3.org/2001/XMLSchema}boolean"/&gt;
 *         &lt;element name="LTT" type="{http://www.w3.org/2001/XMLSchema}boolean"/&gt;
 *         &lt;element name="SayAgain" type="{http://www.w3.org/2001/XMLSchema}boolean"/&gt;
 *         &lt;element name="InhabilitacionRedirec" type="{http://www.w3.org/2001/XMLSchema}boolean"/&gt;
 *         &lt;element name="Glp" type="{http://www.w3.org/2001/XMLSchema}boolean"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/extension&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "TeclasSectorSCV", propOrder = {
    "transConConsultaPrev",
    "transDirecta",
    "conferencia",
    "escucha",
    "retener",
    "captura",
    "redireccion",
    "repeticionUltLlamada",
    "rellamadaAut",
    "teclaPrioridad",
    "tecla55Mas1",
    "monitoring",
    "coordinadorTF",
    "coordinadorRD",
    "integracionRDTF",
    "llamadaSelectiva",
    "grupoBSS",
    "ltt",
    "sayAgain",
    "inhabilitacionRedirec",
    "glp"
})
@XmlSeeAlso({
    TeclasSector.class
})
public class TeclasSectorSCV
    extends Tablas
{

    @XmlElement(name = "TransConConsultaPrev")
    protected boolean transConConsultaPrev;
    @XmlElement(name = "TransDirecta")
    protected boolean transDirecta;
    @XmlElement(name = "Conferencia")
    protected boolean conferencia;
    @XmlElement(name = "Escucha")
    protected boolean escucha;
    @XmlElement(name = "Retener")
    protected boolean retener;
    @XmlElement(name = "Captura")
    protected boolean captura;
    @XmlElement(name = "Redireccion")
    protected boolean redireccion;
    @XmlElement(name = "RepeticionUltLlamada")
    protected boolean repeticionUltLlamada;
    @XmlElement(name = "RellamadaAut")
    protected boolean rellamadaAut;
    @XmlElement(name = "TeclaPrioridad")
    protected boolean teclaPrioridad;
    @XmlElement(name = "Tecla55mas1")
    protected boolean tecla55Mas1;
    @XmlElement(name = "Monitoring")
    protected boolean monitoring;
    @XmlElement(name = "CoordinadorTF")
    protected boolean coordinadorTF;
    @XmlElement(name = "CoordinadorRD")
    protected boolean coordinadorRD;
    @XmlElement(name = "IntegracionRDTF")
    protected boolean integracionRDTF;
    @XmlElement(name = "LlamadaSelectiva")
    protected boolean llamadaSelectiva;
    @XmlElement(name = "GrupoBSS")
    protected boolean grupoBSS;
    @XmlElement(name = "LTT")
    protected boolean ltt;
    @XmlElement(name = "SayAgain")
    protected boolean sayAgain;
    @XmlElement(name = "InhabilitacionRedirec")
    protected boolean inhabilitacionRedirec;
    @XmlElement(name = "Glp")
    protected boolean glp;

    /**
     * Obtiene el valor de la propiedad transConConsultaPrev.
     * 
     */
    public boolean isTransConConsultaPrev() {
        return transConConsultaPrev;
    }

    /**
     * Define el valor de la propiedad transConConsultaPrev.
     * 
     */
    public void setTransConConsultaPrev(boolean value) {
        this.transConConsultaPrev = value;
    }

    /**
     * Obtiene el valor de la propiedad transDirecta.
     * 
     */
    public boolean isTransDirecta() {
        return transDirecta;
    }

    /**
     * Define el valor de la propiedad transDirecta.
     * 
     */
    public void setTransDirecta(boolean value) {
        this.transDirecta = value;
    }

    /**
     * Obtiene el valor de la propiedad conferencia.
     * 
     */
    public boolean isConferencia() {
        return conferencia;
    }

    /**
     * Define el valor de la propiedad conferencia.
     * 
     */
    public void setConferencia(boolean value) {
        this.conferencia = value;
    }

    /**
     * Obtiene el valor de la propiedad escucha.
     * 
     */
    public boolean isEscucha() {
        return escucha;
    }

    /**
     * Define el valor de la propiedad escucha.
     * 
     */
    public void setEscucha(boolean value) {
        this.escucha = value;
    }

    /**
     * Obtiene el valor de la propiedad retener.
     * 
     */
    public boolean isRetener() {
        return retener;
    }

    /**
     * Define el valor de la propiedad retener.
     * 
     */
    public void setRetener(boolean value) {
        this.retener = value;
    }

    /**
     * Obtiene el valor de la propiedad captura.
     * 
     */
    public boolean isCaptura() {
        return captura;
    }

    /**
     * Define el valor de la propiedad captura.
     * 
     */
    public void setCaptura(boolean value) {
        this.captura = value;
    }

    /**
     * Obtiene el valor de la propiedad redireccion.
     * 
     */
    public boolean isRedireccion() {
        return redireccion;
    }

    /**
     * Define el valor de la propiedad redireccion.
     * 
     */
    public void setRedireccion(boolean value) {
        this.redireccion = value;
    }

    /**
     * Obtiene el valor de la propiedad repeticionUltLlamada.
     * 
     */
    public boolean isRepeticionUltLlamada() {
        return repeticionUltLlamada;
    }

    /**
     * Define el valor de la propiedad repeticionUltLlamada.
     * 
     */
    public void setRepeticionUltLlamada(boolean value) {
        this.repeticionUltLlamada = value;
    }

    /**
     * Obtiene el valor de la propiedad rellamadaAut.
     * 
     */
    public boolean isRellamadaAut() {
        return rellamadaAut;
    }

    /**
     * Define el valor de la propiedad rellamadaAut.
     * 
     */
    public void setRellamadaAut(boolean value) {
        this.rellamadaAut = value;
    }

    /**
     * Obtiene el valor de la propiedad teclaPrioridad.
     * 
     */
    public boolean isTeclaPrioridad() {
        return teclaPrioridad;
    }

    /**
     * Define el valor de la propiedad teclaPrioridad.
     * 
     */
    public void setTeclaPrioridad(boolean value) {
        this.teclaPrioridad = value;
    }

    /**
     * Obtiene el valor de la propiedad tecla55Mas1.
     * 
     */
    public boolean isTecla55Mas1() {
        return tecla55Mas1;
    }

    /**
     * Define el valor de la propiedad tecla55Mas1.
     * 
     */
    public void setTecla55Mas1(boolean value) {
        this.tecla55Mas1 = value;
    }

    /**
     * Obtiene el valor de la propiedad monitoring.
     * 
     */
    public boolean isMonitoring() {
        return monitoring;
    }

    /**
     * Define el valor de la propiedad monitoring.
     * 
     */
    public void setMonitoring(boolean value) {
        this.monitoring = value;
    }

    /**
     * Obtiene el valor de la propiedad coordinadorTF.
     * 
     */
    public boolean isCoordinadorTF() {
        return coordinadorTF;
    }

    /**
     * Define el valor de la propiedad coordinadorTF.
     * 
     */
    public void setCoordinadorTF(boolean value) {
        this.coordinadorTF = value;
    }

    /**
     * Obtiene el valor de la propiedad coordinadorRD.
     * 
     */
    public boolean isCoordinadorRD() {
        return coordinadorRD;
    }

    /**
     * Define el valor de la propiedad coordinadorRD.
     * 
     */
    public void setCoordinadorRD(boolean value) {
        this.coordinadorRD = value;
    }

    /**
     * Obtiene el valor de la propiedad integracionRDTF.
     * 
     */
    public boolean isIntegracionRDTF() {
        return integracionRDTF;
    }

    /**
     * Define el valor de la propiedad integracionRDTF.
     * 
     */
    public void setIntegracionRDTF(boolean value) {
        this.integracionRDTF = value;
    }

    /**
     * Obtiene el valor de la propiedad llamadaSelectiva.
     * 
     */
    public boolean isLlamadaSelectiva() {
        return llamadaSelectiva;
    }

    /**
     * Define el valor de la propiedad llamadaSelectiva.
     * 
     */
    public void setLlamadaSelectiva(boolean value) {
        this.llamadaSelectiva = value;
    }

    /**
     * Obtiene el valor de la propiedad grupoBSS.
     * 
     */
    public boolean isGrupoBSS() {
        return grupoBSS;
    }

    /**
     * Define el valor de la propiedad grupoBSS.
     * 
     */
    public void setGrupoBSS(boolean value) {
        this.grupoBSS = value;
    }

    /**
     * Obtiene el valor de la propiedad ltt.
     * 
     */
    public boolean isLTT() {
        return ltt;
    }

    /**
     * Define el valor de la propiedad ltt.
     * 
     */
    public void setLTT(boolean value) {
        this.ltt = value;
    }

    /**
     * Obtiene el valor de la propiedad sayAgain.
     * 
     */
    public boolean isSayAgain() {
        return sayAgain;
    }

    /**
     * Define el valor de la propiedad sayAgain.
     * 
     */
    public void setSayAgain(boolean value) {
        this.sayAgain = value;
    }

    /**
     * Obtiene el valor de la propiedad inhabilitacionRedirec.
     * 
     */
    public boolean isInhabilitacionRedirec() {
        return inhabilitacionRedirec;
    }

    /**
     * Define el valor de la propiedad inhabilitacionRedirec.
     * 
     */
    public void setInhabilitacionRedirec(boolean value) {
        this.inhabilitacionRedirec = value;
    }

    /**
     * Obtiene el valor de la propiedad glp.
     * 
     */
    public boolean isGlp() {
        return glp;
    }

    /**
     * Define el valor de la propiedad glp.
     * 
     */
    public void setGlp(boolean value) {
        this.glp = value;
    }

}

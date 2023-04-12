
package es.cd40;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlSchemaType;
import javax.xml.bind.annotation.XmlSeeAlso;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para ParametrosRDSCV complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="ParametrosRDSCV"&gt;
 *   &lt;complexContent&gt;
 *     &lt;extension base="{http://CD40.es/}ParametrosRecursoGeneral"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="ListaEnlacesRecursosExternos" type="{http://CD40.es/}ArrayOfRecursosExternos" minOccurs="0"/&gt;
 *         &lt;element name="EM" type="{http://www.w3.org/2001/XMLSchema}boolean"/&gt;
 *         &lt;element name="SQ" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="PTT" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="FrqTonoE" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="UmbralTonoE" type="{http://www.w3.org/2001/XMLSchema}int"/&gt;
 *         &lt;element name="FrqTonoM" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="UmbralTonoM" type="{http://www.w3.org/2001/XMLSchema}int"/&gt;
 *         &lt;element name="FrqTonoSQ" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="UmbralTonoSQ" type="{http://www.w3.org/2001/XMLSchema}int"/&gt;
 *         &lt;element name="FrqTonoPTT" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="UmbralTonoPTT" type="{http://www.w3.org/2001/XMLSchema}int"/&gt;
 *         &lt;element name="BSS" type="{http://www.w3.org/2001/XMLSchema}boolean"/&gt;
 *         &lt;element name="NTZ" type="{http://www.w3.org/2001/XMLSchema}boolean"/&gt;
 *         &lt;element name="TipoNTZ" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="Cifrado" type="{http://www.w3.org/2001/XMLSchema}boolean"/&gt;
 *         &lt;element name="SupervPortadoraTx" type="{http://www.w3.org/2001/XMLSchema}boolean"/&gt;
 *         &lt;element name="SupervModuladoraTx" type="{http://www.w3.org/2001/XMLSchema}boolean"/&gt;
 *         &lt;element name="ModoConfPTT" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="RepSQyBSS" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="DesactivacionSQ" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="TimeoutPTT" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="UmbralVAD" type="{http://www.w3.org/2001/XMLSchema}int"/&gt;
 *         &lt;element name="TiempoPTT" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="NumFlujosAudio" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/extension&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "ParametrosRDSCV", propOrder = {
    "listaEnlacesRecursosExternos",
    "em",
    "sq",
    "ptt",
    "frqTonoE",
    "umbralTonoE",
    "frqTonoM",
    "umbralTonoM",
    "frqTonoSQ",
    "umbralTonoSQ",
    "frqTonoPTT",
    "umbralTonoPTT",
    "bss",
    "ntz",
    "tipoNTZ",
    "cifrado",
    "supervPortadoraTx",
    "supervModuladoraTx",
    "modoConfPTT",
    "repSQyBSS",
    "desactivacionSQ",
    "timeoutPTT",
    "umbralVAD",
    "tiempoPTT",
    "numFlujosAudio"
})
@XmlSeeAlso({
    ParametrosRecursoRadio.class
})
public class ParametrosRDSCV
    extends ParametrosRecursoGeneral
{

    @XmlElement(name = "ListaEnlacesRecursosExternos")
    protected ArrayOfRecursosExternos listaEnlacesRecursosExternos;
    @XmlElement(name = "EM")
    protected boolean em;
    @XmlElement(name = "SQ")
    protected String sq;
    @XmlElement(name = "PTT")
    protected String ptt;
    @XmlElement(name = "FrqTonoE")
    @XmlSchemaType(name = "unsignedInt")
    protected long frqTonoE;
    @XmlElement(name = "UmbralTonoE")
    protected int umbralTonoE;
    @XmlElement(name = "FrqTonoM")
    @XmlSchemaType(name = "unsignedInt")
    protected long frqTonoM;
    @XmlElement(name = "UmbralTonoM")
    protected int umbralTonoM;
    @XmlElement(name = "FrqTonoSQ")
    @XmlSchemaType(name = "unsignedInt")
    protected long frqTonoSQ;
    @XmlElement(name = "UmbralTonoSQ")
    protected int umbralTonoSQ;
    @XmlElement(name = "FrqTonoPTT")
    @XmlSchemaType(name = "unsignedInt")
    protected long frqTonoPTT;
    @XmlElement(name = "UmbralTonoPTT")
    protected int umbralTonoPTT;
    @XmlElement(name = "BSS")
    protected boolean bss;
    @XmlElement(name = "NTZ")
    protected boolean ntz;
    @XmlElement(name = "TipoNTZ")
    @XmlSchemaType(name = "unsignedInt")
    protected long tipoNTZ;
    @XmlElement(name = "Cifrado")
    protected boolean cifrado;
    @XmlElement(name = "SupervPortadoraTx")
    protected boolean supervPortadoraTx;
    @XmlElement(name = "SupervModuladoraTx")
    protected boolean supervModuladoraTx;
    @XmlElement(name = "ModoConfPTT")
    @XmlSchemaType(name = "unsignedInt")
    protected long modoConfPTT;
    @XmlElement(name = "RepSQyBSS")
    @XmlSchemaType(name = "unsignedInt")
    protected long repSQyBSS;
    @XmlElement(name = "DesactivacionSQ")
    @XmlSchemaType(name = "unsignedInt")
    protected long desactivacionSQ;
    @XmlElement(name = "TimeoutPTT")
    @XmlSchemaType(name = "unsignedInt")
    protected long timeoutPTT;
    @XmlElement(name = "UmbralVAD")
    protected int umbralVAD;
    @XmlElement(name = "TiempoPTT")
    @XmlSchemaType(name = "unsignedInt")
    protected long tiempoPTT;
    @XmlElement(name = "NumFlujosAudio")
    @XmlSchemaType(name = "unsignedInt")
    protected long numFlujosAudio;

    /**
     * Obtiene el valor de la propiedad listaEnlacesRecursosExternos.
     * 
     * @return
     *     possible object is
     *     {@link ArrayOfRecursosExternos }
     *     
     */
    public ArrayOfRecursosExternos getListaEnlacesRecursosExternos() {
        return listaEnlacesRecursosExternos;
    }

    /**
     * Define el valor de la propiedad listaEnlacesRecursosExternos.
     * 
     * @param value
     *     allowed object is
     *     {@link ArrayOfRecursosExternos }
     *     
     */
    public void setListaEnlacesRecursosExternos(ArrayOfRecursosExternos value) {
        this.listaEnlacesRecursosExternos = value;
    }

    /**
     * Obtiene el valor de la propiedad em.
     * 
     */
    public boolean isEM() {
        return em;
    }

    /**
     * Define el valor de la propiedad em.
     * 
     */
    public void setEM(boolean value) {
        this.em = value;
    }

    /**
     * Obtiene el valor de la propiedad sq.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getSQ() {
        return sq;
    }

    /**
     * Define el valor de la propiedad sq.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setSQ(String value) {
        this.sq = value;
    }

    /**
     * Obtiene el valor de la propiedad ptt.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getPTT() {
        return ptt;
    }

    /**
     * Define el valor de la propiedad ptt.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setPTT(String value) {
        this.ptt = value;
    }

    /**
     * Obtiene el valor de la propiedad frqTonoE.
     * 
     */
    public long getFrqTonoE() {
        return frqTonoE;
    }

    /**
     * Define el valor de la propiedad frqTonoE.
     * 
     */
    public void setFrqTonoE(long value) {
        this.frqTonoE = value;
    }

    /**
     * Obtiene el valor de la propiedad umbralTonoE.
     * 
     */
    public int getUmbralTonoE() {
        return umbralTonoE;
    }

    /**
     * Define el valor de la propiedad umbralTonoE.
     * 
     */
    public void setUmbralTonoE(int value) {
        this.umbralTonoE = value;
    }

    /**
     * Obtiene el valor de la propiedad frqTonoM.
     * 
     */
    public long getFrqTonoM() {
        return frqTonoM;
    }

    /**
     * Define el valor de la propiedad frqTonoM.
     * 
     */
    public void setFrqTonoM(long value) {
        this.frqTonoM = value;
    }

    /**
     * Obtiene el valor de la propiedad umbralTonoM.
     * 
     */
    public int getUmbralTonoM() {
        return umbralTonoM;
    }

    /**
     * Define el valor de la propiedad umbralTonoM.
     * 
     */
    public void setUmbralTonoM(int value) {
        this.umbralTonoM = value;
    }

    /**
     * Obtiene el valor de la propiedad frqTonoSQ.
     * 
     */
    public long getFrqTonoSQ() {
        return frqTonoSQ;
    }

    /**
     * Define el valor de la propiedad frqTonoSQ.
     * 
     */
    public void setFrqTonoSQ(long value) {
        this.frqTonoSQ = value;
    }

    /**
     * Obtiene el valor de la propiedad umbralTonoSQ.
     * 
     */
    public int getUmbralTonoSQ() {
        return umbralTonoSQ;
    }

    /**
     * Define el valor de la propiedad umbralTonoSQ.
     * 
     */
    public void setUmbralTonoSQ(int value) {
        this.umbralTonoSQ = value;
    }

    /**
     * Obtiene el valor de la propiedad frqTonoPTT.
     * 
     */
    public long getFrqTonoPTT() {
        return frqTonoPTT;
    }

    /**
     * Define el valor de la propiedad frqTonoPTT.
     * 
     */
    public void setFrqTonoPTT(long value) {
        this.frqTonoPTT = value;
    }

    /**
     * Obtiene el valor de la propiedad umbralTonoPTT.
     * 
     */
    public int getUmbralTonoPTT() {
        return umbralTonoPTT;
    }

    /**
     * Define el valor de la propiedad umbralTonoPTT.
     * 
     */
    public void setUmbralTonoPTT(int value) {
        this.umbralTonoPTT = value;
    }

    /**
     * Obtiene el valor de la propiedad bss.
     * 
     */
    public boolean isBSS() {
        return bss;
    }

    /**
     * Define el valor de la propiedad bss.
     * 
     */
    public void setBSS(boolean value) {
        this.bss = value;
    }

    /**
     * Obtiene el valor de la propiedad ntz.
     * 
     */
    public boolean isNTZ() {
        return ntz;
    }

    /**
     * Define el valor de la propiedad ntz.
     * 
     */
    public void setNTZ(boolean value) {
        this.ntz = value;
    }

    /**
     * Obtiene el valor de la propiedad tipoNTZ.
     * 
     */
    public long getTipoNTZ() {
        return tipoNTZ;
    }

    /**
     * Define el valor de la propiedad tipoNTZ.
     * 
     */
    public void setTipoNTZ(long value) {
        this.tipoNTZ = value;
    }

    /**
     * Obtiene el valor de la propiedad cifrado.
     * 
     */
    public boolean isCifrado() {
        return cifrado;
    }

    /**
     * Define el valor de la propiedad cifrado.
     * 
     */
    public void setCifrado(boolean value) {
        this.cifrado = value;
    }

    /**
     * Obtiene el valor de la propiedad supervPortadoraTx.
     * 
     */
    public boolean isSupervPortadoraTx() {
        return supervPortadoraTx;
    }

    /**
     * Define el valor de la propiedad supervPortadoraTx.
     * 
     */
    public void setSupervPortadoraTx(boolean value) {
        this.supervPortadoraTx = value;
    }

    /**
     * Obtiene el valor de la propiedad supervModuladoraTx.
     * 
     */
    public boolean isSupervModuladoraTx() {
        return supervModuladoraTx;
    }

    /**
     * Define el valor de la propiedad supervModuladoraTx.
     * 
     */
    public void setSupervModuladoraTx(boolean value) {
        this.supervModuladoraTx = value;
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
     * Obtiene el valor de la propiedad repSQyBSS.
     * 
     */
    public long getRepSQyBSS() {
        return repSQyBSS;
    }

    /**
     * Define el valor de la propiedad repSQyBSS.
     * 
     */
    public void setRepSQyBSS(long value) {
        this.repSQyBSS = value;
    }

    /**
     * Obtiene el valor de la propiedad desactivacionSQ.
     * 
     */
    public long getDesactivacionSQ() {
        return desactivacionSQ;
    }

    /**
     * Define el valor de la propiedad desactivacionSQ.
     * 
     */
    public void setDesactivacionSQ(long value) {
        this.desactivacionSQ = value;
    }

    /**
     * Obtiene el valor de la propiedad timeoutPTT.
     * 
     */
    public long getTimeoutPTT() {
        return timeoutPTT;
    }

    /**
     * Define el valor de la propiedad timeoutPTT.
     * 
     */
    public void setTimeoutPTT(long value) {
        this.timeoutPTT = value;
    }

    /**
     * Obtiene el valor de la propiedad umbralVAD.
     * 
     */
    public int getUmbralVAD() {
        return umbralVAD;
    }

    /**
     * Define el valor de la propiedad umbralVAD.
     * 
     */
    public void setUmbralVAD(int value) {
        this.umbralVAD = value;
    }

    /**
     * Obtiene el valor de la propiedad tiempoPTT.
     * 
     */
    public long getTiempoPTT() {
        return tiempoPTT;
    }

    /**
     * Define el valor de la propiedad tiempoPTT.
     * 
     */
    public void setTiempoPTT(long value) {
        this.tiempoPTT = value;
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

}


package es.cd40;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlSchemaType;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para Sistema complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="Sistema"&gt;
 *   &lt;complexContent&gt;
 *     &lt;extension base="{http://CD40.es/}Tablas"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="IdSistema" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="TiempoPtt" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="TiempoSinJack1" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="TiempoSinJack2" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="TamLiteralEnlExt" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="TamLiteralDA" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="TamLiteralIA" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="TamLiteralAG" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="TamLiteralEmplazamiento" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="VersionIP" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="GrupoMulticastConfiguracion" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="PuertoMulticastConfiguracion" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="EstadoScv1" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="EstadoScv2" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="KeepAlivePeriod" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="KeepAliveMultiplier" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="NumLlamadasEntrantesIda" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="NumLlamadasEnIda" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="NumFrecPagina" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="NumPagFrec" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="NumEnlacesInternosPag" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="NumPagEnlacesInt" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="NumDestinosInternosPag" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="NumPagDestinosInt" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="NumEnlacesAI" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="NumMaxSectores" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/extension&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "Sistema", propOrder = {
    "idSistema",
    "tiempoPtt",
    "tiempoSinJack1",
    "tiempoSinJack2",
    "tamLiteralEnlExt",
    "tamLiteralDA",
    "tamLiteralIA",
    "tamLiteralAG",
    "tamLiteralEmplazamiento",
    "versionIP",
    "grupoMulticastConfiguracion",
    "puertoMulticastConfiguracion",
    "estadoScv1",
    "estadoScv2",
    "keepAlivePeriod",
    "keepAliveMultiplier",
    "numLlamadasEntrantesIda",
    "numLlamadasEnIda",
    "numFrecPagina",
    "numPagFrec",
    "numEnlacesInternosPag",
    "numPagEnlacesInt",
    "numDestinosInternosPag",
    "numPagDestinosInt",
    "numEnlacesAI",
    "numMaxSectores"
})
public class Sistema
    extends Tablas
{

    @XmlElement(name = "IdSistema")
    protected String idSistema;
    @XmlElement(name = "TiempoPtt")
    @XmlSchemaType(name = "unsignedInt")
    protected long tiempoPtt;
    @XmlElement(name = "TiempoSinJack1")
    @XmlSchemaType(name = "unsignedInt")
    protected long tiempoSinJack1;
    @XmlElement(name = "TiempoSinJack2")
    @XmlSchemaType(name = "unsignedInt")
    protected long tiempoSinJack2;
    @XmlElement(name = "TamLiteralEnlExt")
    @XmlSchemaType(name = "unsignedInt")
    protected long tamLiteralEnlExt;
    @XmlElement(name = "TamLiteralDA")
    @XmlSchemaType(name = "unsignedInt")
    protected long tamLiteralDA;
    @XmlElement(name = "TamLiteralIA")
    @XmlSchemaType(name = "unsignedInt")
    protected long tamLiteralIA;
    @XmlElement(name = "TamLiteralAG")
    @XmlSchemaType(name = "unsignedInt")
    protected long tamLiteralAG;
    @XmlElement(name = "TamLiteralEmplazamiento")
    @XmlSchemaType(name = "unsignedInt")
    protected long tamLiteralEmplazamiento;
    @XmlElement(name = "VersionIP")
    @XmlSchemaType(name = "unsignedInt")
    protected long versionIP;
    @XmlElement(name = "GrupoMulticastConfiguracion")
    protected String grupoMulticastConfiguracion;
    @XmlElement(name = "PuertoMulticastConfiguracion")
    @XmlSchemaType(name = "unsignedInt")
    protected long puertoMulticastConfiguracion;
    @XmlElement(name = "EstadoScv1")
    @XmlSchemaType(name = "unsignedInt")
    protected long estadoScv1;
    @XmlElement(name = "EstadoScv2")
    @XmlSchemaType(name = "unsignedInt")
    protected long estadoScv2;
    @XmlElement(name = "KeepAlivePeriod")
    @XmlSchemaType(name = "unsignedInt")
    protected long keepAlivePeriod;
    @XmlElement(name = "KeepAliveMultiplier")
    @XmlSchemaType(name = "unsignedInt")
    protected long keepAliveMultiplier;
    @XmlElement(name = "NumLlamadasEntrantesIda")
    @XmlSchemaType(name = "unsignedInt")
    protected long numLlamadasEntrantesIda;
    @XmlElement(name = "NumLlamadasEnIda")
    @XmlSchemaType(name = "unsignedInt")
    protected long numLlamadasEnIda;
    @XmlElement(name = "NumFrecPagina")
    @XmlSchemaType(name = "unsignedInt")
    protected long numFrecPagina;
    @XmlElement(name = "NumPagFrec")
    @XmlSchemaType(name = "unsignedInt")
    protected long numPagFrec;
    @XmlElement(name = "NumEnlacesInternosPag")
    @XmlSchemaType(name = "unsignedInt")
    protected long numEnlacesInternosPag;
    @XmlElement(name = "NumPagEnlacesInt")
    @XmlSchemaType(name = "unsignedInt")
    protected long numPagEnlacesInt;
    @XmlElement(name = "NumDestinosInternosPag")
    @XmlSchemaType(name = "unsignedInt")
    protected long numDestinosInternosPag;
    @XmlElement(name = "NumPagDestinosInt")
    @XmlSchemaType(name = "unsignedInt")
    protected long numPagDestinosInt;
    @XmlElement(name = "NumEnlacesAI")
    @XmlSchemaType(name = "unsignedInt")
    protected long numEnlacesAI;
    @XmlElement(name = "NumMaxSectores")
    @XmlSchemaType(name = "unsignedInt")
    protected long numMaxSectores;

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
     * Obtiene el valor de la propiedad tiempoPtt.
     * 
     */
    public long getTiempoPtt() {
        return tiempoPtt;
    }

    /**
     * Define el valor de la propiedad tiempoPtt.
     * 
     */
    public void setTiempoPtt(long value) {
        this.tiempoPtt = value;
    }

    /**
     * Obtiene el valor de la propiedad tiempoSinJack1.
     * 
     */
    public long getTiempoSinJack1() {
        return tiempoSinJack1;
    }

    /**
     * Define el valor de la propiedad tiempoSinJack1.
     * 
     */
    public void setTiempoSinJack1(long value) {
        this.tiempoSinJack1 = value;
    }

    /**
     * Obtiene el valor de la propiedad tiempoSinJack2.
     * 
     */
    public long getTiempoSinJack2() {
        return tiempoSinJack2;
    }

    /**
     * Define el valor de la propiedad tiempoSinJack2.
     * 
     */
    public void setTiempoSinJack2(long value) {
        this.tiempoSinJack2 = value;
    }

    /**
     * Obtiene el valor de la propiedad tamLiteralEnlExt.
     * 
     */
    public long getTamLiteralEnlExt() {
        return tamLiteralEnlExt;
    }

    /**
     * Define el valor de la propiedad tamLiteralEnlExt.
     * 
     */
    public void setTamLiteralEnlExt(long value) {
        this.tamLiteralEnlExt = value;
    }

    /**
     * Obtiene el valor de la propiedad tamLiteralDA.
     * 
     */
    public long getTamLiteralDA() {
        return tamLiteralDA;
    }

    /**
     * Define el valor de la propiedad tamLiteralDA.
     * 
     */
    public void setTamLiteralDA(long value) {
        this.tamLiteralDA = value;
    }

    /**
     * Obtiene el valor de la propiedad tamLiteralIA.
     * 
     */
    public long getTamLiteralIA() {
        return tamLiteralIA;
    }

    /**
     * Define el valor de la propiedad tamLiteralIA.
     * 
     */
    public void setTamLiteralIA(long value) {
        this.tamLiteralIA = value;
    }

    /**
     * Obtiene el valor de la propiedad tamLiteralAG.
     * 
     */
    public long getTamLiteralAG() {
        return tamLiteralAG;
    }

    /**
     * Define el valor de la propiedad tamLiteralAG.
     * 
     */
    public void setTamLiteralAG(long value) {
        this.tamLiteralAG = value;
    }

    /**
     * Obtiene el valor de la propiedad tamLiteralEmplazamiento.
     * 
     */
    public long getTamLiteralEmplazamiento() {
        return tamLiteralEmplazamiento;
    }

    /**
     * Define el valor de la propiedad tamLiteralEmplazamiento.
     * 
     */
    public void setTamLiteralEmplazamiento(long value) {
        this.tamLiteralEmplazamiento = value;
    }

    /**
     * Obtiene el valor de la propiedad versionIP.
     * 
     */
    public long getVersionIP() {
        return versionIP;
    }

    /**
     * Define el valor de la propiedad versionIP.
     * 
     */
    public void setVersionIP(long value) {
        this.versionIP = value;
    }

    /**
     * Obtiene el valor de la propiedad grupoMulticastConfiguracion.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getGrupoMulticastConfiguracion() {
        return grupoMulticastConfiguracion;
    }

    /**
     * Define el valor de la propiedad grupoMulticastConfiguracion.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setGrupoMulticastConfiguracion(String value) {
        this.grupoMulticastConfiguracion = value;
    }

    /**
     * Obtiene el valor de la propiedad puertoMulticastConfiguracion.
     * 
     */
    public long getPuertoMulticastConfiguracion() {
        return puertoMulticastConfiguracion;
    }

    /**
     * Define el valor de la propiedad puertoMulticastConfiguracion.
     * 
     */
    public void setPuertoMulticastConfiguracion(long value) {
        this.puertoMulticastConfiguracion = value;
    }

    /**
     * Obtiene el valor de la propiedad estadoScv1.
     * 
     */
    public long getEstadoScv1() {
        return estadoScv1;
    }

    /**
     * Define el valor de la propiedad estadoScv1.
     * 
     */
    public void setEstadoScv1(long value) {
        this.estadoScv1 = value;
    }

    /**
     * Obtiene el valor de la propiedad estadoScv2.
     * 
     */
    public long getEstadoScv2() {
        return estadoScv2;
    }

    /**
     * Define el valor de la propiedad estadoScv2.
     * 
     */
    public void setEstadoScv2(long value) {
        this.estadoScv2 = value;
    }

    /**
     * Obtiene el valor de la propiedad keepAlivePeriod.
     * 
     */
    public long getKeepAlivePeriod() {
        return keepAlivePeriod;
    }

    /**
     * Define el valor de la propiedad keepAlivePeriod.
     * 
     */
    public void setKeepAlivePeriod(long value) {
        this.keepAlivePeriod = value;
    }

    /**
     * Obtiene el valor de la propiedad keepAliveMultiplier.
     * 
     */
    public long getKeepAliveMultiplier() {
        return keepAliveMultiplier;
    }

    /**
     * Define el valor de la propiedad keepAliveMultiplier.
     * 
     */
    public void setKeepAliveMultiplier(long value) {
        this.keepAliveMultiplier = value;
    }

    /**
     * Obtiene el valor de la propiedad numLlamadasEntrantesIda.
     * 
     */
    public long getNumLlamadasEntrantesIda() {
        return numLlamadasEntrantesIda;
    }

    /**
     * Define el valor de la propiedad numLlamadasEntrantesIda.
     * 
     */
    public void setNumLlamadasEntrantesIda(long value) {
        this.numLlamadasEntrantesIda = value;
    }

    /**
     * Obtiene el valor de la propiedad numLlamadasEnIda.
     * 
     */
    public long getNumLlamadasEnIda() {
        return numLlamadasEnIda;
    }

    /**
     * Define el valor de la propiedad numLlamadasEnIda.
     * 
     */
    public void setNumLlamadasEnIda(long value) {
        this.numLlamadasEnIda = value;
    }

    /**
     * Obtiene el valor de la propiedad numFrecPagina.
     * 
     */
    public long getNumFrecPagina() {
        return numFrecPagina;
    }

    /**
     * Define el valor de la propiedad numFrecPagina.
     * 
     */
    public void setNumFrecPagina(long value) {
        this.numFrecPagina = value;
    }

    /**
     * Obtiene el valor de la propiedad numPagFrec.
     * 
     */
    public long getNumPagFrec() {
        return numPagFrec;
    }

    /**
     * Define el valor de la propiedad numPagFrec.
     * 
     */
    public void setNumPagFrec(long value) {
        this.numPagFrec = value;
    }

    /**
     * Obtiene el valor de la propiedad numEnlacesInternosPag.
     * 
     */
    public long getNumEnlacesInternosPag() {
        return numEnlacesInternosPag;
    }

    /**
     * Define el valor de la propiedad numEnlacesInternosPag.
     * 
     */
    public void setNumEnlacesInternosPag(long value) {
        this.numEnlacesInternosPag = value;
    }

    /**
     * Obtiene el valor de la propiedad numPagEnlacesInt.
     * 
     */
    public long getNumPagEnlacesInt() {
        return numPagEnlacesInt;
    }

    /**
     * Define el valor de la propiedad numPagEnlacesInt.
     * 
     */
    public void setNumPagEnlacesInt(long value) {
        this.numPagEnlacesInt = value;
    }

    /**
     * Obtiene el valor de la propiedad numDestinosInternosPag.
     * 
     */
    public long getNumDestinosInternosPag() {
        return numDestinosInternosPag;
    }

    /**
     * Define el valor de la propiedad numDestinosInternosPag.
     * 
     */
    public void setNumDestinosInternosPag(long value) {
        this.numDestinosInternosPag = value;
    }

    /**
     * Obtiene el valor de la propiedad numPagDestinosInt.
     * 
     */
    public long getNumPagDestinosInt() {
        return numPagDestinosInt;
    }

    /**
     * Define el valor de la propiedad numPagDestinosInt.
     * 
     */
    public void setNumPagDestinosInt(long value) {
        this.numPagDestinosInt = value;
    }

    /**
     * Obtiene el valor de la propiedad numEnlacesAI.
     * 
     */
    public long getNumEnlacesAI() {
        return numEnlacesAI;
    }

    /**
     * Define el valor de la propiedad numEnlacesAI.
     * 
     */
    public void setNumEnlacesAI(long value) {
        this.numEnlacesAI = value;
    }

    /**
     * Obtiene el valor de la propiedad numMaxSectores.
     * 
     */
    public long getNumMaxSectores() {
        return numMaxSectores;
    }

    /**
     * Define el valor de la propiedad numMaxSectores.
     * 
     */
    public void setNumMaxSectores(long value) {
        this.numMaxSectores = value;
    }

}

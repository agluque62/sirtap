
package es.cd40;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlSchemaType;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para CfgEnlaceInterno complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="CfgEnlaceInterno"&gt;
 *   &lt;complexContent&gt;
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="Literal" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="PosicionHMI" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="TipoEnlaceInterno" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="ListaRecursos" type="{http://CD40.es/}ArrayOfCfgRecursoEnlaceInternoConInterface" minOccurs="0"/&gt;
 *         &lt;element name="Dependencia" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="Prioridad" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="OrigenR2" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="Dominio" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/restriction&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "CfgEnlaceInterno", propOrder = {
    "literal",
    "posicionHMI",
    "tipoEnlaceInterno",
    "listaRecursos",
    "dependencia",
    "prioridad",
    "origenR2",
    "dominio"
})
public class CfgEnlaceInterno {

    @XmlElement(name = "Literal")
    protected String literal;
    @XmlElement(name = "PosicionHMI")
    @XmlSchemaType(name = "unsignedInt")
    protected long posicionHMI;
    @XmlElement(name = "TipoEnlaceInterno")
    protected String tipoEnlaceInterno;
    @XmlElement(name = "ListaRecursos")
    protected ArrayOfCfgRecursoEnlaceInternoConInterface listaRecursos;
    @XmlElement(name = "Dependencia")
    protected String dependencia;
    @XmlElement(name = "Prioridad")
    @XmlSchemaType(name = "unsignedInt")
    protected long prioridad;
    @XmlElement(name = "OrigenR2")
    protected String origenR2;
    @XmlElement(name = "Dominio")
    protected String dominio;

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
     * Obtiene el valor de la propiedad posicionHMI.
     * 
     */
    public long getPosicionHMI() {
        return posicionHMI;
    }

    /**
     * Define el valor de la propiedad posicionHMI.
     * 
     */
    public void setPosicionHMI(long value) {
        this.posicionHMI = value;
    }

    /**
     * Obtiene el valor de la propiedad tipoEnlaceInterno.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getTipoEnlaceInterno() {
        return tipoEnlaceInterno;
    }

    /**
     * Define el valor de la propiedad tipoEnlaceInterno.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setTipoEnlaceInterno(String value) {
        this.tipoEnlaceInterno = value;
    }

    /**
     * Obtiene el valor de la propiedad listaRecursos.
     * 
     * @return
     *     possible object is
     *     {@link ArrayOfCfgRecursoEnlaceInternoConInterface }
     *     
     */
    public ArrayOfCfgRecursoEnlaceInternoConInterface getListaRecursos() {
        return listaRecursos;
    }

    /**
     * Define el valor de la propiedad listaRecursos.
     * 
     * @param value
     *     allowed object is
     *     {@link ArrayOfCfgRecursoEnlaceInternoConInterface }
     *     
     */
    public void setListaRecursos(ArrayOfCfgRecursoEnlaceInternoConInterface value) {
        this.listaRecursos = value;
    }

    /**
     * Obtiene el valor de la propiedad dependencia.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getDependencia() {
        return dependencia;
    }

    /**
     * Define el valor de la propiedad dependencia.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setDependencia(String value) {
        this.dependencia = value;
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
     * Obtiene el valor de la propiedad origenR2.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getOrigenR2() {
        return origenR2;
    }

    /**
     * Define el valor de la propiedad origenR2.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setOrigenR2(String value) {
        this.origenR2 = value;
    }

    /**
     * Obtiene el valor de la propiedad dominio.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getDominio() {
        return dominio;
    }

    /**
     * Define el valor de la propiedad dominio.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setDominio(String value) {
        this.dominio = value;
    }

}

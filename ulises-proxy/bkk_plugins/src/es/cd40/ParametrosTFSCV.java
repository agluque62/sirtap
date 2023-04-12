
package es.cd40;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlSeeAlso;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para ParametrosTFSCV complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="ParametrosTFSCV"&gt;
 *   &lt;complexContent&gt;
 *     &lt;extension base="{http://CD40.es/}ParametrosRecursoGeneral"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="ListaEnlacesInternos" type="{http://CD40.es/}ArrayOfString" minOccurs="0"/&gt;
 *         &lt;element name="Lado" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="Modo" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/extension&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "ParametrosTFSCV", propOrder = {
    "listaEnlacesInternos",
    "lado",
    "modo"
})
@XmlSeeAlso({
    ParametrosRecursoTelefonia.class
})
public class ParametrosTFSCV
    extends ParametrosRecursoGeneral
{

    @XmlElement(name = "ListaEnlacesInternos")
    protected ArrayOfString listaEnlacesInternos;
    @XmlElement(name = "Lado")
    protected String lado;
    @XmlElement(name = "Modo")
    protected String modo;

    /**
     * Obtiene el valor de la propiedad listaEnlacesInternos.
     * 
     * @return
     *     possible object is
     *     {@link ArrayOfString }
     *     
     */
    public ArrayOfString getListaEnlacesInternos() {
        return listaEnlacesInternos;
    }

    /**
     * Define el valor de la propiedad listaEnlacesInternos.
     * 
     * @param value
     *     allowed object is
     *     {@link ArrayOfString }
     *     
     */
    public void setListaEnlacesInternos(ArrayOfString value) {
        this.listaEnlacesInternos = value;
    }

    /**
     * Obtiene el valor de la propiedad lado.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getLado() {
        return lado;
    }

    /**
     * Define el valor de la propiedad lado.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setLado(String value) {
        this.lado = value;
    }

    /**
     * Obtiene el valor de la propiedad modo.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getModo() {
        return modo;
    }

    /**
     * Define el valor de la propiedad modo.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setModo(String value) {
        this.modo = value;
    }

}

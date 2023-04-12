
package es.cd40;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlSchemaType;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para ListaRedes complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="ListaRedes"&gt;
 *   &lt;complexContent&gt;
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="IdRed" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="Prefijo" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="ListaRecursos" type="{http://CD40.es/}ArrayOfPlanRecursos" minOccurs="0"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/restriction&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "ListaRedes", propOrder = {
    "idRed",
    "prefijo",
    "listaRecursos"
})
public class ListaRedes {

    @XmlElement(name = "IdRed")
    protected String idRed;
    @XmlElement(name = "Prefijo")
    @XmlSchemaType(name = "unsignedInt")
    protected long prefijo;
    @XmlElement(name = "ListaRecursos")
    protected ArrayOfPlanRecursos listaRecursos;

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
     * Obtiene el valor de la propiedad prefijo.
     * 
     */
    public long getPrefijo() {
        return prefijo;
    }

    /**
     * Define el valor de la propiedad prefijo.
     * 
     */
    public void setPrefijo(long value) {
        this.prefijo = value;
    }

    /**
     * Obtiene el valor de la propiedad listaRecursos.
     * 
     * @return
     *     possible object is
     *     {@link ArrayOfPlanRecursos }
     *     
     */
    public ArrayOfPlanRecursos getListaRecursos() {
        return listaRecursos;
    }

    /**
     * Define el valor de la propiedad listaRecursos.
     * 
     * @param value
     *     allowed object is
     *     {@link ArrayOfPlanRecursos }
     *     
     */
    public void setListaRecursos(ArrayOfPlanRecursos value) {
        this.listaRecursos = value;
    }

}
